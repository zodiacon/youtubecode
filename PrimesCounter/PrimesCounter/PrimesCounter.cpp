// PrimesCounter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <Windows.h>
#include <memory>
#include <ppl.h>
#include <atomic>

struct ThreadData {
	int from;
	int to;
	int count;
};

bool IsPrime(int n) {
	int limit = (int)sqrt(n);
	for (int i = 2; i <= limit; i++)
		if (n % i == 0)
			return false;

	return true;
}

DWORD WINAPI CalcPrimesThread(PVOID p) {
	printf("Worker thread %u...\n", GetCurrentThreadId());
	auto data = (ThreadData*)p;
	int count = 0;

	for (int i = data->from; i <= data->to; i++)
		if (IsPrime(i))
			count++;
	data->count = count;
	return 0;
}

int CalcPrimes(int from, int to, int threads) {
	int chunk = (to - from + 1) / threads;

	auto handles = std::make_unique<HANDLE[]>(threads);
	auto data = std::make_unique<ThreadData[]>(threads);

	for (int i = 0; i < threads; i++) {
		data[i].from = from + (i * chunk);
		data[i].to = i == threads - 1 ? to : data[i].from + chunk - 1;
		handles[i] = CreateThread(nullptr, 0, CalcPrimesThread, &data[i], 0, nullptr);
	}
	WaitForMultipleObjects(threads, handles.get(), TRUE, INFINITE);

	int count = 0;
	for (int i = 0; i < threads; i++) {
		count += data[i].count;
		CloseHandle(handles[i]);
	}
	return count;
}

int CalcPrimesPpl(int from, int to, int threads) {
	std::atomic<int> count = 0;
	concurrency::parallel_for(from, to + 1, [&](int n) {
		if (IsPrime(n))
			count++;
		});
	return count;
}

int main(int argc, const char* argv[]) {
	int threads = 1;
	if (argc > 1)
		threads = atoi(argv[1]);

	printf("Using %d threads\n", threads);
	auto start = GetTickCount64();
	int count = CalcPrimesPpl(3, 35000000, threads);
	auto elapsed = GetTickCount64() - start;
	printf("Primes: %d Elapsed: %d msec\n", count, (int)elapsed);

	return 0;
}

