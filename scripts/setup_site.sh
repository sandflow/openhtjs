#!/bin/sh

mkdir -p build/site

cp -r src/test/site build
cp -r src/test/resources/codestreams build/site
cp -r src/js/* build/site
cp build/htjs/build/*.js build/site
cp build/htjs/build/*.wasm build/site