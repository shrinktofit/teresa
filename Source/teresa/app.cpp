
#include "app.h"
#include <glad/glad.h>
#include <iostream>
#include <variant>

namespace webgl
{
    using Float32List = typed_array<float>;

    using Int32List = typed_array<std::int32_t>;

    // The power preference settings are documented in the WebGLContextAttributes
    // section of the specification.
    enum PowerPreference { defaulted, low_power, high_performance };

    struct ContextAttributes
        :public node_compatible
    {
        bool alpha = true;
        bool depth = true;
        bool stencil = false;
        bool antialias = true;
        bool premultipliedAlpha = true;
        bool preserveDrawingBuffer = false;
        PowerPreference powerPreference = PowerPreference::defaulted;
        bool failIfMajorPerformanceCaveat = false;
    };

    struct Object
        :public node_compatible
    {
    public:
        GLuint gl_handle;

        Object(GLuint gl_handle_)
            :gl_handle(gl_handle_)
        {

        }

        void to_node(napi_env env_, napi_value object_) const
        {
            node_compatible::to_node(env_, object_);
            set_node_property(env_, object_, u8"gl_handle_", gl_handle);
        }
    };

    struct Buffer
        :public Object
    {
    public:
        using Object::Object;
    };
    std::list<node_ptr<Buffer>> buffers;

    struct Framebuffer
        :public Object
    {
        using Object::Object;
    };
    std::list<node_ptr<Framebuffer>> framebuffers;

    struct Program
        :public Object
    {
    public:
        using Object::Object;
    };
    std::list<std::unique_ptr<Program>> programs;

    struct Renderbuffer
        :public Object
    {
        using Object::Object;
    };
    std::list<node_ptr<Renderbuffer>> renderbuffers;

    struct Shader
        :public Object
    {
    public:
        using Object::Object;
    };
    std::list<node_ptr<Shader>> shaders;

    struct Texture
        :public Object
    {
    public:
        using Object::Object;
    };
    std::list<node_ptr<Texture>> textures;

    struct UniformLocation
        :public node_compatible
    {
    public:
        UniformLocation(GLint gl_location_)
            :gl_location(gl_location_)
        {

        }

        GLint gl_location;
    };

    struct ActiveInfo
        :public node_compatible
    {
        GLint size;
        GLenum type;
        std::string name;
    };

    struct ShaderPrecisionFormat
        :public node_compatible
    {
        GLint rangeMin;
        GLint rangeMax;
        GLint precision;
    };

    node_ptr<ContextAttributes> getContextAttributes()
    {
        return make_node_ptr<ContextAttributes>();
    }

    bool isContextLost()
    {
        return false;
    }

    std::vector<std::string> getSupportedExtensions()
    {
        return {};
    }

    napi_value getExtension(std::string name)
    {
        return nullptr;
    }

    void activeTexture(GLenum texture)
    {
        glActiveTexture(texture);
    }

    void attachShader(node_ptr<Program> program, node_ptr<Shader> shader)
    {
        glAttachShader(program->gl_handle, shader->gl_handle);
    }

    void bindAttribLocation(node_ptr<Program> program, GLuint index, std::string name)
    {
        glBindAttribLocation(program->gl_handle, index, name.c_str());
    }

    void bindBuffer(GLenum target, node_ptr<Buffer> buffer)
    {
        glBindBuffer(target, buffer->gl_handle);
    }

    void bindFramebuffer(GLenum target, node_ptr<Framebuffer> framebuffer)
    {
        glBindFramebuffer(target, framebuffer->gl_handle);
    }

    void bindRenderbuffer(GLenum target, node_ptr<Renderbuffer> renderbuffer)
    {
        glBindRenderbuffer(target, renderbuffer->gl_handle);
    }

    void bindTexture(GLenum target, node_ptr<Texture> texture)
    {
        glBindTexture(target, texture->gl_handle);
    }

    void blendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        glBlendColor(red, green, blue, alpha);
    }

    void blendEquation(GLenum mode)
    {
        glBlendEquation(mode);
    }

    void blendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        glBlendEquationSeparate(modeRGB, modeAlpha);
    }

    void blendFunc(GLenum sfactor, GLenum dfactor)
    {
        glBlendFunc(sfactor, dfactor);
    }

    void blendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
    {
        glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }

    void bufferData(GLenum target, data_view data, GLenum usage)
    {
        glBufferData(target, data.size, data.data, usage);
    }

    void bufferSubData(GLenum target, GLintptr offset, data_view data)
    {
        glBufferSubData(target, offset, data.size, data.data);
    }

    GLenum checkFramebufferStatus(GLenum target)
    {
        return glCheckFramebufferStatus(target);
    }

    void clear(GLbitfield mask)
    {
        glClear(mask);
    }

    void clearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

    void clearDepth(GLclampf depth)
    {
        glClearDepth(depth);
    }

    void clearStencil(GLint s)
    {
        glClearStencil(s);
    }

    void colorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        glColorMask(red, green, blue, alpha);
    }

    void compileShader(node_ptr<Shader> shader)
    {
        glCompileShader(shader->gl_handle);
    }

    void compressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, data_view data)
    {
        glCompressedTexImage2D(target, level, internalformat, width, height, border, data.size, data.data);
    }

    void compressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, data_view data)
    {
        glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, data.size, data.data);
    }

    void copyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }

    void copyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }

    node_ptr<Buffer> createBuffer()
    {
        GLuint h;
        glCreateBuffers(1, &h);
        auto result = make_node_ptr<Buffer>(h);
        return result;
    }

    node_ptr<Framebuffer> createFramebuffer()
    {
        GLuint h;
        glCreateFramebuffers(1, &h);
        auto result = make_node_ptr<Framebuffer>(h);
        return result;
    }

    node_ptr<Program> createProgram()
    {
        GLuint h = glCreateProgram();
        auto result = make_node_ptr<Program>(h);
        return result;
    }

    node_ptr<Renderbuffer> createRenderbuffer()
    {
        GLuint h;
        glCreateRenderbuffers(1, &h);
        auto result = make_node_ptr<Renderbuffer>(h);
        return result;
    }

    node_ptr<Shader> createShader(GLenum type)
    {
        GLuint h = glCreateShader(type);
        auto result = make_node_ptr<Shader>(h);
        shaders.push_back(result);
        return result;
    }

    node_ptr<Texture> createTexture()
    {
        GLuint h;
        glCreateTextures(GL_TEXTURE_2D, 1, &h);
        auto result = make_node_ptr<Texture>(h);
        return result;
    }

    void cullFace(GLenum mode)
    {
        return glCullFace(mode);
    }

    void deleteBuffer(node_ptr<Buffer> buffer)
    {
        glDeleteBuffers(1, &buffer->gl_handle);
    }

    void deleteFramebuffer(node_ptr<Framebuffer> framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer->gl_handle);
    }

    void deleteProgram(node_ptr<Program> program)
    {
        glDeleteProgram(program->gl_handle);
    }

    void deleteRenderbuffer(node_ptr<Renderbuffer> renderbuffer)
    {
        glDeleteRenderbuffers(1, &renderbuffer->gl_handle);
    }

    void deleteShader(node_ptr<Shader> shader)
    {
        glDeleteShader(shader->gl_handle);
    }

    void deleteTexture(node_ptr<Texture> texture)
    {
        glDeleteTextures(1, &texture->gl_handle);
    }

    void depthFunc(GLenum func)
    {
        glDepthFunc(func);
    }

    void depthMask(GLboolean flag)
    {
        glDepthMask(flag);
    }

    void depthRange(GLclampf zNear, GLclampf zFar)
    {
        glDepthRange(zNear, zFar);
    }

    void detachShader(node_ptr<Program> program, node_ptr<Shader> shader)
    {
        glDetachShader(program->gl_handle, shader->gl_handle);
    }

    void disable(GLenum cap)
    {
        glDisable(cap);
    }

    void disableVertexAttribArray(GLuint index)
    {
        glDisableVertexAttribArray(index);
    }

    void drawArrays(GLenum mode, GLint first, GLsizei count)
    {
        glDrawArrays(mode, first, count);
    }

    void drawElements(GLenum mode, GLsizei count, GLenum type, GLintptr offset)
    {
        glDrawElements(mode, count, type, reinterpret_cast<const void*>(offset));
    }

    void enable(GLenum cap)
    {
        glEnable(cap);
    }

    void enableVertexAttribArray(GLuint index)
    {
        glEnableVertexAttribArray(index);
    }

    void finish()
    {
        glFinish();
    }

    void flush()
    {
        glFlush();
    }

    void framebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, node_ptr<Renderbuffer> renderbuffer)
    {
        glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer->gl_handle);
    }

    void framebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, node_ptr<Texture> texture, GLint level)
    {
        glFramebufferTexture2D(target, attachment, textarget, texture->gl_handle, level);
    }

    void frontFace(GLenum mode)
    {
        glFrontFace(mode);
    }

    void generateMipmap(GLenum target)
    {
        glGenerateMipmap(target);
    }

    node_ptr<ActiveInfo> getActiveAttrib(node_ptr<Program> program, GLuint index)
    {
        GLint len = 0;
        glGetProgramiv(program->gl_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
        std::vector<char> buffer(len);

        ActiveInfo activeInfo;
        glGetActiveAttrib(program->gl_handle, index, buffer.size(), &len, &activeInfo.size, &activeInfo.type, buffer.data());
        activeInfo.name = std::string(buffer.begin(), buffer.end());
        return make_node_ptr<ActiveInfo>(activeInfo);
    }

    node_ptr<ActiveInfo> getActiveUniform(node_ptr<Program> program, GLuint index)
    {
        GLint len = 0;
        glGetProgramiv(program->gl_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &len);
        std::vector<char> buffer(len);

        ActiveInfo activeInfo;
        glGetActiveUniform(program->gl_handle, index, buffer.size(), &len, &activeInfo.size, &activeInfo.type, buffer.data());
        activeInfo.name = std::string(buffer.begin(), buffer.end());
        return make_node_ptr<ActiveInfo>(activeInfo);
    }

    std::vector<node_ptr<Shader>> getAttachedShaders(node_ptr<Program> program)
    {
        GLsizei len = 0;
        glGetProgramiv(program->gl_handle, GL_ATTACHED_SHADERS, &len);
        std::vector<GLuint> buffer(len);

        glGetAttachedShaders(program->gl_handle, buffer.size(), &len, buffer.data());
        std::vector<node_ptr<Shader>> result;
        for (auto h : buffer) {
            auto r = std::find_if(shaders.begin(), shaders.end(),
                [h](auto &shader) { return shader->gl_handle == h; });
            if (r != shaders.end()) {
                result.push_back(*r);
            }
        }
        return result;
    }

    GLint getAttribLocation(node_ptr<Program> program, std::string name)
    {
        return glGetAttribLocation(program->gl_handle, name.c_str());
    }

    GLint getBufferParameter(GLenum target, GLenum pname)
    {
        GLint result;
        glGetBufferParameteriv(target, pname, &result);
        return result;
    }

    using GetParameterResult = std::variant<
    >;

    GLint getParameter(GLenum pname)
    {
        return 0;
    }

    GLenum getError()
    {
        return glGetError();
    }

    using GetFramebufferAttachmentParameterResult = std::variant<
        GLint,
        GLenum,
        node_ptr<Renderbuffer>,
        node_ptr<Texture>
    >;

    GLint getFramebufferAttachmentParameter(GLenum target, GLenum attachment, GLenum pname)
    {
        GLint result;
        glGetFramebufferAttachmentParameteriv(target, attachment, pname, &result);
        return result;
    }

    GLint getProgramParameter(node_ptr<Program> program, GLenum pname)
    {
        GLint result;
        glGetProgramiv(program->gl_handle, pname, &result);
        return result;
    }

    std::string getProgramInfoLog(node_ptr<Program> program)
    {
        GLsizei len = 0;
        glGetProgramiv(program->gl_handle, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> buffer(len);

        glGetProgramInfoLog(program->gl_handle, buffer.size(), &len, buffer.data());
        return std::string(buffer.begin(), buffer.end());
    }

    GLint getRenderbufferParameter(GLenum target, GLenum pname)
    {
        GLint result;
        glGetRenderbufferParameteriv(target, pname, &result);
        return result;
    }

    GLint getShaderParameter(node_ptr<Shader> shader, GLenum pname)
    {
        GLint result;
        glGetShaderiv(shader->gl_handle, pname, &result);
        return result;
    }

    node_ptr<ShaderPrecisionFormat> getShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype)
    {
        ShaderPrecisionFormat shaderPrecisionFormat;
        GLint range[2] = {};
        glGetShaderPrecisionFormat(shadertype, precisiontype, range, &shaderPrecisionFormat.precision);
        shaderPrecisionFormat.rangeMin = range[0];
        shaderPrecisionFormat.rangeMax = range[1];
        return make_node_ptr<ShaderPrecisionFormat>(shaderPrecisionFormat);
    }

    std::string getShaderInfoLog(node_ptr<Shader> shader)
    {
        GLint len = 0;
        glGetShaderiv(shader->gl_handle, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> buffer(len);

        glGetShaderInfoLog(shader->gl_handle, buffer.size(), &len, buffer.data());
        return std::string(buffer.begin(), buffer.end());
    }

    std::string getShaderSource(node_ptr<Shader> shader)
    {
        GLint len = 0;
        glGetShaderiv(shader->gl_handle, GL_SHADER_SOURCE_LENGTH, &len);
        std::vector<char> buffer(len);

        glGetShaderSource(shader->gl_handle, buffer.size(), &len, buffer.data());
        return std::string(buffer.begin(), buffer.end());
    }

    GLint getTexParameter(GLenum target, GLenum pname)
    {
        GLint result;
        glGetTexParameteriv(target, pname, &result);
        return result;
    }

    GLint getUniform(node_ptr<Program> program, node_ptr<UniformLocation> location)
    {
        GLint result;
        glGetUniformiv(program->gl_handle, location->gl_location, &result);
        return result;
    }

    node_ptr<UniformLocation> getUniformLocation(node_ptr<Program> program, std::string name)
    {
        auto l = glGetUniformLocation(program->gl_handle, name.c_str());
        return make_node_ptr<UniformLocation>(l);
    }

    GLint getVertexAttrib(GLuint index, GLenum pname)
    {
        GLint result;
        glGetVertexAttribiv(index, pname, &result);
        return result;
    }

    GLintptr getVertexAttribOffset(GLuint index, GLenum pname)
    {
        throw std::runtime_error("No impl");
    }

    void hint(GLenum target, GLenum mode)
    {
        glHint(target, mode);
    }

    GLboolean isBuffer(node_ptr<Buffer> buffer)
    {
        return glIsBuffer(buffer->gl_handle);
    }

    GLboolean isEnabled(GLenum cap)
    {
        return glIsEnabled(cap);
    }

    GLboolean isFramebuffer(node_ptr<Framebuffer> framebuffer)
    {
        return glIsFramebuffer(framebuffer->gl_handle);
    }

    GLboolean isProgram(node_ptr<Program> program)
    {
        return glIsProgram(program->gl_handle);
    }

    GLboolean isRenderbuffer(node_ptr<Renderbuffer> renderbuffer)
    {
        return glIsRenderbuffer(renderbuffer->gl_handle);
    }

    GLboolean isShader(node_ptr<Shader> shader)
    {
        return glIsShader(shader->gl_handle);
    }

    GLboolean isTexture(node_ptr<Texture> texture)
    {
        return glIsTexture(texture->gl_handle);
    }

    void lineWidth(GLfloat width)
    {
        glLineWidth(width);
    }

    void linkProgram(node_ptr<Program> program)
    {
        glLinkProgram(program->gl_handle);
    }

    void pixelStorei(GLenum pname, GLint param)
    {
        glPixelStorei(pname, param);
    }

    void polygonOffset(GLfloat factor, GLfloat units)
    {
        glPolygonOffset(factor, units);
    }

    void readPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, data_view pixels)
    {
        glReadPixels(x, y, width, height, format, type, pixels.data);
    }

    void renderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        glRenderbufferStorage(target, internalformat, width, height);
    }

    void sampleCoverage(GLclampf value, GLboolean invert)
    {
        glSampleCoverage(value, invert);
    }

    void scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        glScissor(x, y, width, height);
    }

    void shaderSource(node_ptr<Shader> shader, std::string source)
    {
        static const std::string header = "#version 330 core\n";
        static const std::string footer = "";
        const char *parts[] = { header.c_str(), source.c_str(), footer.c_str() };
        GLint partLengths[] = { header.size(), source.size(), footer.size() };
        auto p = source.c_str();
        GLint c = source.size();
        glShaderSource(shader->gl_handle, 3, parts, partLengths);
    }

    void stencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        glStencilFunc(func, ref, mask);
    }

    void stencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        glStencilFuncSeparate(face, func, ref, mask);
    }

    void stencilMask(GLuint mask)
    {
        glStencilMask(mask);
    }

    void stencilMaskSeparate(GLenum face, GLuint mask)
    {
        glStencilMaskSeparate(face, mask);
    }

    void stencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        glStencilOp(fail, zfail, zpass);
    }

    void stencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
    {
        glStencilOpSeparate(face, fail, zfail, zpass);
    }

    void texImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, data_view pixels)
    {
        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels.data);
    }

    void texParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        glTexParameterf(target, pname, param);
    }

    void texParameteri(GLenum target, GLenum pname, GLint param)
    {
        glTexParameteri(target, pname, param);
    }

    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, data_view pixels)
    {
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels.data);
    }

    void uniform1f(node_ptr<UniformLocation> location, GLfloat x)
    {
        glUniform1f(location->gl_location, x);
    }

    void uniform2f(node_ptr<UniformLocation> location, GLfloat x, GLfloat y)
    {
        glUniform2f(location->gl_location, x, y);
    }

    void uniform3f(node_ptr<UniformLocation> location, GLfloat x, GLfloat y, GLfloat z)
    {
        glUniform3f(location->gl_location, x, y, z);
    }

    void uniform4f(node_ptr<UniformLocation> location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        glUniform4f(location->gl_location, x, y, z, w);
    }

    void uniform1i(node_ptr<UniformLocation> location, GLint x)
    {
        glUniform1i(location->gl_location, x);
    }
    
    void uniform2i(node_ptr<UniformLocation> location, GLint x, GLint y)
    {
        glUniform2i(location->gl_location, x, y);
    }

    void uniform3i(node_ptr<UniformLocation> location, GLint x, GLint y, GLint z)
    {
        glUniform3i(location->gl_location, x, y, z);
    }

    void uniform4i(node_ptr<UniformLocation> location, GLint x, GLint y, GLint z, GLint w)
    {
        glUniform4i(location->gl_location, x, y, z, w);
    }

    void uniform1fv(node_ptr<UniformLocation> location, Float32List v)
    {
        glUniform1fv(location->gl_location, 1, v.data);
    }

    void uniform2fv(node_ptr<UniformLocation> location, Float32List v)
    {
        glUniform2fv(location->gl_location, 1, v.data);
    }

    void uniform3fv(node_ptr<UniformLocation> location, Float32List v)
    {
        glUniform3fv(location->gl_location, 1, v.data);
    }

    void uniform4fv(node_ptr<UniformLocation> location, Float32List v)
    {
        glUniform4fv(location->gl_location, 1, v.data);
    }

    void uniform1iv(node_ptr<UniformLocation> location, Int32List v)
    {
        glUniform1iv(location->gl_location, 1, v.data);
    }

    void uniform2iv(node_ptr<UniformLocation> location, Int32List v)
    {
        glUniform2iv(location->gl_location, 1, v.data);
    }

    void uniform3iv(node_ptr<UniformLocation> location, Int32List v)
    {
        glUniform3iv(location->gl_location, 1, v.data);
    }

    void uniform4iv(node_ptr<UniformLocation> location, Int32List v)
    {
        glUniform4iv(location->gl_location, 1, v.data);
    }

    void uniformMatrix2fv(node_ptr<UniformLocation> location, GLboolean transpose, Float32List v)
    {
        glUniformMatrix2fv(location->gl_location, 1, transpose, v.data);
    }

    void uniformMatrix3fv(node_ptr<UniformLocation> location, GLboolean transpose, Float32List v)
    {
        glUniformMatrix3fv(location->gl_location, 1, transpose, v.data);
    }

    void uniformMatrix4fv(node_ptr<UniformLocation> location, GLboolean transpose, Float32List v)
    {
        glUniformMatrix4fv(location->gl_location, 1, transpose, v.data);
    }

    void useProgram(node_ptr<Program> program)
    {
        glUseProgram(program->gl_handle);
    }

    void validateProgram(node_ptr<Program> program)
    {
        glValidateProgram(program->gl_handle);
    }

    void vertexAttrib1fv(GLuint index, Float32List values)
    {
        glVertexAttrib1fv(index, values.data);
    }

    void vertexAttrib2fv(GLuint index, Float32List values)
    {
        glVertexAttrib2fv(index, values.data);
    }

    void vertexAttrib3fv(GLuint index, Float32List values)
    {
        glVertexAttrib3fv(index, values.data);
    }

    void vertexAttrib4fv(GLuint index, Float32List values)
    {
        glVertexAttrib4fv(index, values.data);
    }
}

namespace teresa
{
    webgl_canvas::webgl_canvas()
        :_displayWindow(std::make_unique<glfw_window>(640, 480, "Display screen"))
    {
        _displayWindow->make_current();
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL context");
        }
    }

    void webgl_canvas::flush()
    {
        _displayWindow->swap_buffers();
        _displayWindow->react();
    }

    void webgl_canvas::_registerWebGL_1_0_methods(napi_env env_, napi_value object_) const
    { // from WebGL specification 1.0
#define REGISTER_GL_FUNCTION(webglName, glFunc) set_node_property(env_, object_, u8 ## #webglName, glFunc)

        REGISTER_GL_FUNCTION(getContextAttributes, webgl::getContextAttributes);
        REGISTER_GL_FUNCTION(isContextLost, webgl::isContextLost);
        REGISTER_GL_FUNCTION(getSupportedExtensions, webgl::getSupportedExtensions);
        REGISTER_GL_FUNCTION(getExtension, webgl::getExtension);
        REGISTER_GL_FUNCTION(activeTexture, webgl::activeTexture);
        REGISTER_GL_FUNCTION(attachShader, webgl::attachShader);
        REGISTER_GL_FUNCTION(bindAttribLocation, webgl::bindAttribLocation);
        REGISTER_GL_FUNCTION(bindBuffer, webgl::bindBuffer);
        REGISTER_GL_FUNCTION(bindFramebuffer, webgl::bindFramebuffer);
        REGISTER_GL_FUNCTION(bindRenderbuffer, webgl::bindRenderbuffer);
        REGISTER_GL_FUNCTION(bindTexture, webgl::bindTexture);
        REGISTER_GL_FUNCTION(blendColor, webgl::blendColor);
        REGISTER_GL_FUNCTION(blendEquation, webgl::blendEquation);
        REGISTER_GL_FUNCTION(blendEquationSeparate, webgl::blendEquationSeparate);
        REGISTER_GL_FUNCTION(blendFunc, webgl::blendFunc);
        REGISTER_GL_FUNCTION(blendFuncSeparate, webgl::blendFuncSeparate);
        REGISTER_GL_FUNCTION(bufferData, webgl::bufferData);
        REGISTER_GL_FUNCTION(bufferSubData, webgl::bufferSubData);
        REGISTER_GL_FUNCTION(checkFramebufferStatus, webgl::checkFramebufferStatus);
        REGISTER_GL_FUNCTION(clear, webgl::clear);
        REGISTER_GL_FUNCTION(clearColor, webgl::clearColor);
        REGISTER_GL_FUNCTION(clearDepth, webgl::clearDepth);
        REGISTER_GL_FUNCTION(clearStencil, webgl::clearStencil);
        REGISTER_GL_FUNCTION(colorMask, webgl::colorMask);
        REGISTER_GL_FUNCTION(compileShader, webgl::compileShader);
        REGISTER_GL_FUNCTION(compressedTexImage2D, webgl::compressedTexImage2D);
        REGISTER_GL_FUNCTION(compressedTexSubImage2D, webgl::compressedTexSubImage2D);
        REGISTER_GL_FUNCTION(copyTexImage2D, webgl::copyTexImage2D);
        REGISTER_GL_FUNCTION(copyTexSubImage2D, webgl::copyTexSubImage2D);
        REGISTER_GL_FUNCTION(createBuffer, webgl::createBuffer);
        REGISTER_GL_FUNCTION(createFramebuffer, webgl::createFramebuffer);
        REGISTER_GL_FUNCTION(createProgram, webgl::createProgram);
        REGISTER_GL_FUNCTION(createRenderbuffer, webgl::createRenderbuffer);
        REGISTER_GL_FUNCTION(createShader, webgl::createShader);
        REGISTER_GL_FUNCTION(createTexture, webgl::createTexture);
        REGISTER_GL_FUNCTION(cullFace, webgl::cullFace);
        REGISTER_GL_FUNCTION(deleteBuffer, webgl::deleteBuffer);
        REGISTER_GL_FUNCTION(deleteFramebuffer, webgl::deleteFramebuffer);
        REGISTER_GL_FUNCTION(deleteProgram, webgl::deleteProgram);
        REGISTER_GL_FUNCTION(deleteRenderbuffer, webgl::deleteRenderbuffer);
        REGISTER_GL_FUNCTION(deleteShader, webgl::deleteShader);
        REGISTER_GL_FUNCTION(deleteTexture, webgl::deleteTexture);
        REGISTER_GL_FUNCTION(depthFunc, webgl::depthFunc);
        REGISTER_GL_FUNCTION(depthMask, webgl::depthMask);
        REGISTER_GL_FUNCTION(depthRange, webgl::depthRange);
        REGISTER_GL_FUNCTION(detachShader, webgl::detachShader);
        REGISTER_GL_FUNCTION(disable, webgl::disable);
        REGISTER_GL_FUNCTION(disableVertexAttribArray, webgl::disableVertexAttribArray);
        REGISTER_GL_FUNCTION(drawArrays, webgl::drawArrays);
        REGISTER_GL_FUNCTION(drawElements, webgl::drawElements);
        REGISTER_GL_FUNCTION(enable, webgl::enable);
        REGISTER_GL_FUNCTION(enableVertexAttribArray, webgl::enableVertexAttribArray);
        REGISTER_GL_FUNCTION(finish, webgl::finish);
        REGISTER_GL_FUNCTION(flush, webgl::flush);
        REGISTER_GL_FUNCTION(framebufferRenderbuffer, webgl::framebufferRenderbuffer);
        REGISTER_GL_FUNCTION(framebufferTexture2D, webgl::framebufferTexture2D);
        REGISTER_GL_FUNCTION(frontFace, webgl::frontFace);
        REGISTER_GL_FUNCTION(generateMipmap, webgl::generateMipmap);
        REGISTER_GL_FUNCTION(getActiveAttrib, webgl::getActiveAttrib);
        REGISTER_GL_FUNCTION(getActiveUniform, webgl::getActiveUniform);
        REGISTER_GL_FUNCTION(getAttachedShaders, webgl::getAttachedShaders);
        REGISTER_GL_FUNCTION(getAttribLocation, webgl::getAttribLocation);
        REGISTER_GL_FUNCTION(getBufferParameter, webgl::getBufferParameter);
        REGISTER_GL_FUNCTION(getParameter, webgl::getParameter);
        REGISTER_GL_FUNCTION(getError, webgl::getError);
        REGISTER_GL_FUNCTION(getFramebufferAttachmentParameter, webgl::getFramebufferAttachmentParameter);
        REGISTER_GL_FUNCTION(getProgramParameter, webgl::getProgramParameter);
        REGISTER_GL_FUNCTION(getProgramInfoLog, webgl::getProgramInfoLog);
        REGISTER_GL_FUNCTION(getRenderbufferParameter, webgl::getRenderbufferParameter);
        REGISTER_GL_FUNCTION(getShaderParameter, webgl::getShaderParameter);
        REGISTER_GL_FUNCTION(getShaderPrecisionFormat, webgl::getShaderPrecisionFormat);
        REGISTER_GL_FUNCTION(getShaderInfoLog, webgl::getShaderInfoLog);
        REGISTER_GL_FUNCTION(getShaderSource, webgl::getShaderSource);
        REGISTER_GL_FUNCTION(getTexParameter, webgl::getTexParameter);
        REGISTER_GL_FUNCTION(getUniform, webgl::getUniform);
        REGISTER_GL_FUNCTION(getUniformLocation, webgl::getUniformLocation);
        REGISTER_GL_FUNCTION(getVertexAttrib, webgl::getVertexAttrib);
        REGISTER_GL_FUNCTION(getVertexAttribOffset, webgl::getVertexAttribOffset);
        REGISTER_GL_FUNCTION(hint, webgl::hint);
        REGISTER_GL_FUNCTION(isBuffer, webgl::isBuffer);
        REGISTER_GL_FUNCTION(isEnabled, webgl::isEnabled);
        REGISTER_GL_FUNCTION(isFramebuffer, webgl::isFramebuffer);
        REGISTER_GL_FUNCTION(isProgram, webgl::isProgram);
        REGISTER_GL_FUNCTION(isRenderbuffer, webgl::isRenderbuffer);
        REGISTER_GL_FUNCTION(isShader, webgl::isShader);
        REGISTER_GL_FUNCTION(isTexture, webgl::isTexture);
        REGISTER_GL_FUNCTION(lineWidth, webgl::lineWidth);
        REGISTER_GL_FUNCTION(linkProgram, webgl::linkProgram);
        REGISTER_GL_FUNCTION(pixelStorei, webgl::pixelStorei);
        REGISTER_GL_FUNCTION(polygonOffset, webgl::polygonOffset);
        REGISTER_GL_FUNCTION(readPixels, webgl::readPixels);
        REGISTER_GL_FUNCTION(renderbufferStorage, webgl::renderbufferStorage);
        REGISTER_GL_FUNCTION(sampleCoverage, webgl::sampleCoverage);
        REGISTER_GL_FUNCTION(scissor, webgl::scissor);
        REGISTER_GL_FUNCTION(shaderSource, webgl::shaderSource);
        REGISTER_GL_FUNCTION(stencilFunc, webgl::stencilFunc);
        REGISTER_GL_FUNCTION(stencilFuncSeparate, webgl::stencilFuncSeparate);
        REGISTER_GL_FUNCTION(stencilMask, webgl::stencilMask);
        REGISTER_GL_FUNCTION(stencilMaskSeparate, webgl::stencilMaskSeparate);
        REGISTER_GL_FUNCTION(stencilOp, webgl::stencilOp);
        REGISTER_GL_FUNCTION(stencilOpSeparate, webgl::stencilOpSeparate);
        REGISTER_GL_FUNCTION(texImage2D, webgl::texImage2D);
        REGISTER_GL_FUNCTION(texParameterf, webgl::texParameterf);
        REGISTER_GL_FUNCTION(texParameteri, webgl::texParameteri);
        REGISTER_GL_FUNCTION(texSubImage2D, webgl::texSubImage2D);
        REGISTER_GL_FUNCTION(uniform1f, webgl::uniform1f);
        REGISTER_GL_FUNCTION(uniform2f, webgl::uniform2f);
        REGISTER_GL_FUNCTION(uniform3f, webgl::uniform3f);
        REGISTER_GL_FUNCTION(uniform4f, webgl::uniform4f);
        REGISTER_GL_FUNCTION(uniform1i, webgl::uniform1i);
        REGISTER_GL_FUNCTION(uniform2i, webgl::uniform2i);
        REGISTER_GL_FUNCTION(uniform3i, webgl::uniform3i);
        REGISTER_GL_FUNCTION(uniform4i, webgl::uniform4i);
        REGISTER_GL_FUNCTION(uniform1fv, webgl::uniform1fv);
        REGISTER_GL_FUNCTION(uniform2fv, webgl::uniform2fv);
        REGISTER_GL_FUNCTION(uniform3fv, webgl::uniform3fv);
        REGISTER_GL_FUNCTION(uniform4fv, webgl::uniform4fv);
        REGISTER_GL_FUNCTION(uniform1iv, webgl::uniform1iv);
        REGISTER_GL_FUNCTION(uniform2iv, webgl::uniform2iv);
        REGISTER_GL_FUNCTION(uniform3iv, webgl::uniform3iv);
        REGISTER_GL_FUNCTION(uniform4iv, webgl::uniform4iv);
        REGISTER_GL_FUNCTION(uniformMatrix2fv, webgl::uniformMatrix2fv);
        REGISTER_GL_FUNCTION(uniformMatrix3fv, webgl::uniformMatrix3fv);
        REGISTER_GL_FUNCTION(uniformMatrix4fv, webgl::uniformMatrix4fv);
        REGISTER_GL_FUNCTION(useProgram, webgl::useProgram);
        REGISTER_GL_FUNCTION(validateProgram, webgl::validateProgram);
        REGISTER_GL_FUNCTION(vertexAttrib1f, glVertexAttrib1f);
        REGISTER_GL_FUNCTION(vertexAttrib2f, glVertexAttrib2f);
        REGISTER_GL_FUNCTION(vertexAttrib3f, glVertexAttrib3f);
        REGISTER_GL_FUNCTION(vertexAttrib4f, glVertexAttrib4f);
        REGISTER_GL_FUNCTION(vertexAttrib1fv, webgl::vertexAttrib1fv);
        REGISTER_GL_FUNCTION(vertexAttrib2fv, webgl::vertexAttrib2fv);
        REGISTER_GL_FUNCTION(vertexAttrib3fv, webgl::vertexAttrib3fv);
        REGISTER_GL_FUNCTION(vertexAttrib4fv, webgl::vertexAttrib4fv);
        REGISTER_GL_FUNCTION(vertexAttribPointer, glVertexAttribPointer);
        REGISTER_GL_FUNCTION(viewport, glViewport);

#undef REGISTER_GL_FUNCTION
    }

    void webgl_canvas::_registerWebGL_1_0_properties(napi_env env_, napi_value object_) const
    { // from WebGL specification 1.0
#define REGISTER_GL_ENUM(name) set_node_property(env_, object_, u8 ## #name, GL_ ## name)

      /* ClearBufferMask */
        REGISTER_GL_ENUM(COLOR_BUFFER_BIT);
        REGISTER_GL_ENUM(DEPTH_BUFFER_BIT);
        REGISTER_GL_ENUM(STENCIL_BUFFER_BIT);

        /* BeginMode */
        REGISTER_GL_ENUM(POINTS);
        REGISTER_GL_ENUM(LINES);
        REGISTER_GL_ENUM(LINE_LOOP);
        REGISTER_GL_ENUM(LINE_STRIP);
        REGISTER_GL_ENUM(TRIANGLES);
        REGISTER_GL_ENUM(TRIANGLE_STRIP);
        REGISTER_GL_ENUM(TRIANGLE_FAN);

        /* AlphaFunction (not supported in ES20) */
        /*      NEVER */
        /*      LESS */
        /*      EQUAL */
        /*      LEQUAL */
        /*      GREATER */
        /*      NOTEQUAL */
        /*      GEQUAL */
        /*      ALWAYS */

        /* BlendingFactorDest */
        REGISTER_GL_ENUM(ZERO);
        REGISTER_GL_ENUM(ONE);
        REGISTER_GL_ENUM(SRC_COLOR);
        REGISTER_GL_ENUM(ONE_MINUS_SRC_COLOR);
        REGISTER_GL_ENUM(SRC_ALPHA);
        REGISTER_GL_ENUM(ONE_MINUS_SRC_ALPHA);
        REGISTER_GL_ENUM(DST_ALPHA);
        REGISTER_GL_ENUM(ONE_MINUS_DST_ALPHA);

        /* BlendingFactorSrc */
        /*      ZERO */
        /*      ONE */
        REGISTER_GL_ENUM(DST_COLOR);
        REGISTER_GL_ENUM(ONE_MINUS_DST_COLOR);
        REGISTER_GL_ENUM(SRC_ALPHA_SATURATE);
        /*      SRC_ALPHA */
        /*      ONE_MINUS_SRC_ALPHA */
        /*      DST_ALPHA */
        /*      ONE_MINUS_DST_ALPHA */

        /* BlendEquationSeparate */
        REGISTER_GL_ENUM(FUNC_ADD);
        REGISTER_GL_ENUM(BLEND_EQUATION);
        REGISTER_GL_ENUM(BLEND_EQUATION_RGB);   /* same as BLEND_EQUATION */
        REGISTER_GL_ENUM(BLEND_EQUATION_ALPHA);

        /* BlendSubtract */
        REGISTER_GL_ENUM(FUNC_SUBTRACT);
        REGISTER_GL_ENUM(FUNC_REVERSE_SUBTRACT);

        /* Separate Blend Functions */
        REGISTER_GL_ENUM(BLEND_DST_RGB);
        REGISTER_GL_ENUM(BLEND_SRC_RGB);
        REGISTER_GL_ENUM(BLEND_DST_ALPHA);
        REGISTER_GL_ENUM(BLEND_SRC_ALPHA);
        REGISTER_GL_ENUM(CONSTANT_COLOR);
        REGISTER_GL_ENUM(ONE_MINUS_CONSTANT_COLOR);
        REGISTER_GL_ENUM(CONSTANT_ALPHA);
        REGISTER_GL_ENUM(ONE_MINUS_CONSTANT_ALPHA);
        REGISTER_GL_ENUM(BLEND_COLOR);

        /* Buffer Objects */
        REGISTER_GL_ENUM(ARRAY_BUFFER);
        REGISTER_GL_ENUM(ELEMENT_ARRAY_BUFFER);
        REGISTER_GL_ENUM(ARRAY_BUFFER_BINDING);
        REGISTER_GL_ENUM(ELEMENT_ARRAY_BUFFER_BINDING);

        REGISTER_GL_ENUM(STREAM_DRAW);
        REGISTER_GL_ENUM(STATIC_DRAW);
        REGISTER_GL_ENUM(DYNAMIC_DRAW);

        REGISTER_GL_ENUM(BUFFER_SIZE);
        REGISTER_GL_ENUM(BUFFER_USAGE);

        REGISTER_GL_ENUM(CURRENT_VERTEX_ATTRIB);

        /* CullFaceMode */
        REGISTER_GL_ENUM(FRONT);
        REGISTER_GL_ENUM(BACK);
        REGISTER_GL_ENUM(FRONT_AND_BACK);

        /* DepthFunction */
        /*      NEVER */
        /*      LESS */
        /*      EQUAL */
        /*      LEQUAL */
        /*      GREATER */
        /*      NOTEQUAL */
        /*      GEQUAL */
        /*      ALWAYS */

        /* EnableCap */
        /* TEXTURE_2D */
        REGISTER_GL_ENUM(CULL_FACE);
        REGISTER_GL_ENUM(BLEND);
        REGISTER_GL_ENUM(DITHER);
        REGISTER_GL_ENUM(STENCIL_TEST);
        REGISTER_GL_ENUM(DEPTH_TEST);
        REGISTER_GL_ENUM(SCISSOR_TEST);
        REGISTER_GL_ENUM(POLYGON_OFFSET_FILL);
        REGISTER_GL_ENUM(SAMPLE_ALPHA_TO_COVERAGE);
        REGISTER_GL_ENUM(SAMPLE_COVERAGE);

        /* ErrorCode */
        REGISTER_GL_ENUM(NO_ERROR);
        REGISTER_GL_ENUM(INVALID_ENUM);
        REGISTER_GL_ENUM(INVALID_VALUE);
        REGISTER_GL_ENUM(INVALID_OPERATION);
        REGISTER_GL_ENUM(OUT_OF_MEMORY);

        /* FrontFaceDirection */
        REGISTER_GL_ENUM(CW);
        REGISTER_GL_ENUM(CCW);

        /* GetPName */
        REGISTER_GL_ENUM(LINE_WIDTH);
        REGISTER_GL_ENUM(ALIASED_POINT_SIZE_RANGE);
        REGISTER_GL_ENUM(ALIASED_LINE_WIDTH_RANGE);
        REGISTER_GL_ENUM(CULL_FACE_MODE);
        REGISTER_GL_ENUM(FRONT_FACE);
        REGISTER_GL_ENUM(DEPTH_RANGE);
        REGISTER_GL_ENUM(DEPTH_WRITEMASK);
        REGISTER_GL_ENUM(DEPTH_CLEAR_VALUE);
        REGISTER_GL_ENUM(DEPTH_FUNC);
        REGISTER_GL_ENUM(STENCIL_CLEAR_VALUE);
        REGISTER_GL_ENUM(STENCIL_FUNC);
        REGISTER_GL_ENUM(STENCIL_FAIL);
        REGISTER_GL_ENUM(STENCIL_PASS_DEPTH_FAIL);
        REGISTER_GL_ENUM(STENCIL_PASS_DEPTH_PASS);
        REGISTER_GL_ENUM(STENCIL_REF);
        REGISTER_GL_ENUM(STENCIL_VALUE_MASK);
        REGISTER_GL_ENUM(STENCIL_WRITEMASK);
        REGISTER_GL_ENUM(STENCIL_BACK_FUNC);
        REGISTER_GL_ENUM(STENCIL_BACK_FAIL);
        REGISTER_GL_ENUM(STENCIL_BACK_PASS_DEPTH_FAIL);
        REGISTER_GL_ENUM(STENCIL_BACK_PASS_DEPTH_PASS);
        REGISTER_GL_ENUM(STENCIL_BACK_REF);
        REGISTER_GL_ENUM(STENCIL_BACK_VALUE_MASK);
        REGISTER_GL_ENUM(STENCIL_BACK_WRITEMASK);
        REGISTER_GL_ENUM(VIEWPORT);
        REGISTER_GL_ENUM(SCISSOR_BOX);
        /*      SCISSOR_TEST */
        REGISTER_GL_ENUM(COLOR_CLEAR_VALUE);
        REGISTER_GL_ENUM(COLOR_WRITEMASK);
        REGISTER_GL_ENUM(UNPACK_ALIGNMENT);
        REGISTER_GL_ENUM(PACK_ALIGNMENT);
        REGISTER_GL_ENUM(MAX_TEXTURE_SIZE);
        REGISTER_GL_ENUM(MAX_VIEWPORT_DIMS);
        REGISTER_GL_ENUM(SUBPIXEL_BITS);
        REGISTER_GL_ENUM(RED_BITS);
        REGISTER_GL_ENUM(GREEN_BITS);
        REGISTER_GL_ENUM(BLUE_BITS);
        REGISTER_GL_ENUM(ALPHA_BITS);
        REGISTER_GL_ENUM(DEPTH_BITS);
        REGISTER_GL_ENUM(STENCIL_BITS);
        REGISTER_GL_ENUM(POLYGON_OFFSET_UNITS);
        /*      POLYGON_OFFSET_FILL */
        REGISTER_GL_ENUM(POLYGON_OFFSET_FACTOR);
        REGISTER_GL_ENUM(TEXTURE_BINDING_2D);
        REGISTER_GL_ENUM(SAMPLE_BUFFERS);
        REGISTER_GL_ENUM(SAMPLES);
        REGISTER_GL_ENUM(SAMPLE_COVERAGE_VALUE);
        REGISTER_GL_ENUM(SAMPLE_COVERAGE_INVERT);

        /* GetTextureParameter */
        /*      TEXTURE_MAG_FILTER */
        /*      TEXTURE_MIN_FILTER */
        /*      TEXTURE_WRAP_S */
        /*      TEXTURE_WRAP_T */

        REGISTER_GL_ENUM(COMPRESSED_TEXTURE_FORMATS);

        /* HintMode */
        REGISTER_GL_ENUM(DONT_CARE);
        REGISTER_GL_ENUM(FASTEST);
        REGISTER_GL_ENUM(NICEST);

        /* HintTarget */
        REGISTER_GL_ENUM(GENERATE_MIPMAP_HINT);

        /* DataType */
        REGISTER_GL_ENUM(BYTE);
        REGISTER_GL_ENUM(UNSIGNED_BYTE);
        REGISTER_GL_ENUM(SHORT);
        REGISTER_GL_ENUM(UNSIGNED_SHORT);
        REGISTER_GL_ENUM(INT);
        REGISTER_GL_ENUM(UNSIGNED_INT);
        REGISTER_GL_ENUM(FLOAT);

        /* PixelFormat */
        REGISTER_GL_ENUM(DEPTH_COMPONENT);
        REGISTER_GL_ENUM(ALPHA);
        REGISTER_GL_ENUM(RGB);
        REGISTER_GL_ENUM(RGBA);
        REGISTER_GL_ENUM(LUMINANCE);
        REGISTER_GL_ENUM(LUMINANCE_ALPHA);

        /* PixelType */
        /*      UNSIGNED_BYTE */
        REGISTER_GL_ENUM(UNSIGNED_SHORT_4_4_4_4);
        REGISTER_GL_ENUM(UNSIGNED_SHORT_5_5_5_1);
        REGISTER_GL_ENUM(UNSIGNED_SHORT_5_6_5);

        /* Shaders */
        REGISTER_GL_ENUM(FRAGMENT_SHADER);
        REGISTER_GL_ENUM(VERTEX_SHADER);
        REGISTER_GL_ENUM(MAX_VERTEX_ATTRIBS);
        REGISTER_GL_ENUM(MAX_VERTEX_UNIFORM_VECTORS);
        REGISTER_GL_ENUM(MAX_VARYING_VECTORS);
        REGISTER_GL_ENUM(MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        REGISTER_GL_ENUM(MAX_VERTEX_TEXTURE_IMAGE_UNITS);
        REGISTER_GL_ENUM(MAX_TEXTURE_IMAGE_UNITS);
        REGISTER_GL_ENUM(MAX_FRAGMENT_UNIFORM_VECTORS);
        REGISTER_GL_ENUM(SHADER_TYPE);
        REGISTER_GL_ENUM(DELETE_STATUS);
        REGISTER_GL_ENUM(LINK_STATUS);
        REGISTER_GL_ENUM(VALIDATE_STATUS);
        REGISTER_GL_ENUM(ATTACHED_SHADERS);
        REGISTER_GL_ENUM(ACTIVE_UNIFORMS);
        REGISTER_GL_ENUM(ACTIVE_ATTRIBUTES);
        REGISTER_GL_ENUM(SHADING_LANGUAGE_VERSION);
        REGISTER_GL_ENUM(CURRENT_PROGRAM);

        /* StencilFunction */
        REGISTER_GL_ENUM(NEVER);
        REGISTER_GL_ENUM(LESS);
        REGISTER_GL_ENUM(EQUAL);
        REGISTER_GL_ENUM(LEQUAL);
        REGISTER_GL_ENUM(GREATER);
        REGISTER_GL_ENUM(NOTEQUAL);
        REGISTER_GL_ENUM(GEQUAL);
        REGISTER_GL_ENUM(ALWAYS);

        /* StencilOp */
        /*      ZERO */
        REGISTER_GL_ENUM(KEEP);
        REGISTER_GL_ENUM(REPLACE);
        REGISTER_GL_ENUM(INCR);
        REGISTER_GL_ENUM(DECR);
        REGISTER_GL_ENUM(INVERT);
        REGISTER_GL_ENUM(INCR_WRAP);
        REGISTER_GL_ENUM(DECR_WRAP);

        /* StringName */
        REGISTER_GL_ENUM(VENDOR);
        REGISTER_GL_ENUM(RENDERER);
        REGISTER_GL_ENUM(VERSION);

        /* TextureMagFilter */
        REGISTER_GL_ENUM(NEAREST);
        REGISTER_GL_ENUM(LINEAR);

        /* TextureMinFilter */
        /*      NEAREST */
        /*      LINEAR */
        REGISTER_GL_ENUM(NEAREST_MIPMAP_NEAREST);
        REGISTER_GL_ENUM(LINEAR_MIPMAP_NEAREST);
        REGISTER_GL_ENUM(NEAREST_MIPMAP_LINEAR);
        REGISTER_GL_ENUM(LINEAR_MIPMAP_LINEAR);

        /* TextureParameterName */
        REGISTER_GL_ENUM(TEXTURE_MAG_FILTER);
        REGISTER_GL_ENUM(TEXTURE_MIN_FILTER);
        REGISTER_GL_ENUM(TEXTURE_WRAP_S);
        REGISTER_GL_ENUM(TEXTURE_WRAP_T);

        /* TextureTarget */
        REGISTER_GL_ENUM(TEXTURE_2D);
        REGISTER_GL_ENUM(TEXTURE);

        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP);
        REGISTER_GL_ENUM(TEXTURE_BINDING_CUBE_MAP);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_POSITIVE_X);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_NEGATIVE_X);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_POSITIVE_Y);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_NEGATIVE_Y);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_POSITIVE_Z);
        REGISTER_GL_ENUM(TEXTURE_CUBE_MAP_NEGATIVE_Z);
        REGISTER_GL_ENUM(MAX_CUBE_MAP_TEXTURE_SIZE);

        /* TextureUnit */
        REGISTER_GL_ENUM(TEXTURE0);
        REGISTER_GL_ENUM(TEXTURE1);
        REGISTER_GL_ENUM(TEXTURE2);
        REGISTER_GL_ENUM(TEXTURE3);
        REGISTER_GL_ENUM(TEXTURE4);
        REGISTER_GL_ENUM(TEXTURE5);
        REGISTER_GL_ENUM(TEXTURE6);
        REGISTER_GL_ENUM(TEXTURE7);
        REGISTER_GL_ENUM(TEXTURE8);
        REGISTER_GL_ENUM(TEXTURE9);
        REGISTER_GL_ENUM(TEXTURE10);
        REGISTER_GL_ENUM(TEXTURE11);
        REGISTER_GL_ENUM(TEXTURE12);
        REGISTER_GL_ENUM(TEXTURE13);
        REGISTER_GL_ENUM(TEXTURE14);
        REGISTER_GL_ENUM(TEXTURE15);
        REGISTER_GL_ENUM(TEXTURE16);
        REGISTER_GL_ENUM(TEXTURE17);
        REGISTER_GL_ENUM(TEXTURE18);
        REGISTER_GL_ENUM(TEXTURE19);
        REGISTER_GL_ENUM(TEXTURE20);
        REGISTER_GL_ENUM(TEXTURE21);
        REGISTER_GL_ENUM(TEXTURE22);
        REGISTER_GL_ENUM(TEXTURE23);
        REGISTER_GL_ENUM(TEXTURE24);
        REGISTER_GL_ENUM(TEXTURE25);
        REGISTER_GL_ENUM(TEXTURE26);
        REGISTER_GL_ENUM(TEXTURE27);
        REGISTER_GL_ENUM(TEXTURE28);
        REGISTER_GL_ENUM(TEXTURE29);
        REGISTER_GL_ENUM(TEXTURE30);
        REGISTER_GL_ENUM(TEXTURE31);
        REGISTER_GL_ENUM(ACTIVE_TEXTURE);

        /* TextureWrapMode */
        REGISTER_GL_ENUM(REPEAT);
        REGISTER_GL_ENUM(CLAMP_TO_EDGE);
        REGISTER_GL_ENUM(MIRRORED_REPEAT);

        /* Uniform Types */
        REGISTER_GL_ENUM(FLOAT_VEC2);
        REGISTER_GL_ENUM(FLOAT_VEC3);
        REGISTER_GL_ENUM(FLOAT_VEC4);
        REGISTER_GL_ENUM(INT_VEC2);
        REGISTER_GL_ENUM(INT_VEC3);
        REGISTER_GL_ENUM(INT_VEC4);
        REGISTER_GL_ENUM(BOOL);
        REGISTER_GL_ENUM(BOOL_VEC2);
        REGISTER_GL_ENUM(BOOL_VEC3);
        REGISTER_GL_ENUM(BOOL_VEC4);
        REGISTER_GL_ENUM(FLOAT_MAT2);
        REGISTER_GL_ENUM(FLOAT_MAT3);
        REGISTER_GL_ENUM(FLOAT_MAT4);
        REGISTER_GL_ENUM(SAMPLER_2D);
        REGISTER_GL_ENUM(SAMPLER_CUBE);

        /* Vertex Arrays */
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_ENABLED);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_SIZE);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_STRIDE);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_TYPE);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_NORMALIZED);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_POINTER);
        REGISTER_GL_ENUM(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);

        /* Read Format */
        REGISTER_GL_ENUM(IMPLEMENTATION_COLOR_READ_TYPE);
        REGISTER_GL_ENUM(IMPLEMENTATION_COLOR_READ_FORMAT);

        /* Shader Source */
        REGISTER_GL_ENUM(COMPILE_STATUS);

        /* Shader Precision-Specified Types */
        REGISTER_GL_ENUM(LOW_FLOAT);
        REGISTER_GL_ENUM(MEDIUM_FLOAT);
        REGISTER_GL_ENUM(HIGH_FLOAT);
        REGISTER_GL_ENUM(LOW_INT);
        REGISTER_GL_ENUM(MEDIUM_INT);
        REGISTER_GL_ENUM(HIGH_INT);

        /* Framebuffer Object. */
        REGISTER_GL_ENUM(FRAMEBUFFER);
        REGISTER_GL_ENUM(RENDERBUFFER);

        REGISTER_GL_ENUM(RGBA4);
        REGISTER_GL_ENUM(RGB5_A1);
        REGISTER_GL_ENUM(RGB565);
        REGISTER_GL_ENUM(DEPTH_COMPONENT16);
        REGISTER_GL_ENUM(STENCIL_INDEX8);
        REGISTER_GL_ENUM(DEPTH_STENCIL);

        REGISTER_GL_ENUM(RENDERBUFFER_WIDTH);
        REGISTER_GL_ENUM(RENDERBUFFER_HEIGHT);
        REGISTER_GL_ENUM(RENDERBUFFER_INTERNAL_FORMAT);
        REGISTER_GL_ENUM(RENDERBUFFER_RED_SIZE);
        REGISTER_GL_ENUM(RENDERBUFFER_GREEN_SIZE);
        REGISTER_GL_ENUM(RENDERBUFFER_BLUE_SIZE);
        REGISTER_GL_ENUM(RENDERBUFFER_ALPHA_SIZE);
        REGISTER_GL_ENUM(RENDERBUFFER_DEPTH_SIZE);
        REGISTER_GL_ENUM(RENDERBUFFER_STENCIL_SIZE);

        REGISTER_GL_ENUM(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
        REGISTER_GL_ENUM(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
        REGISTER_GL_ENUM(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
        REGISTER_GL_ENUM(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);

        REGISTER_GL_ENUM(COLOR_ATTACHMENT0);
        REGISTER_GL_ENUM(DEPTH_ATTACHMENT);
        REGISTER_GL_ENUM(STENCIL_ATTACHMENT);
        REGISTER_GL_ENUM(DEPTH_STENCIL_ATTACHMENT);

        REGISTER_GL_ENUM(NONE);

        REGISTER_GL_ENUM(FRAMEBUFFER_COMPLETE);
        REGISTER_GL_ENUM(FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
        REGISTER_GL_ENUM(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
        // REGISTER_GL_ENUM(FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
        REGISTER_GL_ENUM(FRAMEBUFFER_UNSUPPORTED);

        REGISTER_GL_ENUM(FRAMEBUFFER_BINDING);
        REGISTER_GL_ENUM(RENDERBUFFER_BINDING);
        REGISTER_GL_ENUM(MAX_RENDERBUFFER_SIZE);

        REGISTER_GL_ENUM(INVALID_FRAMEBUFFER_OPERATION);

        /* WebGL-specific enums */
        REGISTER_GL_ENUM(UNPACK_FLIP_Y_WEBGL);
        REGISTER_GL_ENUM(UNPACK_PREMULTIPLY_ALPHA_WEBGL);
        REGISTER_GL_ENUM(CONTEXT_LOST_WEBGL);
        REGISTER_GL_ENUM(UNPACK_COLORSPACE_CONVERSION_WEBGL);
        REGISTER_GL_ENUM(BROWSER_DEFAULT_WEBGL);

#undef REGISTER_GL_ENUM
    }
}
