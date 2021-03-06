#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "./stb/stb_image.h"
#include "./stb/stb_image_resize.h"
#include "./stb/stb_image_write.h"

using namespace std;

void create_a_random_pixel_location(int width, int height, int& r, int& c)
{
    int x;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            x = rand() % 100;

        }
    }
}

//
// Image structure
//
struct MyImg {
    unsigned char* data;
    int width;
    int channels;
    int height;
    std::string filename;
};

void print_img_info(MyImg* img)
{

    cout << "width = " << img->width << "\n";
    cout << "height = " << img->height << "\n";
    cout << "channels = " << img->channels << "\n";

}

void delete_img(MyImg* img)
{
    delete img;

}


//
// Image read/write
//
MyImg* load_jpeg_file(const string& filename)
{
    MyImg* img = 0;
    int width, height, channels;
    const char* c = filename.c_str();
    unsigned char* image = stbi_load(c, &width, &height, &channels, 0);
    if (image == 0) {
        cout << "Error loading image file" << endl;

    }
    else {
        img = new MyImg();
        img->width = width;
        img->height = height;
        img->channels = channels;
        img->data = image;

    }
    return img;

}
MyImg* get_subregion(MyImg* src, int top, int left, int bottom, int right) {
    int w, h;
    w = right - left;
    h = bottom - top;
    unsigned char* data = (unsigned char*)malloc(w * h * 3);
    memset(data, 0, w * h * 3);

    int move = (top * src->width + left) * src->channels;
    cout << "move is:" << move << endl;
    unsigned char* p = data;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; c++) {
            *(p) = *(src->data + move);
            *(p + 1) = *(src->data + move + 1);
            *(p + 2) = *(src->data + move + 2);
            p += 3;
            move += 3;
        }
    }

    MyImg* cropped = new MyImg();
    cropped->data = data;
    cropped->width = w;
    cropped->height = h;
    cropped->channels = src->channels;
    return cropped;

}

void save_to_jpeg_file(const string& filename, MyImg* img)
{
    const char* c = filename.c_str();
    stbi_write_jpg(c, img->width, img->height, img->channels, img->data, 100 /* quality = 100 */);
}


void set_pixel_red(MyImg* img, int r, int c, uint8_t val)
{

    int offset = r * img->width * img->channels + (c * img->channels) + 1;
    *(img->data + offset) = val;


}

void set_pixel_green(MyImg* img, int r, int c, uint8_t val)
{

    int offset = r * img->width * img->channels + (c * img->channels) + 1;
    *(img->data + offset + 1) = val;
}

void set_pixel_blue(MyImg* img, int r, int c, uint8_t val)
{
    int offset = r * img->width * img->channels + c * img->channels + 1;
    *(img->data + offset + 2) = val;
}

//
// Getters
//
uint8_t get_pixel_red(MyImg* img, int r, int c)
{
    int offset = r * img->width * img->channels + (c * img->channels) + 1;
    return *(img->data + offset);
}

uint8_t get_pixel_green(MyImg* img, int r, int c)
{
    int offset = r * img->width * img->channels + c * img->channels + 1;
    return *(img->data + offset + 1);
}

uint8_t get_pixel_blue(MyImg* img, int r, int c)
{
    int offset = r * img->width * img->channels + c * img->channels + 1;
    return *(img->data + offset + 2);
}


//
// The main function
//
int main()
{

    MyImg* img = load_jpeg_file("apple.jpg");
    if (img == 0)
    {
        return 1;
    }
    print_img_info(img);
    unsigned char* p = img->data;
    // Set 20% of the total_pixels to black
    for (int r = 0; r < img->height; r++) {
        for (int c = 0; c < img->width; c++) {
            if (rand() % 100 < 20) {
                set_pixel_red(img, r, c, 0);
                set_pixel_green(img, r, c, 0);
                set_pixel_blue(img, r, c, 0);
            }
        }
    }

    //save_to_jpeg_file("apple-noisy.jpg", img);
    //print_img_info(img);
    //stbi_image_free(img->data);
    MyImg* imgn = load_jpeg_file("apple-hh.jpg");
    if (imgn == 0) {
        cout << "Faild to load apple-hh.jpg" << endl;
        return 2;
    }
    print_img_info(imgn);

    MyImg* croppedImage = get_subregion(imgn, 100, 100, 200, 200);
    print_img_info(croppedImage);
    save_to_jpeg_file("apple-crop.jpg", croppedImage);

    // Memory cleanup
    stbi_image_free(croppedImage->data);
    delete_img(croppedImage);

    return 0;
}
