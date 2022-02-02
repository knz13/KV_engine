#pragma once
#include "../global.h"


class Shader {
public:

    Shader();

    
    void SetShader(std::unordered_map<ShaderType,std::vector<std::string>> source);
    void Bind();
    void Unbind();

private:
    
    bool CompileShader(unsigned int shaderID);
    bool LinkShader();


    std::vector<unsigned int> m_CompiledShadersCache;
    std::unique_ptr<unsigned int,std::function<void(unsigned int*)>> m_ID;
};