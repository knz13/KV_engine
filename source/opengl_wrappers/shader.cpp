#include "shader.h"

Shader::Shader() {
    


}



void Shader::Bind() {
    if(!m_ID){
        return;
    }
    GL_CALL(glUseProgram(*m_ID.get()));
}

void Shader::Unbind() {
    GL_CALL(glUseProgram(0));
}



bool Shader::SetShaders(std::unordered_map<ShaderType,std::vector<std::string>> sources) {
    

    for(auto& [type,sources] : sources){
        unsigned int shaderID = -1;
        
        std::string shaderTypeName;
        switch(type){
        case ShaderType::Vertex:
            shaderTypeName = "Vertex";
            break;
        case ShaderType::Fragment:
            shaderTypeName = "Fragment";
            break;
            
        }

        switch(type){
        case ShaderType::Vertex:
            GL_CALL(shaderID = glCreateShader(GL_VERTEX_SHADER));
            break;
        case ShaderType::Fragment:
            GL_CALL(shaderID = glCreateShader(GL_FRAGMENT_SHADER)); 
            break;
        }

        std::vector<GLchar const*> files(sources.size());
        std::vector<GLint> lengths(sources.size());

        int index = 0;
        for(auto& source : sources){
            files[index] = source.c_str();
            lengths[index] = source.size();

            index++;
        }

        GL_CALL(glShaderSource(shaderID,index,files.data(),lengths.data()));

        if(!CompileShader(shaderID,shaderTypeName)){
            return false;
        } 
        m_CompiledShadersCache.push_back(shaderID);
    }

    if(!LinkShader()){
        return false;
    };

    m_CompiledShadersCache.clear();

    return true;

}
bool Shader::CompileShader(unsigned int shaderID,std::string shaderTypeName) {
    GL_CALL(glCompileShader(shaderID));

    GLint success = 0;
    GL_CALL(glGetShaderiv(shaderID,GL_COMPILE_STATUS,&success));

    if(success == GL_FALSE){
        GLint logsize = 0;
        GL_CALL(glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&logsize));

        std::vector<GLchar> errorLog(logsize);
        GL_CALL(glGetShaderInfoLog(shaderID,logsize,&logsize,&errorLog[0]));

        std::string errorStr;
        for(auto& character : errorLog){
            errorStr += character;
        }

        LOG("Shader compilation error: " << errorStr << " at shader of type " + shaderTypeName);

        GL_CALL(glDeleteShader(shaderID));

        return false;

    }

    //TODO: test if changing the m_ID, it calls the destructor
    if(m_ID){
        GL_CALL(glDeleteProgram(*m_ID.get()));
        
    }
    if(!m_ID){
        auto deleter = [](unsigned int* id){
            GL_CALL(glDeleteProgram(*id));
        };

        m_ID = std::shared_ptr<unsigned int>(new unsigned int(0),deleter);
    }

    GL_CALL(*m_ID.get() = glCreateProgram());

    GL_CALL(glAttachShader(*m_ID.get(),shaderID));

    return true;

}

bool Shader::LinkShader() {
    GL_CALL(glLinkProgram(*m_ID.get()));

    int isLinked = 0;
    GL_CALL(glGetProgramiv(*m_ID.get(),GL_LINK_STATUS,&isLinked));

    if(isLinked == GL_FALSE){
        GLint maxLength = 0;
        glGetProgramiv(*m_ID.get(), GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(*m_ID.get(), maxLength, &maxLength, &infoLog[0]);
        
        std::string errorStr;
        for(auto& character : infoLog){
            errorStr += character;
        }

        LOG("Shader linking error: " << errorStr);
        
        m_ID.reset();
        
        
        for(auto id : m_CompiledShadersCache){
            GL_CALL(glDetachShader(*m_ID.get(),id));
            GL_CALL(glDeleteShader(id));
        }

        m_CompiledShadersCache.clear();
        
        return false;
        
    }

    return true;

}

ShaderCreationProperties& ShaderCreationProperties::AddShader(ShaderType type, std::string source) {
    
    if(m_Shaders.find(type) == m_Shaders.end()){
        m_Shaders[type] = {source};
        return *this;
    }

    m_Shaders[type].emplace_back(std::move(source));
    return *this;
}

ShaderCreationProperties Shader::CreateNew() {
    return ShaderCreationProperties(*this);
}   

bool ShaderCreationProperties::Generate() {
    return m_Master->SetShaders(m_Shaders);
}

ShaderCreationProperties::ShaderCreationProperties(Shader& master) {
    m_Master = &master;
}

bool Shader::SetUniform1f(const string& name, float value) {
    this->Bind();
    int location;
    GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));
    if(location == -1){
        this->Unbind();
        return false;
    }
    GL_CALL(glUniform1f(location,value));
    this->Unbind();
}

bool Shader::SetUniform1i(const string& name, int value) {
    this->Bind();
    int location;
    GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));
    if(location == -1){
        this->Unbind();
        return false;
    }
    GL_CALL(glUniform1i(location,value));
    this->Unbind();
}

bool Shader::SetUniform3f(const string& name, float v0, float v1, float v2) {
    this->Bind();
    int location;
    GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));
    if(location == -1){
        this->Unbind();
        return false;
    }
    GL_CALL(glUniform3f(location,v0,v1,v2));
    this->Unbind();
}

bool Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3) {
    this->Bind();
    int location;
    GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));
    if(location == -1){
        this->Unbind();
        return false;
    }
    GL_CALL(glUniform4f(location,v0,v1,v2,v3));
    this->Unbind();
}

bool Shader::SetUniformMat4f(const string& name, const glm::mat4& mat) {
    this->Bind();
    int location;
    GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));
    if(location == -1){
        this->Unbind();
        return false;
    }
    GL_CALL(glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(mat)));
    this->Unbind();
}