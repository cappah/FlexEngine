#include "stdafx.hpp"

#include "Helpers.hpp"

#include <sstream>
#include <iomanip>
#include <fstream>

#include <imgui.h>

#include <glm/gtx/matrix_decompose.hpp>

#include "Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace flex
{
	GLFWimage LoadGLFWimage(const std::string& filePath, bool alpha, bool flipVertically)
	{
		GLFWimage result = {};

		std::string fileName = filePath;
		StripLeadingDirectories(fileName);
		Logger::LogInfo("Loading texture " + fileName);

		stbi_set_flip_vertically_on_load(flipVertically);

		int channels;
		unsigned char* data = stbi_load(filePath.c_str(), &result.width, &result.height, &channels, alpha ? STBI_rgb_alpha : STBI_rgb);

		if (data == 0)
		{
			const char* failureReasonStr = stbi_failure_reason();
			Logger::LogError("Couldn't load image, failure reason: " + std::string(failureReasonStr) + " filepath: " + filePath);
			return result;
		}
		else
		{
			result.pixels = static_cast<unsigned char*>(data);
		}

		return result;
	}

	void DestroyGLFWimage(GLFWimage& image)
	{
		stbi_image_free(image.pixels);
		image.pixels = nullptr;
	}

	std::string FloatToString(float f, int precision)
	{
		std::stringstream stream;

		stream << std::fixed << std::setprecision(precision) << f;

		return stream.str();
	}

	bool ReadFile(const std::string& filePath, std::vector<char>& vec)
	{
		std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

		if (!file)
		{
			Logger::LogError("Unable to read file " + filePath);
			return false;
		}

		std::streampos length = file.tellg();

		vec.resize((size_t)length);

		file.seekg(0, std::ios::beg);
		file.read(vec.data(), length);
		file.close();

		return true;
	}

	void StripLeadingDirectories(std::string& filePath)
	{
		size_t finalSlash = filePath.rfind('/');
		if (finalSlash == std::string::npos) finalSlash = filePath.rfind('\\');

		if (finalSlash == std::string::npos)
		{
			return; // There are no directories to remove
		}
		else
		{
			filePath = filePath.substr(finalSlash + 1);
		}
	}

	float Lerp(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	glm::vec2 Lerp(const glm::vec2 & a, const glm::vec2 & b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	void ToString(const glm::vec2& vec, std::ostream& stream)
	{
		stream << vec.x << " " << vec.y;
	}

	void ToString(const glm::vec3& vec, std::ostream& stream)
	{
		stream << vec.x << " " << vec.y << " " << vec.z;
	}

	void ToString(const glm::vec4& vec, std::ostream& stream)
	{
		stream << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
	}

	void CopyColorToClipboard(const glm::vec3& col)
	{
		CopyColorToClipboard({ col.x, col.y, col.z, 1.0f });
	}

	void CopyColorToClipboard(const glm::vec4& col)
	{
		ImGui::LogToClipboard();

		ImGui::LogText("%.2ff,%.2ff,%.2ff,%.2ff", col.x, col.y, col.z, col.w);

		ImGui::LogFinish();
	}

	glm::vec3 PasteColor3FromClipboard()
	{
		glm::vec4 color4 = PasteColor4FromClipboard();

		return glm::vec3(color4);
	}

	glm::vec4 PasteColor4FromClipboard()
	{
		const std::string clipboardContents = ImGui::GetClipboardText();

		const size_t comma1 = clipboardContents.find(',');
		const size_t comma2 = clipboardContents.find(',', comma1 + 1);
		const size_t comma3 = clipboardContents.find(',', comma2 + 1);

		if (comma1 == std::string::npos ||
			comma2 == std::string::npos ||
			comma3 == std::string::npos)
		{
			// Clipboard doesn't contain correctly formatted color!
			return glm::vec4(0.0f);
		}

		glm::vec4 result(
			stof(clipboardContents.substr(0, comma1)),
			stof(clipboardContents.substr(comma1 + 1, comma2 - comma1 - 1)),
			stof(clipboardContents.substr(comma2 + 1, comma3 - comma2 - 1)),
			stof(clipboardContents.substr(comma3 + 1 ))
		);

		return result;
	}

	void CopyableColorEdit3(const char * label, glm::vec3 & col, const char * copyBtnLabel, const char * pasteBtnLabel, ImGuiColorEditFlags flags)
	{
		ImGui::ColorEdit3(label, &col.r, flags);
		ImGui::SameLine(); if (ImGui::Button(copyBtnLabel)) CopyColorToClipboard(col);
		ImGui::SameLine(); if (ImGui::Button(pasteBtnLabel)) col = PasteColor3FromClipboard();
	}

	void CopyableColorEdit4(const char * label, glm::vec4 & col, const char * copyBtnLabel, const char * pasteBtnLabel, ImGuiColorEditFlags flags)
	{
		ImGui::ColorEdit4(label, &col.r, flags);
		ImGui::SameLine(); if (ImGui::Button(copyBtnLabel)) CopyColorToClipboard(col);
		ImGui::SameLine(); if (ImGui::Button(pasteBtnLabel)) col = PasteColor4FromClipboard();
	}

	bool HDRImage::Load(const std::string& hdrFilePath, bool flipVertically)
	{
		filePath = hdrFilePath;

		std::string fileName = hdrFilePath;
		StripLeadingDirectories(fileName);
		Logger::LogInfo("Loading HDR texture " + fileName);

		stbi_set_flip_vertically_on_load(flipVertically);

		int channelCount;
		pixels = stbi_loadf(filePath.c_str(), &width, &height, &channelCount, STBI_rgb_alpha);

		if (!pixels)
		{
			Logger::LogError("Failed to load HDR image at " + filePath);
			return false;
		}

		return true;
	}

	void HDRImage::Free()
	{
		stbi_image_free(pixels);
	}

} // namespace flex
