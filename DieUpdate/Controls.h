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

void CreateControls(HWND hwnd);  // Function to create all controls
void CreateAdvancedControls(HWND hwnd);
void ToggleAdvancedControls(HWND hwnd, bool show);
void CreateRichEditControl(HWND hwnd);
void ToggleRichEditControl(HWND hwnd, bool show);
void SetProgressBarColor(HWND hProgressBar);
void UpdateProgressBar(HWND hwnd, int progress);
void UpdateRichEdit(const wchar_t* text);
void AddTextAndScroll(HWND hRichTextBox, const wchar_t* text);
INT_PTR CALLBACK CustomDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);