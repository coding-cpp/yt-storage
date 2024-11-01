# YouTube Storage

## Description

I read somewhere that if you upload a video to YouTube, it doesn't take up any space on your Google Drive. So, theoretically, you could zip up all the files you want to store on Google Drive, convert it into a video and upload it to YouTube, right?

But how to do it?

Well, luckily, each file is essentially a stream of 0s and 1s, which can be represented as a black or white pixel in a frame. Multiple such frames would then make a video.

Et voila! That's exactly what I've done. Install the [dependencies](#dependencies) and proceed to build the [examples](./example/) I've provided on both encryption and decryption to and from video files.

> [!WARNING]
> The "encryption" being used in this context is just a fancy way of saying that the file is being converted into a video. It's not actually encrypted in any way that a lay-person might think of when they hear the word "encrypted". The same goes for "decryption".

## Dependencies

To make and read from a video file, I've used OpenCV, a popular library, compatible with both C++ and python. You may install it using:

```bash
sudo apt install libopencv-dev
```

Like the fool I am, I built the library from source, since I wanted to use GPU acceleration (I hear you, setting up CuDA and CuDNN is a hassle). But that's not necessary for this project. You can follow any guide availble on the internet on how to build opencv from scratch for that matter. The cmake command I used was:

```bash
cmake \
-D WITH_QT=ON \
-D WITH_CUDA=ON \
-D WITH_CUDNN=ON \
-D OPENCV_DNN_CUDA=0N \
-D CUDA_ARCH_BIN=7.5 \
-D CMAKE_C_COMPILER=gcc-12 \
-D CMAKE_CXX_COMPILER=g++-12 \
-D OPENCV_EXTRA_MODULES_PATH=/home/adit/Documents/opencv_contrib/modules \
-D PYTHON3_EXECUTABLE=/usr/bin/python3 \
-D PYTHON_LIBRARIES=/usr/local/lib/python3.12/dist-packages \
-D WITH_FFMPEG=ON \
-D VIDEO_CODEC_SDK_PATH=/opt/nvidia_video_codec_sdk \
-D CMAKE_BUILD_TYPE=Release ..
```

Needless to say, change the paths and versions to match your system.

## Running

Clone the project and navigate to the root directory:

```bash
git clone git@github.com:coding-cpp/yt-storage
cd yt-storage
```

Clone the submodules:

```bash
git submodule init
git submodule update
```

Build the project:

```bash
mkdir build
cd build
cmake ..
make
```

> [!IMPORTANT]
> To run the example, you absolutely need to provide the following environment variables:
>
> - `INPUT_FILE`: The absolute or relative path to the file you want to convert into a video
> - `MODE`: `0` for encryption and `1` for decryption (defaults to `0`)

Execute the program:

```bash
./yt-storage
```

> [!TIP]
> Optionally, you can provide multiple other environment variables to customize the output video:
>
> - `OUTPUT_FILE`: The name of the output video file (defaults to `output.mp4`)
> - `WIDTH`: The width of the output video (defaults to `1920`)
> - `HEIGHT`: The height of the output video (defaults to `1080`)
> - `FPS`: The frames per second of the output video (defaults to `30`)

## Internal working

Apart from the actual 0s and 1s, I need to store some (meta)data in the encrypted video file that can be used to decrypt it. For now, I am saving 2 things, namely, the name of the output file (post decryption) and the total size of the original file (in bits).

I am saving this metadata as a json string in the first few bits of the video. Now, to properly read this metadata, I need to know how many bits to read. This is where the `metadata_size` comes in. It is the number of bits required to store the metadata, and occupies the first 16 bits of the first frame of the video.

The next `metadata_size` bits of the video are the metadata, which is essentially strigified json, which is parsed to obtain the metadata. The rest of the video is the actual data, which ends up in the decrypted file.

## Future works

For added security, the read stream can be encoded using the OpenSSL library and require a key to decrypt. Moreover, encrypting entire folders could be a potential. Also, this thing could do with some optimization.

## Disclaimer

This is _just_ a proof-of-concept project that I made for fun and partly because I like C++.
