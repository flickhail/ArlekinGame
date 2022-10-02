#pragma once
#include <string>

namespace GameEngine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		void Use() const;

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat4(const std::string& name, float val1, float val2, float val3, float val4) const;
		void SetMat4f(const std::string& name, const float* matrix) const;

		unsigned int GetID() const;

	private:
		unsigned int ID;
	};
}
