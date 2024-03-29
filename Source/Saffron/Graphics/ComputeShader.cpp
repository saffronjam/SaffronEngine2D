#include "SaffronPCH.h"

#include <glad/glad.h>

#include "Saffron/Core/Filesystem.h"
#include "Saffron/Graphics/ComputeShader.h"

#include <complex>

namespace Se
{
void ComputeShader::Bind()
{
	glUseProgram(_id);
}

void ComputeShader::Unbind()
{
	glUseProgram(0);
}

void ComputeShader::Dispatch(uint xGroup, uint yGroup, uint zGroup)
{
	BindThenUnbind([&]
	{
		glDispatchCompute(xGroup, yGroup, zGroup);
	});
}

bool ComputeShader::LoadFromFile(const std::filesystem::path& filepath)
{
	Debug::Assert(Filesystem::FileExists(filepath), "{} did not exist", filepath.string());

	std::string content;
	std::fstream fileStream(filepath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filepath << ". File does not exist." << std::endl;
	}

	std::string line;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	const auto* contentCStr = content.c_str();
	const auto shader = glCreateShader(GL_COMPUTE_SHADER);
	if (shader < 1)
	{
		return false;
	}

	glShaderSource(shader, 1, &contentCStr, nullptr);
	// Try compile
	{
		glCompileShader(shader);
		int status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == 0)
		{
			return false;
		}
	}

	_id = glCreateProgram();
	if (_id < 1)
	{
		return false;
	}
	glAttachShader(_id, shader);
	// Try link
	{
		glLinkProgram(_id);
		int status;
		glGetShaderiv(shader, GL_LINK_STATUS, &status);
		if (status == 0)
		{
			_id = 0;
			return false;
		}
	}

	return true;
}

void ComputeShader::SetFloat(const std::string& name, float value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		Debug::Assert(loc != -1);
		glUniform1f(loc, value);
	});
}

void ComputeShader::SetDouble(const std::string& name, double value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		Debug::Assert(loc != -1);
		glUniform1d(loc, value);
	});
}

void ComputeShader::SetInt(const std::string& name, int value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		Debug::Assert(loc != -1);
		glUniform1i(loc, value);
	});
}

void ComputeShader::SetVector2d(const std::string& name, const sf::Vector2<double>& value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		Debug::Assert(loc != -1);
		glUniform2d(loc, value.x, value.y);
	});
}

void ComputeShader::SetVector2d(const std::string& name, const std::complex<double>& value)
{
	SetVector2d(name, sf::Vector2<double>(value.real(), value.imag()));
}

void ComputeShader::SetVector4f(const std::string& name, const sf::Vector4f& value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		Debug::Assert(loc != -1);
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	});
}

void ComputeShader::AwaitFinish()
{
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::BindThenUnbind(const std::function<void()>& fn)
{
	Bind();
	fn();
	Unbind();
}
}
