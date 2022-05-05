// WrapCommand.cpp
/*
파일명칭 : WrapCommand.cpp
기능 : 자동개행 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2020.10.14
*/
#include "WrapCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "GlyphExtent.h"

WrapCommand::WrapCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

WrapCommand::~WrapCommand() {
}

void WrapCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* row;
	Glyph* dummyRow;

	RECT rect;

	Long splitCount;
	Long height = this->glyphSubject->glyphExtent->GetHeight();
	Long current = 0;
	Long noteIndex = 0;
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long i = 0;

	bool onIsGreaterThanScreen;
	// 1. 잘리는 줄의 개수를 구한다.(2020.01.22)
	onIsGreaterThanScreen = this->glyphSubject->glyphExtent->GetOnIsGreaterThanScreen();// 한글자가 화면의 크기를 넘어설때 문제가 발생한다.

	// 2. 잘리는 줄의 개수가 화면 크기보다 많으면 수직 스크롤바가 있는 것으로 지정한다.(2020.01.22)
	if (onIsGreaterThanScreen == true) {
		this->glyphSubject->onIsVerticalScrollBar = TRUE;
	}
	else {
		this->glyphSubject->onIsVerticalScrollBar = FALSE;
	}

	this->glyphSubject->GetClientRect(&rect);
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}

	// 1. 이전 줄들이 자동개행 될 경우 미리 현재 줄을 구한다.
	while (i < noteCurrent) {
		row = this->glyphSubject->note->GetAt(i);
		splitCount = this->glyphSubject->glyphExtent->GetSplitCount(row);
		if (splitCount > 0) {
			noteIndex += splitCount;
		}
		i++;
	}
	if (noteIndex > 0) {
		noteCurrent += noteIndex;
	}

	// 2. 자동개행한다.
	i = 0;
	while (i < this->glyphSubject->note->GetLength()) {
		row = this->glyphSubject->note->GetAt(i);
		current = this->glyphSubject->glyphExtent->GetSplitCurrent(row, rect.right);
		if (current > 0) {
			dummyRow = glyphFactory.Create((char*)"\n", false);
			row->Split(dummyRow, current);
			this->glyphSubject->note->Add(i + 1, dummyRow);
		}
		else if (current == 0) {
			dummyRow = glyphFactory.Create((char*)"\n", false);
			row->Split(dummyRow, 1);
			this->glyphSubject->note->Add(i + 1, dummyRow);
		}
		i++;
	}

	// 3. 종이의 현재 위치와 줄의 현재 위치를 계산한다.
	row = this->glyphSubject->note->GetAt(noteCurrent);
	i = noteCurrent;
	while (i < this->glyphSubject->note->GetLength() - 1 && rowCurrent > row->GetLength()) {
		noteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphSubject->note->GetAt(i + 1);
		i++;
	}

	// 4. 계산된 위치로 이동한다.
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	this->glyphSubject->current->Move(rowCurrent);
}