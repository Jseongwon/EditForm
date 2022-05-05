// CopyCommand.cpp
/*
���ϸ�Ī : CopyCommand.cpp
��� : �����Ѵ� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2020.07.03
*/
#include "CopyCommand.h"
#include "GlyphSubject.h"
#include "Clipboard.h"
#include "Glyph.h"

CopyCommand::CopyCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CopyCommand::~CopyCommand() {
}

void CopyCommand::Execute() {
	CString copyString;

	copyString = this->glyphSubject->note->GetSelectString().c_str();
	this->glyphSubject->clipboard->Write(copyString);
}