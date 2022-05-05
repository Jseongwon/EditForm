// UndoCommand.cpp
/*
���ϸ�Ī : UndoCommand.cpp
��� : �ǵ����� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
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