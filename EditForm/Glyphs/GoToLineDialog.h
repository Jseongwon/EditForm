// GoToLineDialog.h
#ifndef _GOTOLINEDIALOG_H
#define _GOTOLINEDIALOG_H
#include <afxwin.h>
#include "resource.h"

class GlyphSubject;
class GoToLineDialog : public CDialog {
public:
	enum { IDD = IDD_GOTOLINEFORM };

public:
	GlyphSubject* glyphSubject;

public:
	GoToLineDialog(GlyphSubject* glyphSubject = NULL);
	~GoToLineDialog();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

#endif // _GOTOLINEDIALOG_H