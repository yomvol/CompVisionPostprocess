#pragma once

namespace gl_utils
{
class Framebuffer
{
private:
    unsigned int m_FBO_ID;
    unsigned int m_RBO_ID;
    unsigned int m_textureID;

public:
    Framebuffer(unsigned int textureID, unsigned int width, unsigned int height);
    virtual ~Framebuffer();
    void update(unsigned int textureID, const float width, const float height);
    void bind() const;
    void unbind() const;

};
}