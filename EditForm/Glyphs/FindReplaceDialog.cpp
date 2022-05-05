// FindReplaceDialog.cpp
/*
���ϸ�Ī : FindReplaceDialog.cpp
��� : ã��/�ٲٱ� ��ȭ���� Ŀ���� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2020.11.02
*/
#include "FindReplaceDialog.h"
#include "GlyphSubject.h"

FindReplaceDialog::FindReplaceDialog()
	: CFindReplaceDialog() {
	AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	AfxGetApp()->GetProfileString("EditSection", "ReplaceString", "");
}

FindReplaceDialog::~FindReplaceDialog() {
}

BOOL FindReplaceDialog::OnInitDialog() {
	this->GetDlgItem(IDC_CHECK_WRAP)->ShowWindow(TRUE);
	this->GetDlgItem(IDC_CHECK_WRAP)->EnableWindow(TRUE);

	LONG matchCase = AfxGetApp()->GetProfileInt("EditSection", "MatchCase", -1);
	LONG wrapAround = AfxGetApp()->GetProfileInt("EditSection", "WrapAround", -1);
	
	if (matchCase == 0) {
		this->CheckDlgButton(IDC_CHECK_CASE, BST_CHECKED);
	}
	if (wrapAround == 0) {
		this->CheckDlgButton(IDC_CHECK_WRAP, BST_CHECKED);
	}

	return CFindReplaceDialog::OnInitDialog();
}

BOOL FindReplaceDialog::Create(BOOL bFindDialogOnly, LPCTSTR lpszFindWhat, LPCTSTR lpszReplaceWith, DWORD dwFlags, CWnd* pParentWnd) {
	this->glyphSubject = (GlyphSubject*)pParentWnd;

	LONG onIsDown = AfxGetApp()->GetProfileInt("EditSection", "OnIsDown", FR_DOWN);
	if (onIsDown == FR_DOWN) {
		this->m_fr.Flags |= FR_DOWN;
	}

	this->m_fr.Flags |= FR_ENABLETEMPLATE;
	this->m_fr.hInstance = AfxGetResourceHandle();
	if (bFindDialogOnly == TRUE) {
		this->m_fr.lpTemplateName = MAKEINTRESOURCE(FindReplaceDialog::IDDFIND);
	}
	else {
		this->m_fr.lpTemplateName = MAKEINTRESOURCE(FindReplaceDialog::IDDREPLACE);
	}

	return CFindReplaceDialog::Create(bFindDialogOnly, lpszFindWhat, lpszReplaceWith, dwFlags, pParentWnd);
}

void FindReplaceDialog::OnOK() {
	LONG matchCase = -1;
	LONG wrapAround = -1;

	UINT checked = ((CButton*)this->GetDlgItem(IDC_CHECK_CASE))->GetCheck();
	if (checked == BST_CHECKED) {
		matchCase = 0;
	}
	checked = ((CButton*)this->GetDlgItem(IDC_CHECK_WRAP))->GetCheck();
	if (checked == BST_CHECKED) {
		wrapAround = 0;
	}

	LONG onIsDown = -1;
	if (this->GetCheckedRadioButton(IDC_RADIO_UP, IDC_RADIO_DOWN) == IDC_RADIO_DOWN) {
		onIsDown = FR_DOWN;
	}

	AfxGetApp()->WriteProfileInt("EditSection", "MatchCase", matchCase);
	AfxGetApp()->WriteProfileInt("EditSection", "WrapAround", wrapAround);
	AfxGetApp()->WriteProfileInt("EditSection", "OnIsDown", onIsDown);

	CFindReplaceDialog::OnOK();
}