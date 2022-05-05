// WrapLineCommand.cpp
/*
���ϸ�Ī : WrapLineCommand.cpp
��� : �� �� �ڵ����� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.08
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
	bool onIsRow = false; // ���� ���� �翬�� ����(onIsRow�� true) ���̰� �ڵ������� �Ϸ� �� �� �������� �������̴� �� �������� �ݺ�

	bool onIsGreaterThanScreen;
	// 1. �߸��� ���� ������ ���Ѵ�.(2020.01.22)
	onIsGreaterThanScreen = this->glyphSubject->glyphExtent->GetOnIsGreaterThanScreen();

	// 2. �߸��� ���� ������ ȭ�� ũ�⺸�� ������ ���� ��ũ�ѹٰ� �ִ� ������ �����Ѵ�.(2020.01.22)
	if (onIsGreaterThanScreen == true) {
		this->glyphSubject->onIsVerticalScrollBar = TRUE;
	}

	this->glyphSubject->GetClientRect(&rect);
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}

	// 1. ���� ���� �ڵ������Ѵ�.
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

	// 2. ������ ���� ��ġ�� ���� ���� ��ġ�� ����Ѵ�.
	row = this->glyphSubject->note->GetAt(noteCurrent);
	i = noteCurrent;
	while (i < this->glyphSubject->note->GetLength() - 1 && rowCurrent > row->GetLength()) {
		noteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphSubject->note->GetAt(i + 1);
		i++;
	}

	// 3. ���� ��ġ�� �̵��Ѵ�.
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	this->glyphSubject->current->Move(rowCurrent);
}