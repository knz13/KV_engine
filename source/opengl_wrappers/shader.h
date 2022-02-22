#pragma once
#include "../global.h"

class Shader;
struct ShaderCreationProperties {
    ShaderCreationProperties(Shader& master);

    ShaderCreationProperties& AddShader(ShaderType type,std::string source);
    void Generate();

private:
    Shader* m_Master;
    std::unordered_map<ShaderType,std::vector<std::string>> m_Shaders;
};


class Shader {
public:

    Shader();

    ShaderCreationProperties CreateNew();

    bool SetUniform1f(const string& name, float value);
	bool SetUniform1i(const string& name, int value);
	bool SetUniform3f(const string& name, float v0, float v1, float v2);
	bool SetUniform4f(const string& name, float v0, float v1, float v2, float v3);
	bool SetUniformMat4f(const string& name, const glm::mat4& mat);


    void Bind();
    void Unbind();

private:
    
    void SetShaders(std::unordered_map<ShaderType,std::vector<std::string>> source);
    bool CompileShader(unsigned int shaderID);
    bool LinkShader();



    std::vector<unsigned int> m_CompiledShadersCache;
    std::unique_ptr<unsigned int,std::function<void(unsigned int*)>> m_ID;

    friend struct ShaderCreationProperties;
};