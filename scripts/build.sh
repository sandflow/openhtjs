#!/bin/sh

BUILD_DIR=build/htjs
DIST_DIR=dist

# transpile OpenJPH
mkdir -p $BUILD_DIR
rm -r $BUILD_DIR/*
docker build -t openht .
IMAGE_ID=$(docker create openht)
docker cp ${IMAGE_ID}:/src/build/ $BUILD_DIR
docker rm ${IMAGE_ID}

# create the distribution
mkdir -p $DIST_DIR
rm -r $DIST_DIR/*
cp build/htjs/build/*.js $DIST_DIR
cp build/htjs/build/*.wasm $DIST_DIR
cp src/js/*.js $DIST_DIR