#include "vertex_array.h"

VertexArray::VertexArray(){
    auto deleter = [](unsigned int* id){
        GL_CALL(glDeleteVertexArrays(1,id));
    };
    m_ID = std::shared_ptr<unsigned int>(new unsigned int,deleter);
    GL_CALL(glGenVertexArrays(1,m_ID.get()));
}
VertexBuffer& VertexArray::SetVertexBuffer(unsigned int numberOfElementsPerVertex) {
    m_VBO = std::make_unique<VertexBuffer>(*this);
    m_VBO.get()->CreateNew(numberOfElementsPerVertex);
    return *m_VBO.get();
}

VertexBuffer& VertexArray::SetVertexBuffer() {
    m_VBO = std::make_unique<VertexBuffer>(*this);
    return *m_VBO.get();
}

void VertexArray::Bind() {
    GL_CALL(glBindVertexArray(*m_ID.get()));
    if(m_VBO){
        m_VBO.get()->Bind();
    }
    if(m_IB){
        m_IB.get()->Bind();
    }
}

void VertexArray::Unbind() {
    GL_CALL(glBindVertexArray(0));
    if(m_VBO){
        m_VBO.get()->Unbind();
    }
    if(m_IB){
        m_IB.get()->Unbind();
    }
}

IndexBuffer& VertexArray::SetIndexBuffer() {
    m_IB = std::make_unique<IndexBuffer>(*this);
    return *m_IB.get();
}

IndexBuffer& VertexArray::GetIndexBuffer() {
    if(!m_IB){
        DEBUG_LOG("Calling VertexArray::GetIndexBuffer without any buffer attached, returning new one...");
        return this->SetIndexBuffer();
    }
    return *m_IB.get();
}

VertexBuffer& VertexArray::GetVertexBuffer() {
    if(!m_VBO){
        DEBUG_LOG("Calling VertexArray::GetVertexBuffer without any buffer attached, returning new one...");
        return this->SetVertexBuffer();
    }
    return *m_VBO.get();
}
