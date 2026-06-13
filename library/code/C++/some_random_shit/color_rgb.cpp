#include <iostream>
#include <vector>

extern "C" {
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
}

using namespace std;

bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if (data != nullptr)
    {
        image = vector<unsigned char>(data, data + x * y * 4);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

int main()
{
    string filename = "robin_and_palu(resized).png";
    freopen("robin_and_palu(resized).txt", "w", stdout);
    int width, height;
    vector<unsigned char> image;
    bool success = load_image(image, filename, width, height);

    // cout << "Image width = " << width << '\n';
    // cout << "Image height = " << height << '\n';
    
    const size_t RGBA = 4;
    cout << "{";
    for (int x = 0; x < width; ++x){
        cout << "{";
        for (int y = 0; y < height; ++y){
            size_t index = RGBA * (y * width + x);
            cout << "{" 
            << static_cast<int>(image[index + 0]) << ", "
            << static_cast<int>(image[index + 1]) << ", "
            << static_cast<int>(image[index + 2])
            << "}";
            if (y != height - 1){
                cout << ",";
            }
            // cout << "RGBA pixel @ (x= " << x <<  ", y= " << y << "): "
            //         << static_cast<int>(image[index + 0]) << " "
            //         << static_cast<int>(image[index + 1]) << " "
            //         << static_cast<int>(image[index + 2]) << " "
            //         << static_cast<int>(image[index + 3]) << endl;
        }
        cout << "}";
        if (x != width - 1){
            cout << ",";
        }
        cout << endl;
    }
    cout << "}";
    
    return 0;
}