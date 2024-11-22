# YouTube Storage

## Description

I read somewhere that if you upload a video to YouTube, it doesn't take up any space on your Google Drive. So, theoretically, you could zip up all the files you want to store on Google Drive, convert it into a video and upload it to YouTube, right?

But how to do it?

Well, luckily, each file is essentially a stream of 0s and 1s, which can be represented as a black or white pixel in a frame. Multiple such frames would then make a video.

Et voila! That's exactly what I've done. Install the [dependencies](#dependencies) and proceed to build the [examples](./example/) I've provided on both encryption and decryption to and from video files.

> [!WARNING]
> The "encryption" being used in this context is just a fancy way of saying that the file is being converted into a video. It's not actually encrypted in any way that a lay-person might think of when they hear the word "encrypted". The same goes for "decryption".

## Dependencies

To make and read from a video file, I've used OpenCV, a popular library, compatible with both C++ and python alongside ffmpeg. You may install them using:

```bash
sudo apt install ffmpeg libopencv-dev
```

Like the fool I am, I built the library from source, since I wanted to use GPU acceleration (I hear you, setting up CuDA and CuDNN is a hassle). But that's not necessary for this project. You can follow any guide availble on the internet on how to build opencv from scratch for that matter. The cmake command I used (for local build on my system) was:

```bash
cmake \
-D WITH_QT=ON \
-D WITH_CUDA=ON \
-D WITH_CUDNN=ON \
-D WITH_FFMPEG=ON \
-D OPENCV_DNN_CUDA=0N \
-D CUDA_ARCH_BIN=7.5 \
-D CMAKE_C_COMPILER=gcc-12 \
-D CMAKE_CXX_COMPILER=g++-12 \
-D OPENCV_EXTRA_MODULES_PATH=/home/adit/Documents/opencv_contrib/modules \
-D PYTHON3_EXECUTABLE=/usr/bin/python3 \
-D PYTHON_LIBRARIES=/usr/local/lib/python3.12/dist-packages \
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
> - `INPUT_PATH`: The absolute or relative path to the file or folder you want to encrypt or decrypt
> - `OUTPUT_DIR`: The absolute or relative path to the directory where you want to save the output video (defaults to `./`)
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

## Docker

Of course, setting up the environment can be a hassle, so I've provided a [Dockerfile](./docker/build.dockerfile) to build the project in a container. To build the image locally, run:

```bash
docker build -t yt-storage -f docker/build.dockerfile .
```

Alternatively, you can use the [pre-buit image](https://hub.docker.com/r/jadit19/yt-storage) over at Docker Hub (supports both amd64 and arm64 architecture):

```bash
docker pull jadit19/yt-storage
```

Make sure you've got the file you want to encrypt / decrypt in the [files](./files) directory. To run the container, execute:

```bash
docker run -v $(pwd)/files:/files -e INPUT_PATH="<file_name>" -e OUTPUT_DIR="/files" -e MODE="<0/1>" jadit19/yt-storage
```

Remember that the `INPUT_PATH` environment variable should be the absolute or relative path to the file or folder you want to encrypt / decrypt, the `OUTPUT_DIR` environment variable should be the absolute or relative path to the directory where you want to save the output video, and the `MODE` environment variable should be `0` for encryption and `1` for decryption.

If you are like me and prefer using docker compose, you can use the [encrypt.compose.yaml](./docker/encrypt.compose.yaml) and [decrypt.compose.yaml](./docker/decrypt.compose.yaml) files to run the encryption and decryption processes respectively after specifying the required environment variables:

```bash
docker compose -f docker/encrypt.compose.yaml up
```

```bash
docker compose -f docker/decrypt.compose.yaml up
```

For testing purposes, I've provided a sample file that you can decrypt post downloading from YouTube: [https://youtu.be/9h4u79ppbXM](https://youtu.be/9h4u79ppbXM)

## Internal working

Apart from the actual 0s and 1s, I need to store some (meta)data in the encrypted video file that can be used to decrypt it. For now, I am saving 3 things, namely, the name of the output file (post decryption), the total size of the original or final zipped file (in bits) and the version of yt-storage used to encrypt the file (backwards compatibility is not guaranteed).

I am saving this metadata as a json string in the first few bits of the video. Now, to properly read this metadata, I need to know how many bits to read. This is where the `metadata_size` comes in. It is the number of bits required to store the metadata, and occupies the first 16 bits of the first frame of the video.

The next `metadata_size` bits of the video are the metadata, which is essentially strigified json, which is parsed to obtain the metadata. The rest of the video is the actual data, which ends up in the decrypted file.

After doing, I uploaded an encrypted file to YouTube and then download it in the resolution I had uploaded it in. Now obviously, the file is not going to be the same size as the original file, so, I was not able to decrypt it. Then, I introduced the concept of redundancy (still in its nascent phase now), where I duplicate the data `yt::REDUNDANCY` times while storing, which might explain the long dashes you see in the video. Similarly, while reading, I read the data `yt::REDUNDANCY` times and take the mode of the bits to get the original data. Essentially, this is a very basic form of error correction a.k.a. redundency for reliability.

## Future works

For added security, the read stream can be encoded using the OpenSSL library and require a key to decrypt. Also, this thing could do with some optimization, which are two ways I can think of right now:

1. Black and white pixels are not the only way to represent bits. I could use the RGB channels to represent 3 bits per pixel, which would reduce the size of the video by a third.

2. Each pixel's intensity is represented by a byte, which is 8 bits long (0-255). Hence, using only this, a single pixel could store 8 bits / 1 byte, which is 8 times more than the current implementation.

## Disclaimer

This is _just_ a proof-of-concept project that I made for fun and partly because I like C++.
