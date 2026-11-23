#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// CLIPBOARD UTILITIES - для роботи з Clipboard Windows
// ============================================================================

// Запис ANSI-тексту у Clipboard
inline int PutTextToClipboard(HWND hWnd, const char *src)
{
	HGLOBAL hglbCopy;
	BYTE *pTmp;
	size_t len;

	if (src == NULL) return 0;
	if (src[0] == 0) return 0;

	len = strlen(src);
	hglbCopy = GlobalAlloc(GHND, len + 1);
	if (hglbCopy == NULL) return 0;

	pTmp = (BYTE *)GlobalLock(hglbCopy);
	memcpy(pTmp, src, len + 1);
	GlobalUnlock(hglbCopy);

	if (!OpenClipboard(hWnd))
	{
		GlobalFree(hglbCopy);
		return 0;
	}

	EmptyClipboard();
	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();
	return 1;
}

// Читання ANSI-тексту з Clipboard
inline long GetTextFromClipboard(HWND hWnd, char *dest, long maxsize)
{
	HGLOBAL hglb;
	LPTSTR lptstr;
	long size, res;

	res = 0;
	if (!IsClipboardFormatAvailable(CF_TEXT)) return 0;
	if (!OpenClipboard(hWnd)) return 0;

	hglb = GetClipboardData(CF_TEXT);
	if (hglb != NULL)
	{
		lptstr = (char *)GlobalLock(hglb);
		if (lptstr != NULL)
		{
			size = (long)strlen(lptstr);
			if (size > maxsize - 1)
			{
				size = maxsize - 1;
			}
			strncpy_s(dest, maxsize, lptstr, size);
			dest[size] = 0;
			res = size;
			GlobalUnlock(hglb);
		}
	}
	CloseClipboard();
	return res;
}

// Надсилання даних через WM_COPYDATA
inline int SendCopyData(HWND hWndDest, HWND hWndSrc, void *lp, long cb)
{
	COPYDATASTRUCT cds;
	if (hWndDest == NULL) return 0;

	cds.dwData = 1;
	cds.cbData = cb;
	cds.lpData = lp;
	return SendMessage(hWndDest, WM_COPYDATA, (WPARAM)hWndSrc, (LPARAM)&cds);
}

// Пошук вікна за класом
inline HWND FindWindowByClass(const char *className)
{
	return FindWindowA(className, NULL);
}

// Запуск програми
inline int StartProgram(const char *exePath, const char *cmdLine)
{
	char fullCmd[512];

	if (cmdLine)
		sprintf_s(fullCmd, sizeof(fullCmd), "%s %s", exePath, cmdLine);
	else
		strcpy_s(fullCmd, sizeof(fullCmd), exePath);

	return WinExec(fullCmd, SW_SHOW);
}
