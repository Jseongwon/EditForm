// SelectAllCommand.cpp
/*
파일명칭 : SelectAllCommand.cpp
기능 : 전체 선택 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
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