#pragma once

#include <string>

class Texture
{
private:
    unsigned int m_textureID;
    std::string m_filePath;
    unsigned char* m_localBuff;
    int m_width, m_height, m_bitsPerPixel;

public:
    explicit Texture(const std::string& path, unsigned int wrap_param);
    ~Texture();
    void bind(unsigned int slot=0) const;
    void unBind();
};
