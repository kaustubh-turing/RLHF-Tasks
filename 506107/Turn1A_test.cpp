#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include <chrono>
#include <cstring>

// Function to perform image blurring using static memory allocation
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

// Function to perform image blurring using dynamic memory allocation
void blurImageDynamic(int width, int height, unsigned char* image) {
    auto blurredImage = std::make_unique<unsigned char[]>(width * height);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int sum = 0;
            int count = 0;
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    int index = (i * width + j);
                    if (i >= 0 && i < height && j >= 0 && j < width) {
                        sum += image[index];
                        count++;
                    }
                }
            }

            int blurredIndex = row * width + col;
            blurredImage[blurredIndex] = static_cast<unsigned char>(sum / count);
        }
    }
    std::memcpy(image, blurredImage.get(), width * height * sizeof(unsigned char));
}

void printImage(const std::vector<std::vector<unsigned char>>& image, int section_size = 5) {
    for (int i = 0; i < section_size && i < image.size(); i++) {
        for (int j = 0; j < section_size && j < image[0].size(); j++) {
            std::cout << static_cast<int>(image[i][j]) << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    const int testSizes[] = {100, 500, 1000};

    for (int size : testSizes) {
        const int width = size;
        const int height = size;

        std::cout << "\nImage size: " << width << "x" << height << std::endl;
        
        // Test static allocation using vector
        std::vector<std::vector<unsigned char>> staticImage(height, std::vector<unsigned char>(width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                staticImage[i][j] = static_cast<unsigned char>(std::rand() % 256);
            }
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        blurImageStatic(width, height, staticImage);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Static allocation time: " << duration.count() << " microseconds\n";

        // Test dynamic allocation
        auto dynamicImage = std::make_unique<unsigned char[]>(width * height);
        for (int i = 0; i < height * width; ++i) {
            dynamicImage[i] = static_cast<unsigned char>(std::rand() % 256);
        }

        start = std::chrono::high_resolution_clock::now();
        blurImageDynamic(width, height, dynamicImage.get());
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Dynamic allocation time: " << duration.count() << " microseconds\n";
    }

    return 0;
} 