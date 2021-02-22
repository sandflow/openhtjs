// Copyright (c) Chris Hafey
// Modifications copyright (c) Pierre-Anthony Lemieux
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice (including the next paragraph) shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <exception>
#include <limits.h>
#include <memory>
#include <algorithm>

#include <ojph_arch.h>
#include <ojph_codestream.h>
#include <ojph_file.h>
#include <ojph_mem.h>
#include <ojph_params.h>

#include <emscripten/val.h>

#include "ImageInfo.hpp"
#include "Point.hpp"
#include "Size.hpp"

class HTDecoder {

public:
  HTDecoder(size_t codestreamSize)
      : encoded_(codestreamSize), state_(INIT) {

    this->codestream_.enable_resilience();
  }

  emscripten::val getCodestreamBuffer() {

    return emscripten::val(emscripten::typed_memory_view(encoded_.size(), encoded_.data()));
  }

  void readHeader() {

    if (this->state_ != INIT) {
      throw "Codestream headers have already been read";
    }

    this->memFile_.open(this->encoded_.data(), this->encoded_.size());

    this->codestream_.read_headers(&this->memFile_);

    ojph::param_siz siz = this->codestream_.access_siz();

    imageInfo_.width = siz.get_image_extent().x - siz.get_image_offset().x;

    imageInfo_.height = siz.get_image_extent().y - siz.get_image_offset().y;

    imageInfo_.componentCount = siz.get_num_components();

    imageInfo_.bitsPerSample = siz.get_bit_depth(0);

    imageInfo_.isSigned = siz.is_signed(0);

    for (size_t i = 0; i < imageInfo_.componentCount; i++) {
      if (siz.is_signed(i)) {
        throw "Signed samples are not yet supported";
      }
    }

    downSamples_.resize(imageInfo_.componentCount);
    for (size_t i = 0; i < imageInfo_.componentCount; i++) {
      downSamples_[i].x = siz.get_downsampling(i).x;
      downSamples_[i].y = siz.get_downsampling(i).y;
    }

    imageOffset_.x = siz.get_image_offset().x;
    imageOffset_.y = siz.get_image_offset().y;
    tileSize_.width = siz.get_tile_size().w;
    tileSize_.height = siz.get_tile_size().h;

    tileOffset_.x = siz.get_tile_offset().x;
    tileOffset_.y = siz.get_tile_offset().y;

    ojph::param_cod cod = this->codestream_.access_cod();
    numDecompositions_ = cod.get_num_decompositions();
    isReversible_ = cod.is_reversible();
    progressionOrder_ = cod.get_progression_order();
    blockDimensions_.width = cod.get_block_dims().w;
    blockDimensions_.height = cod.get_block_dims().h;
    precincts_.resize(numDecompositions_);
    for (size_t i = 0; i < numDecompositions_; i++) {
      precincts_[i].width = cod.get_precinct_size(i).w;
      precincts_[i].height = cod.get_precinct_size(i).h;
    }
    numLayers_ = cod.get_num_layers();
    isUsingColorTransform_ = cod.is_using_color_transform();

    this->state_ = READY_TO_DECODE;
  }

  Size getDecodedImageSize(int decompositionLevel) {

    Size result(imageInfo_.width, imageInfo_.height);

    while (decompositionLevel > 0) {
      result.width = ojph_div_ceil(result.width, 2);
      result.height = ojph_div_ceil(result.height, 2);
      decompositionLevel--;
    }

    return result;
  }

  void startDecoding(int decompositionLevel, bool requestPlanar) {

    if (this->state_ != READY_TO_DECODE) {
      throw "Cannot start decoding twice or before reading the codestream "
            "headers";
    }

    this->codestream_.restrict_input_resolution(decompositionLevel, decompositionLevel);

    if (this->isUsingColorTransform_ && requestPlanar) {
      throw "requestPlanar must be false since the codestream uses color transforms";
    }

    bool isDownsamplingUsed = std::any_of(
      this->downSamples_.begin(),
      this->downSamples_.end(),
      [](const Point& value) {
        return value.x > 1 || value.y > 1;
      }
    );

    if (this->isDownsamplingUsed() && requestPlanar == false) {
      throw "requestPlanar must be false since the codestream uses color transforms";
    }

    this->isPlanarRequested_ = requestPlanar;
    this->codestream_.set_planar(isPlanarRequested_);

    this->codestream_.create();

    this->state_ = DECODING;

  }

  emscripten::val decodeLineAsUnsignedSamples() {

    if (this->state_ != DECODING) {
      throw "Not ready to decode";
    }

    ojph::line_buf* line = this->codestream_.pull(this->compNum_);

    return emscripten::val(emscripten::typed_memory_view(line->size, (uint32_t*) line->i32));
  }

  int getDecodedComponentNumber() const {
    if (this->isPlanarRequested_ == false) {
      throw "Can only be called if planar decoding was requested";
    }

    return this->compNum_;
  }

  const ImageInfo& getImageInfo() const {
    return this->imageInfo_;
  }

  const size_t getNumDecompositions() const {
    return this->numDecompositions_;
  }

  const bool isReversible() const {
    return this->isReversible_;
  }

  const size_t getProgressionOrder() const {
    return this->progressionOrder_;
  }

  Point getDownSample(size_t component) const {
    return this->downSamples_[component];
  }

  Point getImageOffset() const {
    return this->imageOffset_;
  }

  Size getTileSize() const {
    return this->tileSize_;
  }

  Point getTileOffset() const {
    return this->tileOffset_;
  }

  Size getBlockDimensions() const {
    return this->blockDimensions_;
  }

  Size getPrecinct(size_t level) const {
    return this->precincts_[level];
  }

  int32_t getNumLayers() const {
    return this->numLayers_;
  }

  bool isUsingColorTransform() const {
    return this->isUsingColorTransform_;
  }

  bool isDownsamplingUsed() const {
    return std::any_of(
      this->downSamples_.begin(),
      this->downSamples_.end(),
      [](const Point& value) {
        return value.x > 1 || value.y > 1;
      }
    );
  }

private:
  enum State { INIT, READY_TO_DECODE, DECODING };

  HTDecoder::State state_;
  ojph::codestream codestream_;
  ojph::mem_infile memFile_;

  bool isPlanarRequested_;

  int compNum_;

  std::vector<uint8_t> encoded_;
  ImageInfo imageInfo_;
  std::vector<Point> downSamples_;
  int numDecompositions_;
  bool isReversible_;
  int progressionOrder_;
  Point imageOffset_;
  Size tileSize_;
  Point tileOffset_;
  Size blockDimensions_;
  std::vector<Size> precincts_;
  int numLayers_;
  bool isUsingColorTransform_;
};
