// DieMain.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "DieMain.h"
#include <wininet.h>
#include <CommCtrl.h> 
#include <thread>
#include "Update.h"
#include "json.h"
#include <Shlwapi.h>
#include <shellapi.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[] = L"UPDATER";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    //EnableDarkModeForApp();
    COLORREF g_TitleBarColor = RGB(0, 0, 0); // Black color for the title bar
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDC_DieMain, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
 
    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DieMain));
    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DieMain));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL(IDC_DieMain);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    // Specify a smaller width and height for a more professional look
    int windowWidth = 429;
    int windowHeight = 150;
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, 0, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void UpdateRichTextBoxTheme(HWND hRichTextBox, bool darkMode) {
    if (darkMode) {
        // Update RichTextBox background color for dark mode
        SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
        // Set RichTextBox text color to white
        CHARFORMAT2 cf;
        memset(&cf, 0, sizeof(CHARFORMAT2));
        cf.cbSize = sizeof(CHARFORMAT2);
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = RGB(255, 255, 255);
        SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
    }
    else {
        // Update RichTextBox background color for light mode
        SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
        // Set RichTextBox text color to black
        CHARFORMAT2 cf;
        memset(&cf, 0, sizeof(CHARFORMAT2));
        cf.cbSize = sizeof(CHARFORMAT2);
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = RGB(0, 0, 0);
        SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
    static bool isChecked = false;  // Declare isChecked as a static variable
    switch (message)
    {
    case WM_CREATE:
    {
        // Initialize GDI+
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        CreateControls(hWnd);
        // Run CheckForUpdate in a separate thread
        std::thread updateThread([hWnd]() {
            std::string currentVersion = "1.0";
            CheckForUpdate(hWnd, currentVersion);
            });
        updateThread.detach();
        break;
    }
    case WM_COMMAND:
    { 
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
       
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_ADVANCED_CHECKBOX:
            isChecked = !isChecked; // Toggle the state
            CheckDlgButton(hWnd, IDC_ADVANCED_CHECKBOX, isChecked ? BST_CHECKED : BST_UNCHECKED);
            ToggleAdvancedControls(hWnd, isChecked);
            if (isChecked) {
                OutputDebugString(L"Checkbox is checked.\n");

                // If the checkbox is checked, create the RichEdit control
                CreateRichEditControl(hWnd);

                // Debug message to verify that the RichEdit control handle is valid
                if (hRichTextBox != NULL) {
                    OutputDebugString(L"RichEdit control created successfully.\n");
                }
                else {
                    OutputDebugString(L"Failed to create RichEdit control.\n");
                }
              
                // Show the RichEdit control
                ToggleRichEditControl(hWnd, true);
                if (IsDarkMode()) {
                    SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30)); // Dark background color
                }
                else {
                    SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255)); // Light background color
                }
                int newWidth = 429;  // Set your desired width
                int newHeight = 267; // Set your desired height
                SetWindowPos(hWnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);
            
            
            }
            else {
                OutputDebugString(L"Checkbox is unchecked.\n");

                // If the checkbox is unchecked, hide the RichEdit control
                ShowWindow(hRichTextBox, SW_HIDE);
                ToggleRichEditControl(hWnd, false);
                // Checkbox is unchecked, restore the original size
                int originalWidth = 429;  // Set your original width
                int originalHeight = 150; // Set your original height
                SetWindowPos(hWnd, NULL, 0, 0, originalWidth, originalHeight, SWP_NOMOVE | SWP_NOZORDER);
            }
         
     
        break;
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SETTINGCHANGE:
        if (lParam && _wcsicmp(L"ImmersiveColorSet", (LPCWSTR)lParam) == 0) {
           
            // The system theme has changed, so update your application's theme
            if (IsDarkMode()) {
                HBRUSH hBrushDark = CreateSolidBrush(RGB(30, 30, 30, 255)); // Dark background color
                SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrushDark);
                // Set text color to white
                SetTextColor(GetDC(hWnd), RGB(255, 255, 255));
                g_pBitmap = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MY_DARK_IMAGE));
              
            }
            else {
                HBRUSH hBrushLight = CreateSolidBrush(RGB(255, 255, 255)); // Light background color
                SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrushLight);
                // Set text color to black
                SetTextColor(GetDC(hWnd), RGB(0, 0, 0));
                g_pBitmap = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MY_LIGHT_IMAGE));

            }
            if (isChecked) {
                if (IsDarkMode()) {
                    // Update RichTextBox background color for dark mode
                    SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
                    // Set RichTextBox text color to white
                    CHARFORMAT2 cf;
                    memset(&cf, 0, sizeof(CHARFORMAT2));
                    cf.cbSize = sizeof(CHARFORMAT2);
                    cf.dwMask = CFM_COLOR;
                    cf.crTextColor = RGB(255, 255, 255);
                    SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);// Dark background color
                }
                else {
                    // Update RichTextBox background color for light mode
                    SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
                    // Set RichTextBox text color to black
                    CHARFORMAT2 cf;
                    memset(&cf, 0, sizeof(CHARFORMAT2));
                    cf.cbSize = sizeof(CHARFORMAT2);
                    cf.dwMask = CFM_COLOR;
                    cf.crTextColor = RGB(0, 0, 0);
                    SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
                }
            }
            UpdateRichTextBoxTheme(hRichTextBox, IsDarkMode());
            SetProgressBarColor(hProgressBar); // Update the progress bar color
            InvalidateRect(hRichTextBox, NULL, TRUE);
            InvalidateRect(hWnd, NULL, TRUE); // Redraw the window with the new theme colors
        }
        break;
    case WM_PAINT:
    { 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Check if the GDI+ Bitmap has been loaded
        if (g_pBitmap != NULL) {
            // Create a Graphics object from the HDC
            Gdiplus::Graphics graphics(hdc);
            // Set the desired dimensions for the resized image
            int desiredWidth = 120;  // Desired width
            int desiredHeight = 120; // Desired height
            int yPos = 0; // You can change this value based on your preference
            // Draw the image resized to the desired dimensions
            graphics.DrawImage(g_pBitmap, 0, yPos, desiredWidth, desiredHeight);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hStatic = (HWND)lParam;
        if (IsDarkMode()) {
            SetTextColor(hdcStatic, RGB(255, 255, 255)); // White text for dark mode
            SetBkColor(hdcStatic, RGB(30, 30, 30, 255)); // Black background for dark mode
            static HBRUSH hBrushDark = CreateSolidBrush(RGB(30, 30, 30, 255));
            return (INT_PTR)hBrushDark;
        }
        else {
            SetTextColor(hdcStatic, RGB(0, 0, 0)); // Black text for light mode
            SetBkColor(hdcStatic, RGB(255, 255, 255)); // White background for light mode
            static HBRUSH hBrushLight = CreateSolidBrush(RGB(255, 255, 255));
            return (INT_PTR)hBrushLight;
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        if (pdis->CtlID = g_pBitmap != NULL) {
            Gdiplus::Graphics graphics(pdis->hDC);
            Gdiplus::Rect rect(pdis->rcItem.left, pdis->rcItem.top,
                pdis->rcItem.right - pdis->rcItem.left,
                pdis->rcItem.bottom - pdis->rcItem.top);
            graphics.DrawImage(g_pBitmap, rect);
        }
        break;
    }
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hWnd, &rect);
        HBRUSH hBrush;
        if (IsDarkMode()) {
            hBrush = CreateSolidBrush(RGB(30, 30, 30, 255)); // Dark background
        }
        else {
            hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Light background
        }
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
        return 1; // Indicate that the background has been erased
    }
    break;
    case WM_USER + 100:
        // Handle the custom message to perform an action
       // For example, initiate the download or any other task.
        DownloadFile(L"https://example.com/sample.txt", L"sample.txt", hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool DownloadFile(const std::wstring& url, const std::wstring& destination, HWND hwnd) {
    HINTERNET hInternet = InternetOpen(L"DieMain", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        // Handle the error (log, display, etc.)
        return false;
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, 0, 0);
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
        std::string jsonString = R"({"url": "Update to your Url for json"})"; // Replace with your actual JSON string

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

bool ExtractZip(const std::wstring& zipPath, HWND hwndRichEdit) {
    // Ensure the file exists
    if (!PathFileExists(zipPath.c_str())) {
        //MessageBox(hwndRichEdit, L"Zip file does not exist.", L"Error", MB_ICONERROR);
        return false;
    }

    // Get the directory of the executable
    wchar_t exePath[MAX_PATH];
    if (GetModuleFileName(nullptr, exePath, MAX_PATH) == 0) {
        //MessageBox(hwndRichEdit, L"Failed to get the directory of the executable.", L"Error", MB_ICONERROR);
        return false;
    }

    // Extract the directory from the executable path
    PathRemoveFileSpec(exePath);
    //MessageBox(hwndRichEdit, exePath, L"Executable Path", MB_OK);

    // Use the executable directory as the extract path
    std::wstring extractPath = exePath;
    //MessageBox(hwndRichEdit, extractPath.c_str(), L"Extract Path", MB_OK);

    // Build the extraction command
    std::wstring extractCommand = L"powershell -command \"Expand-Archive -Path '";
    extractCommand += zipPath;
    extractCommand += L"' -DestinationPath '";
    extractCommand += extractPath;
    extractCommand += L"' -Force\"";
    //MessageBox(hwndRichEdit, extractCommand.c_str(), L"Extraction Command", MB_OK);

    // Execute the extraction command
    SHELLEXECUTEINFO ShExecInfo = {};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = nullptr;
    ShExecInfo.lpVerb = L"open";
    ShExecInfo.lpFile = L"powershell.exe";
    ShExecInfo.lpParameters = extractCommand.c_str();
    ShExecInfo.lpDirectory = nullptr;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = nullptr;

    if (!ShellExecuteEx(&ShExecInfo)) {
        //MessageBox(hwndRichEdit, L"Failed to execute the extraction command.", L"Error", MB_ICONERROR);
        return false;
    }

    // Wait for the PowerShell process to finish
    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
    CloseHandle(ShExecInfo.hProcess);

    bool extractionSucceeded = true; // You need to determine this based on the result of ShellExecuteEx

    

    //MessageBox(hwndRichEdit, L"Extraction process completed.", L"Info", MB_OK);
   
    // Attempt to delete the zip file
    if (!DeleteFile(zipPath.c_str())) {
        // If the file could not be deleted, log the error message
        DWORD errorMessageID = ::GetLastError();
        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

        std::wstring message(messageBuffer, size);
        //MessageBox(hwndRichEdit, message.c_str(), L"Deletion Error", MB_ICONERROR);

        // Free the buffer allocated by FormatMessage
        LocalFree(messageBuffer);
        return false;
    }

    //MessageBox(hwndRichEdit, L"Zip file deleted successfully.", L"Info", MB_OK);
    return true;
}


