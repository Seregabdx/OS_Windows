#include <windows.h>
#include <iostream>

using namespace std;

void WINAPI ReadCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
	cout << "���������� ��������" << endl;
}

int main()
{
	system("chcp 1251");
	CHAR mes[512];
	BOOL b = FALSE;
	LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	HANDLE EVENT = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE PIPE = CreateFile(lpszPipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	int x;
	OVERLAPPED overlapped = OVERLAPPED();

    do
    {
        cout << "1. ������� ���������" << endl;
        cout << "2. ������������� �� ������������ ������" << endl;
        cout << "0. �����" << endl;
        cin >> x;
        switch (x) {

        case 1:

            overlapped.hEvent = EVENT;
            b = ReadFileEx(PIPE, (LPVOID)mes, 512, &overlapped, ReadCallback);
            if (b)
                cout << mes << endl;
            else
                cout << "������ �� �������" << endl;
            break;

        case 2:
            b = CloseHandle(PIPE);
            if (b) cout << "�� ���� ����������� �� ������������ ������" << endl;
            else cout << "�� ������� �������������" << endl;
            b = FALSE;
            break;
        case 0:
            break;
        default:
            break;
        }
    } while (x != 0);

	return 0;
}
