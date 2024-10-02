#pragma once
typedef unsigned char byte_t;

struct FileInfoNode {
	char* pPathname;
	unsigned int fileSize;
	FileInfoNode* pNext;
	FileInfoNode() : pPathname(nullptr), fileSize(0), pNext(nullptr) {}
	~FileInfoNode() {
		free(pPathname);
	}
};

const wchar_t* GAME_TITLE = L"Plants Vs Zombies GhTr ~ Perfect Voyage ver.0.17b";
const wchar_t* GAME_PROCESS_NAME_CAPITAL = L"PlantsVsZombies.exe";
const wchar_t* GAME_PROCESS_NAME_LOWER = L"plantsvszombies.exe";

constexpr auto MAGIC_NUM = 0xC04AC0BA;
constexpr auto VERSION_NUM = (0xFFFFFFFF ^ 0xFFFFFFFF);
constexpr auto XOR_KEY = (0xFFFFFFFF ^ 0x14141414);
/**
 * 转码UTF8
 *
 * @param wstr 目标转换字符串
 */
std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty())
		return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
	return str;
}
/**
 * 解码UTF8
 *
 * @param str 目标转换字符串
 */
std::wstring utf8_decode(const std::string& str)
{
	if (str.empty())
		return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
	return wstr;
}
/**
 * 查找文件夹内所有文件并建表
 *
 * @param find_path 查找的目录
 * @param files_name 文件名数组
 * @param files_size 文件大小数组
 * @param files_time 文件修改时间数组
 */
void find_files(const std::wstring find_path,
	std::vector<std::wstring>& files_name,
	std::vector<int>& files_size,
	std::vector<FILETIME>& files_time)
{
	std::wstring path = find_path + L"\\*";
	WIN32_FIND_DATA ffd;
	HANDLE hf;

	hf = FindFirstFileW(path.c_str(), &ffd);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring name = ffd.cFileName;
			if (name == L"." || name == L".." //
				|| name == L"thumbs.db" || name == L"Thumbs.db")
				continue;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::wstring sub_path = find_path + L"\\" + name;
				std::vector<std::wstring> _names;
				std::vector<int> _sizes;
				std::vector<FILETIME> _times;
				find_files(sub_path, _names, _sizes, _times);
				for (size_t i = 0; i < _names.size(); i++)
				{
					files_name.push_back(name + L"\\" + _names[i]);
					files_size.push_back(_sizes[i]);
					files_time.push_back(_times[i]);
				}
			}
			else
			{
				files_name.push_back(ffd.cFileName);
				files_size.push_back(ffd.nFileSizeLow);
				files_time.push_back(ffd.ftLastWriteTime);
			}
		} while (FindNextFileW(hf, &ffd) != 0);
		FindClose(hf);
	}
}
/**
 * 创建目标解包路径文件夹
 *
 * @param path 要创建的目录地址
 */
bool create_path(const std::wstring& path)
{
	if (path == L"")
		return false;
	DWORD fa = GetFileAttributesW(path.c_str());
	if ((fa != INVALID_FILE_ATTRIBUTES) && (fa & FILE_ATTRIBUTE_DIRECTORY))
	if (CreateDirectoryW(path.c_str(), nullptr))
		return true;
	if (path.substr(0, path.find_last_of(L"\\")) == path)
		return false;
	if (!create_path(path.substr(0, path.find_last_of(L"\\"))))
		return false;
	return CreateDirectoryW(path.c_str(), nullptr);
}
/**
 * 弹出选择文件夹对话框
 *
 */
std::filesystem::path SelectFolder()
{
	CFolderPickerDialog folderPickerDialog(NULL, OFN_ENABLESIZING | OFN_OVERWRITEPROMPT, NULL, 0);
	std::filesystem::path resultPath;
	if (folderPickerDialog.DoModal() == IDOK)
	{
		resultPath = std::filesystem::path(folderPickerDialog.GetPathName().GetString());
	}
	return resultPath;
}
/**
 * 交换字节顺序
 *
 * @param x 要交换的字节
 */
unsigned char swapNibbles(unsigned char x) {
	return (x << 4) | (x >> 4);
}
/**
 * 交换DWORD所有字节的顺序
 *
 * @param val 要交换的DWORD变量
 */
unsigned long swapBytesInLong(unsigned long val) {
	unsigned long result = 0;
	result |= (unsigned long)swapNibbles((unsigned char)(val & 0xFF)) << 24;
	result |= (unsigned long)swapNibbles((unsigned char)((val >> 8) & 0xFF)) << 16;
	result |= (unsigned long)swapNibbles((unsigned char)((val >> 16) & 0xFF)) << 8;
	result |= (unsigned long)swapNibbles((unsigned char)((val >> 24) & 0xFF));
	return result;
}
/**
 * 打开一个文件夹
 *
 * @param FolderName 要打开的文件夹地址
 */
void OpenFolder(const CString& FolderName)
{
	ShellExecute(NULL, _T("open"), LPCTSTR(FolderName), NULL, NULL, SW_SHOWDEFAULT);
}
/**
 * 打开一个文件夹并选中文件
 *
 * @param itemName 要选中的文件地址
 */
void OpenFolderAndSelectItem(const std::wstring& itemName)
{
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);
	std::wstring currentFolderPath = std::wstring(buffer);
	std::wstring fullPath = currentFolderPath + L"\\" + itemName;
	CComHeapPtr<ITEMIDLIST> pidl;
	if (FAILED(SHParseDisplayName(fullPath.c_str(), NULL, &pidl, 0, NULL)))
	{
		MessageBox(NULL, L"无法找到文件", L"错误", MB_OK);
		return;
	}
	SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
	pidl.Free();
}