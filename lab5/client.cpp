#include <windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <ctime>

int main(int argc, char* argv[]) {
    HANDLE ﻿hReadPipe1 = (HANDLE)atoi(argv[1]);
    HANDLE hWritePipe2 = (HANDLE)atoi(argv[2]);

    HANDLE hEnableRead = OpenEventA(EVENT_ALL_ACCESS, FALSE, "EnableRead");

    int arraySize, N, M;

    DWORD dwBytesRead1;
    if (!ReadFile(﻿hReadPipe1, &arraySize, sizeof(arraySize), &dwBytesRead1, NULL)) {
        _cputs("Read from the pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    _cprintf("The number %.d is read from the pipe.\n", arraySize);

    if (!ReadFile(﻿hReadPipe1, &N, sizeof(N), &dwBytesRead1, NULL)) {
        _cputs("Read from the pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    _cprintf("The number %.d is read from the pipe.\n", N);

    if (!ReadFile(﻿hReadPipe1, &M, sizeof(M), &dwBytesRead1, NULL)) {
        _cputs("Read from the pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    _cprintf("The number %.d is read from the pipe.\n", M);

    _cputs("The process finished reading from the pipe.\n");

    std::vector<long> vec;
    srand(time(NULL));

    for (int i = 0; i < arraySize; i++) {
        long num = rand() % (M - N + 1) + N;
        vec.push_back(num);
    }

    _cputs("Press any key to continue communication.\n");
    _getch();   

    std::cout << "\n";
    for (long x : vec) {
        DWORD dwBytesWritten2;
        if (!WriteFile(hWritePipe2, &x, sizeof(x), &dwBytesWritten2, NULL)) {
            _cputs("Write to file failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The number %.d is written to the pipe.\n", x);
    } 
    SetEvent(hEnableRead); 
    _cputs("The process finished writing to the pipe.\n");

    CloseHandle(﻿hReadPipe1);
    CloseHandle(hWritePipe2);
    CloseHandle(hEnableRead);

    _cputs("Press Q to exit.\n");
    while (_getch() != 'Q') {}
    return 0;
}
