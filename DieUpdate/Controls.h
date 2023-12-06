// Controls.h
#pragma once
#include <windows.h>
#include <richedit.h>
#include <CommCtrl.h>
#include "DieMain.h"
#include <gdiplus.h>


extern HWND hCheckbox;
extern HWND hRichTextBox;
extern Gdiplus::Bitmap* g_pBitmap; // Declare as extern

// CreateControls: Creates various controls in the main window.
void CreateControls(HWND hwnd); 

// CreateAdvancedControls: Creates advanced controls, including a checkbox and a rich text box.
void CreateAdvancedControls(HWND hwnd);

// ToggleAdvancedControls: Toggles the visibility of advanced controls based on the checkbox state.
void ToggleAdvancedControls(HWND hwnd, bool show);

// CreateRichEditControl: Creates a RichEdit control below an image with specified dimensions.
void CreateRichEditControl(HWND hwnd);

// ToggleRichEditControl: Toggles the visibility of the RichEdit control.
void ToggleRichEditControl(HWND hwnd, bool show);

// SetProgressBarColor: Sets the color of the progress bar based on the current mode.
void SetProgressBarColor(HWND hProgressBar);

// UpdateProgressBar: Updates the progress bar in the specified window.
void UpdateProgressBar(HWND hwnd, int progress);

// UpdateRichEdit: Updates the text in the RichEdit control.
void UpdateRichEdit(const wchar_t* text);

// AddTextAndScroll: Adds text to the RichEdit control and scrolls to the end.
void AddTextAndScroll(HWND hRichTextBox, const wchar_t* text);

// CustomDialogProc: Dialog procedure for a custom dialog.
INT_PTR CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// UpdateRichTextBoxTheme: Updates the theme of the RichEdit control based on dark mode.
void UpdateRichTextBoxTheme(HWND hRichTextBox, bool darkMode);