#pragma once

enum class EventType {
	ProcessCreate,
	ProcessExit,
};

struct EventHeader {
	ULONG64 Time;
	EventType Type;
	ULONG Size;
};

struct ProcessCreate : EventHeader {
	ULONG ProcessId;
	ULONG ParentId;
	WCHAR CommandLine[1];
};

struct ProcessExit : EventHeader {
	ULONG ProcessId;
	LONG ExitCode;
};
