# openHTJS

                              __  ________  _______
      ____  ____  ___  ____  / / / /_  __/ / / ___/
     / __ \/ __ \/ _ \/ __ \/ /_/ / / /_  / /\__ \ 
    / /_/ / /_/ /  __/ / / / __  / / / /_/ /___/ / 
    \____/ .___/\___/_/ /_/_/ /_/ /_/\____//____/  
        /_/                                                            

## Introduction

openHTJS is a JavaScript library for decoding HT codestreams. HT codestreams are JPEG 2000 codestreams that use the ultra-fast block decoder
specified in [Rec. ITU-T T.814 | ISO/IEC 15444-15](https://www.itu.int/rec/T-REC-T.814).

It is built on top of the [OpenJPH library](https://github.com/aous72/OpenJPH), which it compiles to JavaScript using
[Emscripten](https://emscripten.org/index.html). It is heavily inspired by the [openjphjs
project](https://github.com/chafey/openjphjs).

## Known issues and limitations

Bugs are tracked at https://github.com/sandflow/openhtjs/issues.

## Dependencies

openHTJS does not depend on external libraries. It requires support for WebAssembly and ES6 modules.

## Quick start

See `src/test/site` for an example that fetches a codestream and decodes into a `Canvas` element.

## API

### General

`HT.js` is an ES6 module that exposes a single `Decoder` class:

```js
import {Decoder} from "./HT.js"
```

### Constructor

```js
Decoder(codestreamSize)
```

* `codestreamSize` is the size of the codestream in bytes that will be decoder. The codestream is not required to be complete.

A `Decoder` object must instantiated for each codestream to be decoded.

### getCodestreamBuffer()

```js
UInt8Array getCodestreamBuffer()
```

Returns a view to the codestream buffer, which the client must fill with the codestream, e.g.:

```js
let decoder = new Decoder(j2c_bytes.length);
const decoder_buffer = decoder.getCodestreamBuffer();
decoder_buffer.set(j2c_bytes);
```

### readHeader()

    readHeader()

Parses the codestream header. Must be called before any method, other than `getCodestreamBuffer()` is called.

### getDecodedImageSize()

    Size getDecodedImageSize(int decompositionLevel)

Returns the decoded image size at decomposition level `decompositionLevel`. `decompositionLevel = 0` means the full image resolution.

### startDecoding()

    Size startDecoding(int decompositionLevel, bool requestPlanar)

Starts the decoding process.

* `decompositionLevel` indicates that the decomposition level to which the image is decoded. `decompositionLevel = 0` indicates that
  the full image is decoded.

* `requestPlanar` indicates whether successive calls to `decodeLine*` output `RRR..., GGG..., BBB..., RRR...,...` or  `RGBRGBRGB...,
  ...` (`false`). `requestPlanar` must be `true` if the encoded image contains any downsampled components, and `false` if the
  codestream uses a color transform.

### decodeLineAsUnsignedSamples()

    UInt32Array decodeLineAsUnsignedSamples()

Decodes a complete line of the image as a sequence of `UInt32` values.

### getDecodedComponentNumber()

    int getDecodedComponentNumber()

Returns the index of the component last decoded by `decodeLineAsUnsignedSamples()`. Can only be called if `requestPlanar` is `true`.

### getImageInfo()

    FrameInfo getImageInfo()

Returns information about the image.

### getNumDecompositions()

    int getNumDecompositions()

Returns the number of decomposition levels available.

### isReversible()

    bool isReversible()

Indicates whether the image coding was lossless.

### getProgressionOrder()

    int getProgressionOrder()

Returns the progression order.

### getDownSample()

    Point getDownSample(int i)

Returns the horizontal and vertical downsampling factor for the ith component.

### getImageOffset()

    Point getImageOffset()

Returns the image offset.

### isUsingColorTransform()

    bool isUsingColorTransform()

Indicates whether the codestream uses color transforms.

### isDownsamplingUsed()

    bool isDownsamplingUsed()

Indicates whether any component of the codestream is downsampled.

### Size

```cpp
struct Size {
    uint32_t width;
    uint32_t height;
}
```

### FrameInfo

```cpp
struct FrameInfo {
    uint16_t width;
    uint16_t height;
    uint8_t bitsPerSample;
    uint8_t componentCount;
    bool isSigned;
}
```

### Point

```cpp
struct Point {
  uint32_t x;
  uint32_t y;
}
```

## Development

### Dependencies

openHTJS uses [docker](https://www.docker.com/) to run emscripten.

### Quick start

Clone the repository:

    git clone --recursive https://github.com/sandflow/openhtjs.git

Build the distribution under `dist`:

    npm run build

Build the demo web page to `build/site`:

    /bin/sh scripts/setup_site.sh
