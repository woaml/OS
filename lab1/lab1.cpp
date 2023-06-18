#include <iostream>
#include <windows.h>
#include <ctime>
#include <process.h>

using namespace std;

struct Data {
	int* arrayInput;
	int arrayInputSize;
	int* arrayOutput;
	int arrayOutputSize;
	int sum;
};

DWORD WINAPI worker(LPVOID arr) {
	auto* input = (Data*)arr;
	input->arrayOutput = new int[input->arrayInputSize];
	input->sum = 0;
	input->arrayOutputSize = 0;
	for (int i = 0; i < input->arrayInputSize; i++) {
		input->sum += input->arrayInput[i];
		Sleep(12);
	}
	input->arrayOutputSize = input->sum / input->arrayInputSize;
	return 0;
}

int main() {
	Data* input = new Data;
	input->arrayInput = new int[100];
	cout << "Enter size of array: ";
	cin >> input->arrayInputSize;
	srand(time(0));
	cout << "Generating random elements for the array..." << endl;
	for (int i = 0; i < input->arrayInputSize; i++) {
		input->arrayInput[i] = rand() % 100;
	}
	cout << "Array: ";
	for (int i = 0; i < input->arrayInputSize; i++) {
		cout << input->arrayInput[i] << " ";
	}
	cout << endl;

	int stopTime;
	cout << "Enter stop time (in milliseconds): ";
	cin >> stopTime;

	HANDLE	hHandle;
	HANDLE  hThread;
	DWORD	IDThread;
	hHandle = CreateThread(NULL, 0, worker, input, 0, &IDThread);
	
	if (hHandle == NULL) {
		return GetLastError();
	}

	SuspendThread(hHandle);
	Sleep(stopTime);
	ResumeThread(hHandle);

	WaitForSingleObject(hHandle, INFINITE);
	CloseHandle(hHandle);

	//hThread = (HANDLE)_beginthreadex(nullptr, 0, (_beginthreadex_proc_type)worker, input, 0, nullptr);

	cout << "Arithmetic mean of array elements: " << input->arrayOutputSize;
	delete input;
	return 0;
}
