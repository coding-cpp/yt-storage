FROM alpine:3.20.2 AS builder

RUN apk update && apk add --no-cache g++ make cmake wget linux-headers yasm nasm pkgconfig \
  libjpeg-turbo-static libpng-static zlib-static

RUN wget https://ffmpeg.org/releases/ffmpeg-6.0.tar.bz2 \
  && tar xjf ffmpeg-6.0.tar.bz2 && cd ffmpeg-6.0 \
  && ./configure --disable-shared --enable-static --disable-asm \
  && make -j$(nproc) && make install

RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/refs/heads/4.x.zip \
  && unzip opencv.zip && cd opencv-4.x \
  && mkdir build && cd build \
  && cmake -D WITH_FFMPEG=ON \
  -D BUILD_SHARED_LIBS=OFF \
  -D CMAKE_BUILD_TYPE=Release \
  -D FFMPEG_INCLUDE_DIR=/usr/local/include \
  -D FFMPEG_LIB_DIR=/usr/local/lib \
  -D PKG_CONFIG_PATH="/usr/local/lib/pkgconfig" .. \
  && make -j$(nproc) && make install

WORKDIR /app

COPY example /app/example
COPY include /app/include
COPY lib /app/lib
COPY src /app/src
COPY CMakeLists.txt /app/CMakeLists.txt

RUN mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_EXE_LINKER_FLAGS="-static" .. && make
RUN chmod +x /app/build/yt-storage

FROM scratch

WORKDIR /app

COPY --from=builder /app/build/yt-storage /app/yt-storage

ENTRYPOINT ["/app/yt-storage"]
