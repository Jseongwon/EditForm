// TestForm.h
#ifndef _TESTFORM_H
#define _TESTFORM_H
#include <afxwin.h>

class GlyphSubject;
class TestForm : public CFrameWnd {
public:
	GlyphSubject* glyphSubject;

public:
	TestForm();

protected:
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pCWnd);
	afx_msg void OnKillFocus(CWnd* pCWnd);
	afx_msg void OnPaint();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};

#endif // _TESTFORM_H