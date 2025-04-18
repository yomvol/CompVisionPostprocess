#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
        glEnableVertexAttribArray(i);
    }
}

void VertexArray::bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
