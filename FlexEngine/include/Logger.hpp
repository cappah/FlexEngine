#pragma once

#include <string>

namespace flex
{
	class Logger
	{
	public:
		enum class LogLevel
		{
			LOG_INFO,
			LOG_WARNING,
			LOG_ERROR
		};

		static void Initialize();

		static void LogInfo(const std::string& message, bool newline = true);
		static void LogWarning(const std::string& message, bool newline = true);
		static void LogError(const std::string& message, bool newline = true);

		static void LogInfo(const std::wstring& message, bool newline = true);
		static void LogWarning(const std::wstring& message, bool newline = true);
		static void LogError(const std::wstring& message, bool newline = true);

		static void Log(const std::string& message, LogLevel logLevel, bool newline);
		static void Log(const std::wstring& message, LogLevel logLevel, bool newline);


		static void SetSuppressInfo(bool suppressInfo);
		static bool GetSuppressInfo();
		static int GetSuppressedInfoCount();

		static void SetSuppressWarnings(bool suppressWarnings);
		static bool GetSuppressWarnings();
		static int GetSuppressedWarningCount();
		
		static void SetSuppressErrors(bool suppressErrors);
		static bool GetSuppressErrors();
		static int GetSuppressedErrorCount();

	private:
		static bool m_SuppressInfo;
		static bool m_SuppressWarnings;
		static bool m_SuppressErrors;

		static int m_SuppressedInfoCount;
		static int m_SuppressedWarningCount;
		static int m_SuppressedErrorCount;

#ifdef _WIN32
		static HANDLE m_ConsoleHandle;
		static const WORD CONSOLE_COLOR_INFO = 0 | FOREGROUND_INTENSITY;
		static const WORD CONSOLE_COLOR_WARNING = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		static const WORD CONSOLE_COLOR_ERROR = FOREGROUND_RED | FOREGROUND_INTENSITY;
#endif

	};
} // namespace flex
