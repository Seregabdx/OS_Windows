#include <iostream>
#include <Windows.h>

using namespace std;

int N = 100000000;
int Block_Size = 830602*10;
int num_Threads;

typedef struct                  // structure with parameters for the  function
{
    volatile long* const cnt;   // number of the block to execute
    double ans;                 // the response of each  is stored here
} MYDATA;

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
    MYDATA* Data = (MYDATA*)lpParam;

    int flag = 1;
    int start = 0;
    int end = 0;
    double result = 0;
    double sum = 0;
    double x = 0;
    long Count = 0;
    while (flag==1)
    {
        Count = InterlockedIncrement(Data->cnt) - 1;
        start = Block_Size * Count;
        end = Block_Size * (Count + 1);

        if (start < N)
        {
            sum = 0;
            x = 0;
            for (int i = start; i < N && i < end; ++i)
            {
                x = (i + 0.5) * (1.0 / N);
                sum += 4 / (1 + x * x);
            }
            result += sum;
        }
        else
        {
            flag = 0;
        }
    }
    Data->ans = result;

    return 0;
}

void winApi(int arg)
{
    num_Threads = arg;
    double sum = 0;
    MYDATA** Data = new MYDATA*[num_Threads];
    LPHANDLE Thread = new HANDLE[num_Threads];
    LPDWORD Thread_Id = new DWORD[num_Threads];
    long* cnt = new long;   // the creation of the atomic variable
    *cnt = 0;               // initialization
    for (int i = 0; i < num_Threads; ++i)
    {
        Data[i] = new MYDATA{ cnt, 0 }; // One "cnt" for all threads
        Thread[i] = CreateThread(NULL, 0, MyThreadFunction, Data[i], CREATE_SUSPENDED, &Thread_Id[i]);
    }
    unsigned int start = GetTickCount();
    for (int i = 0; i < num_Threads; ++i)
    {
        ResumeThread(Thread[i]);
    }
    WaitForMultipleObjects(num_Threads, Thread, TRUE, INFINITE);

    sum = 0;
    for (int i = 0; i < num_Threads; ++i)
    {
        sum += Data[i]->ans;
    }

    sum = sum * (1.0 / N);
    unsigned int end = GetTickCount();

    cout << "\nTime:" << end - start << '\n';
    cout.precision(10);
    cout << "Pi = " << sum << '\n';

    for (int i = 0; i < num_Threads; ++i)
    {
        CloseHandle(Thread[i]);
    }
    delete[] Data;
}

int main()
{
    cout << "For 1 streams: ";
    winApi(1);
    cout << "\nFor 2 streams: ";
    winApi(2);
    cout << "\nFor 4 streams: ";
    winApi(4);
    cout << "\nFor 8 streams: ";
    winApi(8);
    cout << "\nFor 16 streams: ";
    winApi(16);
}
