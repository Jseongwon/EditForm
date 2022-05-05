// FindNextCommand.cpp
/*
파일명칭 : FindNextCommand.cpp
기능 : 다음 찾기 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.20
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

	// 1. 선택 범위를 구한다.
	this->glyphSubject->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);

	// 2. 선택된 범위의 시작 위치가 현재 위치와 같으면
	if (startRowCurrent == rowCurrent && startNoteCurrent == noteCurrent) {
		// 2.1. 현재 위치를 저장한다.
		prevRowCurrent = rowCurrent;
		prevNoteCurrent = noteCurrent;
		// 2.2. 범위의 시작 위치로 이동한다.
		this->glyphSubject->note->Move(endNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(endNoteCurrent);
		this->glyphSubject->current->Move(endRowCurrent);
	}

	// 3. 찾는 내용의 글자 개수를 구한다.
	findStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)findString);

	// 4. 메모지에서 다음으로 찾는다.
	this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);

	// 5. 찾지 못하고 주위의 배치 여부가 참이면
	if (this->glyphSubject->onIsFound != TRUE && findString != "" && onIsWrapAround == 0) {
		// 5.1. 처음 위치의 첫 번째 칸으로 이동한다.
		noteCurrent = this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->First();
		// 5.2. 메모지에서 다음으로 찾는다.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);
	}
	// 6. 찾지 못하면 원래 위치로 돌아간다.
	if (this->glyphSubject->onIsFound != TRUE && onIsWrapAround == 0) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}