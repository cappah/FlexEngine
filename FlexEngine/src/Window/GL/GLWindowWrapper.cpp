﻿#include "stdafx.hpp"
#if COMPILE_OPEN_GL

#include "Window/GL/GLWindowWrapper.hpp"

#include <sstream>

#include "Graphics/GL/GLHelpers.hpp"
#include "Helpers.hpp"
#include "Logger.hpp"

namespace flex
{
	namespace gl
	{
		GLWindowWrapper::GLWindowWrapper(std::string title, glm::vec2i size, glm::vec2i pos, GameContext& gameContext) :
			GLFWWindowWrapper(title, size, gameContext)
		{
			glfwSetErrorCallback(GLFWErrorCallback);

			if (!glfwInit())
			{
				Logger::LogError("Failed to initialize glfw! Exiting");
				exit(EXIT_FAILURE);
			}

#if _DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_Window = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
			if (!m_Window)
			{
				Logger::LogError("Failed to create glfw Window! Exiting");
				glfwTerminate();
				// TODO: Try creating a window manually here
				exit(EXIT_FAILURE);
			}

			glfwSetWindowUserPointer(m_Window, this);

			glfwSetKeyCallback(m_Window, GLFWKeyCallback);
			glfwSetCharCallback(m_Window, GLFWCharCallback);
			glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
			glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);
			glfwSetScrollCallback(m_Window, GLFWScrollCallback);
			glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
			glfwSetFramebufferSizeCallback(m_Window, GLFWFramebufferSizeCallback);

			glfwSetWindowFocusCallback(m_Window, GLFWWindowFocusCallback);

			glfwSetWindowPos(m_Window, pos.x, pos.y);

			glfwFocusWindow(m_Window);
			m_HasFocus = true;

			glfwMakeContextCurrent(m_Window);

			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			CheckGLErrorMessages();

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

			gameContext.monitor.width = vidMode->width;
			gameContext.monitor.height = vidMode->height;
			gameContext.monitor.redBits = vidMode->redBits;
			gameContext.monitor.greenBits = vidMode->greenBits;
			gameContext.monitor.blueBits = vidMode->blueBits;
			gameContext.monitor.refreshRate = vidMode->refreshRate;

			Logger::LogInfo("OpenGL loaded");
			Logger::LogInfo("Vendor:\t\t" + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
			Logger::LogInfo("Renderer:\t" + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
			Logger::LogInfo("Version:\t\t" + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))) + "\n");
			CheckGLErrorMessages();

			if (!m_WindowIcons.empty() && m_WindowIcons[0].pixels)
			{
				glfwSetWindowIcon(m_Window, m_WindowIcons.size(), m_WindowIcons.data());
			}
		}

		GLWindowWrapper::~GLWindowWrapper()
		{
		}

		void GLWindowWrapper::SetSize(int width, int height)
		{
			m_Size = glm::vec2i(width, height);
			m_GameContextRef.renderer->OnWindowSize(width, height);
		}

		void GLWindowWrapper::SetFrameBufferSize(int width, int height)
		{
			m_FrameBufferSize = glm::vec2i(width, height);
			// TODO: Call OnFrameBufferSize here?
			m_GameContextRef.renderer->OnWindowSize(width, height);
		}

		void WINAPI glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam)
		{
			UNREFERENCED_PARAMETER(userParam);
			UNREFERENCED_PARAMETER(length);

			// Ignore insignificant error/warning codes
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

			Logger::LogInfo("---------------");
			Logger::LogInfo("Debug message (" + std::to_string(id) + "): " + message);

			std::stringstream ss;
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:             ss << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     ss << "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           ss << "Source: Other"; break;
			}
			ss << std::endl;

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:               ss << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         ss << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         ss << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              ss << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           ss << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               ss << "Type: Other"; break;
			}
			ss << std::endl;

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         ss << "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       ss << "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          ss << "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "Severity: notification"; break;
			}
			ss << std::endl;

			Logger::LogInfo(ss.str());
		}
	} // namespace gl
} // namespace flex

#endif // COMPILE_OPEN_GL
