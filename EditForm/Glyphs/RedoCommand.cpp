// RedoCommand.cpp
/*
���ϸ�Ī : RedoCommand.cpp
��� : �ǵ����� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
*/
#include "RedoCommand.h"
#include "GlyphSubject.h"
#include "CommandHistory.h"

RedoCommand::RedoCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

RedoCommand::~RedoCommand() {
}

void RedoCommand::Execute() {
	this->glyphSubject->commandHistory->Redo();
}