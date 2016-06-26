#include "ogl.h"

#include "helper.h"

#include <FreeImage.h>
#include <fstream>
#include <stdexcept>

Shader::Shader(const std::string &shader_filepath,
               GLenum shader_type) :
    shader_id(glCreateShader(shader_type))
{
    CHECK_GL_ERRORS;

    std::string shader_code;
    std::ifstream shader_stream(shader_filepath, std::ios::in);

    if(shader_stream.is_open()){
		std::string line = "";
		while (getline(shader_stream, line)) {
			shader_code += line + '\n';
        }
		shader_stream.close();
	} else {
		std::string error = "Cannot open shader file: " + shader_filepath;
		throw std::runtime_error(error);
	}

    // Compile shader
	char const *source_pointer = shader_code.c_str();
	glShaderSource(this->get_value(), 1, &source_pointer , NULL); CHECK_GL_ERRORS;
	glCompileShader(this->get_value()); CHECK_GL_ERRORS;

    // Check shader
    GLint compile_res;
	glGetShaderiv(this->get_value(), GL_COMPILE_STATUS, &compile_res); CHECK_GL_ERRORS;
	GLsizei error_length;
	glGetShaderiv(this->get_value(), GL_INFO_LOG_LENGTH, &error_length); CHECK_GL_ERRORS;
	if (!compile_res && error_length > 0) {
		char *shader_error_message = new char[error_length];
		glGetShaderInfoLog(this->get_value(), error_length, NULL, shader_error_message); CHECK_GL_ERRORS;
        std::string error = shader_error_message;
        delete[] shader_error_message;
        switch (shader_type) {
            case GL_VERTEX_SHADER:
                throw std::runtime_error("Vertex shader error: " + error);
            case GL_FRAGMENT_SHADER:
                throw std::runtime_error("Fragment shader error: " + error);
		    default:
                throw std::runtime_error("Shader error: " + error);
	    }
    }
}

Shader::~Shader()
{
    glDeleteShader(this->get_value());
}

GLuint
Shader::get_value() const
{
    return this->shader_id;
}

ShaderProgram::ShaderProgram(const std::string &vertex_filepath,
                             const std::string &fragment_filepath) :
    program_id(glCreateProgram())
{
    CHECK_GL_ERRORS;
	// Create the shaders
	auto vertex_shader = Shader(vertex_filepath, GL_VERTEX_SHADER);
	auto fragment_shader = Shader(fragment_filepath, GL_FRAGMENT_SHADER);

	// Link the program
	glAttachShader(this->get_value(), vertex_shader.get_value()); CHECK_GL_ERRORS;
	glAttachShader(this->get_value(), fragment_shader.get_value()); CHECK_GL_ERRORS;
	glLinkProgram(this->get_value()); CHECK_GL_ERRORS;

	// Check the program
    GLint link_res;
	glGetProgramiv(this->get_value(), GL_LINK_STATUS, &link_res); CHECK_GL_ERRORS;
    GLsizei error_length;
	glGetProgramiv(this->get_value(), GL_INFO_LOG_LENGTH, &error_length); CHECK_GL_ERRORS;
	if (!link_res && error_length > 0) {
		char *program_error_message = new char[error_length];
		glGetProgramInfoLog(this->get_value(), error_length, NULL, program_error_message);
        std::string error = program_error_message;
        delete[] program_error_message;
		throw std::runtime_error("Shader link error: " + error);
	}

	glDetachShader(this->get_value(), vertex_shader.get_value());
	glDetachShader(this->get_value(), fragment_shader.get_value());
}

ShaderProgram::ShaderProgram(const std::string &fragment_filepath) :
    ShaderProgram("data/shaders/static_vertex.glsl", fragment_filepath)
{
}

GLuint
ShaderProgram::get_value() const
{
    return this->program_id;
}

void
ShaderProgram::set_uniform(const std::string &name, GLfloat value)
{
    this->setup();
    GLint location = glGetUniformLocation(this->get_value(), name.c_str()); CHECK_GL_ERRORS;
    glUniform1f(location, value); CHECK_GL_ERRORS;
}

void
ShaderProgram::set_uniform(const std::string &name, const vec2 &value)
{
    this->setup();
    GLint location = glGetUniformLocation(this->get_value(), name.c_str()); CHECK_GL_ERRORS;
    this->uniforms.push_back(location);
    glUniform2f(location, value.x, value.y); CHECK_GL_ERRORS;
}

void
ShaderProgram::set_uniform(const std::string &name, const vec3 &value)
{
    this->setup();
    GLint location = glGetUniformLocation(this->get_value(), name.c_str()); CHECK_GL_ERRORS;
    this->uniforms.push_back(location);
    glUniform3f(location, value.x, value.y, value.z); CHECK_GL_ERRORS;
}

void
ShaderProgram::set_uniform(const std::string &name,
                           std::shared_ptr<Texture> &texture)
{
    this->setup();
    GLenum tid = this->texture_id;
    this->textures.push_back(std::pair<GLenum, std::shared_ptr<Texture>>(tid, texture));
    ++ this->texture_id;

    glActiveTexture(GL_TEXTURE0 + tid); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, texture->get_value()); CHECK_GL_ERRORS;
    GLint location = glGetUniformLocation(this->get_value(), name.c_str()); CHECK_GL_ERRORS;
    this->uniforms.push_back(location);
    glUniform1i(location, tid); CHECK_GL_ERRORS;
}

void
ShaderProgram::set_attrib(const std::string &name,
                          std::shared_ptr<VBO<vec3>> &vbo)
{
    static ssize_t stride = sizeof(vec3) - 3 * sizeof(GLfloat);

    this->setup();
    GLint location = glGetAttribLocation(this->get_value(), name.c_str()); CHECK_GL_ERRORS;
    this->attributes.push_back(location);
    glEnableVertexAttribArray(location); CHECK_GL_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, vbo->get_value()); CHECK_GL_ERRORS;
    glVertexAttribPointer(location,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          stride,
                          nullptr); CHECK_GL_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, 0); CHECK_GL_ERRORS;
}

void
ShaderProgram::setup()
{
    Context::get_singleton()->set_pid(this->get_value());
}

void
ShaderProgram::shutdown()
{
    for (const auto &location : this->attributes) {
        glDisableVertexAttribArray(location); CHECK_GL_ERRORS;
    }
    this->attributes.clear();
    this->uniforms.clear();
    glBindBuffer(GL_ARRAY_BUFFER, 0); CHECK_GL_ERRORS;
    Context::get_singleton()->set_pid(0);
}

ShaderProgram::~ShaderProgram()
{
    this->shutdown();
    glDeleteProgram(this->get_value()); CHECK_GL_ERRORS;
}

Texture::Texture(const std::string &filename)
{
    FIBITMAP* bitmap = FreeImage_Load(
        FreeImage_GetFileType(filename.c_str(), 0),
        filename.c_str());

    glGenTextures(1, &(this->texture_id));

    glBindTexture(GL_TEXTURE_2D, this->get_value());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
    int nWidth = FreeImage_GetWidth(pImage);
    int nHeight = FreeImage_GetHeight(pImage);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
        0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(pImage)); CHECK_GL_ERRORS;
    // glGenerateMipmap(GL_TEXTURE_2D); CHECK_GL_ERRORS;

    FreeImage_Unload(pImage);
    FreeImage_Unload(bitmap);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &(this->texture_id)); CHECK_GL_ERRORS;
}

GLuint
Texture::get_value() const
{
    return this->texture_id;
}
