#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>
#define PG_SIZE 4096 //������ ��������
#define PAGES_COUNT 16 //���-�� ������� (3+0+8+5)

#pragma comment(lib, "winmm.lib")

using namespace std;
//���� �� ����� � ������ ��� ��������
LPCTSTR  LOG = TEXT("D:\\Qt\\LOG_READER.txt");
LPCTSTR NAME = TEXT("READER_MUTEX");
//���� �� �������� �����
TCHAR mname[] = TEXT("Local\\MyFileMapObj");
int main()
{


	srand(time(NULL));
	HANDLE WriteSemaphores[PAGES_COUNT];
	HANDLE ReadSemaphores[PAGES_COUNT];
	HANDLE READER_MUTEX = OpenMutex(MUTEX_MODIFY_STATE | SYNCHRONIZE, FALSE, NAME); // �������� ��������� ��������� �� �����
	HANDLE Mapping = OpenFileMapping(GENERIC_READ, FALSE, mname);
	LPVOID MAP_OF_FILE = MapViewOfFile(Mapping, FILE_MAP_READ, 0, 0, PG_SIZE * PAGES_COUNT);
	char msg[PG_SIZE];

	DWORD CurrentPage;
	string temp;
	for (int i = 0; i < PAGES_COUNT; i++) {
		//�������� ��������� ��������� �� �����
		temp = "WRITE_SEM_ID_" + to_string(i);
		WriteSemaphores[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, (LPCTSTR)(temp.c_str()));
		temp = "READ_SEM_ID_" + to_string(i);
		ReadSemaphores[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, (LPCTSTR)(temp.c_str()));
	}
	ofstream Log;
	Log.open(LOG, fstream::out | fstream::app);	//���� � ������

	VirtualLock(MAP_OF_FILE, PG_SIZE * PAGES_COUNT); //���������� ������� �������� ������ � ����������� ������

	while (true) {
		//��������� ������� ��������
		CurrentPage = WaitForMultipleObjects(PAGES_COUNT, ReadSemaphores, FALSE, INFINITE) - WAIT_OBJECT_0;
		//������������� �������� � ������ � ����
		WaitForSingleObject(READER_MUTEX, INFINITE);
		{
			Log << "�����: " << (timeGetTime() / 1000.0) << "| ID ��������:  " << GetCurrentProcessId() << "| �������� ����� ������ �� ���: " << CurrentPage << endl;
		}
		//������������ ��������
		ReleaseMutex(READER_MUTEX);
		//������ � ������
		CopyMemory(msg,(LPVOID)((intptr_t)MAP_OF_FILE + (CurrentPage * PG_SIZE)), PG_SIZE);
		//���������
		Sleep(500 + (rand() % 1000));
		//������������� �������� � ������ � ����
		WaitForSingleObject(READER_MUTEX, INFINITE);
		{
			Log << "�����: " << (timeGetTime() / 1000.0) << "| ID ��������:  " << GetCurrentProcessId() << "| �������� �������� ������ �� ���: " << CurrentPage << "| ���������: " << msg[0] << endl;
		}
		//������������ ��������
		ReleaseMutex(READER_MUTEX);
		//������������ �������� - ��������� �� ������
		ReleaseSemaphore(WriteSemaphores[CurrentPage], 1, NULL);

	}
	system("pause");

	CloseHandle(Mapping);
	return 0;
}
