#pragma once
#include <string>
#include "resource.h"
#include "Controls.h"


bool DownloadFile(const std::wstring& url, const std::wstring& destination, HWND hwnd);
void UpdateProgressBar(HWND hwnd, int progress);
LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue);
void SetProgressBarColor(HWND hProgressBar);
extern HWND hProgressBar;  // Declare as extern here
bool IsDarkMode();
bool ExtractZip(const std::wstring& zipPath, HWND hwndRichEdit);