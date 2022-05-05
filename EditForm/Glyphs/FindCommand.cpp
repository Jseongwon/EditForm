// FindCommand.cpp
/*
���ϸ�Ī : FindCommand.cpp
��� : ã�� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.20
*/
#include "FindCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "FindReplaceDialog.h"
//#include <Findtext.dlg>

FindCommand::FindCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

FindCommand::~FindCommand() {
}

void FindCommand::Execute() {
	CString findString;

	this->glyphSubject->onIsOpeningFindReplace = TRUE;
	if (this->glyphSubject->pFindReplaceDialog != 0) {
		this->glyphSubject->pFindReplaceDialog->DestroyWindow();
	}

	findString = this->glyphSubject->note->GetSelectString().c_str();
	if (findString.Compare("") == 0) {
		findString = AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	}

	this->glyphSubject->pFindReplaceDialog = new FindReplaceDialog;
	this->glyphSubject->pFindReplaceDialog->Create(TRUE, findString, NULL, 0, this->glyphSubject);
	this->glyphSubject->pFindReplaceDialog->ShowWindow(SW_SHOW);
}