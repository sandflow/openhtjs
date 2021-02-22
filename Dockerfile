FROM trzeci/emscripten

WORKDIR /src
COPY . .

WORKDIR /src/build
RUN emcmake cmake ..
RUN emmake make