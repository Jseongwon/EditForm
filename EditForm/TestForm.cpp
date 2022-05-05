// TestForm.cpp

#include "TestForm.h"
#include "Glyphs/GlyphSubject.h"

TestForm::TestForm() {
	this->glyphSubject = NULL;
}

BEGIN_MESSAGE_MAP(TestForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int TestForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	CRect clientRect;
	this->GetClientRect(&clientRect);

	this->glyphSubject = new GlyphSubject(this);
	this->glyphSubject->Create(NULL, "Edit", WS_CHILD | WS_BORDER, clientRect, this, NULL);
	this->glyphSubject->ShowWindow(SW_SHOW);
	this->glyphSubject->UpdateWindow();
	this->glyphSubject->SetFocus();

	return 0;
}

void TestForm::OnSize(UINT nType, int cx, int cy) {
	CFrameWnd::OnSize(nType, cx, cy);

	CRect clientRect;
	this->GetClientRect(&clientRect);
	if (this->glyphSubject != NULL) {
		this->glyphSubject->MoveWindow(clientRect);
	}
}

void TestForm::OnSetFocus(CWnd* pCWnd) {
	CFrameWnd::OnSetFocus(pCWnd);
	if (this->glyphSubject != NULL) {
		this->glyphSubject->SetFocus();
	}
}

void TestForm::OnKillFocus(CWnd* pCWnd) {
	CFrameWnd::OnKillFocus(pCWnd);
}

void TestForm::OnPaint() {
	CPaintDC dc(this);
}

void TestForm::OnClose() {
	CWnd* form;
	if (this->glyphSubject != NULL) {
		form = this->glyphSubject;
		this->glyphSubject = NULL;
		form->SendMessage(WM_CLOSE);
	}
	CFrameWnd::OnClose();
}