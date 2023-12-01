# YouTube Storage

## Description

I recently read somewhere that if you upload a video to YouTube, it doesn't take up any space on your Google Drive. So, theoretically, you could zip up all the files you want to store on Google Drive, convert it into a video and upload it to YouTube.

But how to do it?

Well, luckily, each file is essentially a stream of 0s and 1s, which can be represented as a black or white pixel in a frame. Multiple such frames would then make a video.

Et voila! That's exactly what I've done. Install the [dependencies](#dependencies) and proceed to build the [examples](./example/) I've provided on both [encryption](./example/encrypt.cpp) and [decryption](./example/decrypt.cpp) to and from video files.

## Dependencies

To make and read from a video file, I've used OpenCV, a popular library, compatible with both C++ and python.

```bash
sudo apt install libopencv-dev
```

## Running

### Encryption

```bash
./encrpty -i <input_file> -o <output_file> -r <resolution>
```

### Decryption

```bash
./decrypt -i <input_file> -o <output_file>
```

## Future works

For added security, the read stream can be encoded using the OpenSSL library and require a key to decrypt. Also, this thing could do with some optimization.

## Disclaimer

I did this just for fun and because I like C++. Probably to an unhealthy level of obsession.
