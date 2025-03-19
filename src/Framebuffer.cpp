#include "Framebuffer.h"
#include <GL/glew.h>
#include <iostream>

Framebuffer::Framebuffer(unsigned int textureID, unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);
	m_textureID = textureID;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

	glGenRenderbuffers(1, &m_RBO_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n" << status << std::endl;

    unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_FBO_ID);
	glDeleteTextures(1, &m_textureID);
	glDeleteRenderbuffers(1, &m_RBO_ID);
}

void Framebuffer::update(unsigned int textureID, const float width, const float height)
{
    glDeleteTextures(1, &m_textureID);
    m_textureID = textureID;

    bind();
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO_ID);

	unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO_ID);

	// I'm not using render buffer right now
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
