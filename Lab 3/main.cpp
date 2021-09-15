#include <windows.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")
using namespace std;

int main()
{
	system("chcp 1251");
	CHAR mes[512];
	BOOL b=FALSE;
	LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	OVERLAPPED overlapped = OVERLAPPED(), olPipe = OVERLAPPED();
	HANDLE EVENT = CreateEvent(NULL,FALSE,FALSE,NULL);
	HANDLE PIPE= CreateNamedPipe(lpszPipename, PIPE_ACCESS_DUPLEX , PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

	int x;


	if (EVENT != INVALID_HANDLE_VALUE && PIPE != INVALID_HANDLE_VALUE)
	{
		do
		{
			cout << "1. �������������� � ������������ ������" << endl;
			cout << "2. ��������� ���������" << endl;
			cout << "3. ������������� �� ������������ ������" << endl;
			cout << "0. �����" << endl;
			cin >> x;
			switch (x) {

			case 1:
				olPipe.hEvent = EVENT;

				b = ConnectNamedPipe(PIPE, &olPipe);
				WaitForSingleObject(EVENT, INFINITE);

				cout << GetLastError() << endl;
				if (b) cout << "����������� ��������" << endl;
				else cout << "�� ������� ������������ � ������������ ������"<< endl;
				break;
			case 2:
				if (b == FALSE) cout << "��� ����������" << endl;
				else {

					cout << "������� ���������:";
					cin >> mes;

					overlapped.hEvent = EVENT;
					b=WriteFile(PIPE, (LPCVOID)mes, 512, NULL, &overlapped);

					if (WaitForSingleObject(EVENT, 20000) == WAIT_OBJECT_0 && b)
						cout << "������ �������" << endl;
					else
						cout << "������ �� �������"<<endl;

				}
				cout << endl;
				break;
			case 3:
				b = DisconnectNamedPipe(PIPE);
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
	}
	else
		cout << "�� ������� ������� ����������� �����, ������������� ���������" << endl;
	if (PIPE != INVALID_HANDLE_VALUE)
		CloseHandle(PIPE);
	if (EVENT != INVALID_HANDLE_VALUE)
		CloseHandle(EVENT);


	return 0;
}
