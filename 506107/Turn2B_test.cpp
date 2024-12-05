#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <iomanip>
#include <cstring>

const int CHUNK_SIZE = 32;

// Function to blur a single chunk of the image
void blurImageChunk(int height, int width, unsigned char* image, int rowStart, int rowEnd) {
    auto tempImage = std::make_unique<unsigned char[]>(width * (rowEnd - rowStart + 1));
    
    for (int row = rowStart; row <= rowEnd; row++) {
        for (int col = 0; col < width; col++) {
            int sum = 0;
            int count = 0;

            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < height && j >= 0 && j < width) {
                        sum += image[i * width + j];
                        count++;
                    }
                }
            }
            
            int localIndex = (row - rowStart) * width + col;
            tempImage[localIndex] = static_cast<unsigned char>(sum / count);
        }
    }

    std::memcpy(image + rowStart * width, tempImage.get(), width * (rowEnd - rowStart + 1));
}

// Function to perform parallel image blurring
void blurImageParallel(int width, int height, unsigned char* image) {
    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    
    int rowsPerThread = height / numThreads;
    int remainingRows = height % numThreads;
    
    int currentRow = 0;
    for (int i = 0; i < numThreads; i++) {
        int chunkSize = rowsPerThread + (i < remainingRows ? 1 : 0);
        if (chunkSize > 0) {
            threads.push_back(std::thread(
                blurImageChunk,
                height, width, image,
                currentRow, currentRow + chunkSize - 1
            ));
            currentRow += chunkSize;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// Function to perform single-threaded image blurring
void blurImageSingle(int width, int height, unsigned char* image) {
    blurImageChunk(height, width, image, 0, height - 1);
}

void printImageSection(unsigned char* image, int width, int height, int section_size = 5) {
    std::cout << "Image section (" << section_size << "x" << section_size << "):\n";
    for (int i = 0; i < section_size && i < height; i++) {
        for (int j = 0; j < section_size && j < width; j++) {
            std::cout << std::setw(4) << static_cast<int>(image[i * width + j]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    const int sizes[] = {128, 256, 512, 1024};
    const int numThreads = std::thread::hardware_concurrency();

    std::cout << "System has " << numThreads << " hardware threads available\n\n";

    for (int size : sizes) {
        std::cout << "Processing image size: " << size << "x" << size << "\n";
        std::cout << std::string(50, '-') << "\n";
        
        auto image = std::make_unique<unsigned char[]>(size * size);
        for (int i = 0; i < size * size; i++) {
            image[i] = static_cast<unsigned char>(rand() % 256);
        }

        auto imageSingle = std::make_unique<unsigned char[]>(size * size);
        auto imageParallel = std::make_unique<unsigned char[]>(size * size);
        std::memcpy(imageSingle.get(), image.get(), size * size);
        std::memcpy(imageParallel.get(), image.get(), size * size);

        auto start = std::chrono::high_resolution_clock::now();
        blurImageSingle(size, size, imageSingle.get());
        auto end = std::chrono::high_resolution_clock::now();
        auto singleTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        start = std::chrono::high_resolution_clock::now();
        blurImageParallel(size, size, imageParallel.get());
        end = std::chrono::high_resolution_clock::now();
        auto parallelTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "Single-threaded time:   " << std::setw(10) << singleTime.count() << " microseconds\n";
        std::cout << "Multi-threaded time:    " << std::setw(10) << parallelTime.count() << " microseconds\n";
        std::cout << "Speedup:                " << std::fixed << std::setprecision(2) 
                  << static_cast<float>(singleTime.count()) / parallelTime.count() << "x\n\n";

        if (size <= 256) {
            std::cout << "Original image:\n";
            printImageSection(image.get(), size, size);
            std::cout << "Processed image (single-threaded):\n";
            printImageSection(imageSingle.get(), size, size);
            std::cout << "Processed image (multi-threaded):\n";
            printImageSection(imageParallel.get(), size, size);
        }
        
        std::cout << std::string(50, '-') << "\n\n";
    }

    return 0;
} 