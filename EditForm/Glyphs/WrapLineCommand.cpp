// WrapLineCommand.cpp
/*
파일명칭 : WrapLineCommand.cpp
기능 : 한 줄 자동개행 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.08
*/
#include "WrapLineCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "GlyphExtent.h"

WrapLineCommand::WrapLineCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

WrapLineCommand::~WrapLineCommand() {
}

void WrapLineCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* row;
	Glyph* dummyRow;

	RECT rect;

	Long current = 0;
	Long noteIndex = 0;
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long i = noteCurrent;

	bool onIsFirst = true;
	bool onIsRow = false; // 현재 줄은 당연히 실제(onIsRow이 true) 줄이고 자동개행이 완료 된 후 다음줄은 실제줄이니 그 전까지만 반복

	bool onIsGreaterThanScreen;
	// 1. 잘리는 줄의 개수를 구한다.(2020.01.22)
	onIsGreaterThanScreen = this->glyphSubject->glyphExtent->GetOnIsGreaterThanScreen();

	// 2. 잘리는 줄의 개수가 화면 크기보다 많으면 수직 스크롤바가 있는 것으로 지정한다.(2020.01.22)
	if (onIsGreaterThanScreen == true) {
		this->glyphSubject->onIsVerticalScrollBar = TRUE;
	}

	this->glyphSubject->GetClientRect(&rect);
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}

	// 1. 현재 줄을 자동개행한다.
	while (i < this->glyphSubject->note->GetLength() && onIsRow != true) {
		row = this->glyphSubject->note->GetAt(i);
		if (onIsFirst != true) {
			onIsRow = row->GetOnIsRow();
		}
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
		onIsFirst = false;
		i++;
	}

	// 2. 종이의 현재 위치와 줄의 현재 위치를 계산한다.
	row = this->glyphSubject->note->GetAt(noteCurrent);
	i = noteCurrent;
	while (i < this->glyphSubject->note->GetLength() - 1 && rowCurrent > row->GetLength()) {
		noteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphSubject->note->GetAt(i + 1);
		i++;
	}

	// 3. 계산된 위치로 이동한다.
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	this->glyphSubject->current->Move(rowCurrent);
}