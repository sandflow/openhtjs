import {Decoder} from "./HT.js"

let response = await fetch('codestreams/ht-sample.j2c');

const j2c_buffer = await response.arrayBuffer();

const j2c_bytes = new Uint8Array(j2c_buffer);

let decoder = new Decoder(j2c_bytes.length);

const decoder_buffer = decoder.getCodestreamBuffer();

decoder_buffer.set(j2c_bytes);

decoder.readHeader();

const img_info = decoder.getImageInfo();

let c = document.getElementById("img");

c.width = img_info.width;
c.height = img_info.height;

let ctx = c.getContext("2d");

let imageData = ctx.createImageData(img_info.width, img_info.height);

decoder.decodeToRGBAArray(imageData);

ctx.putImageData(imageData, 0, 0);