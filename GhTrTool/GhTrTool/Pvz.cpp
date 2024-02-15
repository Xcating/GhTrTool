#include "stdafx.h"
#include "Pvz.h"
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
CPvz::CPvz()
{
}


CPvz::~CPvz()
{
}

DWORD_PTR GetBaseAddress(const char* moduleName) {
    HMODULE hModule = GetModuleHandleA(moduleName);

    if (hModule == NULL) {
        printf("Failed to get module handle for %s\n", moduleName);
        return 0;
    }

    DWORD_PTR baseAddress = (DWORD_PTR)hModule;
    return baseAddress;
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
DWORD_PTR GetSectionBaseAddress(HANDLE hProcess, const wchar_t* moduleName, const std::string& sectionName) {
    HMODULE hModule;
    if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCWSTR>(GetSectionBaseAddress), &hModule)) {
        return 0;
    }

    MODULEINFO moduleInfo;
    if (!GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(moduleInfo))) {
        return 0;
    }

    IMAGE_DOS_HEADER dosHeader;
    ReadProcessMemory(hProcess, moduleInfo.lpBaseOfDll, &dosHeader, sizeof(dosHeader), nullptr);

    IMAGE_NT_HEADERS ntHeaders;
    ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(reinterpret_cast<DWORD_PTR>(moduleInfo.lpBaseOfDll) + dosHeader.e_lfanew), &ntHeaders, sizeof(ntHeaders), nullptr);

    IMAGE_SECTION_HEADER sectionHeader;
    for (int i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++) {
        ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(reinterpret_cast<DWORD_PTR>(moduleInfo.lpBaseOfDll) + dosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS) + (i * sizeof(IMAGE_SECTION_HEADER))), &sectionHeader, sizeof(sectionHeader), nullptr);
        std::wstring sectionNameW(reinterpret_cast<const wchar_t*>(sectionHeader.Name));
        std::string sectionNameA(sectionNameW.begin(), sectionNameW.end());

        if (sectionNameA == sectionName) {
            return reinterpret_cast<DWORD_PTR>(moduleInfo.lpBaseOfDll) + sectionHeader.VirtualAddress;
        }
    }

    return 0;
}
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
// 修改阳光的值
VOID CPvz::ModifySunValue(DWORD dwSun)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x29CE88;
    DWORD targetAddress = baseAddress + offset;
    // 0044BA45 - add[edi + 00005578], eax EDI = 1E4A0B40
    // 00475373 - mov edi, [esi + 00000868] ESI = 0286B490
    // 7794F8

    DWORD dwNum = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL);
    BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x380), &dwSun, sizeof(DWORD), NULL);
    if (result) {
        MessageBox(NULL, TEXT("写入成功"), TEXT("提示"), MB_OK);
    }
    else {
        MessageBox(NULL, TEXT("写入失败,请检查权限"), TEXT("错误"), MB_OK | MB_ICONERROR);
    }
    CloseHandle(hProcess);
}


// 种植不减阳光
VOID CPvz::SunNop()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x95E39;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0041BA74 2B F3 sub esi,ebx
    // 修改后的指令：
    //     0041BA74 90 nop 
    //     0041BA75 90 nop
    char *nop = "\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, nop, 6, NULL);

    CloseHandle(hProcess);
}


// 种植免冷却
VOID CPvz::NoCd()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xE54A1;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：00488250 75 24 jne 00488276
    // 修改后的指令：00488250 EB 24 jmp 00488276
    char *patch1 = "\xc7\x41\x20\x00\x00\x00\x00\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 10, NULL);

    // 原指令：00488688 0F85 79010000 jne 00488807
    // 修改后的指令：
    //     00488688 E9 7A010000 jmp 00488807
    //     0048868D 90          nop
    offset = 0xE4D91;
    targetAddress = baseAddress + offset;
    char *patch2 = "\xb8\x00\x00\x00\x00\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch2, 7, NULL);

    CloseHandle(hProcess);
}


// 修改金币
VOID CPvz::ModifyCoinValue(DWORD dwCoin)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x29CE88;
    DWORD targetAddress = baseAddress + offset;
    DWORD dwNum = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress, &dwNum, sizeof(DWORD), NULL);
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL);

    WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x164), &dwCoin, sizeof(DWORD), NULL);

    CloseHandle(hProcess);
}


// 重复建造，无需花盆、无需荷叶
VOID CPvz::Build()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x94C74;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0040FE2F 0F84 1F090000 je 00410754
    // 修改后的指令：
    //     0040FE2F E9 20090000 jmp 00410754
    //     0040FE34 90          nop
    char *patch = "\x90\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 7, NULL);

    CloseHandle(hProcess);
}


// 自动收集阳光
VOID CPvz::Auto()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xABD9A;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0043158F 75 08 jne 00431599
    // 修改后的指令：0043158F EB 08 jmp 00431599
    char *patch = "\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 4, NULL);

    CloseHandle(hProcess);
}


// 随机卡槽
VOID CPvz::Card()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xA552E;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：00464A96 0F85 98FEFFFF jne 00464934
    // 修改后的指令：
    //     00464A96 90 nop 
    //     00464A97 90 nop
    //     00464A98 90 nop
    //     00464A99 90 nop
    //     00464A9A 90 nop
    //     00464A9B 90 nop
    char *patch = "\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 4, NULL);
    CloseHandle(hProcess);
}


// 加速僵尸出动
VOID CPvz::More()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x996DB;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0045FA48 83 47 58 FF add dword ptr [edi+58],-01 { 255 }
    // 修改后的指令：0045FA48 83 47 58 9C add dword ptr [edi+58],-64 { 156 }
    char *patch = "\xC7\x87\xFC\x03\x00\x00\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 11, NULL);

    CloseHandle(hProcess);
}


// 更好的高级暂停
VOID CPvz::AllScreen()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x96262;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：00413FD9 0F85 0D010000 jne 004140EC
    // 修改后的指令：
    //     00413FD9 90 nop 
    //     00413FDA 90 nop
    //     00413FDB 90 nop
    //     00413FDC 90 nop
    //     00413FDD 90 nop
    //     00413FDE 90 nop
    char *patch = "\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 3, NULL);

    CloseHandle(hProcess);
}


// \技能无冷却
VOID CPvz::UnAllScreen()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x93BF5;
    DWORD targetAddress = baseAddress + offset;
    // 指令与 AllScreen 相反
    char *patch = "\xC7\x86\xC0\x03\x00\x00\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 11, NULL);

    CloseHandle(hProcess);
}


// 小推车不动
VOID CPvz::Cool()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xBE362;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：0052AB3E E9 BD63EDFF jmp 00400F00
    char *patch1 = "\x90\x90\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 8, NULL);
    CloseHandle(hProcess);
}


// 磁铁车召唤旗帜
VOID CPvz::Stop()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xFEBD0;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\x6A\x01";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 2, NULL);

    /*
    // 在新申请的空间写入的指令
    // 00400F00 60 pushad 
    // 00400F01 9C pushfd
    // 00400F02 D9 46 2C fld dword ptr[esi + 2C]
    // 00400F05 C7 46 2C 56FB4843 mov[esi + 2C], 4348FB56{ 200.98 }
    // 00400F0C D8 56 2C fcom dword ptr[esi + 2C]
    // 00400F0F 9BDFE0 wait:fstsw ax
    // 00400F12 9E sahf
    // 00400F13 77 0A ja 00400F1F
    // 00400F15 C7 86 B0000000 00010000 mov[esi + 000000B0], 00000100 { 256 }
    // 00400F1F D9 5E 2C fstp dword ptr[esi + 2C]
    // 00400F22 9D popfd
    // 00400F23 61 popad
    // 00400F24 0F85 BE9C1200 jne 0052ABE8
    // 00400F2A E9 159C1200 jmp 0052AB44
    char *patch2 = "\x60"
        "\x9c"
        "\xd9\x46\x2c"
        "\xc7\x46\x2c\x56\xFB\x48\x43"
        "\xD8\x56\x2C"
        "\x9B\xDF\xE0"
        "\x9E"
        "\x77\x0A"
        "\xC7\x86\xB0\x00\x00\x00\x00\x01\x00\x00"
        "\xD9\x5E\x2C"
        "\x9D"
        "\x61"
        "\x0F\x85\xBE\x9C\x12\x00"
        "\xE9\x15\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00400f00, patch2, 47, NULL);

    CloseHandle(hProcess);
    */

}


// 植物不会被魅惑
VOID CPvz::Dead()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC5911;
    DWORD targetAddress = baseAddress + offset;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x00345911, patch1, 3, NULL);
    CloseHandle(hProcess);
}


// 植物无敌
VOID CPvz::Attract()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC5997;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 2, NULL);
    CloseHandle(hProcess);
}
// 进入游戏修改荣光
VOID CPvz::Point()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xD1358;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char* patch1 = "\xC7\x85\xF8\x02\x00\x00\x00\x00\x00\x00\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 12, NULL);
    /*
    MessageBox(NULL, L"此功能无法使用", L"提示", MB_OK);
    return;
    if (baseAddress != 0)
    {
        MessageBox(NULL, L"游戏未找11到", L"提示", MB_OK);
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x91FB8;
    DWORD targetAddress = baseAddress + offset;
    offset = 0x4196;
    DWORD targetAddress2 = baseAddress - offset;
    DWORD dwOldProtect = 0;
    BOOL result = VirtualProtectEx(hProcess, (LPVOID)0x01DE0000, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (result) {
        // VirtualProtectEx成功申请到新的空间
        // 可以进行进一步的操作
        MEMORY_BASIC_INFORMATION memInfo;
        VirtualQueryEx(hProcess, (LPVOID)0x01DE0000, &memInfo, sizeof(memInfo));
        // 检查保护属性是否与设置的相匹配
        if (memInfo.Protect == PAGE_EXECUTE_READWRITE) {
            MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        }
    }
    else {
        DWORD error = GetLastError();

        // 转换错误代码为错误消息字符串
        LPWSTR errorMessage = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&errorMessage, 0, nullptr);

        // 弹出错误消息框
        MessageBox(nullptr, errorMessage, L"VirtualProtectEx Error", MB_ICONERROR);

        // 释放错误消息字符串内存
        LocalFree(errorMessage);
    }
    // 原指令：0041BA74 2B F3 sub esi,ebx
    // 修改后的指令：
    //     0041BA74 90 nop 
    //     0041BA75 90 nop
    char* nop = "\xE9\x43\xE0\x33\x05\x66\x0F\x1F\x44\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, nop, 11, NULL);
    char* patch2 = "\xc7\x86\xac\x00\x00\x00\x00\x10\x00\x00\x0F\x85\xD8\x9C\x12\x00\xE9\x2F\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x01DE0000, patch2, 21, NULL);
    CloseHandle(hProcess);
    */
}
VOID CPvz::DX()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe");
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x9424E;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char* patch1 = "\x6A\x09\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 6, NULL);
    offset = 0x95077;
    DWORD targetAddress2 = baseAddress + offset;
    char* patch2 = "\x6A\x0D";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch2, 2, NULL);
    offset = 0xC50F5;
    DWORD targetAddress3 = baseAddress + offset;
    char* patch3 = "\x83\x78\xE8\x0D";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress3, patch3, 4, NULL);
    offset = 0x94F27;
    DWORD targetAddress4 = baseAddress + offset;
    char* patch4 = "\x83\x79\x18\x0D";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress4, patch4, 4, NULL);
    /*
    MessageBox(NULL, L"此功能无法使用", L"提示", MB_OK);
    return;
    if (baseAddress != 0)
    {
        MessageBox(NULL, L"游戏未找11到", L"提示", MB_OK);
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x91FB8;
    DWORD targetAddress = baseAddress + offset;
    offset = 0x4196;
    DWORD targetAddress2 = baseAddress - offset;
    DWORD dwOldProtect = 0;
    BOOL result = VirtualProtectEx(hProcess, (LPVOID)0x01DE0000, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (result) {
        // VirtualProtectEx成功申请到新的空间
        // 可以进行进一步的操作
        MEMORY_BASIC_INFORMATION memInfo;
        VirtualQueryEx(hProcess, (LPVOID)0x01DE0000, &memInfo, sizeof(memInfo));
        // 检查保护属性是否与设置的相匹配
        if (memInfo.Protect == PAGE_EXECUTE_READWRITE) {
            MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        }
    }
    else {
        DWORD error = GetLastError();

        // 转换错误代码为错误消息字符串
        LPWSTR errorMessage = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&errorMessage, 0, nullptr);

        // 弹出错误消息框
        MessageBox(nullptr, errorMessage, L"VirtualProtectEx Error", MB_ICONERROR);

        // 释放错误消息字符串内存
        LocalFree(errorMessage);
    }
    // 原指令：0041BA74 2B F3 sub esi,ebx
    // 修改后的指令：
    //     0041BA74 90 nop
    //     0041BA75 90 nop
    char* nop = "\xE9\x43\xE0\x33\x05\x66\x0F\x1F\x44\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, nop, 11, NULL);
    char* patch2 = "\xc7\x86\xac\x00\x00\x00\x00\x10\x00\x00\x0F\x85\xD8\x9C\x12\x00\xE9\x2F\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x01DE0000, patch2, 21, NULL);
    CloseHandle(hProcess);
    */
}