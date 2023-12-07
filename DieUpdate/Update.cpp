#include "framework.h"
#include "DieMain.h"
#include <string>
#include <iostream>
#include "Update.h"
#include "json.h"
#include <wininet.h>

/**
 * @brief Checks for updates and initiates the update process if a new version is available.
 *
 * This function sends an HTTP request to the specified server URL to check for updates.
 * If a new version is available, it downloads the update and displays release notes.
 *
 * @param hwnd The handle to the main window.
 * @param currentVersion The current version of the application.
 * @return True if an update is performed, false if the application is up to date or the update fails.
 */
bool CheckForUpdate(HWND hwnd, const std::string& currentVersion) {
	OutputDebugStringA("Checking for updates...\n");

	HINTERNET hInternet, hConnect;
	DWORD bytesRead;

	hInternet = InternetOpen(L"HTTP Example Program", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		DWORD error = GetLastError();
		std::cerr << "InternetOpen failed." << std::endl;
		OutputDebugStringA("InternetOpen failed.\n");

		return false;
	}

	const std::string serverUrl = "http://192.168.1.9:8000/updates";
	hConnect = InternetOpenUrlA(hInternet, serverUrl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hConnect == NULL) {
		DWORD error = GetLastError();
		std::cerr << "InternetOpenUrlA failed." << std::endl;
		OutputDebugStringA("InternetOpenUrlA failed.\n");
		InternetCloseHandle(hInternet);
		return false;
	}

	char buffer[4096];
	std::string response;

	while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		response.append(buffer, bytesRead);
	}

	// Print the raw server response for debugging
	OutputDebugStringA(("Server Response: " + response + "\n").c_str());
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	// Print the received JSON response for debugging
	std::cout << "JSON Response: " << response << std::endl;

	// Parse the JSON response
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(response, root)) {
		// Handle JSON parsing error
		std::cerr << "Failed to parse JSON response." << std::endl;
		OutputDebugStringA("Failed to parse JSON response.\n");
		return false;
	}

	// Check if the root is an array and contains at least one element
	if (root.isArray() && !root.empty()) {
		// Extract information from the first object in the array
		Json::Value updateInfo = root[0];
		std::string latestVersion = updateInfo["latestVersion"].asString();
		std::string downloadUrl = updateInfo["downloadUrl"].asString();
		std::string releaseNotes = updateInfo["releaseNotes"].asString();

		// Output debug information
		OutputDebugStringA(("Latest Version: " + latestVersion + "\n").c_str());
		OutputDebugStringA(("Download URL: " + downloadUrl + "\n").c_str());

		// Check for updates
		if (currentVersion != latestVersion) {
			OutputDebugString(L"A new version is available.\n");
			// Convert std::string to std::wstring
			std::wstring downloadUrlW(downloadUrl.begin(), downloadUrl.end());

			// Extract the filename from the download URL
			size_t lastSlashPos = downloadUrl.find_last_of('/');
			std::string filename = (lastSlashPos != std::string::npos) ? downloadUrl.substr(lastSlashPos + 1) : "default_filename";
			std::wstring destinationFilenameW = std::wstring(filename.begin(), filename.end());

			// Define the destination path for the update file using the extracted filename
			// Save in the same directory as the executable
			std::wstring destinationPath = destinationFilenameW;

			// Update the RichEdit control with the download URL
			std::wstring progressText = L"Downloading update from: " + downloadUrlW + L"\n";
			SendMessage(GetDlgItem(hwnd, IDC_RICHTEXTBOX), EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(progressText.c_str()));

			// Download the update
			if (DownloadUpdate(hwnd, downloadUrlW, destinationPath)) {
				OutputDebugString(L"Update downloaded successfully.\n");
				// Perform installation logic here
			}
			else {
				OutputDebugString(L"Failed to download the update.\n");
			}
			ShowReleaseNotesDialog(hwnd, releaseNotes);
			return true;
		}
		else {
			OutputDebugString(L"Your program is up to date.\n");
			return false;
		}
	}
}
/**
 * @brief Displays a custom dialog with release notes.
 *
 * This function creates and displays a custom dialog box containing release notes
 * retrieved from the update server.
 *
 * @param hwnd The handle to the main window.
 * @param releaseNotes A string containing the release notes for the latest version.
 */
void ShowReleaseNotesDialog(HWND hwnd, const std::string& releaseNotes) {
	// Convert the release notes from std::string to std::wstring
	std::wstring releaseNotesW(releaseNotes.begin(), releaseNotes.end());

	// Create and show the custom dialog with the release notes
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CUSTOM_DIALOG), hwnd, CustomDialogProc, reinterpret_cast<LPARAM>(releaseNotesW.c_str()));
}
/**
 * @brief Downloads the update file from the specified URL.
 *
 * This function initiates the download of the update file from the given URL.
 * It displays progress information in the main window's RichEdit control.
 *
 * @param hwnd The handle to the main window.
 * @param updateUrl The URL of the update file.
 * @param destination The local path where the update file will be saved.
 * @return True if the update is downloaded successfully, false otherwise.
 */
bool DownloadUpdate(HWND hwnd, const std::wstring& updateUrl, const std::wstring& destination) {
	HINTERNET hInternet = InternetOpen(L"DieMain", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		// Handle the error (log, display, etc.)
		return false;
	}

	HINTERNET hConnect = InternetOpenUrl(hInternet, updateUrl.c_str(), NULL, 0, 0, 0);
	if (hConnect == NULL) {
		InternetCloseHandle(hInternet);
		// Handle the error (log, display, etc.)
		return false;
	}
	// Get the file size
	DWORD fileSize = 0;
	DWORD fileSizeLength = sizeof(fileSize);
	if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &fileSize, &fileSizeLength, NULL)) {
		// If HttpQueryInfo fails, set fileSize to a default value or handle the error
		fileSize = 1024 * 1024; // Default to 1 MB if size is unknown
	}

	HANDLE hFile = CreateFile(destination.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		// Handle the error (log, display, etc.)
		return false;
	}

	constexpr DWORD BUFFER_SIZE = 4096;
	BYTE buffer[BUFFER_SIZE];
	DWORD bytesRead, bytesWritten, totalBytesWritten = 0;
	int percentage = 0; // Initialize the percentage variable outside the loop

	while (InternetReadFile(hConnect, buffer, BUFFER_SIZE, &bytesRead) && bytesRead > 0) {
		if (!WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL) || bytesRead != bytesWritten) {
			CloseHandle(hFile);
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hInternet);
			// Handle the error (log, display, etc.)
			return false;
		}

		totalBytesWritten += bytesRead;
		int newPercentage = static_cast<int>((static_cast<double>(totalBytesWritten) / fileSize) * 100);
		newPercentage = max(0, min(newPercentage, 100)); // Clamp the value between 0 and 100

		if (newPercentage != percentage) { // Only update if the percentage has changed
			percentage = newPercentage;
			UpdateProgressBar(hwnd, percentage);
		}

		// Assuming 'jsonString' contains the JSON data as a std::string
		std::string jsonString = R"({"url": "http://192.168.1.9:8000/updates"})"; // Replace with your actual JSON string

		// Parse the JSON response
		Json::Value root;
		Json::Reader reader;
		if (!reader.parse(jsonString, root)) {
			// Handle JSON parsing error
			std::cerr << "Failed to parse JSON response." << std::endl;
			return false;
		}

		// Extract the URL from the JSON object
		std::string downloadUrl = root["url"].asString();

		// Convert std::string to std::wstring
		std::wstring downloadUrlW(downloadUrl.begin(), downloadUrl.end());

		// Set the RichEdit control with the download URL only, without the progress
		std::wstring urlText = L"Download URL: " + downloadUrlW + L"\n";
		SendMessage(GetDlgItem(hwnd, IDC_RICHTEXTBOX), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(urlText.c_str()));
	}

	// If extraction failed, close handles and return false
	CloseHandle(hFile);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	bool extractionResult = ExtractZip(destination, hwnd);

	// Check if extraction was successful
	if (extractionResult) {
		// Check if the download was successful
		if (totalBytesWritten > 0) {
			// Update the RichEdit control with the download success message
			SendMessage(GetDlgItem(hwnd, IDC_RICHTEXTBOX), EM_SETSEL, -1, -1); // Set selection to the end
			SendMessage(GetDlgItem(hwnd, IDC_RICHTEXTBOX), EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(L"Download completed.\n"));
		}
		return extractionResult; // Return the result of extraction
	}
	return false;
}
/**
 * @brief Thread function to check for updates and handle the update process.
 *
 * This function runs in a separate thread to asynchronously check for updates
 * using the CheckForUpdate function. It handles the result of the update check.
 *
 * @param hwnd The handle to the main window.
 * @param currentVersion The current version of the application.
 */
void UpdateThreadFunction(HWND hwnd, const std::string& currentVersion) {
	if (CheckForUpdate(hwnd, currentVersion)) {
		// Handle successful update check
	}
	else {
		// Handle no update or failure
	}
}