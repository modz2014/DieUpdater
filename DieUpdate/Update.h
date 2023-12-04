#include "DieMain.h"
#pragma once
bool CheckForUpdate(HWND hwnd, const std::string& currentVersion);
bool DownloadUpdate(HWND hwnd, const std::wstring& updateUrl, const std::wstring& destination);
void ShowReleaseNotesDialog(HWND hwnd, const std::string& releaseNotes);