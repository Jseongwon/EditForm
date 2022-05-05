// GoToLineCommand.cpp
/*
���ϸ�Ī : GoToLineCommand.cpp
��� : �� �̵� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
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