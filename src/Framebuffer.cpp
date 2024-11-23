#include "Framebuffer.h"
#include <GL/glew.h>
#include <iostream>

Framebuffer::Framebuffer(unsigned int textureID, unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
	m_TextureID = textureID;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	glGenRenderbuffers(1, &m_RBO_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n" << status << std::endl;

    Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_FBO_ID);
	glDeleteTextures(1, &m_TextureID);
	glDeleteRenderbuffers(1, &m_RBO_ID);
}

void Framebuffer::Update(unsigned int textureID, const float width, const float height)
{
    glDeleteTextures(1, &m_TextureID);
    m_TextureID = textureID;

    Bind();
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);

	Unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);

	// I'm not using render buffer right now
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
