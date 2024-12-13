#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\stb_image.h"
#include <gl/glew.h>
#include <iostream>

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::load(const std::string& filename)
{
    stbi_set_flip_vertically_on_load(true);
    int channels = 0; 
    unsigned char* image = stbi_load(filename.c_str(), &mWidth, &mHeight, &channels, 0);

    if (image == nullptr) {
        std::cerr << "stbi_load failed to load image " << filename.c_str() << '\n';
        return false;
    }

    int format = (channels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << "stbi_load successed to load image " << filename.c_str() << '\n';
    return true;
}

void Texture::unload()
{
    glDeleteTextures(1, &mTextureID);
}

void Texture::setActive()
{
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
