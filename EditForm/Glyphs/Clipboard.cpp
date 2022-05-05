// Clipboard.cpp
/*
파일명칭 : Clipboard.cpp
기능 : 필기판 클래스를 만든다.
작성자 : 정성원
작성일자 : 2020.07.03
*/
#include "Clipboard.h"
#include <afxole.h>
#include "GlyphSubject.h"
#include "Glyph.h"
#pragma warning(disable:4996)

Clipboard::Clipboard(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

Clipboard::~Clipboard() {
}

void Clipboard::Write(CString copyString) {
	HGLOBAL hGlob;
	PSTR pGlobal;

	hGlob = GlobalAlloc(GHND | GMEM_SHARE, copyString.GetLength() + 1);
	pGlobal = (PSTR)GlobalLock(hGlob);
	_tcscpy(pGlobal, (LPCSTR)copyString);
	GlobalUnlock(hGlob);
	
	this->glyphSubject->OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hGlob);
	CloseClipboard();
}

CString Clipboard::Read() {
	CString clipboardString;
	HGLOBAL hGlob;
	BOOL isOpen = this->glyphSubject->OpenClipboard();
	if (isOpen == TRUE) {
		hGlob = GetClipboardData(CF_TEXT);
		clipboardString = CString((PSTR)GlobalLock(hGlob));
		GlobalUnlock(hGlob);
	}
	CloseClipboard();

	return clipboardString;
}