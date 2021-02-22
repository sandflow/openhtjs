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

struct ImageInfo {
    /// <summary>
    /// Width of the image, range [1, 65535].
    /// </summary>
    uint16_t width;

    /// <summary>
    /// Height of the image, range [1, 65535].
    /// </summary>
    uint16_t height;

    /// <summary>
    /// Number of bits per sample, range [2, 16]
    /// </summary>
    uint8_t bitsPerSample;

    /// <summary>
    /// Number of components contained in the frame, range [1, 255]
    /// </summary>
    uint8_t componentCount;

    /// <summary>
    /// true if signed, false if unsigned
    /// </summary>
    bool isSigned;
};