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


void GetLogDrives()
{
    int n;
	char dd[4];
	system("cls");
	DWORD dr = GetLogicalDrives();

	for( int i = 0; i < 26; i++ )
	{
		n = ((dr>>i)&0x00000001);
		if( n == 1 )
		{
			dd[0] =  char(65+i); dd[1] = ':'; dd[2] = '\\'; dd[3] = 0;
			cout << "Available disk drives : " << dd << endl;
		}
	}
	_getch();
}

void DriveType()
{
    char* str;
    int d;
    cout << "Write Drive: ";
    cin >> str;
    d = GetDriveType(str);

    if( d == DRIVE_UNKNOWN ) cout << " UNKNOWN" << endl;
	if( d == DRIVE_NO_ROOT_DIR ) cout << " DRIVE NO ROOT DIR" << endl;
	if( d == DRIVE_REMOVABLE ) cout << " REMOVABLE" << endl;
	if( d == DRIVE_FIXED ) cout << " FIXED" << endl;
	if( d == DRIVE_REMOTE ) cout << " REMOTE" << endl;
	if( d == DRIVE_CDROM ) cout << " CD-ROM" << endl;
	if( d == DRIVE_RAMDISK ) cout << " RAM-DISK" << endl;
}

void GetDriveInformation()
{
    char Buffer[100];
	char SystemBuffer[100];
	unsigned long SerialNumber;
	string str;

	cout << "INPUT DRIVE ";
	cin >> str;

	BOOL Flag = GetVolumeInformationA(
	_T(str.c_str()),
	Buffer,
	100,
	&SerialNumber,
	NULL, //&MaximumComponentLength,
	NULL, //&FileSystemFlags,
	SystemBuffer,
	100
	);

	if(Flag != 0)
	{
		cout << "	Volume Name is " << Buffer << endl;
		cout << "	Volume Serial Number is " << SerialNumber << endl;
		cout << "	File System is " << SystemBuffer << endl;
	}
	else cout << "	Not Present (GetVolumeInformation)" << endl;
}

void DriveFreeSpace()
{
    BOOL Result;
    string str;
    DWORD SectPerClust, BytesPerSect, FreeClusters, TotalClusters;
    cout << "Enter disk: ";
    cin >> str;
    Result = GetDiskFreeSpace(_T(str.c_str()), &SectPerClust, &BytesPerSect, &FreeClusters, &TotalClusters);

    printf("Sector per cluster = %ul\n", SectPerClust);
    printf("Bytes per sector = %ul\n", BytesPerSect);
    printf("Free cluster = %ul\n", FreeClusters);
    printf("Total cluster = %ul\n", TotalClusters);

}

void DrivesInfo()
{
    char key = 0;
    system("cls");
    cout << "\n";

    while(key!='4')
    {
        cout << "Choose a function to work\n";
        cout << "1) GetDriveType\n";
        cout << "2) GetVolumeInformation\n";
        cout << "3) GetDiskFreeSpace\n";
        cout << "4) EXIT\n";

        cin >> key;

        switch(key)
        {
            case '1': DriveType(); break;
            case '2': GetDriveInformation(); break;
            case '3': DriveFreeSpace(); break;
        }
        cout << "\n--------------------------------\n";
    }
}

void CreateDir()
{
    string str;
    cout << "Enter the path to the directory:  ";
    cin >> str;
    if (CreateDirectory(_T(str.c_str()),NULL))
		cout << "directory create" << endl;
	else
		cout << "error create directory" << endl;
}

void DeleteDir()
{
    string str;
    cout << "Enter the path to the directory: ";
    cin >> str;
    if (RemoveDirectory(_T(str.c_str())))
		cout << "directory remove" << endl;
	else
		cout << "error remove directory" << endl;
}

void Creation_Deletion()
{
    char key = 0;

    system("cls");
    cout << "\n";

    while(key!='3')
    {
        cout << "Choose a function to work\n";
        cout << "1) CreateDirectory\n";
        cout << "2) RemoveDirectory\n";
        cout << "3) EXIT\n";

        cin >> key;

        switch(key)
        {
            case '1': CreateDir(); break;
            case '2': DeleteDir(); break;
        }
        cout << "\n--------------------------------\n";
    }
}

void CreateF()
{
    string str;
    system("cls");
    cout << "Enter the path to the directory: ";
    cin >> str;
    HANDLE hFile = CreateFileA(_T(str.c_str()), GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile)
        cout << endl << "File was created: " << str;
    else
        cout << "Error: " << GetLastError();
    CloseHandle(hFile);
}

void CopyF()
{
    string str1, str2;

    cout << "Input existing file name: ";
    cin >> str1;
    cout << "\n Input new file name: ";
    cin >> str2;
    CopyFile (_T(str1.c_str()), _T(str2.c_str()), false);
}

void MoveF()
{
    string str1, str2;

    cout << "Input the current name of a file or directory on the local computer: ";
    cin >> str1;
    cout << "\nInput new file or directory name: ";
    cin >> str2;
    MoveFile(_T(str1.c_str()), _T(str2.c_str()));
}

void MoveFEx()
{
    string str1, str2;

    cout << "Input the current name of a file or directory on the local computer: ";
    cin >> str1;
    cout << "\nInput new file or directory name: ";
    cin >> str2;
    MoveFileEx(_T(str1.c_str()), _T(str2.c_str()),MOVEFILE_WRITE_THROUGH);
}

void Copy_Move()
{
    char key = 0;

    system("cls");
    cout << "\n";

    while(key!='4')
    {
        cout << "Choose a function to work\n";
        cout << "1) CopyFile\n";
        cout << "2) MoveFile\n";
        cout << "3) MoveFileEx\n";
        cout << "4) EXIT\n";

        cin >> key;

        switch(key)
        {
            case '1': CopyF(); break;
            case '2': MoveF(); break;
            case '3': MoveFEx(); break;
        }
        cout << "\n--------------------------------\n";
    }
}

void GetFAttributes()
{
    string str;
    DWORD key;

    cout << "Enter a file name or directory to check if it is read-only: ";
    cin >> str;
    key = GetFileAttributes(_T(str.c_str()));
    if (key & FILE_ATTRIBUTE_READONLY)
    {
        printf ("\nThe attribute Read_Only is established");
    }
    else
    {
        printf ("\nThe attribute Read_Only isn't established");
    }
}

void SetFAttributes()
{
    string str;
    DWORD key;
    char keey = '0';

    cout << "Enter a file name or directory: ";
    cin >> str;
    while(keey != '3')
    {
        cout << "\nEnter key: ";
        cout << "\n1. Established READONLY\n";
        cout << "2. Established HIDDEN\n";
        cout << "3. EXIT\n";
        cin >> keey;

        switch(keey)
        {
            case '1': SetFileAttributes(_T(str.c_str()), FILE_ATTRIBUTE_READONLY); break;
            case '2': SetFileAttributes(_T(str.c_str()), FILE_ATTRIBUTE_HIDDEN); break;
        }
    }
}

void GetFInformationByHandle()
{
    BY_HANDLE_FILE_INFORMATION info;
    string str;
    bool key;
    HANDLE file;

    cout << "Enter a file name or directory for information: ";
    cin >> str;
    file = CreateFileA(_T(str.c_str()), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    key = GetFileInformationByHandle(file, &info);
    cout << "File attributes: \n";
    if (info.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
        printf("Archive \n");
    }
    else printf("Non Archive \n");
    if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        printf("Directory \n");
    }
    else printf("Non Directory \n");
    if (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
        printf("Read-Only \n");
    }
    else printf("Non Read-Only \n");
    cout << "_____________________\n";
    cout << "\nFile number of links: " << info.nNumberOfLinks;
    cout << "\nFile volume serial number: " << info.dwVolumeSerialNumber;
}

void GetFTime()
{
    string str;
    bool key;
    HANDLE file;
    FILETIME Create;
    SYSTEMTIME UTC;

    cout << "Enter a file name or directory to find out the creation time: ";
    cin >> str;
    file = CreateFileA(_T(str.c_str()), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    GetFileTime(file, &Create, NULL, NULL);
    FileTimeToSystemTime(&Create, &UTC);
    printf("Created on: %02d/%02d/%d %02d:%02d\n", UTC.wDay, UTC.wMonth, UTC.wYear, UTC.wHour, UTC.wMinute);
    CloseHandle(file);
}

void SetFTime()
{
    SYSTEMTIME StrSysTF;
    FILETIME time ;
    string str;
    HANDLE file;
    bool f;
    SYSTEMTIME stUTC;

    GetSystemTime(&StrSysTF);
    SystemTimeToFileTime(&StrSysTF, &time);
    cout << "Enter a file name or directory to change the creation time: ";
    cin >> str;
    file = CreateFileA(_T(str.c_str()), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    f = SetFileTime(file, &time, &time, &time);
    FileTimeToSystemTime(&time, &stUTC);
    printf("Created on: %02d/%02d/%d %02d:%02d\n", stUTC.wDay, stUTC.wMonth, stUTC.wYear, stUTC.wHour, stUTC.wMinute);
    CloseHandle(file);
}

void File_Attributes()
{
    char key = 0;

    system("cls");
    cout << "\n";

    while(key!='6')
    {
        cout << "Choose a function to work\n";
        cout << "1) GetFileAttributes\n";
        cout << "2) SetFileAttributes\n";
        cout << "3) GetFileInformationByHandle\n";
        cout << "4) GetFileTime\n";
        cout << "5) SetFileTime\n";
        cout << "6) EXIT\n";

        cin >> key;

        switch(key)
        {
            case '1': GetFAttributes(); break;
            case '2': SetFAttributes(); break;
            case '3': GetFInformationByHandle(); break;
            case '4': GetFTime(); break;
            case '5': SetFTime(); break;
        }
        cout << "\n--------------------------------\n";
    }
}

int main()
{
    char key = 0;

    while(key!='7')
    {
        system("cls");
        cout << "Select lab item\n";
        cout << "1) Display disk list\n";
        cout << "2) Disc information output\n";
        cout << "3) Creating and deleting specified directories\n";
        cout << "4) Creating files in new directories\n";
        cout << "5) Copying and moving files between directories\n";
        cout << "6) Analysis and modification of file attributes\n";
        cout << "7) EXIT\n";

        cin >> key;

        switch(key)
        {
            case '1': GetLogDrives(); break;
            case '2': DrivesInfo(); break;
            case '3': Creation_Deletion(); break;
            case '4': CreateF(); break;
            case '5': Copy_Move(); break;
            case '6': File_Attributes(); break;
        }
    }
    return 0;
}
