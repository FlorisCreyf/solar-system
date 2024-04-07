#include "Buffer.h"
#include "AndroidOut.h"
#include <cassert>

namespace Solar {

    Buffer::Buffer() : vao(0), buffers{0, 0}
    {
    }

    Buffer::~Buffer()
    {
        clear();
    }

    void Buffer::add(Object &object)
    {
        auto vs = object.getVertices();
        auto is = object.getIndices();
        object.alloc.vertexStart = vertices.size();
        object.alloc.indexStart = indices.size();
        object.alloc.vertexCount = vs.size();
        object.alloc.indexCount = is.size();
        for (size_t i = 0; i < is.size(); i++)
            is[i] += vertices.size();
        vertices.insert(vertices.end(), vs.begin(), vs.end());
        indices.insert(indices.end(), is.begin(), is.end());
    }

    void Buffer::upload()
    {
        deleteVAO();
        glGenVertexArrays(1, &vao);
        glGenBuffers(2, buffers);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        GLsizei stride = sizeof(Vertex);
        GLvoid *ptr = (GLvoid *)(offsetof(Vertex, position));
        glVertexAttribPointer(0, 4, GL_FLOAT, false, stride, ptr);
        glEnableVertexAttribArray(0);
        ptr = (GLvoid *)(offsetof(Vertex, color));
        glVertexAttribPointer(1, 4, GL_FLOAT, false, stride, ptr);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Buffer::update(std::vector<Vertex> data, size_t index)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
        size_t offset = index * sizeof(Vertex);
        size_t size = data.size() * sizeof(Vertex);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
    }

    void Buffer::clear()
    {
        deleteVAO();
        vertices.clear();
        indices.clear();
    }

    void Buffer::deleteVAO()
    {
        if (vao) {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(2, buffers);
            vao = 0;
            buffers[0] = 0;
            buffers[1] = 0;
        }
    }

    void Buffer::bind() const
    {
        glBindVertexArray(vao);
    }

    void Buffer::unbind() const
    {
        glBindVertexArray(0);
    }

}