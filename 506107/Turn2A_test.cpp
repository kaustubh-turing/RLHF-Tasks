#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

const int CHUNK_SIZE = 32;

void blurImageChunk(int width, int height, int rowStart, int rowEnd, unsigned char* image) {
    for (int row = rowStart; row <= rowEnd; row++) {
        for (int col = 0; col < width; col++) {
            int index = row * width + col;
            int sum = 0;
            int count = 0;

            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    int imageIndex = i * width + j;
                    if (i >= 0 && i < height && j >= 0 && j < width) {
                        sum += image[imageIndex];
                        count++;
                    }
                }
            }
            image[index] = static_cast<unsigned char>(sum / count);
        }
    }
}

void blurImageDynamicParallel(int width, int height, unsigned char* image) {
    const int numThreads = std::thread::hardware_concurrency();
    int chunkHeight = (height + numThreads - 1) / numThreads;

    std::vector<std::thread> threads;

    for (int threadId = 0; threadId < numThreads; threadId++) {
        int rowStart = threadId * chunkHeight;
        int rowEnd = std::min((threadId + 1) * chunkHeight - 1, height - 1);
        threads.emplace_back(blurImageChunk, width, height, rowStart, rowEnd, image);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void blurImageDynamic(int width, int height, unsigned char* image) {
    blurImageChunk(width, height, 0, height - 1, image);
}

int main() {
    const int sizes[] = {128, 256, 512, 1024};
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    std::cout << "Using " << std::thread::hardware_concurrency() << " threads\n\n";

    for (int i = 0; i < numSizes; i++) {
        int size = sizes[i];
        int width = size, height = size;

        std::cout << "\nProcessing image size: " << size << "x" << size << std::endl;

        auto imageDynamic = std::make_unique<unsigned char[]>(width * height);
        for (int i = 0; i < height * width; i++) {
            imageDynamic[i] = static_cast<unsigned char>(std::rand() % 256);
        }

        {
            auto start = std::chrono::high_resolution_clock::now();
            blurImageDynamic(width, height, imageDynamic.get());
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Single-threaded time: " << duration.count() << " microseconds\n";
        }

        {
            auto start = std::chrono::high_resolution_clock::now();
            blurImageDynamicParallel(width, height, imageDynamic.get());
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Multi-threaded time:  " << duration.count() << " microseconds\n";
        }
    }

    return 0;
} 