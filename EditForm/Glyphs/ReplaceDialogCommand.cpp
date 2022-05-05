// ReplaceDialogCommand.cpp
/*
파일명칭 : ReplaceDialogCommand.cpp
기능 : 바꾸기 대화상자 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.22
*/
#include "ReplaceDialogCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "FindReplaceDialog.h"
//#include <Findtext.dlg>

ReplaceDialogCommand::ReplaceDialogCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

ReplaceDialogCommand::~ReplaceDialogCommand() {
}

void ReplaceDialogCommand::Execute() {
	CString csFindString;
	CString csReplaceString;

	this->glyphSubject->onIsOpeningFindReplace = TRUE;

	if (this->glyphSubject->pFindReplaceDialog != 0) {
		this->glyphSubject->pFindReplaceDialog->DestroyWindow();
	}

	csFindString = AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	csReplaceString = AfxGetApp()->GetProfileString("EditSection", "ReplaceString", "");

	this->glyphSubject->pFindReplaceDialog = new FindReplaceDialog;
	this->glyphSubject->pFindReplaceDialog->Create(FALSE, csFindString, csReplaceString, 0, this->glyphSubject);
	this->glyphSubject->pFindReplaceDialog->ShowWindow(SW_SHOW);
}