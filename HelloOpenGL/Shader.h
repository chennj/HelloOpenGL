#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string _FilePath;
	unsigned int _RendererID;
	// caching for uniforms 
	std::unordered_map<std::string, unsigned int> _UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

public:
	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniform1f(const std::string& name, float value);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};

#endif
