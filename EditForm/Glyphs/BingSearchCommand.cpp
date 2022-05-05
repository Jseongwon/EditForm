// BingSearchCommand.cpp
/*
���ϸ�Ī : BingSearchCommand.cpp
��� : �������� �˻� ���� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.02.02
*/
#include "BingSearchCommand.h"
#include "GlyphSubject.h"
#include "Glyph.h"

BingSearchCommand::BingSearchCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

BingSearchCommand::~BingSearchCommand() {
}

void BingSearchCommand::Execute() {
	CString text = this->glyphSubject->note->GetSelectString().c_str();
	CString searchMessage;
	searchMessage.Format("https://www.bing.com/search?q=%s", text);
	::ShellExecute(NULL, _T("open"), _T("chrome.exe"), searchMessage, NULL, SW_SHOW);
}