#include <iostream>
#include <windows.h>
using namespace std;

CRITICAL_SECTION cs1, cs2;
HANDLE hSumEvent;
char* finalArray;
double sumResult = 0;
int k;

struct Data {
    int arraySize;
    char* originalArray;
    int k;
};

DWORD WINAPI workThread(void* obj) {
    EnterCriticalSection(&cs1);

    Data* data = (Data*)obj;
    char* arr_ = data->originalArray;

    cout << "--- workThread is started" << endl;

    int timeInterval;
    cout << "Enter time interval to rest after preparing each element: ";
    cin >> timeInterval;

    finalArray = new char[data->arraySize];
    int finalArraySize = 0;
    for (int i = 0; i < data->arraySize; i++) {
        if (isdigit(arr_[i])) {
            finalArray[finalArraySize] = arr_[i];
            finalArraySize++;
        }       
        Sleep(timeInterval);
    }

    if (finalArraySize < data->k) {
        data->k = finalArraySize;
    }

    data->arraySize = finalArraySize;
    cout << "Final array elements: ";

    for (int i = 0; i < data->arraySize; i++) {
        cout << finalArray[i] << " ";
    }
    cout << endl;
   
    cout << "--- workThread is finished" << endl << endl;
    LeaveCriticalSection(&cs1);
    return 0;
}

DWORD WINAPI sumElementThread(void* obj) {
    EnterCriticalSection(&cs2);
  
    cout << "--- sumElementThread is started" << endl;

    Data* data = (Data*)obj;
 
    for (int i = 0; i < data->k; i++) {
        sumResult += finalArray[i] - '0';
    }

    cout << "Sum result: " << sumResult << endl;
    cout << "Arithmetic mean up to position k: " << sumResult / data->k << endl;

    LeaveCriticalSection(&cs2);
    
    cout << "--- sumElementThread is finished" << endl;

    SetEvent(hSumEvent);
    return 0;
}

int main() {
    int arraySize;
    cout << "Enter array size: ";
    cin >> arraySize;

    cout << "Enter array elements: ";
    char* originalArray = new char[arraySize];

    for (int i = 0; i < arraySize; i++) {
        std::cin >> originalArray[i];
    }

    cout << "Original array size: " << arraySize << std::endl;

    cout << "Original array elements: ";
    for (int i = 0; i < arraySize; i++) {
        cout << originalArray[i] << " ";
    }
    cout << endl;

    Data* data = new Data();
    data->originalArray = originalArray;
    data->arraySize = arraySize;

    InitializeCriticalSection(&cs1);
    InitializeCriticalSection(&cs2);    

    hSumEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    HANDLE hThreadWork;
    DWORD IDThreadWork;
    hThreadWork = CreateThread(NULL, 0, workThread, (void*)data, CREATE_SUSPENDED, &IDThreadWork);

    HANDLE hThreadSum;
    DWORD IDThreadSum;

    hThreadSum = CreateThread(NULL, 0, sumElementThread, (void*)data, CREATE_SUSPENDED, &IDThreadSum);

    if (hThreadWork == nullptr || hThreadSum == nullptr) {
        return GetLastError();
    }

    cout << endl;
    cout << "Enter k: ";
    cin >> k;
    data->k = k;
    EnterCriticalSection(&cs2);
    IDThreadWork = ResumeThread(hThreadWork);
    IDThreadSum = ResumeThread(hThreadSum);
    Sleep(1);

    EnterCriticalSection(&cs1);
    cout << "Elements of array: ";
    for (int i = 0; i < data->arraySize; i++) {
        std::cout << finalArray[i] << " ";
    }
    cout << endl;
    cout << endl;

    LeaveCriticalSection(&cs1);
    LeaveCriticalSection(&cs2);

    WaitForSingleObject(hSumEvent, INFINITE); 
   
    cout << "Result of SumElement: " << sumResult << endl;

    DeleteCriticalSection(&cs1);
    DeleteCriticalSection(&cs2);  

    WaitForSingleObject(hThreadWork, INFINITE); 
    WaitForSingleObject(hThreadSum, INFINITE);

    CloseHandle(hThreadWork);
    CloseHandle(hThreadSum);
    return 0;
}
