// GoToLineDialog.cpp
/*
파일명칭 : GoToLineDialog.cpp
기능 : 줄 이동 대화상자 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
*/
#include "GoToLineDialog.h"
#include "GlyphSubject.h"
#include "Glyph.h"

GoToLineDialog::GoToLineDialog(GlyphSubject* glyphSubject)
	: CDialog(GoToLineDialog::IDD, glyphSubject) {
	this->glyphSubject = glyphSubject;
}

GoToLineDialog::~GoToLineDialog() {
}

BOOL GoToLineDialog::OnInitDialog() {
	Glyph* it;

	CString line;

	LONG realRowCount = 0;
	if (this->glyphSubject->onIsRowWrapping != TRUE) {
		realRowCount = this->glyphSubject->note->GetLength();
	}
	LONG i = 0;
	while (i < this->glyphSubject->note->GetLength() && this->glyphSubject->onIsRowWrapping == TRUE) {
		it = this->glyphSubject->note->GetAt(i);
		if (it->GetOnIsRow() == true) {
			realRowCount++;
		}
		i++;
	}
	line.Format("줄 번호(1 - %d)(&L):", realRowCount);
	this->GetDlgItem(IDC_STATIC_ROWNUMBER)->SetWindowText(line);

	return CDialog::OnInitDialog();
}

void GoToLineDialog::OnOK() {
	CString line;
	this->GetDlgItem(IDC_EDIT_ROWNUMBER)->GetWindowText(line);

	LONG lineNumber = _ttoi(line);

	if (lineNumber <= this->glyphSubject->note->GetLength()) {
		this->glyphSubject->note->Move(lineNumber - 1);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(lineNumber - 1);
		this->glyphSubject->current->First();

		CDialog::OnOK();
	}
	else {
		this->MessageBox("줄 번호가 전체 줄 수를 넘습니다.", "메모장 - 줄 이동", MB_OK);
	}
}