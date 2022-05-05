// FindPreviousCommand.cpp
/*
���ϸ�Ī : FindPreviousCommand.cpp
��� : ���� ã�� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.18
*/
#include "FindPreviousCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include <afxdlgs.h>

FindPreviousCommand::FindPreviousCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

FindPreviousCommand::~FindPreviousCommand() {
}

void FindPreviousCommand::Execute() {
	CString findString = AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	LONG onIsMatchCase = AfxGetApp()->GetProfileInt("EditSection", "MatchCase", -1);
	LONG onIsWrapAround = AfxGetApp()->GetProfileInt("EditSection", "WrapAround", -1);

	Long findStringLen;
	Long startRowCurrent;
	Long startNoteCurrent;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long prevRowCurrent = 0;
	Long prevNoteCurrent = 0;
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	
	// 1. ���� ������ ���Ѵ�.
	this->glyphSubject->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);

	// 2. ���õ� ������ ������ ��ġ�� ���� ��ġ�� ������
	if (endRowCurrent == rowCurrent && endNoteCurrent == noteCurrent) {
		// 2.1. ���� ��ġ�� �����Ѵ�.
		prevRowCurrent = rowCurrent;
		prevNoteCurrent = noteCurrent;
		// 2.2. ������ ���� ��ġ�� �̵��Ѵ�.
		this->glyphSubject->note->Move(startNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(startNoteCurrent);
		this->glyphSubject->current->Move(startRowCurrent);
	}

	// 3. ã�� ������ ���� ������ ���Ѵ�.
	findStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)findString);

	// 4. �޸������� ã�´�.
	this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindPrevious((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);

	// 5. ã�� ���ϰ� ������ ��ġ ���ΰ� ���̸�
	if (this->glyphSubject->onIsFound != TRUE && findString != "" && onIsWrapAround == 0) {
		// 5.1. ������ ���� ������ ĭ���� �̵��Ѵ�.
		noteCurrent = this->glyphSubject->note->Last();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Last();
		// 5.2. �ٽ� �ѹ� ã�´�.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindPrevious((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);
	}

	// 6. ã�� ���ϸ� ���� ��ġ�� ���ư���.
	if (this->glyphSubject->onIsFound != TRUE) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}