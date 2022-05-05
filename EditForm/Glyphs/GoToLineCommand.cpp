// GoToLineCommand.cpp
/*
파일명칭 : GoToLineCommand.cpp
기능 : 줄 이동 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
*/
#include "GoToLineCommand.h"
#include "GoToLineDialog.h"
#include "GlyphSubject.h"

GoToLineCommand::GoToLineCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

GoToLineCommand::~GoToLineCommand() {
}

void GoToLineCommand::Execute() {
	GoToLineDialog dlg(glyphSubject);
	dlg.DoModal();
}