// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

int main() {
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
		return -1;

	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return -1;

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	//addr.sin_addr.S_un.S_addr = htonl(0x7f000001);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	if (connect(sock, (const sockaddr*)&addr, sizeof(addr)) < 0)
		return -1;

	for (;;) {
		char text[256]{};
		printf("Text: ");
		gets_s(text);
		if (send(sock, text, (int)strlen(text) + 1, 0) < 0)
			break;

		char reply[256]{};
		int size = recv(sock, reply, sizeof(reply), 0);
		if (size < 0)
			break;

		printf("Received: %s\n", reply);
	}
	closesocket(sock);
	return 0;
}
