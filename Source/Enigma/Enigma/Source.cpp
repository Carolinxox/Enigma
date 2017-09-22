#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;

bool IsOnlyInstance(LPCTSTR gameTitle) {
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);
	if (GetLastError() != ERROR_SUCCESS) {
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd) {
			// An instance of your game is already running.
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			cout << "Instance Not Detected" << endl;

			return false;
		}
	}
	cout << "Instance Detected." << endl;
	return true;
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded) {

	LPCTSTR lpDirectoryName = NULL;
	__int64 lpFreeBytesAvailable;
	__int64 lpTotalNumberOfBytes;
	__int64 lpTotalNumberOfFreeBytes;
	GetDiskFreeSpaceEx(lpDirectoryName, (PULARGE_INTEGER)&lpFreeBytesAvailable, (PULARGE_INTEGER)&lpTotalNumberOfBytes, (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
	if ((DWORDLONG)lpTotalNumberOfBytes <= diskSpaceNeeded)
	{
		cout << "Check Storage Failure: Not enough physical storage." << endl;
		return false;
	}
	else {
		cout << "Disk Space Check: " << endl;
		cout << "Requested " << diskSpaceNeeded << " bytes needed." << endl;
		cout << "There are " << (DWORDLONG)lpTotalNumberOfBytes << " available bytes." << endl;
		
		return true;
	}
}

bool CheckMemory(const DWORDLONG physicalRAMNeeded, const
	DWORDLONG virtualRAMNeeded) {
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

	cout << "RAM Requested: " << physicalRAMNeeded << "." << endl;
	cout << "The total physical RAM available: " << status.ullTotalPhys << "." << endl;
	cout << "VRAM Requested: " << virtualRAMNeeded << "." << endl;
	cout << "The total virtual RAM available: " << status.ullAvailVirtual << "." << endl;

	if (status.ullTotalPhys < physicalRAMNeeded) {
		cout << "CheckMemory Failure : Not enough physical memory." << endl;
		
		return false;
	}

	// Check for enough free memory.
	if (status.ullAvailVirtual < virtualRAMNeeded) {
		// you don’t have enough virtual memory available.
		// Tell the player to shut down the copy of Visual
		// Studio running in the background.
		cout << "CheckMemory Failure : Not enough virtual memory." << endl;

		return false;
	}
}

DWORD ReadCPUSpeed() {
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	DWORD strType = REG_SZ;
	char myBuff[256];
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0,
		KEY_READ, &hKey);
	if (lError == ERROR_SUCCESS) {
		// query the key:
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)
			&dwMHz, &BufSize);
		cout << dwMHz << " MHz CPU Speed" << endl;
	}

	long mError = RegQueryValueEx(hKey, "ProcessorNameString", NULL, &strType, (LPBYTE)&myBuff, &BufSize);

	while (mError == ERROR_MORE_DATA) {
		BufSize++;
		mError = RegQueryValueEx(hKey, "ProcessorNameString", NULL, &strType, (LPBYTE)&myBuff, &BufSize);
	}

	cout << myBuff << endl;

	return dwMHz;
}

int main()
{
	LPCTSTR engineName = "Enigma";
	IsOnlyInstance(engineName);
	DWORDLONG spaceNeeded = 3000000000;
	CheckStorage(spaceNeeded);
	DWORDLONG ramNeeded = 4;
	DWORDLONG vRamNeeded = 16;
	CheckMemory(ramNeeded, vRamNeeded);
	ReadCPUSpeed();

	system("pause");

	return 0;
}

