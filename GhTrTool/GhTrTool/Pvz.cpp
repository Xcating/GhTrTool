#include "stdafx.h"
#include "Pvz.h"
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <sstream>
#include <iomanip>
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
		return -1;
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
VOID check_dwPid(DWORD dwPid)
{
	if (dwPid == -1)
	{
		MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
		return;
	}
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
	BYTE jumpCode[] = { 0x83, 0x3D ,jumpOffset & 0xFF, (jumpOffset >> 8) & 0xFF, (jumpOffset >> 16) & 0xFF, (jumpOffset >> 24) & 0xFF ,0x0E };
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
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x29CE88);
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
		MessageBox(NULL, TEXT("写入内存成功"), TEXT("提示"), MB_OK);
	else
		MessageBox(NULL, TEXT("写入失败, 请联系作者"), TEXT("错误"), MB_OK | MB_ICONERROR);
}
// 修改阳光的值
VOID CPvz::ModifySunValue(DWORD dwSun) //Sun指的是阳光
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x29CE88;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL); //读取targetAddress对应的值(基址->内存基址)
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL); //读取战场基址对应的值(内存基址 +708->战场基址)
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x380), &dwSun, sizeof(DWORD), NULL); //将dwNum写入控制阳光的地址
	check_result(result);
	CloseHandle(hProcess);
}

// 修改卡槽数量
VOID CPvz::SeedPacket(DWORD dwSP) //SP指的是SeedPacket，种子包
{

	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x29CE88;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL); //一层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x14C), &dwNum, sizeof(DWORD), NULL); //二层偏移
	BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x1C), &dwSP, sizeof(DWORD), NULL); //三层偏移
	check_result(result);
	CloseHandle(hProcess);
}
// 使用通用函数的种植不减阳光
VOID CPvz::SunNop() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char nop[] = "\x90\x90\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0x95E39, nop, sizeof(nop) - 1);
}

// 种植免冷却
VOID CPvz::NoCd() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xc7\x41\x20\x00\x00\x00\x00\x90\x90\x90";
	WriteToMemory(dwPid, 0xE54A1, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x90\x90";
	WriteToMemory(dwPid, 0xE55AD, patch2, sizeof(patch2) - 1);
}


// 修改背景ID
VOID CPvz::ModifyBGIdValue(DWORD dwBGId)
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x29CE88;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL);
	BOOL result=WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x164), &dwBGId, sizeof(DWORD), NULL);
	check_result(result);
	CloseHandle(hProcess);
}


// 重复建造，无需花盆
VOID CPvz::Build() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0x94C74, patch, sizeof(patch) - 1);
}


// 自动收集阳光
VOID CPvz::Auto()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0xABD9A, patch, sizeof(patch) - 1);
}

// 全部帧伤
VOID CPvz::Card() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0xA552E, patch, sizeof(patch) - 1);
}

// 加速僵尸出动
VOID CPvz::Fast() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\xC7\x87\xFC\x03\x00\x00\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0x996DB, patch, sizeof(patch) - 1);
}

// 更好的高级暂停
VOID CPvz::TheWorld() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90";
	WriteToMemory(dwPid, 0x96262, patch, sizeof(patch) - 1);
}

//无主动技能冷却
VOID CPvz::NoModelCD() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0x43A);
	const char patch[] = "\xE9\x00\x00\x00\x00\x66\x90";
	WriteToMemory(dwPid, 0x93BF5, patch, 7);
	const char patch2[] = "\xC6\x86\xC0\x03\x00\x00\x00\xE9\xF0\x3B\xDD\xFE";
	WriteToMemory(dwPid, 0x43A, patch2, sizeof(patch2));
	WriteJump(dwPid, 0x93BF5, 0x43A);
	WriteJump(dwPid, 0x43A + 0x7 , 0x93BFC);
}

VOID CPvz::NoSunMax() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0x8E8ED, patch, sizeof(patch) - 1);
}

VOID CPvz::NoBuildTime() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\xC7\x86\x94\x00\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0xA2913, patch, sizeof(patch) - 1);
}

VOID CPvz::IgnoreSun() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\xB8\x3F\x3F\x3F\x3F\x90";
	WriteToMemory(dwPid, 0x8ED0A, patch, sizeof(patch) - 1);
}

VOID CPvz::Mowers() {
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90\x90\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0xBE362, patch, sizeof(patch) - 1);
}
void CPvz::SummonCup() {
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x45E);
	check_dwPid(dwPid);
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	const char shellcode[] = "\x60\x9C\xBA\x80\x00\x00\x00\x8B\x0D\x28\xAE\x29\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x02\x6A\x18\x6A\x7B\xE8\xB3\x94\x00\x00\x9D\x61\xC3";
	DWORD eBaseAddress = ReadTOMemory(dwPid,baseAddress+0x29CE88);
	WriteToMemory(dwPid, 0x45E, shellcode, sizeof(shellcode) - 1);
	WriteMovECX(dwPid, eBaseAddress +0x708, 0x45E + 0x7);
	WriteCall(dwPid,0x45E + 0x1D, 0x953B0);
	RunTheMemory(dwPid, 0x45E);
}
VOID CPvz::Plant(DWORD dwXP, DWORD dwYP, DWORD dwID)
{
	dwXP--; dwYP--;
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	protectAddress(dwPid, 0x348);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	const char shellcode[] = "\x60\x6A\x01\x6A\x07\x8B\x0D\x28\xAE\x29\x00\x6A\x01\x6A\x01\xE8\xB3\x94\x00\x00\x61\xC3";
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x29CE88);
	DWORD value = ReadTOMemory(dwPid, eBaseAddress + 0x320);
	WriteToMemory(dwPid, 0x348, shellcode, sizeof(shellcode) -1);
	WriteMovECX(dwPid, value + 0x7C, 0x348 + 0x5);
	WriteCall(dwPid, 0x348 + 0xF, 0x956F0);
	WritePush(dwPid, dwXP, 0x348 + 0xB);
	WritePush(dwPid, dwYP, 0x348 + 0xD);
	WritePush(dwPid, dwID, 0x348 + 0x3);
	RunTheMemory(dwPid, 0x348);
}
VOID CPvz::PeaSDamage()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0x52F);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xA552E, patch1, sizeof(patch1)-1);
	const char patch2[] = "\x80\x7E\x08\x01\x0F\x84\x2A\x55\x9C\xFE\x80\x7E\x08\x02\x0F\x84\x20\x55\x9C\xFE\x80\x7E\x08\x03\x0F\x84\x16\x55\x9C\xFE\xC6\x46\x24\x01\x0F\x85\x22\x55\x9C\xFE\xE9\x07\x55\x9C\xFE";
	WriteToMemory(dwPid, 0x52F, patch2, sizeof(patch2)-1);
	WriteJump(dwPid, 0xA552E, 0x52F);
	WriteConditionJump(dwPid,0x52F + 0x4,0xA5533,true);
	WriteConditionJump(dwPid, 0x52F + 0xE, 0xA5533, true);
	WriteConditionJump(dwPid, 0x52F + 0x18, 0xA5533, true);
	WriteConditionJump(dwPid, 0x52F + 0x22, 0xA654A, false);
	WriteJump(dwPid, 0x52F + 0x28, 0xA5533);
}
// 僵尸掉卡
VOID CPvz::ZombieDC()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	protectAddress(dwPid, 0xF32);
	protectAddress(dwPid, 0x400);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x66\x90";
	WriteToMemory(dwPid, 0x100AC4, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xC6\x83\x9C\x00\x00\x00\x01\x60\xBA\x80\x00\x00\x00\x8B\x0D\x10\x06\xE1\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x73\x18\xFF\x73\x14\xE8\x86\x53\x85\xFF\x61\xE9\x9B\x0A\x8C\xFF";
	WriteToMemory(dwPid, 0xF32, patch2, sizeof(patch2) - 1);
	const char patch3[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xAB677, patch3, sizeof(patch3) - 1);
	const char patch4[] = "\xFF\x77\x7C\x83\xEC\x08\x81\x7F\x7C\xFF\xFF\xFF\xFF\x0F\x85\x6A\xB6\xC2\xFF\xA1\x00\x00\x00\x00\x89\x47\x7C\xE9\x5D\xB6\xC2\xFF";
	WriteToMemory(dwPid, 0xF62, patch4, sizeof(patch4) - 1);
	const char patch5[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0x95F83, patch5, sizeof(patch5) - 1);
	const char patch6[] = "\xFF\x87\x70\x03\x00\x00\xFF\x05\xC8\xDA\xB9\x00\x83\x3D\xC8\xDA\xB9\x00\x0E\x74\x09\x0F\x1F\x40\x00\xE9\x6B\x5F\x89\xFC\xC7\x05\xC8\xDA\xB9\x00\x00\x00\x00\x00\xE9\x5C\x5F\x89\xFC";
	WriteToMemory(dwPid, 0xF82, patch6, sizeof(patch6) - 1);
	const char patch7[] = "\xE9\x8F\x74\xFE\x00\x66\x90";
	WriteToMemory(dwPid, 0xA8B6C, patch7, sizeof(patch7) - 1);
	const char patch8[] = "\xC7\x47\x64\x00\x00\x00\x00\x60\x8B\xD7\x8B\x0D\x10\x06\x4B\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x84\x53\x00\xFF\x61\xE9\x41\x8B\x01\xFF";
	WriteToMemory(dwPid, 0xFAF, patch8, sizeof(patch8) - 1);
	const char patch9[] = "\xE9\x8F\x74\xFE\x00\x66\x90";
	WriteToMemory(dwPid, 0x100F6F, patch9, sizeof(patch9) - 1);
	const char patch10[] = "\xC7\x47\x40\x01\x00\x00\x00\x80\xBF\x9C\x00\x00\x00\x01\x0F\x84\x62\x0F\xD4\xDF\x60\x8B\xD7\x8B\x0D\x10\x06\x33\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x77\x53\xCD\xDF\x61\xE9\x37\x0F\xD4\xDF";
	WriteToMemory(dwPid, 0x4D9, patch10, sizeof(patch10) - 1);
	WriteJump(dwPid, 0xF5D, 0x100AC9);
	WriteJump(dwPid, 0x100AC4, 0xF32);
	WriteCall(dwPid, 0xF57, 0x953B0);
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x29CE88);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xF3F);
	WriteJump(dwPid, 0xAB677,0xF62);
	WriteConditionJump(dwPid, 0xF6F, 0xAB67C, false);
	WriteJump(dwPid, 0xF7D, 0xAB67C);
	WriteMovEAX(dwPid, baseAddress + 0x29DAC8 , 0xF75);
	WriteJump(dwPid, 0x95F83, 0xF82);
	WriteINC(dwPid, baseAddress + 0x29DAC8, 0xF88);
	WriteCMPXZero(dwPid, baseAddress + 0x29DAC8, 0xF8E);
	WriteJump(dwPid, 0xF9B, 0x95F89);
	WriteJump(dwPid, 0xFAA, 0x95F89);
	WriteMOVXZero(dwPid, baseAddress + 0x29DAC8, 0xFA0);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xFB9);
	WriteCall(dwPid, 0xFD6, 0x953B0);
	WriteJump(dwPid, 0xFDC, 0xA8B73);
	WriteJump(dwPid, 0x100F6F, 0x4D9);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0x4F0);
	WriteCall(dwPid, 0x50D, 0x953B0);
	WriteJump(dwPid, 0x513, 0x100F74);
	WriteConditionJump(dwPid, 0x4E7, 0x100F73, true);
	CloseHandle(hProcess);
}


// 植物不会被魅惑
VOID CPvz::NotSubvert()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90\x90";
	WriteToMemory(dwPid, 0xC5911, patch, sizeof(patch) - 1);
}

// 樱桃瞬爆
VOID CPvz::CherryFast()
{
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x5BD);
	check_dwPid(dwPid);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xC943F, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x83\xBE\x84\x00\x00\x00\x02\x75\x0D\x0F\x1F\x40\x00\xB8\x00\x00\x00\x00\x90\x90\x90\x90\x89\x86\x9C\x00\x00\x00\xE9\x27\x94\x31\xFF";
	WriteToMemory(dwPid, 0x5BD, patch2, sizeof(patch2) - 1);
	WriteJump(dwPid,0xC943F, 0x5BD);
	WriteJump(dwPid, 0x5D9 ,0xC9445);
}
// 樱桃不爆
VOID CPvz::CherryNo()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xC943F, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x83\xBE\x84\x00\x00\x00\x02\x75\x0A\x0F\x1F\x40\x00\x90\xE9\x32\x94\x31\xFF\x89\x86\x9C\x00\x00\x00\xE9\x27\x94\x31\xFF";
	WriteToMemory(dwPid, 0x606, patch2, sizeof(patch2) - 1);
	WriteJump(dwPid, 0xC943F, 0x606);
	WriteJump(dwPid, 0x614, 0xC9445);
	WriteJump(dwPid, 0x61F, 0xC9445);
}
//猫丝子瞬吸
VOID CPvz::MeowFast()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC69AE, patch1, sizeof(patch1) - 1);
}
//荷鲁斯刀刀暴击
VOID CPvz::LoursMC()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0x6BF);
	const char patch1[] = "\xEB\x04";
	WriteToMemory(dwPid, 0xC3BD8, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xC3BCD, patch2, sizeof(patch2) - 1);
	const char patch3[] = "\xC7\x87\xBC\x00\x00\x00\x00\x10\x00\x00\x8B\x8F\xBC\x00\x00\x00\xE9\xBE\x3B\x17\xFF";
	WriteToMemory(dwPid, 0x6BF, patch3, sizeof(patch3) - 1);
	WriteJump(dwPid, 0xC3BCD , 0x6BF);
	WriteJump(dwPid, 0x6CF, 0xC3BD3);
}
// 植物无敌
VOID CPvz::GodMode()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\x90\x90";
	WriteToMemory(dwPid, 0xC5997, patch, sizeof(patch) - 1);
}
// 取消荣光骄傲状态
VOID CPvz::Point()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch[] = "\xC7\x85\xF8\x02\x00\x00\x00\x00\x00\x00\x90\x90"; //硬塞进去的，依靠bug运行（
	WriteToMemory(dwPid, 0xD1358, patch, sizeof(patch) - 1);
}
//导藓批量种植
VOID CPvz::DX()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\x6A\x09\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0x9424E, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x6A\x0D";
	WriteToMemory(dwPid, 0x95077, patch2, sizeof(patch2) - 1);
	const char patch3[] = "\x83\x78\xE8\x0D";
	WriteToMemory(dwPid, 0xC50F5, patch3, sizeof(patch3) - 1);
	const char patch4[] = "\x83\x79\x18\x0D";
	WriteToMemory(dwPid, 0x94F27, patch4, sizeof(patch4) - 1);
}
//进入游戏修改荣光
VOID CPvz::Point2()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0xF00);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x66\x0F\x1F\x44\x00\x00";
	WriteToMemory(dwPid, 0x91FB2, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x66\xC7\x40\x20\x06\x00\x66\xC7\x40\x24\x06\x00\x66\xC7\x40\x28\x06\x00\x66\xC7\x40\x2C\x03\x00\x66\xC7\x40\x30\x02\x00\x0F\xB7\x40\x20\x66\x89\x87\xA8\x03\x00\x00\xE9\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xF00, patch2, sizeof(patch2) - 1);
	WriteJump(dwPid, 0xF29, 0x91FB7);
	WriteJump(dwPid, 0x91FB2, 0xF00);
}
//光菱角帧伤害
VOID CPvz::LingSDamage()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0x7F1);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x0F\x1F\x00";
	WriteToMemory(dwPid, 0xA4B9D, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xB8\x02\x00\x00\x00\xB9\x96\x00\x00\x00\xE9\x96\x4B\x84\xFE";
	WriteToMemory(dwPid, 0x7F1, patch2, sizeof(patch2) - 1);
	WriteJump(dwPid, 0x7F1+0xA, 0xA4BA5);
	WriteJump(dwPid, 0xA4B9D, 0x7F1);
}
//苹果鼓瑟手无冷却
VOID CPvz::ApplayerNoCD()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC6AE0, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC42D1, patch2, sizeof(patch2) - 1);
}
//苹果鼓瑟手无延迟
VOID CPvz::ApplayerNoLag()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC4323, patch1, sizeof(patch1) - 1);
}
//车前草吸收无冷却
VOID CPvz::PlantageNoCD()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x86\x9C\x00\x00\x00\x05\x00\x00\x00";
	WriteToMemory(dwPid, 0xC3728, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xC7\x86\x9C\x00\x00\x00\x92\x00\x00\x00";
	WriteToMemory(dwPid, 0xC5019, patch2, sizeof(patch2) - 1);
}
//车前草吸收无冷却
VOID CPvz::SunFlowerNoCD()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC2CC5, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC68F3, patch2, sizeof(patch2) - 1);
	const char patch3[] = "\xC7\x87\x9C\x00\x00\x00\x24\x00\x00\x00";
	WriteToMemory(dwPid, 0xC2BA1, patch3, sizeof(patch3) - 1);
}
//豌豆无冷却
VOID CPvz::PeaNoCD()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\x90\x90\x90\x90\x90\x90";
	WriteToMemory(dwPid, 0xC27E7, patch1, sizeof(patch1) - 1);
}
//超级闪电芦苇
VOID CPvz::SuperReed()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xB9\x66\x66\x66\x66";
	WriteToMemory(dwPid, 0xC765B, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\x8B\x56\x08";
	WriteToMemory(dwPid, 0xC762B, patch2, sizeof(patch2) - 1);
}
//超级闪电芦苇
VOID CPvz::PowerFlowerNoCD()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	const char patch1[] = "\xC7\x86\x9C\x00\x00\x00\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xC91A2, patch1, sizeof(patch1) - 1);
}
//光菱角帧伤害
VOID CPvz::AwayMax()
{
	DWORD dwPid = GetGamePid();
	check_dwPid(dwPid);
	protectAddress(dwPid, 0x749);
	const char patch1[] = "\xE9\x00\x00\x00\x00\x90";
	WriteToMemory(dwPid, 0xC901B, patch1, sizeof(patch1) - 1);
	const char patch2[] = "\xC7\x81\xBC\x00\x00\x00\x05\x00\x00\x00\xE9\x12\x90\x39\xFF";
	WriteToMemory(dwPid, 0x749, patch2, sizeof(patch2) - 1);
	WriteJump(dwPid, 0x749 + 0xA, 0xC9021);
	WriteJump(dwPid, 0xC901B, 0x749);
}