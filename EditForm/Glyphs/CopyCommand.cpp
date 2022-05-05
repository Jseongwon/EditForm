// CopyCommand.cpp
/*
파일명칭 : CopyCommand.cpp
기능 : 복사한다 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2020.07.03
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