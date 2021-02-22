// Copyright (c) Pierre-Anthony Lemieux
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

import makeHTCodec from './HT_internal.js';

const ht_codec = await makeHTCodec();

ht_codec.HTDecoder.prototype.decodeToRGBAArray = function (array_buffer, decompositionLevel = 0) {
  const img_info = this.getImageInfo();
  const shift = Math.max(0, img_info.bitsPerSample - 8);
  const max_component_value = 1 << img_info.bitsPerSample;

  const img_dim = this.getDecodedImageSize(decompositionLevel);
  
  this.startDecoding(decompositionLevel, false);

  let outLineOffset = 0;

  for (let y = 0; y < img_dim.height; y++) {

    for (let c = 0; c < 3; c++) {

      let outPixelOffset = outLineOffset + c;

      const pixelData = this.decodeLineAsUnsignedSamples();

      for (let x = 0; x < img_dim.width; x++) {
        array_buffer.data[outPixelOffset] = Math.min(Math.max(pixelData[x], 0), max_component_value) >> shift;
        outPixelOffset += 4;
      }

    }

    let outPixelOffset = outLineOffset + 3;

    for (let x = 0; x < img_dim.width; x++) {
      array_buffer.data[outPixelOffset] = 255;
      outPixelOffset += 4;
    }

    outLineOffset += img_dim.width * 4;
  }
};

export const Decoder = ht_codec.HTDecoder
