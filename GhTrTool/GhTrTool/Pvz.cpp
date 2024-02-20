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

unsigned char* convertToAssemblyCode(DWORD address) {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&address); // 将DWORD地址变量视为指向8位字节的指针

    unsigned char* assemblyCode = new unsigned char[8]; // 分配足够的空间来存储转换后的字节码

    for (int i = 0; i < 4; i++) {
        uint8_t value = ptr[i]; // 获取每个字节的值
        uint8_t invertedValue = ~value; // 取反

        assemblyCode[2 * i] = static_cast<unsigned char>((invertedValue >> 4) & 0xF);
        assemblyCode[2 * i + 1] = static_cast<unsigned char>(invertedValue & 0xF);
    }

    return assemblyCode;
};
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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

// 修改卡槽数量
VOID CPvz::SeedPacket(DWORD dwSP)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x14C), &dwNum, sizeof(DWORD), NULL);
    BOOL result = WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x1C), &dwSP, sizeof(DWORD), NULL);
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD offset2 = 0xE55AD;
    DWORD targetAddress2 = baseAddress + offset2;
    char *patch2 = "\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch2, 2, NULL);

    CloseHandle(hProcess);
}


// 修改背景ID
VOID CPvz::ModifyBGIdValue(DWORD dwBGId)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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

    WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x164), &dwBGId, sizeof(DWORD), NULL);

    CloseHandle(hProcess);
}


// 重复建造，无需花盆
VOID CPvz::Build()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
VOID CPvz::Fast()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
VOID CPvz::TheWorld()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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


// 技能无冷却
VOID CPvz::NoModelCD()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    // 指令与 TheWorld 相反
    char *patch = "\xC7\x86\xC0\x03\x00\x00\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch, 11, NULL);

    CloseHandle(hProcess);
}

// 解除阳光限制
VOID CPvz::NoSunMax()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x8E8ED;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：0052AB3E E9 BD63EDFF jmp 00400F00
    char* patch1 = "\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 6, NULL);
    CloseHandle(hProcess);
}

// 解除阳光限制
VOID CPvz::NoBuildTIme()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xA2913;
    DWORD targetAddress = baseAddress + offset;
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：0052AB3E E9 BD63EDFF jmp 00400F00
    char* patch1 = "\xC7\x86\x94\x00\x00\x00\x00\x00\x00\x00\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 14, NULL);
    CloseHandle(hProcess);
}
// 小推车不动
VOID CPvz::Mowers()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
VOID CPvz::SummonCup()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset7 = 0x29CE88;
    DWORD targetAddress7 = baseAddress + offset7;
    DWORD offset2 = 0x45E;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    DWORD offset3 = 0x47B;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x953B0;
    DWORD targetAddress4 = baseAddress + offset4;
    BYTE shellcode[] = {
    0x60,                               // pushad
    0x9C,                               // pushfd
    0xBA, 0x80, 0x00, 0x00, 0x00,       // mov edx,80
    0x8B, 0x0D, 0x28, 0xAE, 0x29, 0x00, // mov ecx,[PlantsVsZombies.exe+29AE28]+708]
    0xBE, 0x32, 0x00, 0x00, 0x00,       // mov esi,32
    0x68, 0x88, 0x87, 0x00, 0x00,       // push 8788
    0x6A, 0x02,                         // push 2
    0x6A, 0x18,                         // push 18
    0x6A, 0x7B,                         // push 123
    0xE8, 0xB3, 0x94, 0x00, 0x00,       // call plantsvszombies.exe.text+943B0
    0x9D,                               // popfd
    0x61,                               // popad
    0xC3                                // retn
    };
    DWORD offsetX = 0x708;
    DWORD valueX = 0;
    DWORD targetValueX = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &valueX, sizeof(DWORD), NULL);
    // 在基址上加上偏移量708，得到目标地址的绝对地址
    DWORD finalAddress = valueX + offsetX;
    DWORD jumpOffsetX = finalAddress;
    // 将第二个偏移值写入操作码数组
    shellcode[9] = jumpOffsetX & 0xFF;
    shellcode[10] = (jumpOffsetX >> 8) & 0xFF;
    shellcode[11] = (jumpOffsetX >> 16) & 0xFF;
    shellcode[12] = (jumpOffsetX >> 24) & 0xFF;
    DWORD jumpOffset2 = targetAddress4 - (targetAddress3 + 5);

    // 将第二个偏移值写入操作码数组
    shellcode[30] = jumpOffset2 & 0xFF;
    shellcode[31] = (jumpOffset2 >> 8) & 0xFF;
    shellcode[32] = (jumpOffset2 >> 16) & 0xFF;
    shellcode[33] = (jumpOffset2 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, shellcode, sizeof(shellcode), NULL);

    // 创建远程线程来执行汇编代码
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)targetAddress2, NULL, 0, NULL);

    //等待线程执行完毕
    WaitForSingleObject(hThread, INFINITE);

    // 清理资源
    CloseHandle(hThread);
    //VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
}
VOID CPvz::Plant(DWORD dwXP,DWORD dwYP,DWORD dwID)
{
    dwXP--; dwYP--;
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset7 = 0x29CE88;
    DWORD targetAddress7 = baseAddress + offset7;
    DWORD offset2 = 0x348;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    DWORD offset3 = 0x357;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x956F0;
    DWORD targetAddress4 = baseAddress + offset4;
    BYTE shellcode[] = {  //60 6A 01 6A 07 8B 0D 4C F6 42 01 6A 01 6A 01 E8 94 53 09 00 61 C3
    0x60,
    0x6A, 0x01,
    0x6A, 0x07,
    0x8B, 0x0D, 0x28, 0xAE, 0x29, 0x00,
    0x6A, 0x01,
    0x6A, 0x01,
    0xE8, 0xB3, 0x94, 0x00, 0x00,
    0x61,
    0xC3
    };
    DWORD offsetX = 0x320;
    DWORD valueX = 0;
    DWORD targetValueX = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &valueX, sizeof(DWORD), NULL);
    //在基址上加上偏移量708，得到目标地址的绝对地址
    DWORD finalAddress = valueX + offsetX;
    valueX = 0;
    ReadProcessMemory(hProcess, (LPCVOID)finalAddress, &valueX, sizeof(DWORD), NULL);
    DWORD finalAddress2 = valueX + 0x7C;
    DWORD jumpOffsetX = finalAddress2;
    // 将第二个偏移值写入操作码数组
    shellcode[7] = jumpOffsetX & 0xFF;
    shellcode[8] = (jumpOffsetX >> 8) & 0xFF;
    shellcode[9] = (jumpOffsetX >> 16) & 0xFF;
    shellcode[10] = (jumpOffsetX >> 24) & 0xFF;
    DWORD jumpOffset2 = targetAddress4 - (targetAddress3 + 5);
    shellcode[4] = dwID & 0xFF;
    shellcode[12] = dwXP & 0xFF;
    shellcode[14] = dwYP & 0xFF;
     //将第二个偏移值写入操作码数组
    shellcode[16] = jumpOffset2 & 0xFF;
    shellcode[17] = (jumpOffset2 >> 8) & 0xFF;
    shellcode[18] = (jumpOffset2 >> 16) & 0xFF;
    shellcode[19] = (jumpOffset2 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, shellcode, sizeof(shellcode), NULL);

    //创建远程线程来执行汇编代码
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)targetAddress2, NULL, 0, NULL);

    //等待线程执行完毕
    WaitForSingleObject(hThread, INFINITE);

    // 清理资源
    CloseHandle(hThread);
    //VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    CloseHandle(hProcess);
}
VOID CPvz::PeaSDamage()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD offset2 = 0x52F;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0x533;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0xA5533;
    DWORD targetAddress4 = baseAddress + offset4;
    DWORD offset5 = 0x53D;
    DWORD targetAddress5 = baseAddress + offset5;
    DWORD offset6 = 0x547;
    DWORD targetAddress6 = baseAddress + offset6;
    DWORD offset7 = 0x551;
    DWORD targetAddress7 = baseAddress + offset7;
    DWORD offset8 = 0xA654A;
    DWORD targetAddress8 = baseAddress + offset8;
    DWORD offset9 = 0x557;
    DWORD targetAddress9 = baseAddress + offset9;
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：0052AB3E E9 BD63EDFF jmp 00400F00
    char* patch1 = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 6, NULL);
    char patch2[] = "\x80\x7E\x08\x01\x0F\x84\x2A\x55\x9C\xFE\x80\x7E\x08\x02\x0F\x84\x20\x55\x9C\xFE\x80\x7E\x08\x03\x0F\x84\x16\x55\x9C\xFE\xC6\x46\x24\x01\x0F\x85\x22\x55\x9C\xFE\xE9\x07\x55\x9C\xFE";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch2, sizeof(patch2) - 1, NULL);

    BYTE opCode5[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset5 = targetAddress2 - (targetAddress + 5);

    // 将第二个偏移值写入操作码数组
    opCode5[1] = jumpOffset5 & 0xFF;
    opCode5[2] = (jumpOffset5 >> 8) & 0xFF;
    opCode5[3] = (jumpOffset5 >> 16) & 0xFF;
    opCode5[4] = (jumpOffset5 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode5, sizeof(opCode5), NULL);
    BYTE opCode1[] = { 0x0F, 0x84,0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset1 = targetAddress4 - (targetAddress3 + 5);

    // 将第二个偏移值写入操作码数组
    opCode1[2] = jumpOffset1 & 0xFF;
    opCode1[3] = (jumpOffset1 >> 8) & 0xFF;
    opCode1[4] = (jumpOffset1 >> 16) & 0xFF;
    opCode1[5] = (jumpOffset1 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress3, opCode1, sizeof(opCode1), NULL);
    BYTE opCode2[] = { 0x0F, 0x84,0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset2 = targetAddress4 - (targetAddress5 + 5);

    // 将第二个偏移值写入操作码数组
    opCode2[2] = jumpOffset2 & 0xFF;
    opCode2[3] = (jumpOffset2 >> 8) & 0xFF;
    opCode2[4] = (jumpOffset2 >> 16) & 0xFF;
    opCode2[5] = (jumpOffset2 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress5, opCode2, sizeof(opCode2), NULL);
    BYTE opCode3[] = { 0x0F, 0x84,0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset3 = targetAddress4 - (targetAddress6 + 5);

    // 将第二个偏移值写入操作码数组
    opCode3[2] = jumpOffset3 & 0xFF;
    opCode3[3] = (jumpOffset3 >> 8) & 0xFF;
    opCode3[4] = (jumpOffset3 >> 16) & 0xFF;
    opCode3[5] = (jumpOffset3 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress6, opCode3, sizeof(opCode3), NULL);
    BYTE opCode4[] = { 0x0F, 0x85,0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset4 = targetAddress8 - (targetAddress7 + 6);

    // 将第二个偏移值写入操作码数组
    opCode4[2] = jumpOffset4 & 0xFF;
    opCode4[3] = (jumpOffset4 >> 8) & 0xFF;
    opCode4[4] = (jumpOffset4 >> 16) & 0xFF;
    opCode4[5] = (jumpOffset4 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress7, opCode4, sizeof(opCode4), NULL);
    BYTE opCode6[] = { 0xE9,0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset6 = targetAddress4 - (targetAddress9 + 4);

    // 将第二个偏移值写入操作码数组
    opCode6[1] = jumpOffset6 & 0xFF;
    opCode6[2] = (jumpOffset6 >> 8) & 0xFF;
    opCode6[3] = (jumpOffset6 >> 16) & 0xFF;
    opCode6[4] = (jumpOffset6 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress9, opCode6, sizeof(opCode6), NULL);
    CloseHandle(hProcess);
}
// 僵尸掉卡(史山)
VOID CPvz::ZombieDC()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x100AC4;
    DWORD targetAddress = baseAddress + offset;
    DWORD offset2 = 0xF2E+4;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0xF59 + 4;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x100AC9;
    DWORD targetAddress4 = baseAddress + offset4;
    DWORD offset5 = 0xF53 + 4;
    DWORD targetAddress5 = baseAddress + offset5;
    DWORD offset6 = 0x953B0;
    DWORD targetAddress6 = baseAddress + offset6;
    DWORD offset7 = 0x29CE88;
    DWORD targetAddress7 = baseAddress + offset7;
    DWORD offset8 = 0xF3B + 4;
    DWORD targetAddress8 = baseAddress + offset8;
    DWORD offset9 = 0xAB677;
    DWORD targetAddress9 = baseAddress + offset9;
    DWORD offset10 = 0xF5E + 4;
    DWORD targetAddress10 = baseAddress + offset10;
    DWORD offset11 = 0xAB67C;
    DWORD targetAddress11 = baseAddress + offset11;
    DWORD offset12 = 0xF6B + 4;
    DWORD targetAddress12 = baseAddress + offset12;
    DWORD offset13 = 0xF79 + 4;
    DWORD targetAddress13 = baseAddress + offset13;
    DWORD offset14 = 0x29DAC8;
    DWORD targetAddress14 = baseAddress + offset14;
    DWORD offset15 = 0xF71 + 4;
    DWORD targetAddress15 = baseAddress + offset15;
    DWORD offset16 = 0x95F83;
    DWORD targetAddress16 = baseAddress + offset16;
    DWORD offset17 = 0x95F89;
    DWORD targetAddress17 = baseAddress + offset17;
    DWORD offset18 = 0xF7E + 4;
    DWORD targetAddress18 = baseAddress + offset18;
    DWORD offset19 = 0xF84 + 4;
    DWORD targetAddress19 = baseAddress + offset19;
    DWORD offset20 = 0xF8A + 4;
    DWORD targetAddress20 = baseAddress + offset20;
    DWORD offset21 = 0xF91 + 4;
    DWORD targetAddress21 = baseAddress + offset21;
    DWORD offset22 = 0xF9B + 4;
    DWORD targetAddress22 = baseAddress + offset22;
    DWORD offset23 = 0xF97 + 4;
    DWORD targetAddress23 = baseAddress + offset23;
    DWORD offset24 = 0xFA6 + 4;
    DWORD targetAddress24 = baseAddress + offset24;
    DWORD offset25 = 0xF9C + 4;
    DWORD targetAddress25 = baseAddress + offset25;
    DWORD offset26 = 0xA8B6C;
    DWORD targetAddress26 = baseAddress + offset26;
    DWORD offset27 = 0xA8B73;
    DWORD targetAddress27 = baseAddress + offset27;
    DWORD offset28 = 0xFAB + 4;
    DWORD targetAddress28 = baseAddress + offset28;
    DWORD offset29 = 0xFB5 + 4;
    DWORD targetAddress29 = baseAddress + offset29;
    DWORD offset30 = 0xFD2 + 4;
    DWORD targetAddress30 = baseAddress + offset30;
    DWORD offset31 = 0xFD8 + 4;
    DWORD targetAddress31 = baseAddress + offset31;
    DWORD offset32 = 0x100F6F;
    DWORD targetAddress32 = baseAddress + offset32;
    DWORD offset33 = 0x100F76;
    DWORD targetAddress33 = baseAddress + offset33;
    DWORD offset34 = 0x4D5 + 4;
    DWORD targetAddress34 = baseAddress + offset34;
    DWORD offset35 = 0xFB5 + 4;
    DWORD targetAddress35 = baseAddress + offset35;
    DWORD offset36 = 0xFD2 + 4;
    DWORD targetAddress36 = baseAddress + offset36;
    DWORD offset37 = 0xFD8 + 4;
    DWORD targetAddress37 = baseAddress + offset37;
    DWORD offset38 = 0x4F0;
    DWORD targetAddress38 = baseAddress + offset38;
    DWORD offset39 = 0x50D;
    DWORD targetAddress39 = baseAddress + offset39;
    DWORD offset40 = 0x513;
    DWORD targetAddress40 = baseAddress + offset40;
    DWORD offset41 = 0x4E7;
    DWORD targetAddress41 = baseAddress + offset41;
    DWORD offset42 = 0x400;
    DWORD targetAddress42 = baseAddress + offset42;
    // 0044BA45 - add[edi + 00005578], eax EDI = 1E4A0B40
    // 00475373 - mov edi, [esi + 00000868] ESI = 0286B490
    // 7794F8

    DWORD dwNum = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &dwNum, sizeof(DWORD), NULL);
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x708), &dwNum, sizeof(DWORD), NULL);
    dwNum = 0x00E10610;
    // 获取目标地址对应的字节码
    //char patch1[] = "\xE9\x43\xEF\xF6\xFF\66\x0F\x1F\x44\x00\x00";  // 初始化为包含了占位字节的数组
    LPVOID baseAddress3 = (LPVOID)targetAddress2;  // 要申请的起始地址
    SIZE_T allocationSize = 128;  // 要申请的内存大小（字节）
    LPVOID allocatedMemory = VirtualAllocEx(hProcess, baseAddress3, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)baseAddress3, 128, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress42, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    //WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch1, sizeof(patch1) - 1, NULL);
    // 修改char数组的前四个位置
    //49E04800
    char patch1[] = "\xE9\x00\x00\x00\x00\x66\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);

    char patch2[] = "\xC6\x83\x9C\x00\x00\x00\x01\x60\xBA\x80\x00\x00\x00\x8B\x0D\x10\x06\xE1\x00\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x73\x18\xFF\x73\x14\xE8\x86\x53\x85\xFF\x61\xE9\x9B\x0A\x8C\xFF";
    WriteProcessMemory(hProcess, (LPVOID)baseAddress3, patch2, sizeof(patch2) - 1, NULL);

    char patch3[] = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress9, patch3, sizeof(patch3) - 1, NULL);

    char patch4[] = "\xFF\x77\x7C\x83\xEC\x08\x81\x7F\x7C\xFF\xFF\xFF\xFF\x0F\x85\x6A\xB6\xC2\xFF\xA1\x00\x00\x00\x00\x89\x47\x7C\xE9\x5D\xB6\xC2\xFF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress10, patch4, sizeof(patch4) - 1, NULL);

    char patch5[] = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress16, patch5, sizeof(patch5) - 1, NULL);

    char patch6[] = "\xFF\x87\x70\x03\x00\x00\xFF\x05\xC8\xDA\xB9\x00\x83\x3D\xC8\xDA\xB9\x00\x0E\x74\x09\x0F\x1F\x40\x00\xE9\x6B\x5F\x89\xFC\xC7\x05\xC8\xDA\xB9\x00\x00\x00\x00\x00\xE9\x5C\x5F\x89\xFC";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress18, patch6, sizeof(patch6) - 1, NULL);

    char patch7[] = "\xE9\x8F\x74\xFE\x00\x66\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress26, patch7, sizeof(patch7) - 1, NULL);

    char patch8[] = "\xC7\x47\x64\x00\x00\x00\x00\x60\x8B\xD7\x8B\x0D\x10\x06\x4B\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x84\x53\x00\xFF\x61\xE9\x41\x8B\x01\xFF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress28, patch8, sizeof(patch8) - 1, NULL);

    char patch9[] = "\xE9\x8F\x74\xFE\x00\x66\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress32, patch9, sizeof(patch9) - 1, NULL);

    char patch10[] = "\xC7\x47\x40\x01\x00\x00\x00\x80\xBF\x9C\x00\x00\x00\x01\x0F\x84\x62\x0F\xD4\xDF\x60\x8B\xD7\x8B\x0D\x10\x06\x33\x01\xBE\x32\x00\x00\x00\x68\x88\x87\x00\x00\x6A\x04\xFF\x72\x18\xFF\x72\x14\xBA\x80\x00\x00\x00\xE8\x77\x53\xCD\xDF\x61\xE9\x37\x0F\xD4\xDF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress34, patch10, sizeof(patch10) - 1, NULL);
    // 定义操作码
    BYTE opCode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 计算跳转的相对偏移
    DWORD jumpOffset = targetAddress4 - (targetAddress3 + 5);
    memcpy(&opCode[1], &jumpOffset, sizeof(jumpOffset));
    BYTE opCode2[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset2 = targetAddress2 - (targetAddress + 5); //前面是跳转目标，后面是当前地址

    // 将第二个偏移值写入操作码数组
    opCode2[1] = jumpOffset2 & 0xFF;
    opCode2[2] = (jumpOffset2 >> 8) & 0xFF;
    opCode2[3] = (jumpOffset2 >> 16) & 0xFF;
    opCode2[4] = (jumpOffset2 >> 24) & 0xFF;
    BYTE opCode3[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset3 = targetAddress6 - (targetAddress5 + 5);

    // 将第二个偏移值写入操作码数组
    opCode3[1] = jumpOffset3 & 0xFF;
    opCode3[2] = (jumpOffset3 >> 8) & 0xFF;
    opCode3[3] = (jumpOffset3 >> 16) & 0xFF;
    opCode3[4] = (jumpOffset3 >> 24) & 0xFF;
    // 将目标值转换为DWORD型变量
    DWORD offsetX = 0x708;
    DWORD valueX = 0;
    DWORD targetValueX = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &valueX, sizeof(DWORD), NULL);
    // 在基址上加上偏移量708，得到目标地址的绝对地址
    DWORD finalAddress = valueX + offsetX;
    // 定义要写入的操作码
    BYTE opCode4[] = { 0x8B, 0x0D, 0x01, 0x01, 0x01, 0x01 };
    // 将finalValue写入到操作码
    ReadProcessMemory(hProcess, (LPCVOID)finalAddress, &targetValueX, sizeof(DWORD), NULL);
    DWORD jumpOffsetX = finalAddress;
    // 将第二个偏移值写入操作码数组
    opCode4[2] = jumpOffsetX & 0xFF;
    opCode4[3] = (jumpOffsetX >> 8) & 0xFF;
    opCode4[4] = (jumpOffsetX >> 16) & 0xFF;
    opCode4[5] = (jumpOffsetX >> 24) & 0xFF;
    BYTE opCode5[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset5 = targetAddress10 - (targetAddress9 + 5);

    // 将第二个偏移值写入操作码数组
    opCode5[1] = jumpOffset5 & 0xFF;
    opCode5[2] = (jumpOffset5 >> 8) & 0xFF;
    opCode5[3] = (jumpOffset5 >> 16) & 0xFF;
    opCode5[4] = (jumpOffset5 >> 24) & 0xFF;
    BYTE opCode6[] = { 0x0F,0x85, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset6 = targetAddress11 - (targetAddress12 + 5);

    // 将第二个偏移值写入操作码数组
    opCode6[2] = jumpOffset6 & 0xFF;
    opCode6[3] = (jumpOffset6 >> 8) & 0xFF;
    opCode6[4] = (jumpOffset6 >> 16) & 0xFF;
    opCode6[5] = (jumpOffset6 >> 24) & 0xFF;
    BYTE opCode7[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset7 = targetAddress11 - (targetAddress13 + 5);

    // 将第二个偏移值写入操作码数组
    opCode7[1] = jumpOffset7 & 0xFF;
    opCode7[2] = (jumpOffset7 >> 8) & 0xFF;
    opCode7[3] = (jumpOffset7 >> 16) & 0xFF;
    opCode7[4] = (jumpOffset7 >> 24) & 0xFF;
    // 在基址上加上偏移量708，得到目标地址的绝对地址
    // 定义要写入的操作码
    BYTE opCode8[] = { 0xA1, 0x00, 0x00, 0x00, 0x00 };
    // 将finalValue写入到操作码
    memcpy(&opCode8[1], &targetAddress14, sizeof(DWORD));    // 将目标值转换为DWORD型变量
    BYTE opCode9[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset9 = targetAddress18 - (targetAddress16 + 5);

    // 将第二个偏移值写入操作码数组
    opCode9[1] = jumpOffset9 & 0xFF;
    opCode9[2] = (jumpOffset9 >> 8) & 0xFF;
    opCode9[3] = (jumpOffset9 >> 16) & 0xFF;
    opCode9[4] = (jumpOffset9 >> 24) & 0xFF;
    BYTE opCode10[] = { 0xFF, 0x05 ,0x00, 0x00, 0x00, 0x00 };

    memcpy(&opCode10[2], &targetAddress14, sizeof(DWORD));    // 将目标值转换为DWORD型变量
    BYTE opCode11[] = { 0x83, 0x3D ,0x00, 0x00, 0x00, 0x00 ,0x0E};

    memcpy(&opCode11[2], &targetAddress14, sizeof(DWORD));    // 将目标值转换为DWORD型变量
    BYTE opCode12[] = { 0x74, 0x09};
    BYTE opCode13[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset13 = targetAddress17 - (targetAddress23 + 5);

    // 将第二个偏移值写入操作码数组
    opCode13[1] = jumpOffset13 & 0xFF;
    opCode13[2] = (jumpOffset13 >> 8) & 0xFF;
    opCode13[3] = (jumpOffset13 >> 16) & 0xFF;
    opCode13[4] = (jumpOffset13 >> 24) & 0xFF;
    BYTE opCode14[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset14 = targetAddress17 - (targetAddress24 + 5);

    // 将第二个偏移值写入操作码数组
    opCode14[1] = jumpOffset14 & 0xFF;
    opCode14[2] = (jumpOffset14 >> 8) & 0xFF;
    opCode14[3] = (jumpOffset14 >> 16) & 0xFF;
    opCode14[4] = (jumpOffset14 >> 24) & 0xFF;
    BYTE opCode15[] = { 0xC7, 0x05, 0x00, 0x00, 0x00 ,0x00, 0x00, 0x00, 0x00 ,0x00 };
    memcpy(&opCode15[2], &targetAddress14, sizeof(DWORD));    // 将目标值转换为DWORD型变量

    DWORD valueZ = 0;
    DWORD targetValueZ = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &valueZ, sizeof(DWORD), NULL);
    // 在基址上加上偏移量708，得到目标地址的绝对地址
    DWORD finalAddressZ = valueZ + 0x708;
    // 定义要写入的操作码
    BYTE opCode16[] = { 0x8B, 0x0D, 0x01, 0x01, 0x01, 0x01 };
    // 将finalValue写入到操作码
    DWORD jumpOffsetZ = finalAddressZ;
    // 将第二个偏移值写入操作码数组
    opCode16[2] = jumpOffsetZ & 0xFF;
    opCode16[3] = (jumpOffsetZ >> 8) & 0xFF;
    opCode16[4] = (jumpOffsetZ >> 16) & 0xFF;
    opCode16[5] = (jumpOffsetZ >> 24) & 0xFF;
    BYTE opCode17[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset17 = targetAddress6 - (baseAddress+0xFD6 + 5);

    // 将第二个偏移值写入操作码数组
    opCode17[1] = jumpOffset17 & 0xFF;
    opCode17[2] = (jumpOffset17 >> 8) & 0xFF;
    opCode17[3] = (jumpOffset17 >> 16) & 0xFF;
    opCode17[4] = (jumpOffset17 >> 24) & 0xFF;
    BYTE opCode18[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset18 = targetAddress27 - (baseAddress + 0xFDC + 5);

    // 将第二个偏移值写入操作码数组
    opCode18[1] = jumpOffset18 & 0xFF;
    opCode18[2] = (jumpOffset18 >> 8) & 0xFF;
    opCode18[3] = (jumpOffset18 >> 16) & 0xFF;
    opCode18[4] = (jumpOffset18 >> 24) & 0xFF;
    BYTE opCode19[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset19 = targetAddress34 - (targetAddress32 + 5);

    // 将第二个偏移值写入操作码数组
    opCode19[1] = jumpOffset19 & 0xFF;
    opCode19[2] = (jumpOffset19 >> 8) & 0xFF;
    opCode19[3] = (jumpOffset19 >> 16) & 0xFF;
    opCode19[4] = (jumpOffset19 >> 24) & 0xFF;


    DWORD valueY = 0;
    DWORD targetValueY = 0;
    ReadProcessMemory(hProcess, (LPCVOID)targetAddress7, &valueY, sizeof(DWORD), NULL);
    // 在基址上加上偏移量708，得到目标地址的绝对地址
    DWORD finalAddressY = valueY + 0x708;
    // 定义要写入的操作码
    BYTE opCode20[] = { 0x8B, 0x0D, 0x01, 0x01, 0x01, 0x01 };
    // 将finalValue写入到操作码
    DWORD jumpOffsetY = finalAddressY;
    // 将第二个偏移值写入操作码数组
    opCode20[2] = jumpOffsetY & 0xFF;
    opCode20[3] = (jumpOffsetY >> 8) & 0xFF;
    opCode20[4] = (jumpOffsetY >> 16) & 0xFF;
    opCode20[5] = (jumpOffsetY >> 24) & 0xFF;
    BYTE opCode21[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset21 = targetAddress6 - (targetAddress39 + 5);

    // 将第二个偏移值写入操作码数组
    opCode21[1] = jumpOffset21 & 0xFF;
    opCode21[2] = (jumpOffset21 >> 8) & 0xFF;
    opCode21[3] = (jumpOffset21 >> 16) & 0xFF;
    opCode21[4] = (jumpOffset21 >> 24) & 0xFF;
    BYTE opCode22[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset22 = targetAddress33 - (targetAddress40 + 5);

    // 将第二个偏移值写入操作码数组
    opCode22[1] = jumpOffset22 & 0xFF;
    opCode22[2] = (jumpOffset22 >> 8) & 0xFF;
    opCode22[3] = (jumpOffset22 >> 16) & 0xFF;
    opCode22[4] = (jumpOffset22 >> 24) & 0xFF;
    BYTE opCode23[] = { 0x0F,0x84, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset23 = targetAddress33 - (baseAddress + 0x4E8 + 5);

    // 将第二个偏移值写入操作码数组
    opCode23[2] = jumpOffset23 & 0xFF;
    opCode23[3] = (jumpOffset23 >> 8) & 0xFF;
    opCode23[4] = (jumpOffset23 >> 16) & 0xFF;
    opCode23[5] = (jumpOffset23 >> 24) & 0xFF;
    BYTE opCode24[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset24 = targetAddress28 - (targetAddress26 + 5);

    // 将第二个偏移值写入操作码数组
    opCode24[1] = jumpOffset24 & 0xFF;
    opCode24[2] = (jumpOffset24 >> 8) & 0xFF;
    opCode24[3] = (jumpOffset24 >> 16) & 0xFF;
    opCode24[4] = (jumpOffset24 >> 24) & 0xFF;
    // 将finalValue写入到操作码
    //memcpy(&opCode4[2], &finalValue, sizeof(DWORD));
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress3, opCode, sizeof(opCode), NULL))
    {
        MessageBox(NULL, L"写入失败", L"提示", MB_OK);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode2, sizeof(opCode2), NULL))
    {
        MessageBox(NULL, L"写入失败", L"提示", MB_OK);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress5, opCode3, sizeof(opCode3), NULL))
    {
        MessageBox(NULL, L"写入失败", L"提示", MB_OK);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress8, opCode4, sizeof(opCode4), NULL))
    {
        MessageBox(NULL, L"写入失败", L"提示", MB_OK);
        CloseHandle(hProcess);
        return;
    }
    WriteProcessMemory(hProcess, (LPVOID)targetAddress9, opCode5, sizeof(opCode5), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress12, opCode6, sizeof(opCode6), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress13, opCode7, sizeof(opCode7), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress15, opCode8, sizeof(opCode8), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress16, opCode9, sizeof(opCode9), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress19, opCode10, sizeof(opCode10), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress20, opCode11, sizeof(opCode11), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress21, opCode12, sizeof(opCode12), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress23, opCode13, sizeof(opCode13), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress24, opCode14, sizeof(opCode14), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress25, opCode15, sizeof(opCode15), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress29, opCode16, sizeof(opCode16), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress30, opCode17, sizeof(opCode17), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress31, opCode18, sizeof(opCode18), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress32, opCode19, sizeof(opCode19), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress38, opCode20, sizeof(opCode20), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress39, opCode21, sizeof(opCode21), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress40, opCode22, sizeof(opCode22), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress41, opCode23, sizeof(opCode23), NULL);
    WriteProcessMemory(hProcess, (LPVOID)targetAddress26, opCode24, sizeof(opCode24), NULL);
    CloseHandle(hProcess);

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
VOID CPvz::NotSubvert()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, 3, NULL);
    CloseHandle(hProcess);
}

// 樱桃瞬爆
VOID CPvz::CherryFast()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC943F;
    DWORD targetAddress = baseAddress + offset;
    DWORD offset2 = 0x5BD;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0xC9445;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset5 = 0x5D9;
    DWORD targetAddress5 = baseAddress + offset5;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char patch1[] = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    char patch2[] = "\x83\xBE\x84\x00\x00\x00\x02\x75\x0D\x0F\x1F\x40\x00\xB8\x00\x00\x00\x00\x90\x90\x90\x90\x89\x86\x9C\x00\x00\x00\xE9\x27\x94\x31\xFF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch2, sizeof(patch2) - 1, NULL);
    BYTE opCode5[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset5 = targetAddress2 - (targetAddress + 5);

    // 将第二个偏移值写入操作码数组
    opCode5[1] = jumpOffset5 & 0xFF;
    opCode5[2] = (jumpOffset5 >> 8) & 0xFF;
    opCode5[3] = (jumpOffset5 >> 16) & 0xFF;
    opCode5[4] = (jumpOffset5 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode5, sizeof(opCode5), NULL);
    BYTE opCode2[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset2 = targetAddress3 - (targetAddress5 + 5);

    // 将第二个偏移值写入操作码数组
    opCode2[1] = jumpOffset2 & 0xFF;
    opCode2[2] = (jumpOffset2 >> 8) & 0xFF;
    opCode2[3] = (jumpOffset2 >> 16) & 0xFF;
    opCode2[4] = (jumpOffset2 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress5, opCode2, sizeof(opCode2), NULL);
    CloseHandle(hProcess);
}
// 樱桃瞬爆
VOID CPvz::CherryNo()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC943F;
    DWORD targetAddress = baseAddress + offset;
    DWORD offset2 = 0x606;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0xC9445;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x614;
    DWORD targetAddress4 = baseAddress + offset4;
    DWORD offset5 = 0x61F;
    DWORD targetAddress5 = baseAddress + offset5;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char patch1[] = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    char patch2[] = "\x83\xBE\x84\x00\x00\x00\x02\x75\x0A\x0F\x1F\x40\x00\x90\xE9\x32\x94\x31\xFF\x89\x86\x9C\x00\x00\x00\xE9\x27\x94\x31\xFF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch2, sizeof(patch2) - 1, NULL);
    BYTE opCode5[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset5 = targetAddress2 - (targetAddress + 5);

    // 将第二个偏移值写入操作码数组
    opCode5[1] = jumpOffset5 & 0xFF;
    opCode5[2] = (jumpOffset5 >> 8) & 0xFF;
    opCode5[3] = (jumpOffset5 >> 16) & 0xFF;
    opCode5[4] = (jumpOffset5 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode5, sizeof(opCode5), NULL);
    BYTE opCode1[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset1 = targetAddress3 - (targetAddress4 + 5);

    // 将第二个偏移值写入操作码数组
    opCode1[1] = jumpOffset1 & 0xFF;
    opCode1[2] = (jumpOffset1 >> 8) & 0xFF;
    opCode1[3] = (jumpOffset1 >> 16) & 0xFF;
    opCode1[4] = (jumpOffset1 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress4, opCode1, sizeof(opCode1), NULL);
    BYTE opCode2[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    // 设置第二个偏移值
    DWORD jumpOffset2 = targetAddress3 - (targetAddress5 + 5);

    // 将第二个偏移值写入操作码数组
    opCode2[1] = jumpOffset2 & 0xFF;
    opCode2[2] = (jumpOffset2 >> 8) & 0xFF;
    opCode2[3] = (jumpOffset2 >> 16) & 0xFF;
    opCode2[4] = (jumpOffset2 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress5, opCode2, sizeof(opCode2), NULL);
    CloseHandle(hProcess);
}
VOID CPvz::MeowFast()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC69AE;
    DWORD targetAddress = baseAddress + offset;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char patch1[] = "\xC7\x83\x9C\x00\x00\x00\x00\x00\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);
    CloseHandle(hProcess);
}
VOID CPvz::LoursMC()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0xC3BD8;
    DWORD targetAddress = baseAddress + offset;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char patch1[] = "\xEB\x04";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);
    offset = 0xC3BCD;
    targetAddress = baseAddress + offset;
    char patch2[] = "\xE9\x00\x00\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch2, sizeof(patch2) - 1, NULL);
    DWORD offset2 = 0x5BF+100;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0xC3BD3;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x633;
    DWORD targetAddress4 = baseAddress + offset4;

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    DWORD dwOldProtect2 = 0;
    VirtualProtectEx(hProcess, (LPVOID)targetAddress2, 1024, PAGE_EXECUTE_READWRITE, &dwOldProtect2);
    char patch3[] = "\xC7\x87\xBC\x00\x00\x00\x00\x10\x00\x00\x8B\x8F\xBC\x00\x00\x00\xE9\xBE\x3B\x17\xFF";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch3, sizeof(patch3) - 1, NULL);
    BYTE opCode5[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    DWORD jumpOffset5 = targetAddress2 - (targetAddress + 5);
    // 将第二个偏移值写入操作码数组
    opCode5[1] = jumpOffset5 & 0xFF;
    opCode5[2] = (jumpOffset5 >> 8) & 0xFF;
    opCode5[3] = (jumpOffset5 >> 16) & 0xFF;
    opCode5[4] = (jumpOffset5 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode5, sizeof(opCode5), NULL);
    BYTE opCode1[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
    DWORD jumpOffset1 = targetAddress3 - (targetAddress4 + 5);
    opCode1[1] = jumpOffset1 & 0xFF;
    opCode1[2] = (jumpOffset1 >> 8) & 0xFF;
    opCode1[3] = (jumpOffset1 >> 16) & 0xFF;
    opCode1[4] = (jumpOffset1 >> 24) & 0xFF;
    WriteProcessMemory(hProcess, (LPVOID)targetAddress4, opCode1, sizeof(opCode1), NULL);
    CloseHandle(hProcess);
}
// 植物无敌
VOID CPvz::GodMode()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
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
}
const char* getAssemblyBytes(DWORD address) {
    // 创建一个临时的char数组来存储字节码
    static char assemblyBytes[5] = { 0 };

    // 将DWORD地址拆分为4个字节并存储到字节码数组中
    for (int i = 0; i < 4; i++) {
        assemblyBytes[i] = (address >> (i * 8)) & 0xFF;
    }

    // 返回字节码数组的指针
    return assemblyBytes;
}

VOID CPvz::Point2()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    DWORD baseAddress = GetModuleBaseAddress(hProcess, L"PlantsVsZombies.exe"); //首先读取大写的
    if (baseAddress != 0)
    {
        printf("test");
    }
    else
    {
        baseAddress = GetModuleBaseAddress(hProcess, L"plantsvszombies.exe");
    }
    DWORD offset = 0x91FB2;
    DWORD targetAddress = baseAddress + offset;
    DWORD offset2 = 0xF00;
    DWORD targetAddress2 = baseAddress + offset2;
    DWORD offset3 = 0xF29;
    DWORD targetAddress3 = baseAddress + offset3;
    DWORD offset4 = 0x91FB7;
    DWORD targetAddress4 = baseAddress + offset4;
    // 获取目标地址对应的字节码
    //char patch1[] = "\xE9\x43\xEF\xF6\xFF\66\x0F\x1F\x44\x00\x00";  // 初始化为包含了占位字节的数组
    LPVOID baseAddress3 = (LPVOID)targetAddress2;  // 要申请的起始地址
    SIZE_T allocationSize = 64;  // 要申请的内存大小（字节）
    LPVOID allocatedMemory = VirtualAllocEx(hProcess, baseAddress3, allocationSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)baseAddress3, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    //WriteProcessMemory(hProcess, (LPVOID)targetAddress2, patch1, sizeof(patch1) - 1, NULL);
    // 修改char数组的前四个位置
    //49E04800
    char patch1[] = "\xE9\x00\x00\x00\x00\x66\x0F\x1F\x44\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)targetAddress, patch1, sizeof(patch1) - 1, NULL);

    char patch2[] = "\x66\xC7\x40\x20\x06\x00\x66\xC7\x40\x24\x06\x00\x66\xC7\x40\x28\x06\x00\x66\xC7\x40\x2C\x03\x00\x66\xC7\x40\x30\x02\x00\x0F\xB7\x40\x20\x66\x89\x87\xA8\x03\x00\x00\xE9\x00\x00\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)baseAddress3, patch2, sizeof(patch2) - 1, NULL);
    // 定义操作码
    BYTE opCode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 计算跳转的相对偏移
    DWORD jumpOffset = targetAddress4 - (targetAddress3 + 5);
    memcpy(&opCode[1], &jumpOffset, sizeof(jumpOffset));
    BYTE opCode2[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

    // 设置第二个偏移值
    DWORD jumpOffset2 = targetAddress2 - (targetAddress + 5);

    // 将第二个偏移值写入操作码数组
    opCode2[1] = jumpOffset2 & 0xFF;
    opCode2[2] = (jumpOffset2 >> 8) & 0xFF;
    opCode2[3] = (jumpOffset2 >> 16) & 0xFF;
    opCode2[4] = (jumpOffset2 >> 24) & 0xFF;
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress3, opCode, sizeof(opCode), NULL))
    {
        MessageBox(NULL, L"写入失败", L"提示", MB_OK);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, (LPVOID)targetAddress, opCode2, sizeof(opCode2), NULL))
        {
            MessageBox(NULL, L"写入失败", L"提示", MB_OK);
            CloseHandle(hProcess);
            return;
        }
    CloseHandle(hProcess);
}