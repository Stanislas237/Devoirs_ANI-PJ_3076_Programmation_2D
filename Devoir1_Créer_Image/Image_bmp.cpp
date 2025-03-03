#include <iostream>
#include <fstream>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DIBHeader {
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount; // Nbre de bits par pixel (RGB = 24bits = 3octets)
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPixelsPerMeter;
    uint32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t importantColors;
};
#pragma pack(pop)

int main(){
    const int width = 256;
    const int height = 256;

    std::ofstream image("output_bmp.bmp", std::ios::binary);

    BMPHeader bmpHeader = {0x4D42, 0, 0, sizeof(BMPHeader) + sizeof(DIBHeader)};
    DIBHeader dibHeader = {sizeof(DIBHeader), width, height, 1, 24, 0, width * height * 3, 2835, 2835, 0, 0};
    bmpHeader.fileSize = bmpHeader.dataOffset + dibHeader.imageSize;

    image.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
    image.write(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));
        
    uint8_t pixel[3] = { 0, 0, 0 };
    int dPixel[3] = { 2, 3, 5 };
    //Remplissage : écrire pour chaque pixel les valeurs B, G, R
    for(int i = 0; i < width * height; i++){
        image.write(reinterpret_cast<char*>(pixel), 3);

        // Ajout d'un jeu de couleurs
        for(int j = 0; j < 3; j++){
            if (pixel[j] + dPixel[j] < 0 || pixel[j] + dPixel[j] > 255)
                dPixel[j] *= -1;
            pixel[j] += dPixel[j];
        }
    }

    image.close();
    std::cout << "Image générée" << std::endl;
    return 0;
}