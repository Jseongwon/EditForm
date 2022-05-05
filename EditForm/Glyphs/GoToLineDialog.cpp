// GoToLineDialog.cpp
/*
���ϸ�Ī : GoToLineDialog.cpp
��� : �� �̵� ��ȭ���� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
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
	line.Format("�� ��ȣ(1 - %d)(&L):", realRowCount);
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
		this->MessageBox("�� ��ȣ�� ��ü �� ���� �ѽ��ϴ�.", "�޸��� - �� �̵�", MB_OK);
	}
}