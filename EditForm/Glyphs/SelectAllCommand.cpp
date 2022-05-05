// SelectAllCommand.cpp
/*
���ϸ�Ī : SelectAllCommand.cpp
��� : ��ü ���� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
*/
#include "SelectAllCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"

SelectAllCommand::SelectAllCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

SelectAllCommand::~SelectAllCommand() {
}

void SelectAllCommand::Execute() {
	Long noteCurrent;
	this->glyphSubject->note->SelectAll(true);
	noteCurrent = this->glyphSubject->note->Last();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	this->glyphSubject->current->Last();
}