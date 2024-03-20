#pragma once
#include <string>
#include <map>
#include <exception>
namespace lk
{
namespace gfx
{


class RenderException : public std::exception
{
	public:
		RenderException ( const std::string & msg )
		: m_Msg ( msg )
		{};

		RenderException ( std::string && msg )
		: m_Msg ( std::move ( msg ) )
		{};

		const char * what() const noexcept override
		{
			return m_Msg.c_str();
		}
	private:
		std::string m_Msg;
};

struct ShaderAttributeData
{
	std::string Name;
	int Size;
	uint32_t Type;
	uint32_t Location;
};

class Shader
{
	public:
		Shader ( const std::string & VertexShader, const std::string & FragmentShader );
		~Shader ();

		// Delete copy and assignment constructors;
		Shader ( const Shader & ) = delete;
		Shader & operator = ( const Shader & ) = delete;
		uint32_t GetHandle() const;
		void Bind() const;
		void Unbind() const;
		bool GetIsUniformExists ( const std::string & UniformName ) const;
		uint32_t GetUniformLocation ( const std::string & UniformName ) const;
		bool GetIsAttributeExists ( const std::string & AttributeName ) const;
		uint32_t GetAttributeLocation ( const std::string & AttributeName ) const;



	private:
		void Load ( const std::string & VertexShader, const std::string & FragmentShader );
		void ReadShaders ( const std::string & VertexShader, const std::string & FragmentShader, std::string & OutVS, std::string & OutFS);
		uint32_t CompileVertexShader ( const std::string & VertexSource );
		uint32_t CompileFragmentShader ( const std::string & ShaderSource );
		void LinkShaders( uint32_t VSHandle, uint32_t FSHandle );
		void PopulateAttributes();
		void PopulateUniforms();

		uint32_t m_Handle;
		std::map<std::string, uint32_t> m_Attributes;
		std::map<std::string, uint32_t> m_Uniforms;
};

} // lk
} // gfx
