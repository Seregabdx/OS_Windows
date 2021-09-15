#define _WIN32_WINNT 0x0501

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>

#ifdef _UNICODE
    #define _T(c) L##c
    #define TEXT(c) L##c
#else
    #define _T(c) c
    #define TEXT(c) c
#endif // _UNICODE

using namespace std;

int Size, Num, Quantity_Operations;
LARGE_INTEGER FSize;
char** Buffer;
HANDLE File1;
HANDLE File2;
OVERLAPPED *OVERLAPPED2, *OVERLAPPED1;
int Count;

void WINAPI  ReadCb(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

void WINAPI WriteCb(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    std::cout << "WRITE: " << (int)lpOverlapped->hEvent << '\n';
    Count++;

    int index = (int)lpOverlapped->hEvent;

    OVERLAPPED1[index].Offset = lpOverlapped->Offset + Size * Num;
    OVERLAPPED1[index].OffsetHigh = lpOverlapped->OffsetHigh;

    if(OVERLAPPED1[index].Offset < FSize.QuadPart)
        ReadFileEx(File1, Buffer[index], Size, &OVERLAPPED1[index], ReadCb);
}

void WINAPI ReadCb(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{

    int index = (int)lpOverlapped->hEvent;
    Count++;
    std::cout << "READ: " << index << '\n';

    OVERLAPPED2[index].hEvent = lpOverlapped->hEvent;
    OVERLAPPED2[index].Offset = lpOverlapped->Offset;
    OVERLAPPED2[index].OffsetHigh = lpOverlapped->OffsetHigh;
    WriteFileEx(File2, Buffer[index], Size, &OVERLAPPED2[index], WriteCb);
}

int main()
{
    Count = 0;
    int key;
    string str;
    cout << "Input first name ";
    cin >> str;

    HANDLE File1 = CreateFileA(_T(str.c_str()), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
    if (File1)
        cout << endl << "File was opened: " << str;
    else
        cout << "Error: " << GetLastError();

    cout << endl << "Input second name ";
    cin >> str;

    HANDLE File2 = CreateFileA(_T(str.c_str()), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);
    if (File2)
        cout << endl << "File was created: " << str;
    else
        cout << "Error: " << GetLastError();

    int value = 3;

    Size = 4096 * value;
    Num = 2;
    GetFileSizeEx(File1, &FSize);
    Quantity_Operations = 2 * ((FSize.QuadPart + Size - 1) / Size);

    OVERLAPPED1 = new OVERLAPPED[Size];
    OVERLAPPED2 = new OVERLAPPED[Size];
    Buffer = new char* [Size];

    for (auto i = 0; i < Num; ++i)
        Buffer[i] = new char[Size];

    /*for (auto i = 0; i < Num; ++i)
        for (int j = 0; j < Size; ++j)
            Buffer[i][j] = '\0';
    */
    LARGE_INTEGER cur;
    cur.QuadPart = 0;

    DWORD Start_Time = GetTickCount();
    for (auto i = 0; i < Num; ++i)
    {
        //cout << i << " ";
        OVERLAPPED1[i].hEvent = (HANDLE)i;
        OVERLAPPED1[i].Offset = cur.LowPart;
        OVERLAPPED1[i].OffsetHigh = 0;

        if (cur.QuadPart < FSize.QuadPart) {
            key = ReadFileEx(File1, Buffer[i], Size, &OVERLAPPED1[i], (LPOVERLAPPED_COMPLETION_ROUTINE)ReadCb);
        }
        cur.QuadPart += Size;
    }

    while(Count > Quantity_Operations)
    {
        SleepEx(INFINITE, true);
        cout << "Xuy ";
    }


    DWORD End_Time = GetTickCount();

    std::cout << Count << " / " << Quantity_Operations << '\n';
    SetFilePointerEx(File2, FSize, 0, FILE_BEGIN);
    SetEndOfFile(File2);

    std::cout << "Time to copy: " << End_Time - Start_Time << " \n";
    delete[] OVERLAPPED1, OVERLAPPED2;
    for (auto i = 0; i < Num; ++i)
        delete[] Buffer[i];
    delete[] Buffer;

            if (!CloseHandle(File1))
            {
                auto code = GetLastError();
                std::cout << "ERROR: " << code << '\n';
                std::cout << "ERROR: Could not close the handle!\n";
            }

            if (!CloseHandle(File2))
            {
                auto code = GetLastError();
                std::cout << "ERROR: " << code << '\n';
                std::cout << "ERROR: Could not close the handle!\n";
            }
    return 0;
}
