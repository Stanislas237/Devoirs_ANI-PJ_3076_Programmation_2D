#include <iostream>
#include <fstream>
#include <cstdint>

int main(){
    const int width = 256;
    const int height = 256;

    std::ofstream image("output_uint32.ppm");

    // Ecriture de l'en-tête ppm
    image << "P3\n" << width << " " << height << "\n255\n";

    int numPixels = width * height;
    
    //1 entier uint32 par pixel
    uint32_t* pixels = new uint32_t[numPixels];

    // Initialisation de tous les pixels à blanc
    for(int i = 0; i < numPixels; i++)
        pixels[i] = 0xFFFFFFFF;

    // Remplissage de l'image en blanc
    for(int i = 0; i < numPixels; i++)
        image << static_cast<int>((pixels[i] & 0xFF0000) >> 24) << " "
            << static_cast<int>((pixels[i] & 0x00FF00) >> 16) << " "
            << static_cast<int>((pixels[i] & 0x00FF00) >> 8) << " ";

    delete[] pixels;
    image.close();
    std::cout << "Image générée" << std::endl;
    return 0;
}