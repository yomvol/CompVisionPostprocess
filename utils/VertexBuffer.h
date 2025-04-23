#pragma once

namespace gl_utils
{

class VertexBuffer
{
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    virtual ~VertexBuffer();

    void bind() const;
    void unbind() const;
};
}