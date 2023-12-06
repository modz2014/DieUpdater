#include "DieMain.h"
#pragma once

// CheckForUpdate: Checks for available updates and performs update logic if needed.
bool CheckForUpdate(HWND hwnd, const std::string& currentVersion);

// DownloadUpdate: Downloads and installs an update from the specified URL.
bool DownloadUpdate(HWND hwnd, const std::wstring& updateUrl, const std::wstring& destination);

// ShowReleaseNotesDialog: Displays a dialog with release notes.
void ShowReleaseNotesDialog(HWND hwnd, const std::string& releaseNotes);
