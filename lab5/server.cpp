#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>

int main() {
    int arraySize;
    int N, M;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;
    std::cout << "Enter N: ";
    std::cin >> N;
    std::cout << "Enter M: ";
    std::cin >> M;

    if (N >= M) {
        std::cout << "error";
        return 1;
    }

    HANDLE hWritePipe1, hReadPipe1;
    HANDLE hWritePipe2, hReadPipe2;
    HANDLE hEnableRead = CreateEventA(NULL, FALSE, FALSE, "EnableRead");
   
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    SECURITY_ATTRIBUTES sa; 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0)) {
        std::cout << "Create pipe failed.\n";
        _getch();
        return GetLastError();
    }

    if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
        std::cout << "Create pipe failed.\n";
        _getch();
        return GetLastError();
    }

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    std::wstring commandLine = L"Client.exe " + std::to_wstring((int)hReadPipe1) + L" " + std::to_wstring((int)hWritePipe2);
    if (!CreateProcess(NULL, const_cast<wchar_t*>(commandLine.c_str()), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        _cputs("Create process failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    std::cout << "\n";

    DWORD dwBytesWritten1;
    if (!WriteFile(hWritePipe1, &arraySize, sizeof(arraySize), &dwBytesWritten1, NULL)) {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    if (!WriteFile(hWritePipe1, &N, sizeof(N), &dwBytesWritten1, NULL)) {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    if (!WriteFile(hWritePipe1, &M, sizeof(M), &dwBytesWritten1, NULL)) {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }

    WaitForSingleObject(hEnableRead, INFINITE);

    std::vector<long> vec;

    for (int i = 0; i < arraySize; i++) {
        long nData;
        DWORD dwBytesRead2;
        if (!ReadFile(hReadPipe2, &nData, sizeof(nData), &dwBytesRead2, NULL)) {
            _cputs("Read from the pipe failed.\n");
            std::cout << (GetLastError()) << "\n";
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The number %.d is read from the pipe.\n", nData);
        vec.push_back(nData);
    }
    _cputs("The process finished reading from the pipe.\n");

    std::cout << "Sent array: ";
    std::cout << "[ ";
    for (const auto& x : vec) {
        std::cout << x << " ";
    }
    std::cout << "]";

    _cputs("\nThe process finished reading from the pipe.\n");
    _cputs("Press Q to exit.\n");
    while (_getch() != 'Q') {}

    CloseHandle(hWritePipe1);
    CloseHandle(hReadPipe1);
    CloseHandle(hWritePipe2);
    CloseHandle(hReadPipe2);
    CloseHandle(hEnableRead);
}
