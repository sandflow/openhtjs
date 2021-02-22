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

#include "HTDecoder.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(ImageInfo) {
  value_object<ImageInfo>("ImageInfo")
    .field("width", &ImageInfo::width)
    .field("height", &ImageInfo::height)
    .field("bitsPerSample", &ImageInfo::bitsPerSample)
    .field("componentCount", &ImageInfo::componentCount)
    .field("isSigned", &ImageInfo::isSigned)
       ;
}

EMSCRIPTEN_BINDINGS(Point) {
  value_object<Point>("Point")
    .field("x", &Point::x)
    .field("y", &Point::y)
       ;
}

EMSCRIPTEN_BINDINGS(Size) {
  value_object<Size>("Size")
    .field("width", &Size::width)
    .field("height", &Size::height)
       ;
}

EMSCRIPTEN_BINDINGS(HTDecoder) {
  class_<HTDecoder>("HTDecoder")
    .constructor<size_t>()
    .function("getCodestreamBuffer", &HTDecoder::getCodestreamBuffer)
    .function("readHeader", &HTDecoder::readHeader)
    .function("getDecodedImageSize", &HTDecoder::getDecodedImageSize)
    .function("startDecoding", &HTDecoder::startDecoding)
    .function("decodeLineAsUnsignedSamples", &HTDecoder::decodeLineAsUnsignedSamples)
    .function("getDecodedComponentNumber", &HTDecoder::getDecodedComponentNumber)
    .function("getImageInfo", &HTDecoder::getImageInfo)
    .function("getDownSample", &HTDecoder::getDownSample)
    .function("getNumDecompositions", &HTDecoder::getNumDecompositions)
    .function("isReversible", &HTDecoder::isReversible)
    .function("getProgressionOrder", &HTDecoder::getProgressionOrder)
    .function("getImageOffset", &HTDecoder::getImageOffset)
    .function("getTileSize", &HTDecoder::getTileSize)
    .function("getTileOffset", &HTDecoder::getTileOffset)
    .function("getBlockDimensions", &HTDecoder::getBlockDimensions)
    .function("getPrecinct", &HTDecoder::getPrecinct)
    .function("getNumLayers", &HTDecoder::getNumLayers)
    .function("isReversible", &HTDecoder::isReversible)
    .function("isDownsamplingUsed", &HTDecoder::isDownsamplingUsed)
   ;
}