#include "framework.h"
#include "DieMain.h"


bool IsDarkMode() {
    bool result{ false };
    HKEY hKey;
    const auto lRes = RegOpenKeyExW(HKEY_CURRENT_USER,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0, KEY_READ, &hKey);
    if (lRes == ERROR_SUCCESS) {
        DWORD usesDarkTheme;
        const auto res = GetDWORDRegKey(hKey, L"AppsUseLightTheme", usesDarkTheme);
        if (res == ERROR_SUCCESS) {
            result = (usesDarkTheme == 0);

            // Add debugging MessageBox to display the value
            std::wstring message = L"Apps use light theme: " + std::to_wstring(usesDarkTheme);
            //MessageBox(nullptr, message.c_str(), L"Debug Info", MB_OK | MB_ICONINFORMATION);
        }
    }
    RegCloseKey(hKey);
    return result;
}

LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue) {
    nValue = -1;
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = ::RegQueryValueEx(hKey,
        strValueName.c_str(),
        nullptr,
        nullptr,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize);
    if (ERROR_SUCCESS == nError) {
        nValue = nResult;
    }
    return nError;
}