#include <iostream>
#include <fstream>
#include <cstdint>

int main(){
    const int width = 256;
    const int height = 256;

    std::ofstream image("output.ppm");

    // Ecriture de l'en-tête ppm
    image << "P3\n" << width << " " << height << "\n255\n";

    int numPixels = width * height;
    
    //Allocation de 3 octets par pixel
    uint8_t* pixels = new uint8_t[numPixels * 3];

    // Initialisation de tous les pixels à blanc
    for(int i = 0; i < numPixels * 3; i++)
        pixels[i] = 255;

    // Remplissage de l'image en blanc
    for(int i = 0; i < numPixels; i++)
        image << static_cast<int>(pixels[i * 3]) << " "
            << static_cast<int>(pixels[i * 3 + 1]) << " "
            << static_cast<int>(pixels[i * 3 + 2]) << " ";

    delete[] pixels;
    image.close();
    std::cout << "Image générée" << std::endl;
    return 0;
}