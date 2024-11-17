#pragma once

class Framebuffer
{
private:
    unsigned int m_FBO_ID;
    unsigned int m_RBO_ID;
    unsigned int m_TextureID;


public:
    Framebuffer(unsigned int textureID, unsigned int width, unsigned int height);
    virtual ~Framebuffer();
    void Update(unsigned int textureID, const float width, const float height);
    void Bind() const;
    void Unbind() const;

};