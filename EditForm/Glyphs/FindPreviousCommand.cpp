// FindPreviousCommand.cpp
/*
파일명칭 : FindPreviousCommand.cpp
기능 : 이전 찾기 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.18
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
	
	// 1. 선택 범위를 구한다.
	this->glyphSubject->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);

	// 2. 선택된 범위의 마지막 위치가 현재 위치와 같으면
	if (endRowCurrent == rowCurrent && endNoteCurrent == noteCurrent) {
		// 2.1. 현재 위치를 저장한다.
		prevRowCurrent = rowCurrent;
		prevNoteCurrent = noteCurrent;
		// 2.2. 범위의 시작 위치로 이동한다.
		this->glyphSubject->note->Move(startNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(startNoteCurrent);
		this->glyphSubject->current->Move(startRowCurrent);
	}

	// 3. 찾는 내용의 글자 개수를 구한다.
	findStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)findString);

	// 4. 메모지에서 찾는다.
	this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindPrevious((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);

	// 5. 찾지 못하고 주위의 배치 여부가 참이면
	if (this->glyphSubject->onIsFound != TRUE && findString != "" && onIsWrapAround == 0) {
		// 5.1. 마지막 줄의 마지막 칸으로 이동한다.
		noteCurrent = this->glyphSubject->note->Last();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Last();
		// 5.2. 다시 한번 찾는다.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindPrevious((LPCTSTR)findString, findStringLen, (bool)!onIsMatchCase);
	}

	// 6. 찾지 못하면 원래 위치로 돌아간다.
	if (this->glyphSubject->onIsFound != TRUE) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}