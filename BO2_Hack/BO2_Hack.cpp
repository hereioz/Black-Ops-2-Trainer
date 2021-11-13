#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>


HANDLE hProc = NULL;
DWORD pID;

bool attachProc(char* procName) 
{
	PROCESSENTRY32 procEntry32;

	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE) 
	{
		std::cout << "Failed to take snapshot of processes" << std::endl;
		return false;
	}

	while (Process32Next(hProcSnap, &procEntry32)) 
	{
		if (!strcmp(procName, procEntry32.szExeFile)) 
		{
			std::cout << "Found Process: " << procEntry32.szExeFile << " With Process ID " << procEntry32.th32ProcessID << std::endl;
			std::cout << "Trying to Inject " << procName << std::endl;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			pID = procEntry32.th32ProcessID;
			std::cout << "Injecting Succesfully to " << procName << std::endl;
			CloseHandle(hProcSnap);
			return true;
		}
	}

	CloseHandle(hProcSnap);
	return false;
}

template <class dataType>
void wpm(dataType valToWrie, DWORD addressToWrite) 
{
	WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrie, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addressToRead)
{
	dataType rpmBuffer;

	ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);

	return rpmBuffer;
}

int main()
{
	bool Ammo = false;
	bool Cash = false;
	bool Gernade = false;
	bool Health = false;

	std::cout << "Trying to find " << "t6zm.exe (Black Ops 2)" << std::endl;
	for (;;)
	{
		if (attachProc((char*)"t6zm.exe") == true)
		{
			break;
		}
		Sleep(100);
	}

	system("cls");
	std::cout << "BO2 Hacking Guide : " << std::endl << "[F1] Infinite Ammo" << std::endl << "[F2] Infinite Cash" << std::endl << "[F3] Infinite Gernade" << std::endl << "[F4] Infinite Health";

	while (true) 
	{
		if (Cash == true) 
		{
			wpm<int>(999999, 0x234C068);
		}

		if (Ammo == true) 
		{
			wpm<int>(1337, 0x2346ECC);
			wpm<int>(1337, 0x02346ED4);
		}

		if (Gernade == true) 
		{
			wpm<int>(10, 0x02346ED0);
		}

		if (Health == true) 
		{
			wpm<int>(1000, 0x021C5868);
		}

		if (GetAsyncKeyState(VK_F2)) 
		{
			if (Cash == true) 
			{
				Cash = false;
			}
			if (Cash == false) 
			{
				Cash = true;
			}
		}

		if (GetAsyncKeyState(VK_F1))
		{
			if (Ammo == true)
			{
				Ammo = false;
			}
			if (Ammo == false)
			{
				Ammo = true;
			}
		}

		if (GetAsyncKeyState(VK_F3)) 
		{
			if (Gernade == true) 
			{
				Gernade = false;
			}
			if (Gernade == false) 
			{
				Gernade = true;
			}
		}

		if (GetAsyncKeyState(VK_F4)) 
		{
			if (Health == true) 
			{
				Health = false;
			}
			if (Health == false) 
			{
				Health = true;
			}
		}

		Sleep(50);
	}
}