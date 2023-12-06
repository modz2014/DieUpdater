#pragma once
#include "framework.h"
#include "DieMain.h"
#include "Controls.h"

Gdiplus::Bitmap* g_pBitmap = NULL;  // Define here
HWND hProgressBar = NULL;  // Define the variable here
HWND hCheckbox = NULL;
HWND hRichTextBox = NULL;

/**
 * @brief Creates various controls in the main window.
 *
 * This function creates labels, checkboxes, and a progress bar in the main window.
 *
 * @param hwnd The handle to the main window.
 */
void CreateControls(HWND hwnd) {
	// Create a label for indicating downloading update
	HWND hLabel = CreateWindowEx(
		0, L"STATIC", L"Downloading Update...",
		WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
		155, 10, 200, 20, hwnd, (HMENU)IDC_LABEL, GetModuleHandle(NULL), NULL);

	// Create the "Advanced" checkbox next to the label
	HWND hCheckbox = CreateWindowEx(
		0, L"BUTTON", L"Advanced", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		320, 10, 200, 20, hwnd, (HMENU)IDC_ADVANCED_CHECKBOX, GetModuleHandle(NULL), NULL);

	if (IsDarkMode()) {
		// Load the dark mode image
		g_pBitmap = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MY_DARK_IMAGE));
	}
	else {
		// Load the light mode image
		g_pBitmap = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_MY_LIGHT_IMAGE));
	}

	// Check if the image was loaded successfully
	if (!g_pBitmap || g_pBitmap->GetLastStatus() != Gdiplus::Ok) {
		MessageBox(hwnd, L"Failed to load image with GDI+.", L"Error", MB_OK | MB_ICONERROR);
	}

	// Create a progress bar next to the picture box
	HWND hProgressBar = CreateWindowEx(
		0, PROGRESS_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		125, 50, 280, 20, hwnd, (HMENU)IDC_PROGRESS_BAR, GetModuleHandle(NULL), NULL);

	// Set the range and increment of the progress bar
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)10, 0);
	SetProgressBarColor(hProgressBar);

	HWND hPercentageLabel = CreateWindowEx(
		0, L"STATIC", L"0%",
		WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
		205, 80, 50, 20, hwnd, (HMENU)IDC_PERCENTAGE_LABEL, GetModuleHandle(NULL), NULL);
}
/**
 * @brief Creates advanced controls, including a checkbox and a rich text box.
 *
 * This function creates advanced controls like a checkbox and a rich text box.
 *
 * @param hwnd The handle to the main window.
 */
void CreateAdvancedControls(HWND hwnd) {
	// Create the "Advanced" checkbox
	hCheckbox = CreateWindowEx(0, L"BUTTON", L"Advanced", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		10, 50, 100, 20, hwnd, (HMENU)IDC_ADVANCED_CHECKBOX, GetModuleHandle(NULL), NULL);

	// Create the rich textbox initially hidden
	hRichTextBox = CreateWindowEx(0, L"RICHEDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL,
		10, 80, 400, 200, hwnd, (HMENU)IDC_RICHTEXTBOX, GetModuleHandle(NULL), NULL);

	// Set the initial state of the rich textbox
	ToggleAdvancedControls(hwnd, false);
}
/**
 * @brief Toggles the visibility of advanced controls based on the checkbox state.
 *
 * This function shows or hides the rich text box based on the checkbox state.
 *
 * @param hwnd The handle to the main window.
 * @param show If true, advanced controls are shown; otherwise, they are hidden.
 */
void ToggleAdvancedControls(HWND hwnd, bool show) {
	// Show or hide the rich textbox based on the checkbox state
	ShowWindow(hRichTextBox, show ? SW_SHOWNORMAL : SW_HIDE);
}
/**
 * @brief Creates a RichEdit control below an image with specified dimensions.
 *
 * This function creates a RichEdit control below an image with specified dimensions.
 *
 * @param hwnd The handle to the main window.
 */
void CreateRichEditControl(HWND hwnd) {

	HMODULE hRichEditLibrary = LoadLibrary(TEXT("Msftedit.dll"));
	if (hRichEditLibrary == NULL) {
		OutputDebugString(L"Failed to load RichEdit library.\n");
		return;
	}
	// Assume imgWidth and imgHeight are the dimensions of your image
	int imgWidth = 200;  // Replace with the actual width of your image
	int imgHeight = 100; // Replace with the actual height of your image

	// Position and size for the RichEdit control below the image

	int richTextBoxX = 0;              // X-coordinate remains the same as the image
	int richTextBoxY = 25 + imgHeight; // Y-coordinate is the sum of the image's Y-coordinate and height
	int richTextBoxWidth = 400;         // Adjust the width as needed
	int richTextBoxHeight = 100;        // Adjust the height as needed

	// Create the RichEdit control
	hRichTextBox = CreateWindowEx(0, MSFTEDIT_CLASS, L"",
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL,
		richTextBoxX, richTextBoxY, richTextBoxWidth, richTextBoxHeight,
		hwnd, (HMENU)IDC_RICHTEXTBOX, GetModuleHandle(NULL), NULL);
	// Check if the RichEdit control was created successfully
	if (!hRichTextBox) {
		OutputDebugString(L"Failed to create RichEdit control.\n");
		return;
	}
	// Set the font for the RichEdit control (optional)
	SendMessage(hRichTextBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
	// Set dark mode colors
	if (IsDarkMode()) {
		// Set the background color to a darker color
		SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(45, 45, 45));
		// Set the text color to white
		CHARFORMAT2 cf;
		memset(&cf, 0, sizeof(CHARFORMAT2));
		cf.cbSize = sizeof(CHARFORMAT2);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = RGB(255, 255, 255);
		SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
	}
	else {
		// Set the background color to a lighter color
		SendMessage(hRichTextBox, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(255, 255, 255));
		// Set the text color to black
		CHARFORMAT2 cf;
		memset(&cf, 0, sizeof(CHARFORMAT2));
		cf.cbSize = sizeof(CHARFORMAT2);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = RGB(0, 0, 0);
		SendMessage(hRichTextBox, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
	}
	OutputDebugString(L"RichEdit control created.\n");
}
/**
 * @brief Toggles the visibility of the RichEdit control.
 *
 * This function shows or hides the RichEdit control based on the specified flag.
 *
 * @param hwnd The handle to the main window.
 * @param show If true, the RichEdit control is shown; otherwise, it is hidden.
 */
 // Function to show or hide the RichEdit control
void ToggleRichEditControl(HWND hwnd, bool show) {
	if (show) {
		ShowWindow(hRichTextBox, SW_SHOWNORMAL);
		OutputDebugString(L"RichEdit control shown.\n");
	}
	else {
		ShowWindow(hRichTextBox, SW_HIDE);
		OutputDebugString(L"RichEdit control hidden.\n");
	}
}
/**
 * @brief Updates the theme of the RichEdit control based on dark mode.
 *
 * This function updates the background and text colors of the RichEdit control based on dark mode.
 *
 * @param hRichTextBox The handle to the RichEdit control.
 * @param darkMode If true, dark mode is enabled; otherwise, light mode is assumed.
 */
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
/**
 * @brief Adds text to the RichEdit control and scrolls to the end.
 *
 * This function adds text to the RichEdit control and scrolls to the end.
 *
 * @param hRichTextBox The handle to the RichEdit control.
 * @param text The text to be added.
 */
void AddTextAndScroll(HWND hRichTextBox, const wchar_t* text) {
	// Get the current text length
	int textLength = GetWindowTextLength(hRichTextBox);

	// Set the selection to the end of the text
	SendMessage(hRichTextBox, EM_SETSEL, (WPARAM)textLength, (LPARAM)textLength);

	// Replace the selected text with the new text
	SendMessage(hRichTextBox, EM_REPLACESEL, FALSE, (LPARAM)text);

	// Scroll to the end of the RichTextBox
	SendMessage(hRichTextBox, EM_SCROLLCARET, 0, 0);
}
/**
 * @brief Updates the text in the RichEdit control.
 *
 * This function updates the text in the RichEdit control.
 *
 * @param text The new text to be set.
 */
void UpdateRichEdit(const wchar_t* text) {
	// Get the current text in the RichEdit control
	int textLength = GetWindowTextLength(hRichTextBox);
	SendMessage(hRichTextBox, EM_SETSEL, (WPARAM)textLength, (LPARAM)textLength);

	// Replace the selected text with the new text
	SendMessage(hRichTextBox, EM_REPLACESEL, 0, (LPARAM)text);
}

/**
 * @brief Updates the progress bar during file download.
 *
 * This function updates the progress bar position and percentage label during file download.
 *
 * @param hwnd The handle to the main window.
 * @param progress The progress value (0-100) to be set.
 */
 // Function to update the progress bar during file download
void UpdateProgressBar(HWND hwnd, int progress) {
	HWND hProgressBar = GetDlgItem(hwnd, IDC_PROGRESS_BAR);
	HWND hPercentageLabel = GetDlgItem(hwnd, IDC_PERCENTAGE_LABEL);
	// Set the progress bar position
	SendMessage(hProgressBar, PBM_SETPOS, progress, 0);
	// Update the percentage label
	std::wstring percentageText = std::to_wstring(progress) + L"%";
	SetWindowText(hPercentageLabel, percentageText.c_str());
	SetProgressBarColor(hProgressBar);
}
/**
 * @brief Sets the color of the progress bar based on the current mode.
 *
 * This function sets the color of the progress bar based on the current mode (dark or light).
 *
 * @param hProgressBar The handle to the progress bar.
 */
void SetProgressBarColor(HWND hProgressBar) {
	if (IsDarkMode()) {
		// Set dark mode colors for the progress bar
		SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, RGB(0, 136, 239, 255));  // Set the bar color
		SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, RGB(100, 100, 100));    // Set the background color
	}
	else {
		// Set light mode colors for the progress bar
		SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, RGB(0, 136, 239, 255));  // Set the bar color
		SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, RGB(255, 255, 255)); // Set the background color
	}
}
/**
 * @brief Dialog procedure for a custom dialog.
 *
 * This function is the dialog procedure for a custom dialog displaying release notes.
 *
 * @param hDlg The handle to the dialog.
 * @param message The message identifier.
 * @param wParam Additional message-specific information.
 * @param lParam Additional message-specific information.
 * @return The result of the message processing.
 */
INT_PTR CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static HBRUSH hbrBkgnd = NULL; // Handle to the background brush
	switch (message) {
	case WM_INITDIALOG:
		// Set the release notes text in the dialog
		SetDlgItemTextW(hDlg, IDC_EDIT_MESSAGE, reinterpret_cast<LPCWSTR>(lParam));

		// Create a solid brush for the dialog background if dark mode is enabled
		if (IsDarkMode()) {
			hbrBkgnd = CreateSolidBrush(RGB(30, 30, 30, 255)); // Dark background color
		}
		return (INT_PTR)TRUE;

	case WM_CTLCOLORDLG:
		if (IsDarkMode()) {
			return (INT_PTR)hbrBkgnd; // Use the dark background brush
		}
		break;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT: {
		// Introduce a new scope for the variable hdcEdit
		HDC hdcEdit = (HDC)wParam;
		if (IsDarkMode()) {
			SetBkColor(hdcEdit, RGB(30, 30, 30, 255)); // Dark background color
			SetTextColor(hdcEdit, RGB(255, 255, 255)); // White text color
			return (INT_PTR)hbrBkgnd; // Use the dark background brush
		}
		else {
			SetBkColor(hdcEdit, RGB(255, 255, 255)); // Light background color
			SetTextColor(hdcEdit, RGB(0, 0, 0)); // Black text color
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		}
	} break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_DESTROY:
		// Clean up the created brush to avoid memory leaks
		if (hbrBkgnd) {
			DeleteObject(hbrBkgnd);
			hbrBkgnd = NULL;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
