#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>
#include <cstring>

void blurImageStatic(int width, int height, std::vector<std::vector<unsigned char>>& image) {
    std::vector<std::vector<unsigned char>> blurredImage(height, std::vector<unsigned char>(width));

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int sum = 0;
            int count = 0;

            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < height && j >= 0 && j < width) {
                        sum += image[i][j];
                        count++;
                    }
                }
            }
            blurredImage[row][col] = static_cast<unsigned char>(sum / count);
        }
    }

    image = blurredImage;
}

void blurImageDynamic(int width, int height, unsigned char* image) {
    auto blurredImage = std::make_unique<unsigned char[]>(width * height);

    for (int row = 0; row < height; row++) {
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
            blurredImage[index] = static_cast<unsigned char>(sum / count);
        }
    }

    std::memcpy(image, blurredImage.get(), width * height * sizeof(unsigned char));
}

void printImageSection(const std::vector<std::vector<unsigned char>>& image, int section_size = 5) {
    for (int i = 0; i < section_size && i < image.size(); i++) {
        for (int j = 0; j < section_size && j < image[0].size(); j++) {
            std::cout << static_cast<int>(image[i][j]) << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    const int sizes[] = {32, 64, 128, 256, 512, 1024};
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < numSizes; i++) {
        int size = sizes[i];
        int width = size, height = size;

        std::cout << "\nProcessing image size: " << size << "x" << size << std::endl;

        // Static memory allocation using vector
        std::vector<std::vector<unsigned char>> imageStatic(height, std::vector<unsigned char>(width));
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                imageStatic[row][col] = static_cast<unsigned char>(std::rand() % 256);
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        blurImageStatic(width, height, imageStatic);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Static memory allocation: " << duration.count() << " microseconds" << std::endl;

        if (size <= 64) {
            std::cout << "\nStatic allocation result (5x5 section):\n";
            printImageSection(imageStatic);
        }

        // Dynamic memory allocation
        auto imageDynamic = std::make_unique<unsigned char[]>(width * height);
        for (int i = 0; i < height * width; i++) {
            imageDynamic[i] = static_cast<unsigned char>(std::rand() % 256);
        }

        start = std::chrono::high_resolution_clock::now();
        blurImageDynamic(width, height, imageDynamic.get());
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Dynamic memory allocation: " << duration.count() << " microseconds" << std::endl;
    }

    return 0;
} 