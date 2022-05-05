// FindNextCommand.cpp
/*
���ϸ�Ī : FindNextCommand.cpp
��� : ���� ã�� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.20
*/
#include "FindNextCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include <afxdlgs.h>

FindNextCommand::FindNextCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

FindNextCommand::~FindNextCommand() {
}

void FindNextCommand::Execute() {
	CString findString = AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	LONG onIsMatchCase = AfxGetApp()->GetProfileInt("EditSection", "MatchCase", -1);
	LONG onIsWrapAround = AfxGetApp()->GetProfileInt("EditSection", "WrapAround", -1);

	LONG findStringLen;
	LONG startRowCurrent;
	LONG startNoteCurrent;
	LONG endRowCurrent;
	LONG endNoteCurrent;
	LONG prevRowCurrent = 0;
	LONG prevNoteCurrent = 0;
	LONG rowCurrent = this->glyphSubject->current->GetCurrent();
	LONG noteCurrent = this->glyphSubject->note->GetCurrent();

	// 1. ���� ������ ���Ѵ�.
	this->glyphSubject->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);

	// 2. ���õ� ������ ���� ��ġ�� ���� ��ġ�� ������
	if (startRowCurrent == rowCurrent && startNoteCurrent == noteCurrent) {
		// 2.1. ���� ��ġ�� �����Ѵ�.
		prevRowCurrent = rowCurrent;
		prevNoteCurrent = noteCurrent;
		// 2.2. ������ ���� ��ġ�� �̵��Ѵ�.
		this->glyphSubject->note->Move(endNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(endNoteCurrent);
		this->glyphSubject->current->Move(endRowCurrent);
	}

	// 3. ã�� ������ ���� ������ ���Ѵ�.
	findStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)findString);

	// 4. �޸������� �������� ã�´�.
	this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);

	// 5. ã�� ���ϰ� ������ ��ġ ���ΰ� ���̸�
	if (this->glyphSubject->onIsFound != TRUE && findString != "" && onIsWrapAround == 0) {
		// 5.1. ó�� ��ġ�� ù ��° ĭ���� �̵��Ѵ�.
		noteCurrent = this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->First();
		// 5.2. �޸������� �������� ã�´�.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);
	}
	// 6. ã�� ���ϸ� ���� ��ġ�� ���ư���.
	if (this->glyphSubject->onIsFound != TRUE && onIsWrapAround == 0) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}