#pragma once
#include <string>
#include "resource.h"
#include "Controls.h"


// DownloadFile: Downloads a file from the specified URL to the given destination.
bool DownloadFile(const std::wstring& url, const std::wstring& destination, HWND hwnd);

// UpdateProgressBar: Updates the progress bar in the specified window.
void UpdateProgressBar(HWND hwnd, int progress);

// GetDWORDRegKey: Retrieves a DWORD value from the registry.
LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue);

// SetProgressBarColor: Sets the color of the progress bar based on the current mode.
void SetProgressBarColor(HWND hProgressBar);

extern HWND hProgressBar;  // Declare as extern here to be used across multiple source files.

// IsDarkMode: Checks if the system is currently in dark mode.
bool IsDarkMode();

// ExtractZip: Extracts the contents of a ZIP file to the specified RichEdit control.
bool ExtractZip(const std::wstring& zipPath, HWND hwndRichEdit);
