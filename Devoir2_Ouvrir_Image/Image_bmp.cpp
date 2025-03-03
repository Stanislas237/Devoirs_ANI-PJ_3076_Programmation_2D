#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct DIBHeader {
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

int main() {
    std::ifstream image("output_bmp.bmp", std::ios::binary);

    if (!image) {
        std::cerr << "Erreur lors de l'ouverture de l'image BMP" << std::endl;
        return 1;
    }

    // Lire les entêtes du fichier BMP
    BMPHeader bmpHeader;
    DIBHeader dibHeader;
    image.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
    image.read(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));

    std::cout << "Taille du fichier : " << bmpHeader.fileSize << " bytes" << std::endl;
    std::cout << "Largeur : " << dibHeader.width << " pixels" << std::endl;
    std::cout << "Hauteur : " << dibHeader.height << " pixels" << std::endl;

    // Lire les données de l'image
    std::vector<uint8_t> imageData(dibHeader.imageSize);
    image.seekg(bmpHeader.dataOffset, std::ios::beg);
    image.read(reinterpret_cast<char*>(imageData.data()), dibHeader.imageSize);
    image.close();

    // Modifier les pixels entre les positions (70, 70) et (130, 130) en violet
    for (uint32_t x = 70; x <= 130; x++)
        for (uint32_t y = 70; y <= 130; y++) {
            uint32_t index = (y * dibHeader.width + x) * 3;
            imageData[index] = 255;   // Blue
            imageData[index + 1] = 0; // Green
            imageData[index + 2] = 255; // Red
        }

    std::ofstream newImage("new_image.bmp", std::ios::binary);
    if (!newImage) {
        std::cerr << "Erreur lors de la création du nouveau fichier BMP" << std::endl;
        return 1;
    }

    newImage.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
    newImage.write(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));
    newImage.write(reinterpret_cast<char*>(imageData.data()), dibHeader.imageSize);
    newImage.close();
    return 0;
}
