// WrapCommand.cpp
/*
���ϸ�Ī : WrapCommand.cpp
��� : �ڵ����� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2020.10.14
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
	// 1. �߸��� ���� ������ ���Ѵ�.(2020.01.22)
	onIsGreaterThanScreen = this->glyphSubject->glyphExtent->GetOnIsGreaterThanScreen();// �ѱ��ڰ� ȭ���� ũ�⸦ �Ѿ�� ������ �߻��Ѵ�.

	// 2. �߸��� ���� ������ ȭ�� ũ�⺸�� ������ ���� ��ũ�ѹٰ� �ִ� ������ �����Ѵ�.(2020.01.22)
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

	// 1. ���� �ٵ��� �ڵ����� �� ��� �̸� ���� ���� ���Ѵ�.
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

	// 2. �ڵ������Ѵ�.
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

	// 3. ������ ���� ��ġ�� ���� ���� ��ġ�� ����Ѵ�.
	row = this->glyphSubject->note->GetAt(noteCurrent);
	i = noteCurrent;
	while (i < this->glyphSubject->note->GetLength() - 1 && rowCurrent > row->GetLength()) {
		noteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphSubject->note->GetAt(i + 1);
		i++;
	}

	// 4. ���� ��ġ�� �̵��Ѵ�.
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	this->glyphSubject->current->Move(rowCurrent);
}