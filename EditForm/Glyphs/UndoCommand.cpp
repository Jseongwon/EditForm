// UndoCommand.cpp
/*
파일명칭 : UndoCommand.cpp
기능 : 되돌리기 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
*/
#include "UndoCommand.h"
#include "GlyphSubject.h"
#include "CommandHistory.h"

UndoCommand::UndoCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

UndoCommand::~UndoCommand() {
}

void UndoCommand::Execute() {
	this->glyphSubject->commandHistory->Undo();
}