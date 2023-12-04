#include "framework.h"
#include "DieMain.h"
#include <string>
#include <iostream>
#include "Update.h"
#include "json.h"
#include <wininet.h>

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
    const std::string serverUrl = "Update to your Url for json ";
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

void ShowReleaseNotesDialog(HWND hwnd, const std::string& releaseNotes) {
    // Convert the release notes from std::string to std::wstring
    std::wstring releaseNotesW(releaseNotes.begin(), releaseNotes.end());

    // Create and show the custom dialog with the release notes
    DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CUSTOM_DIALOG), hwnd, CustomDialogProc, reinterpret_cast<LPARAM>(releaseNotesW.c_str()));
}

bool DownloadUpdate(HWND hwnd, const std::wstring& updateUrl, const std::wstring& destination) {
    // Display a message or perform any actions indicating the start of the update download
    std::wstring updateStartText = L"Downloading update...\n";
    UpdateRichEdit(updateStartText.c_str());

    // Download the update file
    if (DownloadFile(updateUrl, destination, hwnd)) {
        // Successful download, perform any additional update steps
        // For example, you can call a function to extract and apply the update
        std::wstring updateCompleteText = L"Update download complete.\n";
        UpdateRichEdit(updateCompleteText.c_str());

        // Now you can perform further actions, such as extracting and applying the update
        // ExtractAndApplyUpdate(destination);

        return true;
    }
    else {
        // Handle download failure
        std::wstring updateErrorText = L"Failed to download the update.\n";
        UpdateRichEdit(updateErrorText.c_str());
        return false;
    }
}

void UpdateThreadFunction(HWND hwnd, const std::string& currentVersion) {
    if (CheckForUpdate(hwnd, currentVersion)) {
        // Handle successful update check
    }
    else {
        // Handle no update or failure
    }
}