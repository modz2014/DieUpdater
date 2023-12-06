#include "framework.h"
#include "DieMain.h"

/**
 * @brief Checks if the system is currently in Dark Mode.
 *
 * This function queries the Windows Registry to determine whether the system is
 * configured to use Dark Mode.
 *
 * @return True if Dark Mode is enabled, false if it is not.
 */
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
/**
 * @brief Retrieves a DWORD value from the Windows Registry.
 *
 * This function queries the specified registry key for a DWORD value with the
 * given name and retrieves the result through the nValue parameter.
 *
 * @param hKey The handle to the open registry key.
 * @param strValueName The name of the registry value to retrieve.
 * @param nValue Reference to a variable where the retrieved DWORD value will be stored.
 * @return The result of the registry query operation.
 */
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