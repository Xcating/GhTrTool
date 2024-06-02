#include "stdafx.h"
#include "Constant.h"
#include "GhTr.h"
#include "json.hpp"
#include"Macro.h"
#include <chrono>
#include <corecrt_io.h>
#include <cstdlib>
#include <ctime>
#include<cwchar>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <Psapi.h>
#include <sstream>
#include <string>
#include <TlHelp32.h>

GhTrManager::GhTrManager()
{
}

GhTrManager::~GhTrManager()
{
}
/**
 * 获取工具配置文件的路径
 *
 * @return std::filesystem::path 返回工具配置文件的路径
 */
std::filesystem::path GhTrManager::GetConfigFilePath() {
	std::filesystem::path config_folder_path("C:\\ProgramData\\GhTrTool");
	if (!std::filesystem::exists(config_folder_path)) {
		std::filesystem::create_directory(config_folder_path);
	}
	return config_folder_path / "config.json";
}

/**
 * 读取配置文件的内容到JSON对象
 * @param config_file_path 包含配置文件路径的filesystem::path对象
 * @return nlohmann::json 返回配置信息的JSON对象
 */
nlohmann::json GhTrManager::ReadConfigFile(
	const std::filesystem::path& config_file_path) {
	nlohmann::json config_json;
	if (std::filesystem::exists(config_file_path)) {
		std::ifstream config_file_in(config_file_path);
		config_file_in >> config_json;
		config_file_in.close();
	}
	return config_json;
}

/**
 * 将配置信息写入指定的配置文件
 * @param config_file_path 包含配置文件路径的filesystem::path对象
 * @param config_json 包含配置信息的JSON对象
 */
void GhTrManager::WriteConfigFile(
	const std::filesystem::path& config_file_path,
	const nlohmann::json& config_json) {
	std::ofstream config_file_out(config_file_path);
	config_file_out << config_json.dump(4);
	config_file_out.close();
}

/**
 * 从指定的地址读取内存中的DWORD值
 * @param h_process 要读取的进程的句柄
 * @param address 要读取的内存地址
 * @return DWORD 返回从内存中读取的DWORD值
 */
DWORD GhTrManager::ReadTheMemory(HANDLE h_process, DWORD address) {
	DWORD dw_num = 0;
	ReadProcessMemory(h_process, reinterpret_cast<LPCVOID>(address), &dw_num,
		sizeof(DWORD), nullptr);
	return dw_num;
}

/**
 * 向指定的地址写入BYTE值到内存中
 * @param h_process 要写入的进程的句柄
 * @param address 要写入的内存地址
 * @param value 要写入的BYTE值
 * @return bool 返回是否写入成功
 */
bool GhTrManager::WriteByteMemory(HANDLE h_process, DWORD address,
	BYTE value) {
	return WriteProcessMemory(h_process, reinterpret_cast<LPVOID>(address),
		&value, sizeof(BYTE), nullptr) != 0;
}

/**
 * 获取游戏的PID
 *
 * @return DWORD 返回游戏的PID
 */
DWORD GhTrManager::GetGamePid() {
	HWND h_wnd = nullptr;
	wchar_t title[256];
	DWORD dw_pid = 0;
	while ((h_wnd = FindWindowExW(nullptr, h_wnd, nullptr, nullptr)) != nullptr) {
		GetWindowTextW(h_wnd, title, _countof(title));
		std::wstring window_title(title);
		if (window_title.find(GAME_TITLE) == 0) {
			GetWindowThreadProcessId(h_wnd, &dw_pid);
			return dw_pid;
		}
	}
	return static_cast<DWORD>(-1);
}

/**
 * 获取指定进程的模块基址
 *
 * @param h_process 进程句柄
 * @param module_name 模块名称
 * @return DWORD 返回指定进程的模块基址
 */
DWORD GetModulebase_address(HANDLE h_process, const wchar_t* module_name) {
	MODULEENTRY32W module_entry = { sizeof(module_entry) };
	DWORD base_address = 0;

	HANDLE h_snapshot = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(h_process));
	if (h_snapshot != INVALID_HANDLE_VALUE) {
		if (Module32FirstW(h_snapshot, &module_entry)) {
			do {
				if (wcscmp(module_entry.szModule, module_name) == 0) {
					base_address = reinterpret_cast<DWORD>(module_entry.modBaseAddr);
					break;
				}
			} while (Module32NextW(h_snapshot, &module_entry));
		}
		CloseHandle(h_snapshot);
	}

	return base_address;
}

/**
 * 获取游戏进程的模块基址
 *
 * @param h_process 进程句柄
 * @return DWORD 返回游戏进程的模块基址
 */
DWORD GetGamebase_address(HANDLE h_process) {
	DWORD base_address =
		GetModulebase_address(h_process, L"PlantsVsZombies.exe");  // 首先读取大写的
	if (base_address == 0) {
		base_address =
			GetModulebase_address(h_process, L"plantsvszombies.exe");
	}
	return base_address;
}

/**
 * 在指定进程内创建线程，并运行指定内存地址的代码
 *
 * @param dw_pid 目标进程PID
 * @param code_cave_offset 代码偏移量
 */
void RunTheMemory(DWORD dw_pid, DWORD code_cave_offset) {
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(h_process);
	if (base_address == 0) {
		CloseHandle(h_process);
		return;
	}

	DWORD target_address = base_address + code_cave_offset;
	HANDLE h_thread = CreateRemoteThread(
		h_process, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(target_address), nullptr, 0, nullptr);
	if (h_thread != nullptr) {
		WaitForSingleObject(h_thread, INFINITE);
		CloseHandle(h_thread);
	}
	else {
		MessageBoxW(nullptr, L"游戏未找到或没有访问权限", L"提示", MB_OK);
	}
	CloseHandle(h_process);
}

/**
 * 检查游戏进程的PID是否有效
 *
 * @param dw_pid 游戏进程PID
 * @param is_message 是否会提示消息，true为提示，false不提示
 * @return bool 如果PID有效返回true，否则返回false
 */
bool GhTrManager::CheckGamePid(DWORD dw_pid, bool is_message) {
	if (dw_pid == static_cast<DWORD>(-1)) {
		if (is_message) {
			MessageBoxW(nullptr, L"游戏未找到，请打开游戏后点击功能", L"Info", MB_OK);
		}
		return false;
		}
	return true;
	}

/**
 * 枚举Windows窗口进程，为找到的窗口设置标题
 *
 * @param hwnd 窗口句柄
 * @param l_param 附加的参数
 * @return bool 枚举是否继续
 */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM l_param) {

	DWORD dw_pid;
	GetWindowThreadProcessId(hwnd, &dw_pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x80C), &dwNum, sizeof(DWORD), NULL);
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch())
		.count();
#ifdef _DEBUG
	std::wstring wstr =
		(std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage "
			<< L"ver.0.16n - [Debug] [已被GhTrTool修改] "
			<< L"[βver.0.11x] ["
			<< millis << L"]")
		.str();
#else
	std::wstring wstr =
		(std::wstringstream() << L"Plants Vs Zombies GhTr ~ Perfect Voyage "
			<< L"ver.0.16n - [已被GhTrTool修改] [ver.0.11x] ["
			<< millis << L"]" << L" [Save" << dwNum<<L"]")
		.str();
#endif
	if (dw_pid == l_param) {
		SetWindowTextW(hwnd, wstr.c_str());
		return FALSE;
	}
	return TRUE;
}

/**
 * 从指定进程的内存中读取数据
 *
 * @param dw_pid 目标进程PID
 * @param target_address 目标内存地址
 * @return DWORD 返回读取的数据
 */
DWORD ReadTheMemory(DWORD dw_pid, DWORD target_address) {
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD value = 0;
	ReadProcessMemory(h_process, reinterpret_cast<LPCVOID>(target_address),
		&value, sizeof(DWORD), nullptr);
	return value;
}

/**
 * 向指定进程的内存中写入数据
 *
 * @param dw_pid 目标进程PID
 * @param offset 目标内存偏移
 * @param data 要写入的数据指针
 * @param size 要写入的数据大小
 * @return bool 写入成功返回true，否则返回false
 */
bool WriteToMemory(DWORD dw_pid, DWORD offset, const char* data,
	size_t size) {
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	if (h_process == nullptr) {
		return false;
	}
	DWORD base_address = GetGamebase_address(h_process);
	if (base_address == 0) {
		CloseHandle(h_process);
		return false;
	}
	DWORD target_address = base_address + offset;
	bool result =
		WriteProcessMemory(h_process, reinterpret_cast<LPVOID>(target_address),
			data, size, nullptr);
	CloseHandle(h_process);
	return result;
}

/**
 * 修改指定进程内存保护设置
 *
 * @param dw_pid 目标进程PID
 * @param offset 内存偏移量
 */
void ProtectAddress(DWORD dw_pid, DWORD offset) {
	HANDLE h_proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD dw_old_protect;
	DWORD base_address = GetGamebase_address(h_proc);
	DWORD target_address = base_address + offset;
	VirtualProtectEx(h_proc, reinterpret_cast<LPVOID>(target_address), 1024,
		PAGE_EXECUTE_READWRITE, &dw_old_protect);
}

/**
 * 在指定内存写入Jump操作码
 *
 * @param dw_pid 目标进程PID
 * @param source_offset 源内存偏移
 * @param target_offset 目标内存偏移
 * @return bool 写入成功返回true，否则返回false
 */
bool WriteJump(DWORD dw_pid, DWORD source_offset, DWORD target_offset) {
	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	if (h_process == nullptr) {
		return false;
	}

	DWORD base_address = GetGamebase_address(h_process);
	DWORD source_address = base_address + source_offset;
	DWORD target_address = base_address + target_offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(h_process, reinterpret_cast<LPVOID>(source_address), 1024,
		PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = target_address - (source_address + 5);
	BYTE jump_code[5] = { 0xE9, static_cast<BYTE>(jump_offset & 0xFF),
						 static_cast<BYTE>((jump_offset >> 8) & 0xFF),
						 static_cast<BYTE>((jump_offset >> 16) & 0xFF),
						 static_cast<BYTE>((jump_offset >> 24) & 0xFF) };

	bool result = WriteProcessMemory(h_process, reinterpret_cast<LPVOID>(source_address),
		jump_code, sizeof(jump_code), nullptr);
	CloseHandle(h_process);
	return result;
}

/**
 * @brief 在指定内存写入条件跳转(Je或Jne)操作码
 *
 * @param dw_pid 目标进程PID
 * @param source_offset 源内存偏移
 * @param target_offset 目标内存偏移
 * @param je 如果为true写入Je,false写入Jne
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteConditionJump(DWORD dw_pid, DWORD source_offset, DWORD target_offset, bool je)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	if (hProcess == nullptr) {
		return false;
	}

	DWORD base_address = GetGamebase_address(hProcess);
	DWORD source_address = base_address + source_offset;
	DWORD target_address = base_address + target_offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(source_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = target_address - (source_address + 5);
	BYTE jump_code[6] = {
		0x0F,
		static_cast<BYTE>(je ? 0x84 : 0x85),
		static_cast<BYTE>(jump_offset & 0xFF),
		static_cast<BYTE>((jump_offset >> 8) & 0xFF),
		static_cast<BYTE>((jump_offset >> 16) & 0xFF),
		static_cast<BYTE>((jump_offset >> 24) & 0xFF)
	};

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(source_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Push操作码
 *
 * @param dw_pid 目标进程PID
 * @param number 要写入的数值
 * @param offset 内存偏移量
 * @return true 写入成功
 * @return false 写入失败
 */
bool WritePush(DWORD dw_pid, DWORD number, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	if (hProcess == nullptr) {
		return false;
	}

	DWORD base_address = GetGamebase_address(hProcess);
	DWORD source_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(source_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	BYTE jump_code[2] = { 0x6A, static_cast<BYTE>(number & 0xFF) };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(source_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Mov Ecx,[地址]操作码
 *
 * @param dw_pid 目标进程PID
 * @param address 要写入的地址
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteMovECX(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0x8B, 0x0D, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}
/**
 * @brief 在指定内存写入Mov Eax,[地址]操作码
 *
 * @param dw_pid 目标进程PID
 * @param address 要写入的地址
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteMovEAX(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0xA1, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Mov EAX,0x00000操作码
 *
 * @param dw_pid 目标进程PID
 * @param address 要写入的十六进制值
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteMovEAXToHEX(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0xB3, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Mov指令,将值设为0
 *
 * @param dw_pid 目标进程PID
 * @param address 要写入的地址
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteMOVXZero(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0xC7, 0x05, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF, 0x00, 0x00, 0x00, 0x00 };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Cmp指令,比较值是否为0
 *
 * @param dw_pid 目标进程PID
 * @param address 要比较的地址
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteCMPXZero(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0x83, 0x3D, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF, 0xF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Inc指令,增加地址处的值
 *
 * @param dw_pid 目标进程PID
 * @param address 要增加值的地址
 * @param offset 内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteINC(DWORD dw_pid, DWORD address, DWORD offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(target_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = address;
	BYTE jump_code[] = { 0xFF, 0x05, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(target_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}

/**
 * @brief 在指定内存写入Call指令,用于调用函数
 *
 * @param dw_pid 目标进程PID
 * @param source_offset 源内存偏移
 * @param target_offset 目标函数的内存偏移
 * @return true 写入成功
 * @return false 写入失败
 */
bool WriteCall(DWORD dw_pid, DWORD source_offset, DWORD target_offset)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	if (hProcess == nullptr) {
		return false;
	}

	DWORD base_address = GetGamebase_address(hProcess);
	DWORD source_address = base_address + source_offset;
	DWORD target_address = base_address + target_offset;
	DWORD dw_old_protect = 0;
	VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(source_address), 1024, PAGE_EXECUTE_READWRITE, &dw_old_protect);

	DWORD jump_offset = target_address - (source_address + 5);
	BYTE jump_code[5] = { 0xE8, jump_offset & 0xFF, (jump_offset >> 8) & 0xFF, (jump_offset >> 16) & 0xFF, (jump_offset >> 24) & 0xFF };

	bool result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(source_address), jump_code, sizeof(jump_code), nullptr);
	CloseHandle(hProcess);
	return result;
}
/**
 * @brief 检查是否进入草坪
 *
 * @param dw_pid 目标进程PID
 * @return true 进入草坪
 * @return false 未进入草坪
 */
bool GhTrManager::CheckLawn(DWORD dw_pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD e_base_address = ReadTheMemory(hProcess, base_address + GAME_BASE_OFFSET);
	DWORD check_point = ReadTheMemory(hProcess, e_base_address + GAME_BOARD_OFFSET);

	CloseHandle(hProcess);
	return check_point != 0;
}

/**
 * @brief 压入新的僵尸
 *
 * @param the_out 目标总文本
 * @param the_wave 当前波数
 */
void PutNewString(std::string& the_out, int& the_wave)
{
	if (the_wave != 1) {
		the_out += "{{WaveZombieList|endwave}}\n";
	}

	the_out += "{{WaveZombieList|startwave|" + std::to_string(the_wave) + "}}\n";
	the_wave++;
}

/**
 * @brief 复制到剪切板
 *
 * @param text 目标复制文本
 */
void SetClipboardText(const std::string& text)
{
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

/**
 * @brief 写入配置文件如果config.json不存在,则创建文件并设置"isCheat"为true
 */
void GhTrManager::WriteConfig()
{
	DWORD dw_pid = GetGamePid();
	if (!CheckGamePid(dw_pid, false)) {
		return;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dw_pid);

	// 获取游戏主程序位置
	wchar_t sz_exe_path[MAX_PATH];
	if (GetModuleFileNameEx(hProcess, nullptr, sz_exe_path, MAX_PATH) == 0) {
		CloseHandle(hProcess);
		return;
	}
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x80C), &dwNum, sizeof(DWORD), NULL);
	std::filesystem::path exe_path(sz_exe_path);
	std::filesystem::path config_path = exe_path.parent_path() / "definition" / "config.json";

	// 在definition/config.json 写入GhTr信息
	nlohmann::json config_json;
	if (!std::filesystem::exists(config_path)) {
		std::ofstream config_file_out(config_path);
		config_json["isCheat"] = true;
		config_file_out << config_json.dump(4);
		config_file_out.close();
	}
	EnumWindows(EnumWindowsProc, dw_pid);
	CloseHandle(hProcess);
	if (dwNum == 0)
		return;
	std::ostringstream file_path_stream;

	file_path_stream << "C:\\ProgramData\\PerfectVoyage\\userdata\\save" << dwNum << "\\SaveInfor.ghtr";
	std::string file_path = file_path_stream.str();
	// 读取JSON文件
	nlohmann::json j;
	std::ifstream input_file(file_path);
	if (input_file.is_open()) {
		input_file >> j;
		input_file.close();
	}
	// 修改Cheat为true，如果不存在则创建
	j["Cheat"] = true;
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch())
		.count();
	j["LastCheatTime"] = millis;
	// 写入修改后的JSON到文件
	std::ofstream output_file(file_path);
	if (output_file.is_open()) {
		output_file << j.dump(4); // 以漂亮打印的格式写入文件（缩进为4个空格）
		output_file.close();
	}
	else {
		MessageBoxA(NULL, "无法打开文件进行写入！请检查你的GhTr是否能正确的存档", "警告", MB_OK);
	}
}
/**
 * 修改游戏中的阳光数量
 *
 * @param DWORD dwSun 要设置的阳光数值
 */
void GhTrManager::ModifySunValue(DWORD dwSun)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + GAME_BOARD_OFFSET), &dwNum, sizeof(DWORD), NULL); 
	bool result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + GAME_SUN_OFFSET), &dwSun, sizeof(DWORD), NULL); 
	CloseHandle(hProcess);
}
/**
 * 修改游戏中的（卡槽）数量
 *
 * @param DWORD dwSP 要设置的卡槽数量
 */
void GhTrManager::ModifyEnableFrameDamageNumber(DWORD dwSP)
{

	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + GAME_BOARD_OFFSET), &dwNum, sizeof(DWORD), NULL); //一层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + GAME_CARD_OFFSET), &dwNum, sizeof(DWORD), NULL); //二层偏移
	bool result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + GAME_CARD_NUMBER_OFFSET), &dwSP, sizeof(DWORD), NULL); //三层偏移
	CloseHandle(hProcess);
}
/**
 * 替换种子包中特定卡槽的植物
 *
 * @param DWORD dwID 要替换的种子包ID
 * @param DWORD dwNum 要替换的卡槽编码
 */
void GhTrManager::ModifyEnableFrameDamageData(DWORD dwID,DWORD dwNum)
{
	dwNum--;
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum1 = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum1, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + GAME_BOARD_OFFSET), &dwNum1, sizeof(DWORD), NULL); //一层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + GAME_CARD_OFFSET), &dwNum1, sizeof(DWORD), NULL); //二层偏移
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum1 + GAME_CARD_ID_OFFSET), &dwNum1, sizeof(DWORD), NULL); //三层偏移
	WriteProcessMemory(hProcess, (LPVOID)(dwNum1 + GAME_CARD_ID_START_OFFSET + GAME_CARD_SIZE*dwNum), &dwID, sizeof(DWORD), NULL); //四层偏移
	CloseHandle(hProcess);
}
/**
 * 设置阳光消耗的NOP开关，使种植植物不减少阳光值
 *
 * @param bool isFeatureEnabled 控制功能开关
 */
void GhTrManager::PlantWithoutSunReduction(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? GAME_PLANT_SUB_SUN_PATCH_OPCODE : GAME_PLANT_SUB_SUN_ORIGINAL_OPCODE;
	WriteToMemory(dwPid, GAME_PLANT_SUB_SUN_OFFSET, operational_code, 6);
}
/**
 * 设置种植植物无冷却的开关
 *
 * @param bool isFeatureEnabled 控制功能开关
 */
void GhTrManager::DisableCooldowns(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? GAME_CARD_NO_CD_PATCH_OPCODE : GAME_CARD_NO_CD_ORIGINAL_OPCODE;
	WriteToMemory(dwPid, GAME_PLANT_NO_CD_OFFSET, operational_code1, 10);
	const char* operational_code2 = (isFeatureEnabled == 1) ? GAME_CARD_IGNORE_CD_PATCH_OPCODE : GAME_CARD_IGNORE_CD_ORIGINAL_OPCODE;
	WriteToMemory(dwPid, GAME_PLANT_IGNORE_CD_OFFSET, operational_code2, 2); //bug
}
/**
 * 修改背景ID
 *
 * @param dwBGId 要设置的背景ID
 */
void GhTrManager::ChangeBackgroundID(DWORD dwBGId)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + GAME_BOARD_OFFSET), &dwNum, sizeof(DWORD), NULL);
	bool result=WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x164), &dwBGId, sizeof(DWORD), NULL);
	CloseHandle(hProcess);
}
/**
 * 重复建造，无需荷叶
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::Build(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90\x90" : "\xFF\x24\x85\xAC\x9F\xF0\x00";
	WriteToMemory(dwPid, 0x99DA1, operational_code, 7);
}
/**
 * 自动收集阳光
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::EnableAutoCollectSun(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90":"\x0F\x84\xC4\x01\x00\x00";
	WriteToMemory(dwPid, 0xB4B4E, operational_code, 6);
}
/**
 * 全部帧伤
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::EnableFrameDamage(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90" : "\xC6\x46\x24\x01"; 
	WriteToMemory(dwPid, 0xADE8E, operational_code, 4);
}
/**
 * 加速僵尸出动
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::Fast(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x89\x97\x00\x04\x00\x00":"\x89\x8F\x00\x04\x00\x00";
	WriteToMemory(dwPid, 0x9FFD1, operational_code, 6);
}
/**
 * 更好的高级暂停
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::TheWorld(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x83\xF8\x32";
	WriteToMemory(dwPid, 0x9CB32, operational_code, 3);
}
/**
 * 无主动技能冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::DisableSkillCooldown(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0x43A);
	const char* operational_code = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\x0F\xBF\x87\xC4\x03\x00\x00";
	WriteToMemory(dwPid, 0x91798, operational_code, 7);
	const char* operational_code2 = "\x66\xC7\x87\xC4\x03\x00\x00\x00\x00\x0F\xBF\x87\xC4\x03\x00\x00\xE9\xD4\x16\x29\xFF";
	WriteToMemory(dwPid, 0x43A, operational_code2, 21);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x91798, 0x43A);
	WriteJump(dwPid, 0x44A , 0x9179F);
}
/**
 * 无限制阳光
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::RemoveSunProductionLimit(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x89\x81\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8F78D, operational_code, 6);
}
/**
 * 建筑物秒消失
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::InstantObstacleDisappearance(bool isFeatureEnabled){
	DWORD dwPid = GetGamePid(); 
	const char* operational_code = (isFeatureEnabled == 1) ? "\xC7\x86\x94\x00\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90" : "\x89\x86\x94\x00\x00\x00\x85\xC0\x0F\x85\xDC\x01\x00\x00";
	WriteToMemory(dwPid, 0xAACB3, operational_code, 14);
}
/**
 * 忽略阳光
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::PlantWithoutSunCost(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\xB8\x3F\x3F\x3F\x3F\x90" : "\x8B\x83\x84\x03\x00\x00";
	WriteToMemory(dwPid, 0x8FC5A, operational_code, 6);
}
/**
 * 割草机不动
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::DisableLawnMowerMovement(bool isFeatureEnabled) {
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90\x90\x90":"\xF3\x0F\x11\x87\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0xC5602, operational_code, 8);
}
/**
 * 召唤奖杯
 * 只在草坪中有效，如果未进入草坪，则会弹出提示
 */
void GhTrManager::CompleteLevelWithTrophy() {
	DWORD dwPid = GetGamePid();
	ProtectAddress(dwPid, 0x45E);
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	const char* shellcode = "\x60\x9C\xBA\x80\x00\x00\x00\x8B\x0D\x28\xAE\x29\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x02\x6A\x18\x6A\x7B\xE8\xB3\x94\x00\x00\x9D\x61\xC3";
	DWORD ebase_address = ReadTheMemory(hProcess,base_address+GAME_BASE_OFFSET);
	WriteToMemory(dwPid, 0x45E, shellcode, 37);
	WriteMovECX(dwPid, ebase_address +GAME_BOARD_OFFSET, 0x45E + 0x7);
	WriteCall(dwPid,0x45E + 0x1D, 0x9A4E0);
	RunTheMemory(dwPid, 0x45E);
}
/**
 * 在游戏中种植指定植物
 *
 * @param dwXP 植物的X坐标位置（通过传递值自动减1处理）
 * @param dwYP 植物的Y坐标位置（通过传递值自动减1处理）
 * @param dwID 植物的ID
 */
void GhTrManager::Plant(DWORD dwXP, DWORD dwYP, DWORD dwID)
{
	dwXP--; dwYP--;
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	ProtectAddress(dwPid, 0x348);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	const char* shellcode = "\x60\x6A\x01\x6A\x07\x8B\x0D\x28\xAE\x29\x00\x6A\x01\x6A\x01\xE8\xB3\x94\x00\x00\x61\xC3";
	DWORD ebase_address = ReadTheMemory(hProcess, base_address + GAME_BASE_OFFSET);
	DWORD value = ReadTheMemory(hProcess, ebase_address + 0x320);
	WriteToMemory(dwPid, 0x348, shellcode, 22);
	WriteMovECX(dwPid, value + 0x7C, 0x348 + 0x5);
	WriteCall(dwPid, 0x348 + 0xF, 0x9B550);
	WritePush(dwPid, dwXP, 0x348 + 0xD);
	WritePush(dwPid, dwYP, 0x348 + 0xB);
	WritePush(dwPid, dwID, 0x348 + 0x3);
	RunTheMemory(dwPid, 0x348);
}
/**
 * 豌豆子弹造成帧伤
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::EnablePeashooterFrameDamage(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0x52F);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xC6\x46\x24\x01\x75\x16";
	WriteToMemory(dwPid, 0xADE8E, operational_code1, 6);
	const char* operational_code2 = "\x80\x7E\x08\x01\x0F\x84\x2A\x55\x9C\xFE\x80\x7E\x08\x02\x0F\x84\x20\x55\x9C\xFE\x80\x7E\x08\x03\x0F\x84\x16\x55\x9C\xFE\xC6\x46\x24\x01\x0F\x85\x22\x55\x9C\xFE\xE9\x07\x55\x9C\xFE";
	WriteToMemory(dwPid, 0x52F, operational_code2, 45);
	if(isFeatureEnabled == 1) WriteJump(dwPid, 0xADE8E, 0x52F);
	WriteConditionJump(dwPid,0x52F + 0x4,0xADE93,true);
	WriteConditionJump(dwPid, 0x52F + 0xE, 0xADE93, true);
	WriteConditionJump(dwPid, 0x52F + 0x18, 0xADE93, true);
	WriteConditionJump(dwPid, 0x52F + 0x22, 0xADEA9, false);
	WriteJump(dwPid, 0x52F + 0x28, 0xADE94);
}
/**
 * 改变僵尸死亡时是否掉落卡片
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::ZombiesDropCardsOnDeath(bool isFeatureEnabled)
{
	MessageBoxA(NULL, "本功能已弃用", "提示", MB_OK);
	return;
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	ProtectAddress(dwPid, 0xF32);
	ProtectAddress(dwPid, 0x400);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x66\x90" : "\xC6\x83\x9C\x00\x00\x00\x01"; //濒死判断
	WriteToMemory(dwPid, 0x1073C4, operational_code1, 7);
	const char* operational_code2 = "\xC6\x83\x9C\x00\x00\x00\x01\x60\xBA\x80\x00\x00\x00\x8B\x0D\x10\x06\xE1\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x73\x18\xFF\x73\x14\xE8\x86\x53\x85\xFF\x61\xE9\x9B\x0A\x8C\xFF";
	WriteToMemory(dwPid, 0xF32, operational_code2, 48);
	const char* operational_code3 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x77\x7C\x83\xEC\x08";  //掉落物品随机 
	WriteToMemory(dwPid, 0xB2F17, operational_code3, 6);
	const char* operational_code4 = "\xFF\x77\x7C\x83\xEC\x08\x81\x7F\x7C\xFF\xFF\xFF\xFF\x0F\x85\x6A\xB6\xC2\xFF\xA1\x00\x00\x00\x00\x89\x47\x7C\xE9\x5D\xB6\xC2\xFF";
	WriteToMemory(dwPid, 0xF62, operational_code4, 32);
	const char* operational_code5 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\xFF\x87\x74\x03\x00\x00"; //修改随机数
	WriteToMemory(dwPid, 0x9C663, operational_code5, 6);
	const char* operational_code6 = "\xFF\x87\x74\x03\x00\x00\xFF\x05\xC8\xDA\xB9\x00\x83\x3D\xC8\xDA\xB9\x00\x10\x74\x09\x0F\x1F\x40\x00\xE9\x6B\x5F\x89\xFC\xC7\x05\xC8\xDA\xB9\x00\x00\x00\x00\x00\xE9\x5C\x5F\x89\xFC";
	WriteToMemory(dwPid, 0xF82, operational_code6, 45);
	const char* operational_code7 = (isFeatureEnabled == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x64\x00\x00\x00\x00"; //灰烬判断
	WriteToMemory(dwPid, 0xB011C, operational_code7, 7);
	const char* operational_code8 = "\xC7\x47\x64\x00\x00\x00\x00\x60\x8B\xD7\x8B\x0D\x10\x06\x4B\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x84\x53\x00\xFF\x61\xE9\x41\x8B\x01\xFF";
	WriteToMemory(dwPid, 0xFAF, operational_code8, 50);
	const char* operational_code9 = (isFeatureEnabled == 1) ? "\xE9\x8F\x74\xFE\x00\x66\x90" : "\xC7\x47\x40\x01\x00\x00\x00"; //小推车判断
	WriteToMemory(dwPid, 0x107C0F, operational_code9, 7);
	const char* operational_code10 = "\xC7\x47\x40\x01\x00\x00\x00\x80\xBF\x9C\x00\x00\x00\x01\x0F\x84\x62\x0F\xD4\xDF\x60\x8B\xD7\x8B\x0D\x10\x06\x33\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x77\x53\xCD\xDF\x61\xE9\x37\x0F\xD4\xDF";
	WriteToMemory(dwPid, 0x4D9, operational_code10, 63);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x1073C4, 0xF32);
	WriteJump(dwPid, 0xF5D, 0x1073CB);
	WriteCall(dwPid, 0xF57, 0x9A2F0);
	DWORD ebase_address = ReadTheMemory(hProcess, base_address + GAME_BASE_OFFSET);
	WriteMovECX(dwPid, ebase_address + GAME_BOARD_OFFSET, 0xF3F);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xB2F17,0xF62);
	WriteConditionJump(dwPid, 0xF6F, 0xB2F1C, false); //-1
	WriteJump(dwPid, 0xF7D, 0xB2F1D);
	WriteMovEAX(dwPid, base_address + 0x28B0B8, 0xF75);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x9C663, 0xF82);
	WriteINC(dwPid, base_address + 0x28B0B8, 0xF88);
	WriteCMPXZero(dwPid, base_address + 0x28B0B8, 0xF8E);
	WriteJump(dwPid, 0xF9B, 0x9C669);
	WriteJump(dwPid, 0xFAA, 0x9C669);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xB011C, 0xFAF);
	WriteMOVXZero(dwPid, base_address + 0x28B0B8, 0xFA0);
	WriteMovECX(dwPid, ebase_address + GAME_BOARD_OFFSET, 0xFB9);
	WriteCall(dwPid, 0xFD6, 0x9A2F0);
	WriteJump(dwPid, 0xFDC, 0xB0123);

	if (isFeatureEnabled == 1)WriteJump(dwPid, 0x107C0F, 0x4D9);
	WriteMovECX(dwPid, ebase_address + GAME_BOARD_OFFSET, 0x4F0);
	WriteCall(dwPid, 0x50D, 0x9A2F0);
	WriteJump(dwPid, 0x513, 0x107C16);
	WriteConditionJump(dwPid, 0x4E7, 0x107C15, true); //-1
	CloseHandle(hProcess);
}
/**
 * 确保植物不会被魅惑
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::PreventPlantCharm(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x89\x46\x10";
	WriteToMemory(dwPid, 0xCD431, operational_code, 3);
}
/**
 * 改变樱桃炸弹的爆炸速度，使之可以瞬间爆炸
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::InstantCherryBombExplosion(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	ProtectAddress(dwPid, 0x5BD);
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xCE2DE, operational_code1,10);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x64\x00\x00\x00";
	WriteToMemory(dwPid, 0xCA1CB, operational_code2, 10);
}
/**
 * 防止樱桃炸弹爆炸
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::PreventCherryBombExplosion(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\xE7\x01\x00\x00\x90" : "\x0F\x8F\xE6\x01\x00\x00";
	WriteToMemory(dwPid, 0xCA006, operational_code1, 6);
}
/**
 * 加快猫丝子的效果
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::EnableInstantCascutaProjectile(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCE37E, operational_code1, 10);
}
/**
 * 荷鲁斯每次攻击都暴击
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::EnableCriticalHitsForLorus(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0x6BF);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\x89\xA7\xBC\x00\x00\x00" :"\x89\x8F\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xCAE2F, operational_code1, 6);
}
/**
 * 植物无敌模式
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::MakePlantsInvincible(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\x90\x90" :"\x2B\xC7";
	WriteToMemory(dwPid, 0xCD4C2, operational_code, 2);
}
/**
 * 取消荣光骄傲状态
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::RefusePrideEffect(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code = (isFeatureEnabled == 1) ? "\xC7\x85\xF8\x02\x00\x00\x00\x00\x00\x00\x90\x90":"\x88\x95\xF8\x02\x00\x00\x8B\x80\x14\x08\x00\x00"; //硬塞进去的，依靠bug运行（
	WriteToMemory(dwPid, 0xD9638, operational_code, 12);
}
/**
 * 导藓批量种植效果的开关
 *
 * @param isFeatureEnabled 控制功能开关
 * @return void 该函数不返回任何值
 */
void GhTrManager::DX(bool isFeatureEnabled) //已废弃，不更新，但保留
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\x6A\x09\x90\x90\x90\x90" : "\xFF\xB6\x84\x00\x00\x00";
	WriteToMemory(dwPid, 0x9424E, operational_code1, 6);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\x6A\x0D" : "\x6A\x09";
	WriteToMemory(dwPid, 0x95077, operational_code2, 2);
	const char* operational_code3 = (isFeatureEnabled == 1) ? "\x83\x78\xE8\x0D" : "\x83\x78\xE8\x09";
	WriteToMemory(dwPid, 0xC50F5, operational_code3, 4);
	const char* operational_code4 = (isFeatureEnabled == 1) ? "\x83\x79\x18\x0D" :"\x83\x79\x18\x0D";
	WriteToMemory(dwPid, 0x94F27, operational_code4, 4);
}
/**
 * 进入游戏修改荣光(废弃)
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::RefusePrideEffect2(bool isFeatureEnabled) //已废弃，不更新，但保留
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0xF00);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90" :"\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66"; //含bug运行
	WriteToMemory(dwPid, 0x93B42, operational_code1, 10);
	const char* operational_code2 = "\x66\xC7\x47\x20\x06\x00\x66\xC7\x47\x24\x06\x00\x66\xC7\x47\x28\x06\x00\x66\xC7\x47\x2C\x03\x00\x66\xC7\x47\x30\x02\x00\x8B\x81\x14\x08\x00\x00\x0F\xB7\x40\x20\x66\x89\x87\xA8\x03\x00\x00\xE9\x00\x00\x00\x00";
	WriteToMemory(dwPid, 0xF00, operational_code2, 46);
	WriteJump(dwPid, 0xF00+0x29+0x6, 0x93B4C);
	if(isFeatureEnabled == 1)WriteJump(dwPid, 0x93B42, 0xF00);
}
/**
 * 光菱角帧伤
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::EnableOpticaltropFrameDamage(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0x7F1);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x0F\x1F\x00" : "\x8B\x47\x38\xB9\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xAD0FD, operational_code1, 8);
	const char* operational_code2 = "\xB8\x02\x00\x00\x00\xB9\x96\x00\x00\x00\xE9\x96\x4B\x84\xFE";
	WriteToMemory(dwPid, 0x7F1, operational_code2, 15);
	WriteJump(dwPid, 0x7F1+0xA, 0xAD105);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xAD0FD, 0x7F1);
}
/**
 * 苹果鼓瑟手无冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::ApplayerDisableCooldowns(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xB0\x04\x00\x00";
	WriteToMemory(dwPid, 0xCE51A, operational_code1, 10);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x60\x09\x00\x00";
	WriteToMemory(dwPid, 0xCB521, operational_code2, 10);
}
/**
 * 苹果鼓瑟手无延迟
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::RemoveApplayerDelay(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCB573, operational_code1, 10);
}
/**
 * 车前草无延迟无冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::PlantageDisableCooldowns(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x86\x9C\x00\x00\x00\x05\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xD0\x07\x00\x00";
	WriteToMemory(dwPid, 0xCA9AE, operational_code1, 10);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\xC7\x86\x9C\x00\x00\x00\x92\x00\x00\x00" : "\xC7\x86\x9C\x00\x00\x00\xFA\x00\x00\x00";
	WriteToMemory(dwPid, 0xCCB09, operational_code2, 10);
}
/**
 * 向日葵无冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::SunFlowerDisableCooldowns(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\xB8\x0B\x00\x00";
	WriteToMemory(dwPid, 0xC84D5, operational_code1, 10);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00" : "\xC7\x83\x9C\x00\x00\x00\xEE\x02\x00\x00";
	WriteToMemory(dwPid, 0xCBF2E, operational_code2, 10);
	const char* operational_code3 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x24\x00\x00\x00" : "\xC7\x87\x9C\x00\x00\x00\x55\x00\x00\x00";
	WriteToMemory(dwPid, 0xC83B1, operational_code3, 10);
}
/**
 * 豌豆射手无冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::PeaDisableCooldowns(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\x90\x90\x90\x90\x90\x90" : "\x0F\x85\x6C\x03\x00\x00";
	WriteToMemory(dwPid, 0xC7FF7, operational_code1, 6);
}
/**
 * 闪电芦苇第一次就到达最高伤害
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::EnhanceLightningReedAbility(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xB9\x66\x66\x66\x66" : "\xB9\x2C\x01\x00\x00";
	WriteToMemory(dwPid, 0xCF1AB, operational_code1, 5);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\x8B\x56\x08" : "\x8B\x52\x08";
	WriteToMemory(dwPid, 0xCF17B, operational_code2, 3);
}
/**
 * 滇池牡丹无冷却
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::PowerFlowerDisableCooldowns(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xC7\x87\x9C\x00\x00\x00\x00\x00\x00\x00" :"\xC7\x87\x9C\x00\x00\x00\x96\x00\x00\x00";
	WriteToMemory(dwPid, 0xD10B9, operational_code1, 10);
}
/**
 * 滇池牡丹子弹永远最大
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::MaintainMaximumPowerPlantSize(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	ProtectAddress(dwPid, 0x749);
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90" : "\x89\xB9\xBC\x00\x00\x00";
	WriteToMemory(dwPid, 0xD0EBB, operational_code1, 6);
	const char* operational_code2 = "\xC7\x81\xBC\x00\x00\x00\x05\x00\x00\x00\xE9\x12\x90\x39\xFF";
	WriteToMemory(dwPid, 0x749, operational_code2, 15);
	WriteJump(dwPid, 0x749 + 0xA, 0xD0EC1);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xD0EBB, 0x749);
}
/**
 * 物品不消失
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::PreventItemDeterioration(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\x90\x90\x90" : "\x83\xC0\xFF";
	WriteToMemory(dwPid, 0xB4A21, operational_code1, 3);
}
/**
 * 天上掉阳光无延迟
 *
 * @param isFeatureEnabled 控制功能开关
 */
void GhTrManager::InstantSunGeneration(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\xB8\x01\x00\x00\x00\x90" : "\x8B\x87\xD4\x03\x00\x00";
	WriteToMemory(dwPid, 0x9FE12, operational_code1, 6);
}
/**
 * 生成排除某些项的随机数
 *
 * @param const std::vector<int>& invalidIDs 传入排除的列表
 * @return int 返回随机的结果
 */
int GhTrManager::GenerateValidRandomID(const std::vector<int>& invalidIDs) {
	int ID;
	do {
		ID = rand() % 15;
	} while (std::find(invalidIDs.begin(), invalidIDs.end(), ID) != invalidIDs.end());
	return ID;
}
/**
 * 随机全屏布阵
 */
void GhTrManager::DeployFormationInstantly()
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = "\xC6\x47\x2F\x00";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
	Sleep(10);
	operational_code1 = "\x80\x7F\x2F\x00";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
	for (int X = 1; X <= 9; ++X) {
		Sleep(80);
		for (int Y = 1; Y <= 5; ++Y) {
			Sleep(80);
			int ID;
			do {
				ID = rand() % 15; // 生成1~15范围内的随机ID
			} while (ID == 2 || ID == 6);
			if (ID == 4 || ID == 9 || ID == 13)
			{
				if (ID == 9 || ID == 13)
				{
					Plant(X, Y + 1, ID);
					Plant(X+1, Y, ID);
					Sleep(10);
					Plant(X + 1, Y + 1, ID);
				}
				Plant(X, Y, ID);
				do {
					ID = rand() % 15; // 生成0~15范围内的随机数
				} while (ID == 4 || ID == 9 || ID == 13 || ID == 2 || ID == 6);
				Plant(X, Y, ID);
			}
			else
			{
				Plant(X, Y, ID);
			}
		}
	}
}
/**
 * 清空植物
 */
void GhTrManager::RemoveAllPlants()
{
	DWORD dwPid = GetGamePid();
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = "\xC6\x47\x2F\x00";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
	Sleep(10);
	operational_code1 = "\x80\x7F\x2F\x00";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
}
/**
 * 清空子弹
 */
void GhTrManager::RemoveAllProjectiles()
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	const char* operational_code1 = "\xC6\x42\x24\x01";
	WriteToMemory(dwPid, 0x908B8, operational_code1, 4);
	Sleep(10);
	operational_code1 = "\x80\x7A\x24\x00";
	WriteToMemory(dwPid, 0x908B8, operational_code1, 4);
}
/**
 * 清空僵尸
 */
void GhTrManager::RemoveAllZombies()
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	if (!CheckLawn(dwPid))
	{
		MessageBox(NULL, L"未进入战场", L"提示", MB_OK);
		return;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	const char* operational_code1 = "\xC6\x47\x2F\x01";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
	Sleep(10);
	operational_code1 = "\x80\x7F\x2F\x00 ";
	WriteToMemory(dwPid, 0x9B824, operational_code1, 4);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	DWORD dwTimer = 0x0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL); 
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + GAME_BOARD_OFFSET), &dwNum, sizeof(DWORD), NULL); 
	bool result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x400), &dwTimer, sizeof(DWORD), NULL); 
}
/**
 * 修复新版本进关卡崩溃bug
 */
void GhTrManager::CorrectCrashIssue() //已废弃，不更新，但保留
{
	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	const char* operational_code1 = "\xEB\x14";
	WriteToMemory(dwPid, 0x185C7A, operational_code1, 2);
}
/**
 * 当前存档切换至红针花线
 */
void GhTrManager::SwitchToRedStingerMode()
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	DWORD dwNum = ReadTheMemory(hProcess, target_address);
	dwNum = ReadTheMemory(hProcess, dwNum + 0x818);
	bool result = WriteByteMemory(hProcess, dwNum + 0x4, 0x1);
}
/**
 * 当前存档切换至导向蓟线
 */
void GhTrManager::SwitchToHomingThistleMode()
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	DWORD dwNum = ReadTheMemory(hProcess, target_address);
	dwNum = ReadTheMemory(hProcess, dwNum + 0x818);
	bool result = WriteByteMemory(hProcess, dwNum + 0x4, 0x0);
}
/**
 * 苹果鼓瑟手无延迟
 *
 * @param dwDiff 难度的ID
 */
void GhTrManager::ShowDifficultyNotification(DWORD dwDiff)
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
 * 切换是否可以创建Ub存档的变量
 */
void GhTrManager::EnableCreationOfUbSaves()
{
	bool ifNoProcess=false;
	DWORD dwPid = GetGamePid();
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;
	DWORD dwNum = 0;
	DWORD dwNumF = 0;
	DWORD dwNumCheck = 0;
	ReadProcessMemory(hProcess, (LPCVOID)target_address, &dwNum, sizeof(DWORD), NULL);
	ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x7EC), &dwNumCheck, sizeof(BYTE), NULL);
	if (dwNumCheck == 0)
	{
		dwNumF = 1;
		WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x7EC), &dwNumF, sizeof(BYTE), NULL);
		MessageBox(NULL, L"成功{允许}创建Unbalanced存档的创建，再次点击禁止。", L"提示", MB_OK);
	}
	else
	{
		WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x7EC), &dwNumF, sizeof(BYTE), NULL);
		MessageBox(NULL, L"成功{禁止}创建Unbalanced存档的创建，再次点击允许。", L"提示", MB_OK);
	}
}
/**
 * 切换当前存档难度
 *
 * @param dwDiff 目标切换难度的ID
 */
void GhTrManager::DifficultySwitcher(DWORD dwDiff)
{
	std::filesystem::path configFilePath = GetConfigFilePath();
	nlohmann::json configJson = ReadConfigFile(configFilePath);

	DWORD dwPid = GetGamePid();
	if (!CheckGamePid(dwPid, true)) return;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	DWORD base_address = GetGamebase_address(hProcess);
	DWORD target_address = base_address + GAME_BASE_OFFSET;

	if (dwDiff > 4) { MessageBox(NULL, L"你所选的难度不存在，请查看难度ID表后填写", L"警告", MB_ICONWARNING | MB_OK); return; }

	int response = 0;
	if (!std::filesystem::exists(configFilePath)) {
		MessageBox(NULL, L"使用该功能前，请先备份存档本功能会污染你的存档！！！", L"提示", MB_OK);
		response = MessageBox(NULL, L"是否继续修改你的存档？", L"提示", MB_OK | MB_ICONQUESTION | MB_OKCANCEL);
	}
	if (response == IDCANCEL) return;

	configJson["isModifyArchive"] = true;
	WriteConfigFile(configFilePath, configJson);

	ShowDifficultyNotification(dwDiff);

	DWORD dwNum = ReadTheMemory(hProcess, target_address);
	dwNum = ReadTheMemory(hProcess, dwNum + 0x818);
	bool result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x8), &dwDiff, sizeof(DWORD), NULL);
}
/**
 * Json出怪表格式转换成Wiki出怪表格式
 *
 * @param RawData Json出怪表格式
 */
void GhTrManager::ConvertToWiki(CString RawData)
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
	{ "MAGTRUCKZOMBIE", "磁铁车僵尸"},
	{ "HAMSTERZOMBIE", "仓鼠球僵尸"}
	};
	std::string tempPath = getenv("TEMP");
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
/**
 * 禁用Unbalanced存档破碎
 *
 * @param isFeatureEnabled 功能开启状态
 */
void GhTrManager::DisableUbSaveDestroy(bool isFeatureEnabled)
{
	DWORD dwPid = GetGamePid();
	ProtectAddress(dwPid, 0x300);
	if (!CheckGamePid(dwPid, true)) return;
	const char* operational_code1 = (isFeatureEnabled == 1) ? "\x83\x38\x05" : "\x83\x38\xFE";
	WriteToMemory(dwPid, 0xD4421, operational_code1, 3);
	const char* operational_code2 = (isFeatureEnabled == 1) ? "\xE9\x00\x00\x00\x00\x90\x90" : "\x89\x47\x08\xC6\x45\xFC\x07";
	WriteToMemory(dwPid, 0xD2883, operational_code2, 7);
	const char* operational_code3 = "\x83\x7F\x08\xFE\x0F\x85\x0C\x00\x00\x00\xC7\x47\x08\x04\x00\x00\x00\xE9\x07\x00\x00\x00\xC7\x47\x08\x04\x00\x00\x00\xC6\x45\xFC\x07\xE9\x64\x28\xDB\xFE";
	WriteToMemory(dwPid, 0x620, operational_code3, 38);
	WriteJump(dwPid, 0x641, 0xD288A);
	if (isFeatureEnabled == 1)WriteJump(dwPid, 0xD2883, 0x620);
	const char* operational_code4 = (isFeatureEnabled == 1) ? "\x83\x7F\x08\x05" : "\x83\x7F\x08\x04";
	WriteToMemory(dwPid, 0xD28A0, operational_code4, 4);
	const char* operational_code5 = (isFeatureEnabled == 1) ? "\x83\x7F\x08\x05" : "\x83\x7F\x08\x04";
	WriteToMemory(dwPid, 0xD28B7, operational_code5, 4);
}
#ifdef _DEBUG
/**
 * 解包grp文件 [DEBUG ONLY]
 *
 */
void GhTrManager::UnpackGrpFile()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("GhTr Resource Pack (*.grp;*.pak)|*.grp;*.pak||"), NULL);
	if (fileDlg.DoModal() != IDOK)
		return;
	CString filePath = fileDlg.GetPathName();
	CString fileName = fileDlg.GetFileTitle();
	CTime currentTime = CTime::GetCurrentTime();
	time_t unixTimeStamp = currentTime.GetTime();
	CString FolderName;
	FolderName.Format(_T(".\\Unpack%lu"), (unsigned long)unixTimeStamp);
	if (!CreateDirectory(FolderName, NULL))
	{

		MessageBox(NULL, _T("无法创建输出文件夹，请检查目录下是否有同名文件夹 [FOLDER_CORRUPTION]"), _T("错误"), MB_ICONERROR);
		return;
	}
	FILE* fpMainPak = _wfopen(filePath, _T("rb"));
	if (NULL == fpMainPak)
	{
		MessageBox(NULL, _T("无法打开你选择的文件，请在本程序开源页面发起议题 [FILE_CORRUPTION]"), _T("错误"), MB_ICONERROR);
		return;
	}
	fseek(fpMainPak, 0, SEEK_END);
	long sizeMainPak = ftell(fpMainPak);
	rewind(fpMainPak);
	if (!(sizeMainPak > 0)) {
		MessageBox(NULL, _T("您的grp文件校验错误，请确认文件无损坏 [SIZE_ERROR]"), _T("错误"), MB_ICONERROR);
		return;
	}
	byte_t* pMainPak = (byte_t*)malloc(sizeMainPak);
	if (NULL == pMainPak) {
		MessageBox(NULL, _T("无法申请足够的RAM内存空间存入输入文件 [MEMORY_ERROR]"), _T("错误"), MB_ICONERROR);
		//释放空间
		free(pMainPak);
		pMainPak = NULL;
		return;
	}
	if (fread(pMainPak, sizeof(byte_t), sizeMainPak, fpMainPak) != sizeMainPak) {
		MessageBox(NULL, _T("无法申请足够的RAM内存空间存入输出文件 [MEMORY_ERROR]"), _T("错误"), MB_ICONERROR);
		//释放空间
		free(pMainPak);
		pMainPak = NULL;
		return;
	}
	fclose(fpMainPak);
	fpMainPak = NULL;
	for (int i = 0; i < sizeMainPak; i += 1)
		pMainPak[i] ^= (byte_t)(0xAA ^ 0x41);
	for (int i = 0; i < sizeMainPak; i += 1)
		pMainPak[i] = swapNibbles(pMainPak[i]);
	FileInfoNode headNode;
	headNode.pNext = NULL;
	FileInfoNode* pNode = &headNode;
	unsigned int byteIndex = 0;
	while (1) {
		byteIndex += 8;
		if (0 != pMainPak[byteIndex]) {
			byteIndex += 1;
			break;
		}
		pNode->pNext = (FileInfoNode*)malloc(sizeof(FileInfoNode));
		pNode = pNode->pNext;
		if (NULL == pNode) {
			MessageBox(NULL, _T("无法申请足够的RAM内存空间存入节点 [MEMORY_ERROR]"), _T("错误"), MB_ICONERROR);
			//释放空间
			pNode = headNode.pNext;
			headNode.pNext = NULL;
			while (pNode != NULL) {
				FileInfoNode* pNextNode = pNode->pNext;
				free(pNode->pPathname);
				free(pNode);
				pNode = pNextNode;
			}
			free(pMainPak);
			pMainPak = NULL;
			return;
		}
		if (pNode != NULL) {
			pNode->pNext = NULL;
		}
		byteIndex += 1;
		pNode->pPathname = (char*)malloc(pMainPak[byteIndex] + 1);
		if (NULL == pNode->pPathname) {
			MessageBox(NULL, _T("无法申请足够的RAM内存空间链接节点 [MEMORY_ERROR]"), _T("错误"), MB_ICONERROR);
			//释放空间
			pNode = headNode.pNext;
			headNode.pNext = NULL;
			while (pNode != NULL) {
				FileInfoNode* pNextNode = pNode->pNext;
				free(pNode->pPathname);
				free(pNode);
				pNode = pNextNode;
			}
			free(pMainPak);
			pMainPak = NULL;
			return;
		}
		if (pNode != NULL) {
			(pNode->pPathname)[pMainPak[byteIndex]] = '\0';
		}
		for (int i = 0; i < pMainPak[byteIndex]; i += 1)
			(pNode->pPathname)[i] = pMainPak[byteIndex + 1 + i];
		byteIndex += 1 + pMainPak[byteIndex];
		pNode->fileSize = *((unsigned int*)(pMainPak + byteIndex));
		byteIndex += 4;
	}
	{
		unsigned int totalSize = 0;
		for (pNode = headNode.pNext; pNode != NULL; pNode = pNode->pNext)
			totalSize += pNode->fileSize;
		if (totalSize != sizeMainPak - byteIndex) {
			MessageBox(NULL, _T("文件校验失败 [SIZE_ERROR]"), _T("错误"), MB_ICONERROR);
		}
	}
	for (pNode = headNode.pNext; pNode != NULL; pNode = pNode->pNext)
	{
		CString outputFilePath = FolderName + _T("\\") + CString(pNode->pPathname);
		{
			char* pSlash = pNode->pPathname;

			while (1) {
				while (!(*pSlash == '\\' || *pSlash == '\0'))
					pSlash += 1;
				if (*pSlash == '\0')
					break;
				*pSlash = '\0';
				CString folderPath = FolderName + _T("\\") + CString(pNode->pPathname);
				std::filesystem::path dir(folderPath.GetString());
				std::filesystem::create_directories(dir);
				*pSlash = '\\';
				pSlash += 1;
			}
		}
		std::ofstream ofs(outputFilePath, std::ios::binary);
		ofs.write(reinterpret_cast<char*>(pMainPak + byteIndex), pNode->fileSize);
		ofs.close();
		byteIndex += pNode->fileSize;
	}
	//释放空间
	pNode = headNode.pNext;
	headNode.pNext = NULL;
	while (pNode != NULL) {
		FileInfoNode* pNextNode = pNode->pNext;
		free(pNode->pPathname);
		free(pNode);
		pNode = pNextNode;
	}
	free(pMainPak);
	pMainPak = NULL;
	MessageBox(NULL, L"解包成功！点击确认打开输出文件夹",L"提示", MB_OK);
	OpenFolder(FolderName);
	return;
}
/**
 * 打包grp文件 [DEBUG ONLY]
 *
 */
void GhTrManager::PackGrpFile()
{
	std::filesystem::path folderPath = SelectFolder();
	DWORD fa = GetFileAttributesW(folderPath.c_str());
	if ((fa == INVALID_FILE_ATTRIBUTES) || !(fa & FILE_ATTRIBUTE_DIRECTORY))
		return;
	unsigned int files_count = 0;         // 文件总数
	std::vector<std::wstring> files_name; // pak 里保存的路径
	std::vector<int> files_size;          // 大小
	std::vector<FILETIME> files_time;     // 时间
	find_files(folderPath, files_name, files_size, files_time);
	files_count = files_name.size();
	time_t now = time(0);
	std::wstring dst_file = L"main_" + std::to_wstring(now) + L".grp";
	HANDLE hfw = CreateFileW(dst_file.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 
		nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD write_size = 0;
	unsigned long file_header_magic;
	unsigned long file_header_version;
	file_header_magic = MAGIC_NUM;
	file_header_version = VERSION_NUM;
	file_header_magic = swapBytesInLong(file_header_magic);
	file_header_version = swapBytesInLong(file_header_version);
	file_header_magic ^= XOR_KEY;
	file_header_version ^= XOR_KEY;
	WriteFile(hfw, (void*)&file_header_magic, sizeof(unsigned long), &write_size, nullptr);
	if (write_size != sizeof(unsigned long))
	{
		CloseHandle(hfw);
		return;
	}

	WriteFile(hfw, (void*)&file_header_version, sizeof(unsigned long), &write_size, nullptr);
	if (write_size != sizeof(unsigned long))
	{
		CloseHandle(hfw);
		return;
	}
	unsigned char eof_flag;   // 结束标志
	unsigned char name_width; // 文件名长度
	char file_name[256];      // 文件名
	int file_size;            // 大小
	FILETIME file_time;       // 时间
	for (size_t i = 0; i < files_count; i++)
	{
		eof_flag = (unsigned char)(0x00);
		std::string file_name_str = utf8_encode(files_name.at(i));
		unsigned int name_size = file_name_str.size();
		name_width = static_cast<unsigned char>(name_size);
		for (size_t i = 0; i < name_size; i++)
			file_name[i] = file_name_str.at(i);
		file_name[name_size] = 0;
		file_size = files_size.at(i);
		file_time = files_time.at(i);
		unsigned int struct_size = sizeof(unsigned char) 
			+ sizeof(unsigned char)
			+ name_size
			+ sizeof(int)
			+ sizeof(FILETIME);
		char* buff = new char[struct_size];
		unsigned int index = 0;
		buff[index] = eof_flag;
		index += sizeof(unsigned char);
		buff[index] = name_width;
		index += sizeof(unsigned char);
		for (size_t j = 0; j < name_size; j++)
			buff[index + j] = file_name[j];
		index += name_size;
		(int&)buff[index] = file_size;
		index += sizeof(int);
		(FILETIME&)buff[index] = file_time;
		index += sizeof(FILETIME);
		assert(index == struct_size);
		for (size_t k = 0; k < struct_size; k++)
		{
			buff[k] = swapNibbles(buff[k]);
			buff[k] = buff[k] ^ (byte_t)(0xAA ^ 0x41);
		}
		WriteFile(hfw, buff, struct_size, &write_size, nullptr);
		if (write_size != struct_size)
		{
			CloseHandle(hfw);
			delete[] buff;
			return;
		}
		delete[] buff;
	}
	eof_flag = (unsigned char)(0x80);
	eof_flag = swapNibbles(eof_flag);
	eof_flag ^= (byte_t)(0xAA ^ 0x41);
	WriteFile(hfw, (void*)&eof_flag, sizeof(unsigned char), &write_size, nullptr);
	if (write_size != sizeof(unsigned char))
	{
		CloseHandle(hfw);
		return;
	}
	std::wstring folderWstring = folderPath.wstring();
	for (size_t i = 0; i < files_count; i++)
	{
		auto file_path = folderWstring + L"\\" + files_name.at(i);
		HANDLE hfr = CreateFileW(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, //
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hfr == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hfw);
			return;
		}
		unsigned int size = files_size.at(i);
		DWORD read_size = 0;
		char* buffer = new char[size];
		BOOL ret = ReadFile(hfr, buffer, size, &read_size, nullptr);
		if (ret == FALSE || read_size != size)
		{
			CloseHandle(hfw);
			CloseHandle(hfr);
			delete[] buffer;
			return;
		}
		CloseHandle(hfr);
		for (size_t i = 0; i < size; ++i)
		{
			buffer[i] = swapNibbles(buffer[i]);
			buffer[i] = buffer[i] ^ (byte_t)(0xAA ^ 0x41);
		}
		DWORD write_size = 0;
		WriteFile(hfw, buffer, size, &write_size, nullptr);
		if (write_size != size)
		{
			CloseHandle(hfw);
			delete[] buffer;
			return;
		}
		delete[] buffer;
	}
	CloseHandle(hfw);
	MessageBox(NULL, L"打包成功，点击确认选择输出文件", L"提示", MB_OK);
	OpenFolderAndSelectItem(dst_file);
}
#endif