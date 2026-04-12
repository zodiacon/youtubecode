// RunETW.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <tdh.h>
#include <memory>
#include <cassert>

#pragma comment(lib, "tdh")

void OnEvent(PEVENT_RECORD rec) {
	auto& header = rec->EventHeader;
	FILETIME ft;
	FileTimeToLocalFileTime((FILETIME*)&header.TimeStamp, &ft);
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	printf("%02d:%02d:%02d.%03d PID: %u TID: %u\n", 
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		header.ProcessId, header.ThreadId);

	ULONG size = 0;
	TdhGetEventInformation(rec, 0, nullptr, nullptr, &size);
	assert(size);
	auto buffer = std::make_unique<BYTE[]>(size);
	auto info = (TRACE_EVENT_INFO*)buffer.get();
	TdhGetEventInformation(rec, 0, nullptr, info, &size);

	if (info->EventNameOffset)
		printf(" Event name: %ws", (PCWSTR)(buffer.get() + info->EventNameOffset));
	if(info->KeywordsNameOffset)
		printf(" Keywords: %ws", (PCWSTR)(buffer.get() + info->KeywordsNameOffset));
	if (info->OpcodeNameOffset)
		printf(" Opcode: %ws", (PCWSTR)(buffer.get() + info->OpcodeNameOffset));
	if (info->TaskNameOffset)
		printf(" Task: %ws ", (PCWSTR)(buffer.get() + info->TaskNameOffset));
	if (info->LevelNameOffset)
		printf(" Level: %ws ", (PCWSTR)(buffer.get() + info->LevelNameOffset));
	printf("\n");

	auto len = rec->UserDataLength;
	auto data = (PBYTE)rec->UserData;
	auto pointerSize = (rec->EventHeader.Flags & EVENT_HEADER_FLAG_32_BIT_HEADER) ? 4 : 8;
	ULONG start = 0;

	WCHAR text[256];
	for (DWORD i = 0; i < info->TopLevelPropertyCount && len > 0; i++) {
		EVENT_PROPERTY_INFO& pi = info->EventPropertyInfoArray[i];
		printf("  Name: %ws ", (PCWSTR)(buffer.get() + pi.NameOffset));
		if (pi.Flags == 0) {
			USHORT consumed;
			ULONG textLen = _countof(text);
			if (ERROR_SUCCESS == TdhFormatProperty(info, nullptr, pointerSize, pi.nonStructType.InType, pi.nonStructType.OutType,
				pi.length, len, data, &textLen, text, &consumed)) {
				printf("%ws\n", text);
				data += consumed;
				len -= consumed;
			}
		}
	}
}

int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 2) {
		printf("Usage: RunETW <guid>\n");
		return 0;
	}

	GUID guid;
	CLSIDFromString(argv[1], &guid);

	WCHAR sessionName[] = L"YoutubeSession";
	TRACEHANDLE hTrace;
	auto size = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(sessionName);
	auto buffer = std::make_unique<uint8_t[]>(size);
	auto props = (EVENT_TRACE_PROPERTIES*)buffer.get();
	memset(props, 0, size);

	props->Wnode.BufferSize = (ULONG)size;
	props->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
	props->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

	auto status = StartTrace(&hTrace, sessionName, props);
	if (status == ERROR_ALREADY_EXISTS) {
		ControlTrace(hTrace, sessionName, props, EVENT_TRACE_CONTROL_STOP);
		status = StartTrace(&hTrace, sessionName, props);
	}
	if (ERROR_SUCCESS != status) {
		printf("Error: %u\n", status);
		return status;
	}

	status = EnableTraceEx2(hTrace, &guid, EVENT_CONTROL_CODE_ENABLE_PROVIDER,
		TRACE_LEVEL_VERBOSE, 0, 0, 0, nullptr);
	if (ERROR_SUCCESS != status) {
		printf("Error: %u\n", status);
		return status;
	}

	EVENT_TRACE_LOGFILE etl{};
	etl.LoggerName = sessionName;
	etl.ProcessTraceMode = PROCESS_TRACE_MODE_REAL_TIME | PROCESS_TRACE_MODE_EVENT_RECORD;
	etl.EventRecordCallback = OnEvent;

	auto hParse = OpenTrace(&etl);
	if (hParse == INVALID_PROCESSTRACE_HANDLE) {
		printf("Error: %u\n", GetLastError());
		return status;
	}

	ProcessTrace(&hParse, 1, nullptr, nullptr);

	return 0;
}

