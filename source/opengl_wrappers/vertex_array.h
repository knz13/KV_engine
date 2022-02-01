#pragma once
#include "vertex_buffer.h"
#include "index_buffer.h"

class VertexArray {
public:
    VertexArray();


    VertexBuffer& SetVertexBuffer(unsigned int numberOfElementsPerVertex);
    VertexBuffer& SetVertexBuffer();
    IndexBuffer& SetIndexBuffer();

    VertexBuffer& GetVertexBuffer();
    IndexBuffer& GetIndexBuffer();

    void Bind();
    void Unbind();


protected:
    

private:
    std::unique_ptr<IndexBuffer> m_IB;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<unsigned int> m_ID;
;
};