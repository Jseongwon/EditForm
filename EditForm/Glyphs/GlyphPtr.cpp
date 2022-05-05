// GlyphPtr.cpp
/*
���ϸ�Ī : GlyphPtr.cpp
��    �� : ���Ͻ� ������ ������ �۸��� ������ Ŭ������ �����.
�ۼ����� : 2022.01.13
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

	// 1. ��Ʈ�� �����.
	this->note = glyphFactory.Create((char*)"\0");

	// 2. �ӽ������� �����Ѵ�.
	// =====================================================================
	// 1. �ӽ����� ��θ� �д´�.
	dwRetVal = GetTempPath(MAX_PATH, tempPathName);

	// 2. �ӽ����ϸ��� ���Ѵ�.
	uRetVal = GetTempFileName(tempPathName, "TXT", 0/*0�̸� ���ϸ��� �������� �˻�, 0�� �ƴϸ� �˻� X*/, tempFileName);
	this->tempFileName = tempFileName;

	// 3. �ӽ����ϸ����� �ӽ������� �����.
	hTempFile = CreateFile((LPTSTR)this->tempFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// 4. �����Ѵ�.
	CloseHandle(hTempFile);
}

GlyphPtr::GlyphPtr(const GlyphPtr& source)
	: pathFileName(source.pathFileName), tempFileName(source.tempFileName) {
	this->note = source.note->Clone();
}

GlyphPtr::~GlyphPtr() {
	// 1. �ӽ������� ���ش�.
	DeleteFile((LPTSTR)this->tempFileName.c_str());

	// 2. ��Ʈ�� ���ش�.
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

	// 1. ����� ������ ������ �ӽ������� �����.
	CopyFile(this->pathFileName.c_str(), this->tempFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::Open(string pathName, string fileName) {
	this->pathFileName = pathName + "\\" + fileName;

	// 1. ����� ������ ������ �ӽ������� �����.
	CopyFile(this->pathFileName.c_str(), this->tempFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::Save() {
	// 1. �ӽ������� ������ ����� ���Ͽ� �����.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), FALSE);

	return true;
}

bool GlyphPtr::SaveAs(string pathFileName) {
	this->pathFileName = pathFileName;

	// 1. �ӽ������� ������ �Է¹��� ����� ���Ͽ��� �����.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), TRUE);

	return true;
}

bool GlyphPtr::SaveAs(string pathName, string fileName) {
	this->pathFileName = pathName + "\\" + fileName;

	// 1. �ӽ������� ������ �Է¹��� ����� ���Ͽ��� �����.
	CopyFile(this->tempFileName.c_str(), this->pathFileName.c_str(), TRUE);

	return true;
}


bool GlyphPtr::Add(size_t index, char(*text)) {
	FILE* file;

	// 1. �ӽ� ������ ����
	//file = fopen((char*)this->tempFileName.c_str(), "wt");
	//if (file != 0) {
	//	// 2. ��ġ�� �̵��Ѵ�.
	//	fseek(file, this->offSet, SEEK_CUR);
	//}
	return true;
}

bool GlyphPtr::Remove(size_t index, size_t size) {
	return true;
}