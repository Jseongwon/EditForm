// GlyphPtr.cpp
/*
파일명칭 : GlyphPtr.cpp
기    능 : 프록시 패턴을 응용한 글리프 포인터 클래스를 만든다.
작성일자 : 2022.01.13
*/
#include "GlyphPtr.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include <afxwin.h>

GlyphPtr::GlyphPtr() {
	GlyphFactory glyphFactory;

	HANDLE hTempFile = INVALID_HANDLE_VALUE;

	DWORD dwRetVal = 0;
	UINT uRetVal = 0;

	TCHAR tempFileName[MAX_PATH];
	TCHAR tempPathName[MAX_PATH];

	// 1. 노트를 만든다.
	this->note = glyphFactory.Create((char*)"\0");

	// 2. 임시파일을 생성한다.
	// =====================================================================
	// 1. 임시파일 경로를 읽는다.
	dwRetVal = GetTempPath(MAX_PATH, tempPathName);

	// 2. 임시파일명을 구한다.
	uRetVal = GetTempFileName(tempPathName, "TXT", 0/*0이면 파일명이 고유한지 검사, 0이 아니면 검사 X*/, tempFileName);
	this->tempFileName = tempFileName;

	// 3. 임시파일명으로 임시파일을 만든다.
	hTempFile = CreateFile((LPTSTR)this->tempFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// 4. 종료한다.
	CloseHandle(hTempFile);
}

GlyphPtr::GlyphPtr(const GlyphPtr& source)
	: pathFileName(source.pathFileName), tempFileName(source.tempFileName) {
	this->note = source.note->Clone();
}

GlyphPtr::~GlyphPtr() {
	// 1. 임시파일을 없앤다.
	DeleteFile((LPTSTR)this->tempFileName.c_str());

	// 2. 노트를 없앤다.
	if (this->note != 0) {
		delete this->note;
	}
}

GlyphPtr& GlyphPtr::operator=(const GlyphPtr& source) {
	if (this->note != 0) {
		delete this->note;
	}
	this->note = source.note->Clone();
	this->pathFileName = source.pathFileName;
	this->tempFileName = source.tempFileName;

	return* this;
}

Glyph* GlyphPtr::operator->() {
	return this->note;
}

bool GlyphPtr::Open(string pathFileName) {
	this->pathFileName = pathFileName;

	// 1. 경로의 파일을 복사해 임시파일을 만든다.
	CopyFile(this->pathFileName.c_str(), this->tempFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::Open(string pathName, string fileName) {
	this->pathFileName = pathName + "\\" + fileName;

	// 1. 경로의 파일을 복사해 임시파일을 만든다.
	CopyFile(this->pathFileName.c_str(), this->tempFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::Save() {
	// 1. 임시파일을 복사해 경로의 파일에 덮어쓴다.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::SaveAs(string pathFileName) {
	this->pathFileName = pathFileName;

	// 1. 임시파일을 복사해 입력받은 경로의 파일에서 만든다.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), TRUE);

	return true;
}

bool GlyphPtr::SaveAs(string pathName, string fileName) {
	this->pathFileName = pathName + "\\" + fileName;

	// 1. 임시파일을 복사해 입력받은 경로의 파일에서 만든다.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), TRUE);

	return true;
}


bool GlyphPtr::Add(size_t index, char(*text)) {
	FILE* file;

	// 1. 임시 파일을 연다
	//file = fopen((char*)this->tempFileName.c_str(), "wt");
	//if (file != 0) {
	//	// 2. 위치로 이동한다.
	//	fseek(file, this->offSet, SEEK_CUR);
	//}
	return true;
}

bool GlyphPtr::Remove(size_t index, size_t size) {
	return true;
}