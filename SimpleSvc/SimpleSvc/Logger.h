#pragma once

enum class LogLevel {
	Critical,
	Error,
	Warning,
	Info,
	Debug,
	_Count
};

struct LogMessage {
	LogLevel Level;
	char Text[1];
};
