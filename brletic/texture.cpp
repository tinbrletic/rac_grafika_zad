#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include "GL/glew.h"

Texture::Texture(const std::string& path, unsigned int wrap_param)
    : m_textureID(0),
      m_filePath(path),
      m_localBuff(nullptr),
      m_width(0), m_height(0),
      m_bitsPerPixel(0)
{
    stbi_set_flip_vertically_on_load(true);
    m_localBuff = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsPerPixel, 4);

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param /*GL_CLAMP_TO_EDGE*/);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param /*GL_CLAMP_TO_EDGE*/);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, m_localBuff);

    glGenerateMipmap(GL_TEXTURE_2D);
    unBind();
    if(m_localBuff)
        stbi_image_free(m_localBuff);
}
Texture::~Texture()
{

}
void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}
void Texture::unBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
