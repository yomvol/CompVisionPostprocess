#pragma once
#include <vector>
#include <GL/glew.h>

namespace gl_utils
{
struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }

        #ifdef MSVC
        _ASSERT(false);
        #endif
        return 0;
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {}

    template<typename T>
    void Push(unsigned int count);

    inline unsigned int GetStride() const { return m_Stride; }
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

};

template<typename T>
struct always_false : std::false_type {};

template<typename T>
inline void VertexBufferLayout::Push(unsigned int count)
{
    static_assert(always_false<T>::value, "Unsupported type");
}

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * sizeof(GLfloat);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * sizeof(GLuint);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * sizeof(GLubyte);
}
}
