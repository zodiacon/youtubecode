// SimpleSharing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>

#pragma data_seg(".shared")
int x = 0;
#pragma data_seg()
#pragma comment(linker, "/section:.shared,RWS")

int main() {
	while (true) {
		printf("%d\n", x);
		Sleep(1000);
		x++;
	}
}

