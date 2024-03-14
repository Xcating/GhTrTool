#include "stdafx.h"
#include "Pvz.h"
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <chrono>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <\GhTrTool\GhTrTool\GhTrTool\json.hpp>
CPvz::CPvz()
{
}

CPvz::~CPvz()
{
}
// 获取游戏的 PID
DWORD CPvz::GetGamePid()
{
	HWND hWnd = ::FindWindow(NULL, GAME_NAME);

	if (hWnd == NULL)
	{
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::wstring wstr = (std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l - [已被GhTrTool修改] [ver.0.11p] [" << millis << L"]").str();
		HWND hWnd = ::FindWindow(NULL, wstr.c_str());
		if (hWnd == NULL)
		{
			return -1;
		}
	}

	DWORD dwPid = 0;
	::GetWindowThreadProcessId(hWnd, &dwPid);

	return dwPid;
}
//获取模块基址
DWORD GetModuleBaseAddress(HANDLE hProcess, const wchar_t* moduleName) {
	MODULEENTRY32W moduleEntry = { sizeof(moduleEntry) };
	DWORD baseAddress = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(hProcess));
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		if (Module32FirstW(hSnapshot, &moduleEntry)) {
			do {
				if (wcscmp(moduleEntry.szModule, moduleName) == 0) {
					baseAddress = reinterpret_cast<DWORD>(moduleEntry.modBaseAddr);
					break;
				}
			} while (Module32NextW(hSnapshot, &moduleEntry));
		}
		CloseHandle(hSnapshot);
	}

	return baseAddress;
}
//获取模块基址(函数通用)
DWORD get_baseAddress(HANDLE hProcess)
{
	DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
	if (baseAddress == 0)
		baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
	return baseAddress;
}
//创建线程，运行指定内存
VOID RunTheMemory(DWORD dwPid, DWORD codeCaveOffset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	if (baseAddress == 0) {
		CloseHandle(hProcess);
		return;
	}

	DWORD targetAddress = baseAddress + codeCaveOffset;
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)targetAddress, NULL, 0, NULL);
	if (hThread != NULL) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	else {
		MessageBox(NULL, L"游戏未找到或没有访问权限", L"提示", MB_OK);
	}
	CloseHandle(hProcess);
}

//
BOOL check_dwPid(DWORD dwPid)
{
	if (dwPid == -1)
	{
		MessageBox(NULL, L"游戏未找到，请打开游戏后点击功能", L"Info", MB_OK);
		return false;
	}
	return true;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD dwPid;
	GetWindowThreadProcessId(hwnd, &dwPid);
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::wstring wstr = (std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l - [已被GhTrTool修改] [ver.0.11p] [" << millis << L"]").str();
	if (dwPid == lParam) {
		SetWindowText(hwnd, wstr.c_str());
		return FALSE;
	}
	return TRUE;
}
DWORD ReadTOMemory(DWORD dwPid, DWORD targetAddress) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return 0;
	}
	DWORD value = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &value, sizeof(DWORD), NULL);
	return value;
}
// 通用的内存写入函数
bool WriteToMemory(DWORD dwPid, DWORD offset, const char* data, size_t size) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	if (baseAddress == 0) {
		CloseHandle(hProcess);
		return false;
	}
	DWORD targetAddress = baseAddress + offset;
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, data, size, NULL);
	CloseHandle(hProcess);
	return result;
}
//修改内存保护措施，用于写入空地址
void protectAddress(DWORD dwPid, DWORD offset) {
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD dwOldProtect;
	DWORD baseAddress = get_baseAddress(hProc);
	DWORD targetAddress = baseAddress + offset;
	VirtualProtectEx(hProc, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
}
//在指定内存写入Jump操作码
bool WriteJump(DWORD dwPid, DWORD sourceOffset, DWORD targetOffset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return false;
	}

	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD sourceAddress = baseAddress + sourceOffset;
	DWORD targetAddress = baseAddress + targetOffset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)sourceAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	DWORD jumpOffset = targetAddress - (sourceAddress + 5);
	BYTE jumpCode[5] = { 0xE9, jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, (LPVOID)sourceAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Je或者Jne操作码
bool WriteConditionJump(DWORD dwPid, DWORD sourceOffset, DWORD targetOffset,bool JE) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return false;
	}

	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD sourceAddress = baseAddress + sourceOffset;
	DWORD targetAddress = baseAddress + targetOffset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)sourceAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	DWORD jumpOffset = targetAddress - (sourceAddress + 5);
	BYTE jumpCode[6] = {
	0x0F,
	static_cast<BYTE>(JE ? 0x84 : 0x85),
	static_cast<BYTE>(jumpOffset & 0xFF),
	static_cast<BYTE>((jumpOffset >> 8) & 0xFF),
	static_cast<BYTE>((jumpOffset >> 16) & 0xFF),
	static_cast<BYTE>((jumpOffset >> 24) & 0xFF)
	};


	bool result = WriteProcessMemory(hProcess, (LPVOID)sourceAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Push操作码
bool WritePush(DWORD dwPid, DWORD Number,DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD sourceAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)sourceAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	BYTE jumpCode[2] = { 0x6A, Number & 0xFF};

	bool result = WriteProcessMemory(hProcess, (LPVOID)sourceAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Mov Ecx,[PlantsVsZombies.exe+0x00000]操作码
bool WriteMovECX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0x8B, 0x0D ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Mov Eax,[PlantsVsZombies.exe+0x00000]操作码
bool WriteMovEAX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0xA1 ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Mov EAX,0x00000 操作码
bool WriteMovEAXToHEX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0xB3 ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Mov [PlantsVsZombies.exe+0x00000], 00000000 操作码
bool WriteMOVXZero(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0xC7, 0x05 ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF ,0x00, 0x00,0x00, 0x00 };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Cmp [PlantsVsZombies.exe+0x00000], 00000000 操作码
bool WriteCMPXZero(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0x83, 0x3D ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF ,0xF };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Inc [PlantsVsZombies.exe+0x00000] 操作码
bool WriteINC(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + offset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	DWORD jumpOffset = Address;
	BYTE jumpCode[] = { 0xFF, 0x05 ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };
	bool result = WriteProcessMemory(hProcess, (LPVOID)targetAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
//在指定内存写入Call 操作码
bool WriteCall(DWORD dwPid, DWORD sourceOffset, DWORD targetOffset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return false;
	}
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD sourceAddress = baseAddress + sourceOffset;
	DWORD targetAddress = baseAddress + targetOffset;
	DWORD dwOldProtect = 0;
	VirtualProtectEx(hProcess, (LPVOID)sourceAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	DWORD jumpOffset = targetAddress - (sourceAddress + 5);
	BYTE jumpCode[5] = { 0xE8, jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, (LPVOID)sourceAddress, jumpCode, sizeof(jumpCode), NULL);
	CloseHandle(hProcess);
	return result;
}
BOOL check_battlefield(DWORD dwPid) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x297C54);
	DWORD checkPoint = ReadTOMemory(dwPid, eBaseAddress + 0x708);
	if (checkPoint == 0)
		return false;
	else
		return true;
	return false;
	CloseHandle(hProcess);
}
//检查是否可以写入
VOID check_result(BOOL result)
{
	if (result)
		MessageBox(NULL, TEXT("写入内存成功"), TEXT("Info"), MB_OK);
	else
		MessageBox(NULL, TEXT("写入失败，请联系作者"), TEXT("Error"), MB_OK | MB_ICONERROR);
}
VOID CPvz::WriteConfig()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	wchar_t szExePath[MAX_PATH];
	if (GetModuleFileNameEx(hProcess, NULL, szExePath, MAX_PATH) == 0)
	{
		CloseHandle(hProcess);
		return;
	}
	std::filesystem::path exePath(szExePath);
	std::filesystem::path configPath = exePath.parent_path() / "definition" / "config.json";
	nlohmann::json configJson2;
	if (!std::filesystem::exists(configPath)) {
		std::ofstream configFileOut(configPath);
		configJson2["isCheat"] = true;
		configFileOut << configJson2.dump(4);
		configFileOut.close();
	}
	EnumWindows(EnumWindowsProc, dwPid);
}

// 修改阳光的值
VOID CPvz::ModifySunValue(DWORD dwSun) //Sun指的是阳光
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL); 
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x384), &dwSun, sizeof(DWORD), NULL); 
	check_result(result);
	CloseHandle(hProcess);
}

// 修改卡槽数量
VOID CPvz::SeedPacket(DWORD dwSP) //SP指的是SeedPacket，种子包
{

	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL); //一层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x14C), &dwNum, sizeof(DWORD), NULL); //二层偏移
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x24), &dwSP, sizeof(DWORD), NULL); //三层偏移
	check_result(result);
	CloseHandle(hProcess);
}

// 修改卡槽数量
VOID CPvz::ModifySeedPacket(DWORD dwID,DWORD dwNum) //SP指的是SeedPacket，种子包 dwId指要替换的id，dwNum，要替换的卡槽号
{
	dwNum--;
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum1 = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum1, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + 0x708), &dwNum1, sizeof(DWORD), NULL); //一层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + 0x14C), &dwNum1, sizeof(DWORD), NULL); //二层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + 0x2C), &dwNum1, sizeof(DWORD), NULL); //三层偏移
	WriteProcessMemory(hProcess, (LPVOID)(dwNum1 + 0x1C + 0x38*dwNum), &dwID, sizeof(DWORD), NULL); //四层偏移
	CloseHandle(hProcess);
}

// 使用通用函数的种植不减阳光
VOID CPvz::SunNop(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* nop = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x29\xBE\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x9C439, nop, 6);
}

// 种植免冷却
VOID CPvz::NoCd(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x42\x24\x00\x00\x00\x00\x90\x90\x90" : "\x89\x42\x24\xC7\x42\x20\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xEA679, patch1, 10);
	const char* patch2 = (dwSwitch == 1) ? "\x90\x90" : "\x39\x08";
	WriteToMemory(dwPid, 0xEA91D, patch2, 2); //bug
}


// 修改背景ID
VOID CPvz::ModifyBGIdValue(DWORD dwBGId)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL);
	BOOL result=WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x164), &dwBGId, sizeof(DWORD), NULL);
	check_result(result);
	CloseHandle(hProcess);
}


// 重复建造，无需荷叶
VOID CPvz::Build(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90\x90" : "\xFF\x24\x85\xDC\x9C\x3A\x00";
	WriteToMemory(dwPid, 0x99AD1, patch, 7);
}


// 自动收集阳光
VOID CPvz::Auto(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90":"\x0F\x84\xC4\x01\x00\x00";
	WriteToMemory(dwPid, 0xB31EE, patch, 6);
}

// 全部帧伤
VOID CPvz::Card(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90\x90" : "\xC6\x46\x24\x01"; 
	WriteToMemory(dwPid, 0xAC55E, patch, 4);
}

// 加速僵尸出动
VOID CPvz::Fast(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x89\x97\x00\x04\x00\x00":"\x89\x8F\x00\x04\x00\x00";
	WriteToMemory(dwPid, 0x9FD01, patch, 6);
}

// 更好的高级暂停
VOID CPvz::TheWorld(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90" : "\x83\xF8\x32";
	WriteToMemory(dwPid, 0x9C862, patch, 3);
}

//无主动技能冷却
VOID CPvz::NoModelCD(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0x43A);
	const char* patch = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\x0F\xBF\x87\xC4\x03\x00\x00";
	WriteToMemory(dwPid, 0x916E2, patch, 7);
	const char* patch2 = "\x66\xC7\x87\xC4\x03\x00\x00\x00\x00\x0F\xBF\x87\xC4\x03\x00\x00\xE9\xD4\x16\x29\xFF";
	WriteToMemory(dwPid, 0x43A, patch2, 21);
	if (dwSwitch == 1)WriteJump(dwPid, 0x916E2, 0x43A);
	WriteJump(dwPid, 0x44A , 0x916E7);
}

VOID CPvz::NoSunMax(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x89\x81\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8F64D, patch, 6);
}

VOID CPvz::NoBuildTime(bool dwSwitch){
	DWORD dwPid = GetGamePid(); 
	const char* patch = (dwSwitch == 1) ? "\xC7\x86\x94\x00\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90" : "\x89\x86\x94\x00\x00\x00\x85\xC0\x0F\x85\xDC\x01\x00\x00";
	WriteToMemory(dwPid, 0xA98E3, patch, 14);
}

VOID CPvz::IgnoreSun(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\xB8\x3F\x3F\x3F\x3F\x90" : "\x8B\x83\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8FADA, patch, 6);
}

VOID CPvz::Mowers(bool dwSwitch) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90\x90\x90":"\xF3\x0F\x11\x87\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0xC28B2, patch, 8);
}
//召唤奖杯
void CPvz::SummonCup() {
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x45E);
	if (!check_dwPid(dwPid)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	const char* shellcode = "\x60\x9C\xBA\x80\x00\x00\x00\x8B\x0D\x28\xAE\x29\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x02\x6A\x18\x6A\x7B\xE8\xB3\x94\x00\x00\x9D\x61\xC3";
	DWORD eBaseAddress = ReadTOMemory(dwPid,baseAddress+0x297C54);
	WriteToMemory(dwPid, 0x45E, shellcode, 37);
	WriteMovECX(dwPid, eBaseAddress +0x708, 0x45E + 0x7);
	WriteCall(dwPid,0x45E + 0x1D, 0x9A210);
	RunTheMemory(dwPid, 0x45E);
}
//种植
VOID CPvz::Plant(DWORD dwXP, DWORD dwYP, DWORD dwID)
{
	dwXP--; dwYP--;
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	protectAddress(dwPid, 0x348);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	const char* shellcode = "\x60\x6A\x01\x6A\x07\x8B\x0D\x28\xAE\x29\x00\x6A\x01\x6A\x01\xE8\xB3\x94\x00\x00\x61\xC3";
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x297C54);
	DWORD value = ReadTOMemory(dwPid, eBaseAddress + 0x320);
	WriteToMemory(dwPid, 0x348, shellcode, 22);
	WriteMovECX(dwPid, value + 0x7C, 0x348 + 0x5);
	WriteCall(dwPid, 0x348 + 0xF, 0x9B280);
	WritePush(dwPid, dwXP, 0x348 + 0xD);
	WritePush(dwPid, dwYP, 0x348 + 0xB);
	WritePush(dwPid, dwID, 0x348 + 0x3);
	RunTheMemory(dwPid, 0x348);
}
//豌豆子弹帧伤
VOID CPvz::PeaSDamage(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0x52F);
	const char* patch1 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xC6\x46\x24\x01\x75\x16";
	WriteToMemory(dwPid, 0xAC55E, patch1, 6);
	const char* patch2 = "\x80\x7E\x08\x01\x0F\x84\x2A\x55\x9C\xFE\x80\x7E\x08\x02\x0F\x84\x20\x55\x9C\xFE\x80\x7E\x08\x03\x0F\x84\x16\x55\x9C\xFE\xC6\x46\x24\x01\x0F\x85\x22\x55\x9C\xFE\xE9\x07\x55\x9C\xFE";
	WriteToMemory(dwPid, 0x52F, patch2, 45);
	if(dwSwitch == 1) WriteJump(dwPid, 0xAC55E, 0x52F);
	WriteConditionJump(dwPid,0x52F + 0x4,0xAC563,true);
	WriteConditionJump(dwPid, 0x52F + 0xE, 0xAC563, true);
	WriteConditionJump(dwPid, 0x52F + 0x18, 0xAC563, true);
	WriteConditionJump(dwPid, 0x52F + 0x22, 0xAC579, false);
	WriteJump(dwPid, 0x52F + 0x28, 0xAC564);
}
// 僵尸掉卡
VOID CPvz::ZombieDC(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	protectAddress(dwPid, 0xF32);
	protectAddress(dwPid, 0x400);
	const char* patch1 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\xC6\x83\x9C\x00\x00\x00\x01"; //濒死判断
	WriteToMemory(dwPid, 0x105EE4, patch1, 7);
	const char* patch2 = "\xC6\x83\x9C\x00\x00\x00\x01\x60\xBA\x80\x00\x00\x00\x8B\x0D\x10\x06\xE1\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x73\x18\xFF\x73\x14\xE8\x86\x53\x85\xFF\x61\xE9\x9B\x0A\x8C\xFF";
	WriteToMemory(dwPid, 0xF32, patch2, 48);
	const char* patch3 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x77\x7C\x83\xEC\x08";  //掉落物品随机 
	WriteToMemory(dwPid, 0xB2AC7, patch3, 6);
	const char* patch4 = "\xFF\x77\x7C\x83\xEC\x08\x81\x7F\x7C\xFF\xFF\xFF\xFF\x0F\x85\x6A\xB6\xC2\xFF\xA1\x00\x00\x00\x00\x89\x47\x7C\xE9\x5D\xB6\xC2\xFF";
	WriteToMemory(dwPid, 0xF62, patch4, 32);
	const char* patch5 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x87\x74\x03\x00\x00"; //修改随机数
	WriteToMemory(dwPid, 0x9C583, patch5, 6);
	const char* patch6 = "\xFF\x87\x74\x03\x00\x00\xFF\x05\xC8\xDA\xB9\x00\x83\x3D\xC8\xDA\xB9\x00\x10\x74\x09\x0F\x1F\x40\x00\xE9\x6B\x5F\x89\xFC\xC7\x05\xC8\xDA\xB9\x00\x00\x00\x00\x00\xE9\x5C\x5F\x89\xFC";
	WriteToMemory(dwPid, 0xF82, patch6, 45);
	const char* patch7 = (dwSwitch == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x64\x00\x00\x00\x00"; //灰烬判断
	WriteToMemory(dwPid, 0xAFCCC, patch7, 7);
	const char* patch8 = "\xC7\x47\x64\x00\x00\x00\x00\x60\x8B\xD7\x8B\x0D\x10\x06\x4B\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x84\x53\x00\xFF\x61\xE9\x41\x8B\x01\xFF";
	WriteToMemory(dwPid, 0xFAF, patch8, 50);
	const char* patch9 = (dwSwitch == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x40\x01\x00\x00\x00"; //小推车判断
	WriteToMemory(dwPid, 0x10672F, patch9, 7);
	const char* patch10 = "\xC7\x47\x40\x01\x00\x00\x00\x80\xBF\x9C\x00\x00\x00\x01\x0F\x84\x62\x0F\xD4\xDF\x60\x8B\xD7\x8B\x0D\x10\x06\x33\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x77\x53\xCD\xDF\x61\xE9\x37\x0F\xD4\xDF";
	WriteToMemory(dwPid, 0x4D9, patch10, 63);
	if (dwSwitch == 1)WriteJump(dwPid, 0x105EE4, 0xF32);
	WriteJump(dwPid, 0xF5D, 0x105EE9);
	WriteCall(dwPid, 0xF57, 0x9A210);
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x297C54);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xF3F);

	if (dwSwitch == 1)WriteJump(dwPid, 0xB2AC7,0xF62);
	WriteConditionJump(dwPid, 0xF6F, 0xB2ACB, false);
	WriteJump(dwPid, 0xF7D, 0xB2ACC);
	WriteMovEAX(dwPid, baseAddress + 0x28B0B8, 0xF75);

	if (dwSwitch == 1)WriteJump(dwPid, 0x9C583, 0xF82);
	WriteINC(dwPid, baseAddress + 0x28B0B8, 0xF88);
	WriteCMPXZero(dwPid, baseAddress + 0x28B0B8, 0xF8E);
	WriteJump(dwPid, 0xF9B, 0x9C588);
	WriteJump(dwPid, 0xFAA, 0x9C588);

	if (dwSwitch == 1)WriteJump(dwPid, 0xAFCCC, 0xFAF);
	WriteMOVXZero(dwPid, baseAddress + 0x28B0B8, 0xFA0);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xFB9);
	WriteCall(dwPid, 0xFD6, 0x9A210);
	WriteJump(dwPid, 0xFDC, 0xAFCD1);

	if (dwSwitch == 1)WriteJump(dwPid, 0x10672F, 0x4D9);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0x4F0);
	WriteCall(dwPid, 0x50D, 0x9A210);
	WriteJump(dwPid, 0x513, 0x106734);
	WriteConditionJump(dwPid, 0x4E7, 0x106733, true);
	CloseHandle(hProcess);
}


// 植物不会被魅惑
VOID CPvz::NotSubvert(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90\x90" : "\x89\x46\x10";
	WriteToMemory(dwPid, 0xC9E81 , patch, 3);
}

// 樱桃瞬爆
VOID CPvz::CherryFast(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x5BD);
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xCAEEE, patch1,10);
	const char* patch2 = (dwSwitch == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xC747B, patch2, 10);
}
// 樱桃不爆
VOID CPvz::CherryNo(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xE9\xE7\x01\x00\x00\x90" : "\x0F\x8F\xE6\x01\x00\x00";
	WriteToMemory(dwPid, 0xC72B6, patch1, 6);
}
//猫丝子瞬吸
VOID CPvz::MeowFast(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCAF89, patch1, 10);
}
//荷鲁斯刀刀暴击
VOID CPvz::LoursMC(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0x6BF);
	const char* patch1 = (dwSwitch == 1) ? "\x89\xA7\xBC\x00\x00\x00" :"\x89\x8F\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xC813F, patch1, 6);
}
// 植物无敌
VOID CPvz::GodMode(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\x90\x90" :"\x2B\xC7";
	WriteToMemory(dwPid, 0xC9F07, patch, 2);
}
// 取消荣光骄傲状态
VOID CPvz::Point(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch = (dwSwitch == 1) ? "\xC7\x85\xF8\x02\x00\x00\x00\x00\x00\x00\x90\x90":"\x88\x95\xF8\x02\x00\x00\x8B\x80\x14\x08\x00\x00"; //硬塞进去的，依靠bug运行（
	WriteToMemory(dwPid, 0xD5EA8, patch, 12);
}
//导藓批量种植
VOID CPvz::DX(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\x6A\x09\x90\x90\x90\x90" : "\xFF\xB6\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0x9424E, patch1, 6);
	const char* patch2 = (dwSwitch == 1) ? "\x6A\x0D" : "\x6A\x09";
	WriteToMemory(dwPid, 0x95077, patch2, 2);
	const char* patch3 = (dwSwitch == 1) ? "\x83\x78\xE8\x0D" : "\x83\x78\xE8\x09";
	WriteToMemory(dwPid, 0xC50F5, patch3, 4);
	const char* patch4 = (dwSwitch == 1) ? "\x83\x79\x18\x0D" :"\x83\x79\x18\x0D";
	WriteToMemory(dwPid, 0x94F27, patch4, 4);
}
//进入游戏修改荣光
VOID CPvz::Point2(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0xF00);
	const char* patch1 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90" :"\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66"; //含bug运行
	WriteToMemory(dwPid, 0x93B42, patch1, 10);
	const char* patch2 = "\x66\xC7\x47\x20\x06\x00\x66\xC7\x47\x24\x06\x00\x66\xC7\x47\x28\x06\x00\x66\xC7\x47\x2C\x03\x00\x66\xC7\x47\x30\x02\x00\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66\x89\x87\xA8\x03\x00\x00\xE9\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xF00, patch2, 46);
	WriteJump(dwPid, 0xF00+0x29+0x6, 0x93B4C);
	if(dwSwitch == 1)WriteJump(dwPid, 0x93B42, 0xF00);
}
//光菱角帧伤害
VOID CPvz::LingSDamage(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0x7F1);
	const char* patch1 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x0F\x1F\x00" : "\x8B\x47\x38\xB9\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xABBDD, patch1, 8);
	const char* patch2 = "\xB8\x02\x00\x00\x00\xB9\x96\x00\x00\x00\xE9\x96\x4B\x84\xFE";
	WriteToMemory(dwPid, 0x7F1, patch2, 15);
	WriteJump(dwPid, 0x7F1+0xA, 0xABBE5);
	if (dwSwitch == 1)WriteJump(dwPid, 0xABBDD, 0x7F1);
}
//苹果鼓瑟手无冷却
VOID CPvz::ApplayerNoCD(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xB0\x04\x00\x00";
	WriteToMemory(dwPid, 0xCB124, patch1, 10);
	const char* patch2 = (dwSwitch == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x60\x09\x00\x00";
	WriteToMemory(dwPid, 0xC8831, patch2, 10);
}
//苹果鼓瑟手无延迟
VOID CPvz::ApplayerNoLag(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xC8883, patch1, 10);
}
//车前草吸收无冷却
VOID CPvz::PlantageNoCD(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x86\x9C\x00\x00\x00\x05\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xD0\x07\x00\x00";
	WriteToMemory(dwPid, 0xC7C78, patch1, 10);
	const char* patch2 = (dwSwitch == 1) ? "\xC7\x86\x9C\x00\x00\x00\x92\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xFA\x00\x00\x00";
	WriteToMemory(dwPid, 0xC9579, patch2, 10);
}
//向日葵无cd生产
VOID CPvz::SunFlowerNoCD(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\xB8\x0B\x00\x00";
	WriteToMemory(dwPid, 0xC7215, patch1, 10);
	const char* patch2 = (dwSwitch == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xEE\x02\x00\x00";
	WriteToMemory(dwPid, 0xCAECE, patch2, 10);
	const char* patch3 = (dwSwitch == 1) ? "\xC7\x87\x9C\x00\x00\x00\x24\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x55\x00\x00\x00";
	WriteToMemory(dwPid, 0xC70F1, patch3, 10);
}
//豌豆无冷却
VOID CPvz::PeaNoCD(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x0F\x85\x6C\x03\x00\x00";
	WriteToMemory(dwPid, 0xC6D37, patch1, 6);
}
//超级闪电芦苇
VOID CPvz::SuperReed(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xB9\x66\x66\x66\x66" : "\xB9\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCBCBB, patch1, 5);
	const char* patch2 = (dwSwitch == 1) ? "\x8B\x56\x08" : "\x8B\x52\x08";
	WriteToMemory(dwPid, 0xCBC8B, patch2, 3);
}
//滇池花
VOID CPvz::PowerFlowerNoCD(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xC7\x86\x9C\x00\x00\x00\x00\x00\x00\x00" :"\xC7\x86\x9C\x00\x00\x00\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xCDA09, patch1, 10);
}
//永远最大
VOID CPvz::AwayMax(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	protectAddress(dwPid, 0x749);
	const char* patch1 = (dwSwitch == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\x89\xB9\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xCD80B, patch1, 6);
	const char* patch2 = "\xC7\x81\xBC\x00\x00\x00\x05\x00\x00\x00\xE9\x12\x90\x39\xFF";
	WriteToMemory(dwPid, 0x749, patch2, 15);
	WriteJump(dwPid, 0x749 + 0xA, 0xCD811);
	if (dwSwitch == 1)WriteJump(dwPid, 0xCD80B, 0x749);
}
//物品不消失
VOID CPvz::ItemNoDie(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\x90\x90\x90" : "\x83\xC0\xFF";
	WriteToMemory(dwPid, 0xB30C1, patch1, 3);
}
//天上狂掉阳光
VOID CPvz::SunNoDelay(bool dwSwitch)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = (dwSwitch == 1) ? "\xB8\x01\x00\x00\x00\x90" : "\x8B\x87\xD4\x03\x00\x00";
	WriteToMemory(dwPid, 0x9FB39, patch1, 6);
}
//一键布阵
VOID CPvz::BuildTheArray()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = "\xC6\x40\x2F\x00";
	WriteToMemory(dwPid, 0x982C3, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x78\x2F\x00";
	WriteToMemory(dwPid, 0x982C3, patch1, 4);
	for (int X = 1; X <= 9; ++X) {
		Sleep(80);
		for (int Y = 1; Y <= 5; ++Y) {
			Sleep(80);
			int ID;
			do {
				ID = rand() % 15; // 生成1~15范围内的随机ID
			} while (ID == 2 || ID == 6);
			if (ID == 4  || ID == 9 || ID == 13)
			{
				if (ID == 9 || ID == 13)
				{
					Plant(X, Y+1, ID);
					Plant(X, Y + 1, ID);
					Sleep(10);
					Plant(X + 1, Y + 1, ID);
				}
				Plant(X, Y, ID);
				do {
					ID = rand() % 15; // 生成0~15范围内的随机数
				} while (ID==4 || ID == 9 || ID == 13 || ID == 2 || ID == 6);
				Plant(X, Y, ID);
			}
			else
			{
				Plant(X, Y, ID);
			}
		}
	}
}
//清空植物
VOID CPvz::ClearPlant()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = "\xC6\x47\x2F\x00";
	WriteToMemory(dwPid, 0x9B304, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x7F\x2F\x00";
	WriteToMemory(dwPid, 0x9B304, patch1, 4);
}
//清空子弹
VOID CPvz::ClearBullet()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	const char* patch1 = "\xC6\x42\x24\x01";
	WriteToMemory(dwPid, 0x90738, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x7A\x24\x00";
	WriteToMemory(dwPid, 0x90738, patch1, 4);
}
//清空僵尸
VOID CPvz::ClearZombie()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	const char* patch1 = "\xC6\x47\x2F\x01";
	WriteToMemory(dwPid, 0x9B374, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x7F\x2F\x00 ";
	WriteToMemory(dwPid, 0x9B374, patch1, 4);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	DWORD dwTimer = 0x0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL); 
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x400), &dwTimer, sizeof(DWORD), NULL); 
}
//修复崩溃bug
VOID CPvz::FixCrashBug()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	const char* patch1 = "\xEB\x14";
	WriteToMemory(dwPid, 0x185C7A, patch1, 2);
}
//红针花线
VOID CPvz::ToHongZhen()
{
	std::filesystem::path configFolderPath("C:\\ProgramData\\GhTrTool");
	if (!std::filesystem::exists(configFolderPath)) {
		std::filesystem::create_directory(configFolderPath);
	}
	std::filesystem::path configFilePath = configFolderPath / "config.json";
	nlohmann::json configJson;
	if (std::filesystem::exists(configFilePath)) {
		std::ifstream configFileIn(configFilePath);
		configFileIn >> configJson;
		configFileIn.close();
	}
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	DWORD dwSwitcher = 0x1;
	int response;
	if (!std::filesystem::exists(configFilePath)) MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
	if (!std::filesystem::exists(configFilePath)) response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	if (response == IDCANCEL) return;
	configJson["isModifyArchive"] = true;
	std::ofstream configFileOut(configFilePath);
	configFileOut << configJson.dump(4);
	configFileOut.close();
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x814), &dwNum, sizeof(DWORD), NULL);
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x4), &dwSwitcher, sizeof(BYTE), NULL);
}
//导向寄线
VOID CPvz::ToDaoXiangJi()
{
	std::filesystem::path configFolderPath("C:\\ProgramData\\GhTrTool");
	if (!std::filesystem::exists(configFolderPath)) {
		std::filesystem::create_directory(configFolderPath);
	}
	std::filesystem::path configFilePath = configFolderPath / "config.json";
	nlohmann::json configJson;
	if (std::filesystem::exists(configFilePath)) {
		std::ifstream configFileIn(configFilePath);
		configFileIn >> configJson;
		configFileIn.close();
	}
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	DWORD dwSwitcher = 0x0;
	int response;
	if (!std::filesystem::exists(configFilePath)) MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
	if (!std::filesystem::exists(configFilePath)) response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	if (response == IDCANCEL) return;
	configJson["isModifyArchive"] = true;
	std::ofstream configFileOut(configFilePath);
	configFileOut << configJson.dump(4);
	configFileOut.close();
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x814), &dwNum, sizeof(DWORD), NULL); 
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x4), &dwSwitcher, sizeof(BYTE), NULL);
}
//切换难度
VOID CPvz::DifficultySwitcher(DWORD dwDiff)
{
	std::filesystem::path configFolderPath("C:\\ProgramData\\GhTrTool");
	if (!std::filesystem::exists(configFolderPath)) {
		std::filesystem::create_directory(configFolderPath);
	}
	std::filesystem::path configFilePath = configFolderPath / "config.json";
	nlohmann::json configJson;
	if (std::filesystem::exists(configFilePath)) {
		std::ifstream configFileIn(configFilePath);
		configFileIn >> configJson;
		configFileIn.close();
	}
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;
	DWORD dwNum = 0;
	int response;
	if (dwDiff > 4) { MessageBox(NULL, L"你所选的难度不存在，请查看难度ID表后填写", L"警告", MB_ICONWARNING | MB_OK); return; }
	if(!std::filesystem::exists(configFilePath)) MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
	if (!std::filesystem::exists(configFilePath)) response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	if (response == IDCANCEL) return;
	configJson["isModifyArchive"] = true;
	std::ofstream configFileOut(configFilePath);
	configFileOut << configJson.dump(4);
	configFileOut.close();
	switch (dwDiff) {
		case 0x0:
			MessageBox(NULL, L"将切换到简单 Easy模式", L"提示", MB_OK);
			break;
		case 0x1:
			MessageBox(NULL, L"将切换到正常 Medium模式", L"提示", MB_OK);
			break;
		case 0x2:
			MessageBox(NULL, L"将切换到较难 Hard模式", L"提示", MB_OK);
			break;
		case 0x3:
			MessageBox(NULL, L"将切换到特难 Impossible模式", L"提示", MB_OK);
			break;
		case 0x4:
			MessageBox(NULL, L"将切换到失衡 Unbalanced模式", L"提示", MB_OK);
			break;
	}
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x814), &dwNum, sizeof(DWORD), NULL); 
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x8), &dwDiff, sizeof(DWORD), NULL); 
}