#include "stdafx.h"
#include "json.hpp"
#include "Pvz.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <Psapi.h>
#include <sstream>
#include <string>
#include <TlHelp32.h>
CPvz::CPvz()
{
}

CPvz::~CPvz()
{
}
/**
 * 获取工具配置文件的路径。
 *
 * @return std::filesystem::path 返回工具配置文件的路径。
 */
std::filesystem::path CPvz::GetConfigFilePath()
{
	std::filesystem::path configFolderPath("C:\\ProgramData\\GhTrTool");
	if (!std::filesystem::exists(configFolderPath)) {
		std::filesystem::create_directory(configFolderPath);
	}
	return configFolderPath / "config.json";
}
/**
 * 读取配置文件的内容到JSON对象。
 *
 * @param configFilePath 包含配置文件路径的filesystem::path对象。
 * @return nlohmann::json 返回配置信息的JSON对象。
 */
nlohmann::json CPvz::ReadConfigFile(const std::filesystem::path& configFilePath)
{
	nlohmann::json configJson;
	if (std::filesystem::exists(configFilePath)) {
		std::ifstream configFileIn(configFilePath);
		configFileIn >> configJson;
		configFileIn.close();
	}
	return configJson;
}
/**
 * 将配置信息写入指定的配置文件。
 *
 * @param configFilePath 包含配置文件路径的filesystem::path对象。
 * @param configJson 包含配置信息的JSON对象。
 */
void CPvz::WriteConfigFile(const std::filesystem::path& configFilePath, const nlohmann::json& configJson)
{
	std::ofstream configFileOut(configFilePath);
	configFileOut << configJson.dump(4);
	configFileOut.close();
}
/**
 * 从指定的地址读取内存中的DWORD值。
 *
 * @param hProcess 要读取的进程的句柄。
 * @param address 要读取的内存地址。
 * @return DWORD 返回从内存中读取的DWORD值。
 */
DWORD CPvz::ReadMemory(HANDLE hProcess, DWORD address)
{
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)address, &dwNum, sizeof(DWORD), NULL);
	return dwNum;
}
/**
 * 向指定的地址写入DWORD值到内存中。
 *
 * @param hProcess 要写入的进程的句柄。
 * @param address 要写入的内存地址。
 * @param value 要写入的DWORD值。
 * @return BOOL 返回是否写入成功。
 */
BOOL CPvz::WriteMemory(HANDLE hProcess, DWORD address, DWORD value)
{
	return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(BYTE), NULL);
}
/**
 * 获取游戏的PID。
 *
 * @return DWORD 返回游戏的PID。
 */
DWORD CPvz::GetGamePid()
{
	HWND hWnd = nullptr;
	while ((hWnd = FindWindowEx(nullptr, hWnd, nullptr, nullptr)) != nullptr)
	{
		wchar_t title[256];
		GetWindowText(hWnd, title, sizeof(title) / sizeof(title[0]));
		std::wstring windowTitle = title;
		std::wstring targetTitle = L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l";
		if (windowTitle.find(targetTitle) == 0)
		{
			DWORD dwPid = 0;
			GetWindowThreadProcessId(hWnd, &dwPid);
			return dwPid;
		}
	}

	return -1;
}
/**
 * 获取指定进程的模块基址。
 *
 * @param hProcess 进程句柄
 * @return DWORD 返回指定进程的模块基址。
 */
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
/**
 * 获取游戏进程的模块基址。
 *
 * @param hProcess 进程句柄
 * @return DWORD 返回游戏进程的模块基址。
 */
DWORD get_baseAddress(HANDLE hProcess)
{
	DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
	if (baseAddress == 0)
		baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
	return baseAddress;
}
/**
 * 在指定进程内创建线程，并运行指定内存地址的代码。
 *
 * @param dwPid 目标进程PID
 * @param codeCaveOffset 代码偏移量
 */
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
/**
 * 检查游戏进程的PID是否有效。
 *
 * @param dwPid 游戏进程PID
 * @param isMessage 是否会提示消息，true为提示，false不提示
 * @return BOOL 如果PID有效返回true，否则返回false。
 */
BOOL CPvz::check_dwPid(DWORD dwPid,BOOL isMessage)
{
	if (dwPid == -1)
	{
		if(isMessage)
			MessageBox(NULL, L"游戏未找到，请打开游戏后点击功能", L"Info", MB_OK);
		return false;
	}
	return true;
}
/**
 * 枚举Windows窗口进程，为找到的窗口设置标题。
 *
 * @param hwnd 窗口句柄
 * @param lParam 附加的参数
 * @return BOOL 枚举是否继续。
 */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD dwPid;
	GetWindowThreadProcessId(hwnd, &dwPid);
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	#ifdef _DEBUG
		std::wstring wstr = (std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l - [Debug Mode] [已被GhTrTool修改] [βver.0.11q] [" << millis << L"]").str();
	#else
		std::wstring wstr = (std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.16l - [已被GhTrTool修改] [ver.0.11q] [" << millis << L"]").str();
	#endif
	if (dwPid == lParam) {
		SetWindowText(hwnd, wstr.c_str());
		return FALSE;
	}
	return TRUE;
}
/**
 * 从指定进程的内存中读取数据。
 *
 * @param dwPid 目标进程PID
 * @param targetAddress 目标内存地址
 * @return DWORD 返回读取的数据。
 */
DWORD ReadTOMemory(DWORD dwPid, DWORD targetAddress) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD value = 0;
	ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &value, sizeof(DWORD), NULL);
	return value;
}
/**
 * 向指定进程的内存中写入数据。
 *
 * @param dwPid 目标进程PID
 * @param offset 目标内存偏移
 * @param data 要写入的数据指针
 * @param size 要写入的数据大小
 * @return bool 写入成功返回true，否则返回false。
 */
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
/**
 * 修改指定进程内存保护设置。
 *
 * @param dwPid 目标进程PID
 * @param offset 内存偏移量
 */
void protectAddress(DWORD dwPid, DWORD offset) {
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD dwOldProtect;
	DWORD baseAddress = get_baseAddress(hProc);
	DWORD targetAddress = baseAddress + offset;
	VirtualProtectEx(hProc, (LPVOID)targetAddress, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect);
}
/**
 * 在指定内存写入Jump操作码。
 *
 * @param dwPid 目标进程PID
 * @param sourceOffset 源内存偏移
 * @param targetOffset 目标内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
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
/**
 * 在指定内存写入条件跳转(Je或Jne)操作码。
 *
 * @param dwPid 目标进程PID
 * @param sourceOffset 源内存偏移
 * @param targetOffset 目标内存偏移
 * @param JE 如果为true写入Je，false写入Jne
 * @return bool 写入成功返回true，否则返回false。
 */
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
/**
 * 在指定内存写入Push操作码。
 *
 * @param dwPid 目标进程PID
 * @param Number 要写入的数值
 * @param offset 内存偏移量
 * @return bool 写入成功返回true，否则返回false。
 */
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
/**
 * 在指定内存写入Mov Ecx,[地址]操作码。
 *
 * @param dwPid 目标进程PID
 * @param Address 要写入的地址
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteMovECX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指定内存写入Mov Eax,[地址]操作码。
 *
 * @param dwPid 目标进程PID
 * @param Address 要写入的地址
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteMovEAX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指定内存写入Mov EAX,0x00000操作码。
 *
 * @param dwPid 目标进程PID
 * @param Address 要写入的十六进制值
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteMovEAXToHEX(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指定内存写入Mov指令，将值设为0。
 *
 * @param dwPid 目标进程PID
 * @param Address 要写入的地址
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteMOVXZero(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指指定内存写入Cmp指令，比较值是否为0。
 *
 * @param dwPid 目标进程PID
 * @param Address 要比较的地址
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteCMPXZero(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指定内存写入Inc指令，增加地址处的值。
 *
 * @param dwPid 目标进程PID
 * @param Address 要增加值的地址
 * @param offset 内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
bool WriteINC(DWORD dwPid, DWORD Address, DWORD offset) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
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
/**
 * 在指定内存写入Call指令，用于调用函数。
 *
 * @param dwPid 目标进程PID
 * @param sourceOffset 源内存偏移
 * @param targetOffset 目标函数的内存偏移
 * @return bool 写入成功返回true，否则返回false。
 */
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
/**
 * 检查战场状态是否可用。
 *
 * @param dwPid 目标进程PID
 * @return BOOL 如果战场可用返回true，否则返回false。
 */
BOOL CPvz::check_battlefield(DWORD dwPid) {
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
/**
 * 检查内存写入结果，并弹出相应消息框。
 *
 * @param result 写入结果
 */
VOID check_result(BOOL result)
{
	if (result)
		MessageBox(NULL, TEXT("写入内存成功"), TEXT("Info"), MB_OK);
	else
		MessageBox(NULL, TEXT("写入失败，请联系作者"), TEXT("Error"), MB_OK | MB_ICONERROR);
}
/**
 * 写入配置文件。如果config.json不存在，则创建文件并设置"isCheat"为true。
 */
VOID CPvz::WriteConfig()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, false)) return;
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
/**
 * 修改游戏中的阳光数量。
 *
 * @param DWORD dwSun 要设置的阳光数值。
 */
VOID CPvz::ModifySunValue(DWORD dwSun)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
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
/**
 * 修改游戏中的种子包（卡槽）数量。
 *
 * @param DWORD dwSP 要设置的卡槽数量。
 */
VOID CPvz::SeedPacket(DWORD dwSP)
{

	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
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
/**
 * 替换种子包中特定卡槽的植物。
 *
 * @param DWORD dwID 要替换的种子包ID。
 * @param DWORD dwNum 要替换的卡槽号码。
 */
VOID CPvz::ModifySeedPacket(DWORD dwID,DWORD dwNum)
{
	dwNum--;
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
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
/**
 * 设置阳光消耗的NOP开关，使种植植物不减少阳光值。
 *
 * @param bool isFeatureEnabled 设置为true启用NOP，false则恢复原代码。
 */
VOID CPvz::SunNop(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* nop = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x29\xBE\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x9C439, nop, 6);
}
/**
 * 设置种植植物无冷却的开关。
 *
 * @param bool isFeatureEnabled 设置为true启用无冷却，false则恢复原代码。
 */
VOID CPvz::NoCd(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x42\x24\x00\x00\x00\x00\x90\x90\x90" : "\x89\x42\x24\xC7\x42\x20\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xEA679, patch1, 10);
	const char* patch2 = (isFeatureEnabled == 1) ? "\x90\x90" : "\x39\x08";
	WriteToMemory(dwPid, 0xEA91D, patch2, 2); //bug
}
/**
 * 修改背景ID。
 *
 * @param dwBGId 要设置的背景ID。
 */
VOID CPvz::ModifyBGIdValue(DWORD dwBGId)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
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
/**
 * 重复建造，无需荷叶。
 *
 * @param isFeatureEnabled 如果设为true，允许重复建造而无需荷叶；如果为false，则恢复原始状态。
 */
VOID CPvz::Build(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90\x90" : "\xFF\x24\x85\xDC\x9C\x3A\x00";
	WriteToMemory(dwPid, 0x99AD1, patch, 7);
}
/**
 * 自动收集阳光。
 *
 * @param isFeatureEnabled 如果设为true，启用自动收集阳光功能；如果为false，则关闭该功能。
 */
VOID CPvz::Auto(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90":"\x0F\x84\xC4\x01\x00\x00";
	WriteToMemory(dwPid, 0xB31EE, patch, 6);
}
/**
 * 全部帧伤。
 *
 * @param isFeatureEnabled 如果设为true，启用全部帧伤功能；如果为false，则关闭该功能。
 */
VOID CPvz::Card(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90" : "\xC6\x46\x24\x01"; 
	WriteToMemory(dwPid, 0xAC55E, patch, 4);
}
/**
 * 加速僵尸出动。
 *
 * @param isFeatureEnabled 如果设为true，启用加速僵尸出动功能；如果为false，则关闭该功能。
 */
VOID CPvz::Fast(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x89\x97\x00\x04\x00\x00":"\x89\x8F\x00\x04\x00\x00";
	WriteToMemory(dwPid, 0x9FD01, patch, 6);
}
/**
 * 更好的高级暂停。
 *
 * @param isFeatureEnabled 如果设为true，启用更好的高级暂停功能；如果为false，则关闭该功能。
 */
VOID CPvz::TheWorld(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x83\xF8\x32";
	WriteToMemory(dwPid, 0x9C862, patch, 3);
}
/**
 * 无主动技能冷却。
 *
 * @param isFeatureEnabled 如果设为true，取消主动技能冷却时间；如果为false，则恢复正常冷却时间。
 */
VOID CPvz::NoModelCD(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0x43A);
	const char* patch = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\x0F\xBF\x87\xC4\x03\x00\x00";
	WriteToMemory(dwPid, 0x916E2, patch, 7);
	const char* patch2 = "\x66\xC7\x87\xC4\x03\x00\x00\x00\x00\x0F\xBF\x87\xC4\x03\x00\x00\xE9\xD4\x16\x29\xFF";
	WriteToMemory(dwPid, 0x43A, patch2, 21);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x916E2, 0x43A);
	WriteJump(dwPid, 0x44A , 0x916E7);
}
/**
 * 无限制阳光。
 *
 * @param isFeatureEnabled 如果设为true，阳光被设置为无限制；如果为false，则恢复原始阳光限制。
 */
VOID CPvz::NoSunMax(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x89\x81\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8F64D, patch, 6);
}
/**
 * 建筑物秒消失。
 *
 * @param isFeatureEnabled 如果设为true，建筑物秒消失；如果为false，则需要建造消失时间正常。
 */
VOID CPvz::NoBuildTime(bool isFeatureEnabled){
	DWORD dwPid = GetGamePid(); 
	const char* patch = (isFeatureEnabled == 1) ? "\xC7\x86\x94\x00\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90" : "\x89\x86\x94\x00\x00\x00\x85\xC0\x0F\x85\xDC\x01\x00\x00";
	WriteToMemory(dwPid, 0xA98E3, patch, 14);
}
/**
 * 忽略阳光。
 *
 * @param isFeatureEnabled 如果设为true，可以无视阳光种植植物；如果为false，则需要阳光足够才可种植。
 */
VOID CPvz::IgnoreSun(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\xB8\x3F\x3F\x3F\x3F\x90" : "\x8B\x83\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8FADA, patch, 6);
}
/**
 * 割草机不动。
 *
 * @param isFeatureEnabled 如果设为true，割草机不动；如果为false，则割草机正常。
 */
VOID CPvz::Mowers(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90\x90\x90":"\xF3\x0F\x11\x87\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0xC28B2, patch, 8);
}
/**
 * 召唤奖杯。
 * 只在草坪中有效，如果未进入草坪，则会弹出提示。
 */
void CPvz::SummonCup() {
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x45E);
	if (!check_dwPid(dwPid, true)) return;
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
/**
 * 在游戏中种植指定植物。
 *
 * @param dwXP 植物的X坐标位置（通过传递值自动减1处理）。
 * @param dwYP 植物的Y坐标位置（通过传递值自动减1处理）。
 * @param dwID 植物的ID。
 */
VOID CPvz::Plant(DWORD dwXP, DWORD dwYP, DWORD dwID)
{
	dwXP--; dwYP--;
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
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
/**
 * 豌豆子弹造成帧伤。
 *
 * @param isFeatureEnabled 如果设为true，豌豆射手的子弹造成帧伤；如果为false，则恢复原始消失模式。
 */
VOID CPvz::PeaSDamage(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0x52F);
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xC6\x46\x24\x01\x75\x16";
	WriteToMemory(dwPid, 0xAC55E, patch1, 6);
	const char* patch2 = "\x80\x7E\x08\x01\x0F\x84\x2A\x55\x9C\xFE\x80\x7E\x08\x02\x0F\x84\x20\x55\x9C\xFE\x80\x7E\x08\x03\x0F\x84\x16\x55\x9C\xFE\xC6\x46\x24\x01\x0F\x85\x22\x55\x9C\xFE\xE9\x07\x55\x9C\xFE";
	WriteToMemory(dwPid, 0x52F, patch2, 45);
	if(isFeatureEnabled == 1) WriteJump(dwPid, 0xAC55E, 0x52F);
	WriteConditionJump(dwPid,0x52F + 0x4,0xAC563,true);
	WriteConditionJump(dwPid, 0x52F + 0xE, 0xAC563, true);
	WriteConditionJump(dwPid, 0x52F + 0x18, 0xAC563, true);
	WriteConditionJump(dwPid, 0x52F + 0x22, 0xAC579, false);
	WriteJump(dwPid, 0x52F + 0x28, 0xAC564);
}
/**
 * 改变僵尸死亡时是否掉落卡片。
 *
 * @param isFeatureEnabled 控制僵尸死亡掉落卡片功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::ZombieDC(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	protectAddress(dwPid, 0xF32);
	protectAddress(dwPid, 0x400);
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\xC6\x83\x9C\x00\x00\x00\x01"; //濒死判断
	WriteToMemory(dwPid, 0x105EE4, patch1, 7);
	const char* patch2 = "\xC6\x83\x9C\x00\x00\x00\x01\x60\xBA\x80\x00\x00\x00\x8B\x0D\x10\x06\xE1\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x73\x18\xFF\x73\x14\xE8\x86\x53\x85\xFF\x61\xE9\x9B\x0A\x8C\xFF";
	WriteToMemory(dwPid, 0xF32, patch2, 48);
	const char* patch3 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x77\x7C\x83\xEC\x08";  //掉落物品随机 
	WriteToMemory(dwPid, 0xB2AC7, patch3, 6);
	const char* patch4 = "\xFF\x77\x7C\x83\xEC\x08\x81\x7F\x7C\xFF\xFF\xFF\xFF\x0F\x85\x6A\xB6\xC2\xFF\xA1\x00\x00\x00\x00\x89\x47\x7C\xE9\x5D\xB6\xC2\xFF";
	WriteToMemory(dwPid, 0xF62, patch4, 32);
	const char* patch5 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x87\x74\x03\x00\x00"; //修改随机数
	WriteToMemory(dwPid, 0x9C583, patch5, 6);
	const char* patch6 = "\xFF\x87\x74\x03\x00\x00\xFF\x05\xC8\xDA\xB9\x00\x83\x3D\xC8\xDA\xB9\x00\x10\x74\x09\x0F\x1F\x40\x00\xE9\x6B\x5F\x89\xFC\xC7\x05\xC8\xDA\xB9\x00\x00\x00\x00\x00\xE9\x5C\x5F\x89\xFC";
	WriteToMemory(dwPid, 0xF82, patch6, 45);
	const char* patch7 = (isFeatureEnabled == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x64\x00\x00\x00\x00"; //灰烬判断
	WriteToMemory(dwPid, 0xAFCCC, patch7, 7);
	const char* patch8 = "\xC7\x47\x64\x00\x00\x00\x00\x60\x8B\xD7\x8B\x0D\x10\x06\x4B\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x84\x53\x00\xFF\x61\xE9\x41\x8B\x01\xFF";
	WriteToMemory(dwPid, 0xFAF, patch8, 50);
	const char* patch9 = (isFeatureEnabled == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x40\x01\x00\x00\x00"; //小推车判断
	WriteToMemory(dwPid, 0x10672F, patch9, 7);
	const char* patch10 = "\xC7\x47\x40\x01\x00\x00\x00\x80\xBF\x9C\x00\x00\x00\x01\x0F\x84\x62\x0F\xD4\xDF\x60\x8B\xD7\x8B\x0D\x10\x06\x33\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x77\x53\xCD\xDF\x61\xE9\x37\x0F\xD4\xDF";
	WriteToMemory(dwPid, 0x4D9, patch10, 63);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x105EE4, 0xF32);
	WriteJump(dwPid, 0xF5D, 0x105EE9);
	WriteCall(dwPid, 0xF57, 0x9A210);
	DWORD eBaseAddress = ReadTOMemory(dwPid, baseAddress + 0x297C54);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xF3F);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xB2AC7,0xF62);
	WriteConditionJump(dwPid, 0xF6F, 0xB2ACB, false);
	WriteJump(dwPid, 0xF7D, 0xB2ACC);
	WriteMovEAX(dwPid, baseAddress + 0x28B0B8, 0xF75);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x9C583, 0xF82);
	WriteINC(dwPid, baseAddress + 0x28B0B8, 0xF88);
	WriteCMPXZero(dwPid, baseAddress + 0x28B0B8, 0xF8E);
	WriteJump(dwPid, 0xF9B, 0x9C588);
	WriteJump(dwPid, 0xFAA, 0x9C588);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xAFCCC, 0xFAF);
	WriteMOVXZero(dwPid, baseAddress + 0x28B0B8, 0xFA0);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0xFB9);
	WriteCall(dwPid, 0xFD6, 0x9A210);
	WriteJump(dwPid, 0xFDC, 0xAFCD1);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x10672F, 0x4D9);
	WriteMovECX(dwPid, eBaseAddress + 0x708, 0x4F0);
	WriteCall(dwPid, 0x50D, 0x9A210);
	WriteJump(dwPid, 0x513, 0x106734);
	WriteConditionJump(dwPid, 0x4E7, 0x106733, true);
	CloseHandle(hProcess);
}
/**
 * 确保植物不会被魅惑。
 *
 * @param isFeatureEnabled 控制防止植物被魅惑功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::NotSubvert(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x89\x46\x10";
	WriteToMemory(dwPid, 0xC9E81 , patch, 3);
}
/**
 * 改变樱桃炸弹的爆炸速度，使之可以瞬间爆炸。
 *
 * @param isFeatureEnabled 控制樱桃炸弹瞬爆功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::CherryFast(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	protectAddress(dwPid, 0x5BD);
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xCAEEE, patch1,10);
	const char* patch2 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xC747B, patch2, 10);
}
/**
 * 防止樱桃炸弹爆炸。
 *
 * @param isFeatureEnabled 控制防止樱桃炸弹爆炸功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::CherryNo(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\xE7\x01\x00\x00\x90" : "\x0F\x8F\xE6\x01\x00\x00";
	WriteToMemory(dwPid, 0xC72B6, patch1, 6);
}
/**
 * 加快猫丝子的效果。
 *
 * @param isFeatureEnabled 控制加快猫丝子效果功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::MeowFast(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCAF89, patch1, 10);
}
/**
 * 荷鲁斯每次攻击都暴击。
 *
 * @param isFeatureEnabled 控制荷鲁斯每次攻击都暴击的功能开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::LoursMC(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0x6BF);
	const char* patch1 = (isFeatureEnabled == 1) ? "\x89\xA7\xBC\x00\x00\x00" :"\x89\x8F\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xC813F, patch1, 6);
}
/**
 * 植物无敌模式。
 *
 * @param isFeatureEnabled 控制植物是否无敌的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::GodMode(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\x90\x90" :"\x2B\xC7";
	WriteToMemory(dwPid, 0xC9F07, patch, 2);
}
/**
 * 取消荣光骄傲状态。
 *
 * @param isFeatureEnabled 控制取消荣光骄傲状态功能的开关。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::Point(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch = (isFeatureEnabled == 1) ? "\xC7\x85\xF8\x02\x00\x00\x00\x00\x00\x00\x90\x90":"\x88\x95\xF8\x02\x00\x00\x8B\x80\x14\x08\x00\x00"; //硬塞进去的，依靠bug运行（
	WriteToMemory(dwPid, 0xD5EA8, patch, 12);
}
/**
 * 导藓批量种植效果的开关。
 *
 * @param isFeatureEnabled 控制导藓批量种植效果是否开启的参数。
 * @return void 该函数不返回任何值。
 */
VOID CPvz::DX(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\x6A\x09\x90\x90\x90\x90" : "\xFF\xB6\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0x9424E, patch1, 6);
	const char* patch2 = (isFeatureEnabled == 1) ? "\x6A\x0D" : "\x6A\x09";
	WriteToMemory(dwPid, 0x95077, patch2, 2);
	const char* patch3 = (isFeatureEnabled == 1) ? "\x83\x78\xE8\x0D" : "\x83\x78\xE8\x09";
	WriteToMemory(dwPid, 0xC50F5, patch3, 4);
	const char* patch4 = (isFeatureEnabled == 1) ? "\x83\x79\x18\x0D" :"\x83\x79\x18\x0D";
	WriteToMemory(dwPid, 0x94F27, patch4, 4);
}
/**
 * 进入游戏修改荣光(废弃)
 *
 * @param isFeatureEnabled 如果设为true，进入草坪会修改荣光；如果为false，则恢复正常。
 */
VOID CPvz::Point2(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0xF00);
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90" :"\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66"; //含bug运行
	WriteToMemory(dwPid, 0x93B42, patch1, 10);
	const char* patch2 = "\x66\xC7\x47\x20\x06\x00\x66\xC7\x47\x24\x06\x00\x66\xC7\x47\x28\x06\x00\x66\xC7\x47\x2C\x03\x00\x66\xC7\x47\x30\x02\x00\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66\x89\x87\xA8\x03\x00\x00\xE9\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xF00, patch2, 46);
	WriteJump(dwPid, 0xF00+0x29+0x6, 0x93B4C);
	if(isFeatureEnabled == 1)WriteJump(dwPid, 0x93B42, 0xF00);
}
/**
 * 光菱角帧伤
 *
 * @param isFeatureEnabled 如果设为true，光菱角造成帧伤；如果为false，则恢复正常。
 */
VOID CPvz::LingSDamage(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0x7F1);
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x0F\x1F\x00" : "\x8B\x47\x38\xB9\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xABBDD, patch1, 8);
	const char* patch2 = "\xB8\x02\x00\x00\x00\xB9\x96\x00\x00\x00\xE9\x96\x4B\x84\xFE";
	WriteToMemory(dwPid, 0x7F1, patch2, 15);
	WriteJump(dwPid, 0x7F1+0xA, 0xABBE5);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xABBDD, 0x7F1);
}
/**
 * 苹果鼓瑟手无冷却
 *
 * @param isFeatureEnabled 如果设为true，苹果鼓瑟手无冷却；如果为false，则恢复正常。
 */
VOID CPvz::ApplayerNoCD(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xB0\x04\x00\x00";
	WriteToMemory(dwPid, 0xCB124, patch1, 10);
	const char* patch2 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x60\x09\x00\x00";
	WriteToMemory(dwPid, 0xC8831, patch2, 10);
}
/**
 * 苹果鼓瑟手无延迟
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::ApplayerNoLag(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xC8883, patch1, 10);
}
/**
 * 车前草无延迟无冷却
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::PlantageNoCD(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x86\x9C\x00\x00\x00\x05\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xD0\x07\x00\x00";
	WriteToMemory(dwPid, 0xC7C78, patch1, 10);
	const char* patch2 = (isFeatureEnabled == 1) ? "\xC7\x86\x9C\x00\x00\x00\x92\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xFA\x00\x00\x00";
	WriteToMemory(dwPid, 0xC9579, patch2, 10);
}
/**
 * 向日葵无冷却
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::SunFlowerNoCD(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\xB8\x0B\x00\x00";
	WriteToMemory(dwPid, 0xC7215, patch1, 10);
	const char* patch2 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xEE\x02\x00\x00";
	WriteToMemory(dwPid, 0xCAECE, patch2, 10);
	const char* patch3 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x24\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x55\x00\x00\x00";
	WriteToMemory(dwPid, 0xC70F1, patch3, 10);
}
/**
 * 豌豆射手无冷却
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::PeaNoCD(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x0F\x85\x6C\x03\x00\x00";
	WriteToMemory(dwPid, 0xC6D37, patch1, 6);
}
/**
 * 闪电芦苇第一次就到达最高伤害
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::SuperReed(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xB9\x66\x66\x66\x66" : "\xB9\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCBCBB, patch1, 5);
	const char* patch2 = (isFeatureEnabled == 1) ? "\x8B\x56\x08" : "\x8B\x52\x08";
	WriteToMemory(dwPid, 0xCBC8B, patch2, 3);
}
/**
 * 滇池牡丹无冷却
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::PowerFlowerNoCD(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xC7\x86\x9C\x00\x00\x00\x00\x00\x00\x00" :"\xC7\x86\x9C\x00\x00\x00\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xCDA09, patch1, 10);
}
/**
 * 滇池牡丹子弹永远最大
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::AwayMax(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	protectAddress(dwPid, 0x749);
	const char* patch1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\x89\xB9\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xCD80B, patch1, 6);
	const char* patch2 = "\xC7\x81\xBC\x00\x00\x00\x05\x00\x00\x00\xE9\x12\x90\x39\xFF";
	WriteToMemory(dwPid, 0x749, patch2, 15);
	WriteJump(dwPid, 0x749 + 0xA, 0xCD811);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xCD80B, 0x749);
}
/**
 * 物品不消失
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::ItemNoDie(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x83\xC0\xFF";
	WriteToMemory(dwPid, 0xB30C1, patch1, 3);
}
/**
 * 天上掉阳光无延迟
 *
 * @param isFeatureEnabled 如果设为true，功能开启；如果为false，则恢复正常。
 */
VOID CPvz::SunNoDelay(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = (isFeatureEnabled == 1) ? "\xB8\x01\x00\x00\x00\x90" : "\x8B\x87\xD4\x03\x00\x00";
	WriteToMemory(dwPid, 0x9FB39, patch1, 6);
}
/**
 * 生成排除某些项的随机数
 *
 * @param const std::vector<int>& invalidIDs 传入排除的列表。
 * @return int 返回随机的结果。
 */
int CPvz::GenerateValidRandomID(const std::vector<int>& invalidIDs) {
	int ID;
	do {
		ID = rand() % 15;
	} while (std::find(invalidIDs.begin(), invalidIDs.end(), ID) != invalidIDs.end());
	return ID;
}
/**
 * 随机全屏布阵
 */
VOID CPvz::BuildTheArray() {
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	const char* patch = "\xC6\x40\x2F\x00";
	WriteToMemory(dwPid, 0x982C3, patch, 4);
	std::vector<int> invalidIDs = { 2, 4, 6, 9, 13 };
	for (int X = 1; X <= 9; ++X) {
		for (int Y = 1; Y <= 5; ++Y) {
			int ID = GenerateValidRandomID(invalidIDs);
			Plant(X, Y, ID); 
			if (ID == 9 || ID == 13) {
				Plant(X, Y + 1, ID);
				Plant(X + 1, Y + 1, ID);
			}
		}
	}
}
/**
 * 清空植物
 */
VOID CPvz::ClearPlant()
{
	DWORD dwPid = GetGamePid();
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	if (!check_dwPid(dwPid, true)) return;
	const char* patch1 = "\xC6\x47\x2F\x00";
	WriteToMemory(dwPid, 0x9B304, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x7F\x2F\x00";
	WriteToMemory(dwPid, 0x9B304, patch1, 4);
}
/**
 * 清空子弹
 */
VOID CPvz::ClearBullet()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	const char* patch1 = "\xC6\x42\x24\x01";
	WriteToMemory(dwPid, 0x90738, patch1, 4);
	Sleep(10);
	patch1 = "\x80\x7A\x24\x00";
	WriteToMemory(dwPid, 0x90738, patch1, 4);
}
/**
 * 清空僵尸
 */
VOID CPvz::ClearZombie()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	if (!check_battlefield(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
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
/**
 * 修复新版本进关卡崩溃bug
 */
VOID CPvz::FixCrashBug()
{
	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	const char* patch1 = "\xEB\x14";
	WriteToMemory(dwPid, 0x185C7A, patch1, 2);
}
/**
 * 当前存档切换至红针花线
 */
VOID CPvz::ToHongZhen()
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	DWORD dwNum = ReadMemory(hProcess, targetAddress);
	dwNum = ReadMemory(hProcess, dwNum + 0x814);
	BOOL result = WriteMemory(hProcess, dwNum + 0x4, 0x1);
}
/**
 * 当前存档切换至导向蓟线
 */
VOID CPvz::ToDaoXiangJi()
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	DWORD dwNum = ReadMemory(hProcess, targetAddress);
	dwNum = ReadMemory(hProcess, dwNum + 0x814);
	BOOL result = WriteMemory(hProcess, dwNum + 0x4, 0x0);
}
/**
 * 苹果鼓瑟手无延迟
 *
 * @param dwDiff 难度的ID。
 */
void CPvz::ShowDiffBox(DWORD dwDiff)
{
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
}
/**
 * 切换当前存档难度
 *
 * @param dwDiff 目标切换难度的ID。
 */
VOID CPvz::DifficultySwitcher(DWORD dwDiff)
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!check_dwPid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD baseAddress = get_baseAddress(hProcess);
	DWORD targetAddress = baseAddress + 0x297C54;

	if (dwDiff > 4) { MessageBox(NULL, L"你所选的难度不存在，请查看难度ID表后填写", L"警告", MB_ICONWARNING | MB_OK); return; }

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档。本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	ShowDiffBox(dwDiff);

	DWORD dwNum = ReadMemory(hProcess, targetAddress);
	dwNum = ReadMemory(hProcess, dwNum + 0x814);
	BOOL result = WriteMemory(hProcess, dwNum + 0x8, dwDiff);
}
void PutNewString(std::string& theOut, int& theWave) {
	if (theWave != 1)
		theOut += "{{WaveZombieList|endwave}}\n";
	theOut += "{{WaveZombieList|startwave|" + std::to_string(theWave) + "}}\n";
	theWave++;
}
void SetClipboardText(const std::string& text) {
	if (OpenClipboard(nullptr)) {
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(char));
		if (hMem != nullptr) {
			char* pData = static_cast<char*>(GlobalLock(hMem));
			if (pData != nullptr) {
				strcpy_s(pData, text.length() + 1, text.c_str());
				GlobalUnlock(hMem);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
			}
		}
		CloseClipboard();
	}
}
std::wstring GetClipboardText()
{
	std::wstring text;

	if (OpenClipboard(nullptr))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		if (hData != nullptr)
		{
			wchar_t* pData = static_cast<wchar_t*>(GlobalLock(hData));
			if (pData != nullptr)
			{
				text = pData;
				GlobalUnlock(hData);
			}
		}
		CloseClipboard();
	}

	return text;
}
VOID CPvz::ConvertToWiki(CString RawData)
{
	std::wstring wData(RawData.GetString());
	std::string aData(CW2A(wData.c_str()));
	using json = nlohmann::json;
	json aLevelJson;
	try {
		aLevelJson = nlohmann::json::parse(aData);
	}
	catch (nlohmann::json::parse_error) {
		MessageBox(NULL, L"该功能将把框内的关卡文件转化成WIKI形式的文本，复制到剪切板中", L"提示", MB_OK);
		std::string errorMessage = "JSON解析失败，请你提供有效的关卡Json文件";
		MessageBoxA(nullptr, errorMessage.c_str(), "Json解析错误", MB_OK);
		return;
	}
	std::map<std::string, std::string> gNameMap = {
	{ "NORMALZOMBIE" , "普通僵尸"},
	{ "FLAGZOMBIE", "摇旗僵尸"},
	{ "CONEZOMBIE", "路障星仪僵尸"},
	{ "BUCKETZOMBIE", "持铁桶僵尸"},
	{ "POLEZOMBIE", "撑魅惑杆僵尸"},
	{ "FIREFLYZOMBIE", "萤火虫僵尸"},
	{ "ARROWZOMBIE", "弓箭僵尸"},
	{ "WITCHZOMBIE", "女巫僵尸"},
	{ "INFROLLZOMBIE", "永动轮僵尸"},
	{ "MAGTRUCKZOMBIE", "磁铁车僵尸"}
	};
	std::string tempPath = std::getenv("TEMP");
	std::ofstream aWrite(tempPath + "\\Temp_Output_Json_Level_File_InWiki.txt");
	std::string aOutString = "{{WaveZombieList|header}}\n";
	bool isStart = true;
	int aWave = 1;

	if (aLevelJson.contains("ZombieList")) {
		json aZombieArray = aLevelJson["ZombieList"];

		for (const auto& aZombieJson : aZombieArray) {
			std::string aZombieString = aZombieJson["Type"].get<std::string>();

			if (aZombieString == "ENDNULLZOMBIE")
				break;

			if (aZombieString == "NULLZOMBIE") {
				isStart = true;
				continue;
			}

			if (isStart) {
				PutNewString(aOutString, aWave);
				isStart = false;
			}

			aOutString += "{{WaveZombieList|" + gNameMap[aZombieString] + "|";
			aOutString += std::to_string(aZombieJson.contains("Row") ? aZombieJson["Row"].get<int>() : -1);
			aOutString += "|";
			aOutString += std::to_string(aZombieJson.contains("LowDif") ? aZombieJson["LowDif"].get<int>() : 0);
			aOutString += "}}\n";
		}

		aOutString += "{{WaveZombieList|endwave}}\n";
	}

	aOutString += "{{WaveZombieList|foot}}";
	aWrite << aOutString;
	aWrite.close();
	std::ifstream aRead(tempPath + "\\Temp_Output_Json_Level_File_InWiki.txt");
	std::string outputData((std::istreambuf_iterator<char>(aRead)), std::istreambuf_iterator<char>());
	aRead.close();
	Sleep(500); //典
	SetClipboardText(outputData);
	MessageBox(NULL, L"内容生成完毕，已复制到剪切板中", L"提示", MB_OK);
}