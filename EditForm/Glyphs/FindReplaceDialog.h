// FindReplaceDialog.h
#ifndef _FINDREPLACEDIALOG_H
#define _FINDREPLACEDIALOG_H
#include <afxdlgs.h>
#include "resource.h"

class GlyphSubject;
class FindReplaceDialog : public CFindReplaceDialog {
public:
	enum { IDDFIND = IDD_FINDFORM, IDDREPLACE = IDD_REPLACEFORM };

public:
	GlyphSubject* glyphSubject;

public:
	FindReplaceDialog();
	~FindReplaceDialog();
	virtual BOOL OnInitDialog();
	virtual BOOL Create(BOOL bFindDialogOnly, LPCTSTR lpszFindWhat, LPCTSTR lpszReplaceWith = NULL, DWORD dwFlags = FR_DOWN, CWnd* pParentWnd = NULL);
	virtual void OnOK();
	virtual BOOL MatchCase() const;
	BOOL WrapAround() const;
};

inline BOOL FindReplaceDialog::MatchCase() const {
	BOOL ret = FALSE;
	UINT checked = ((CButton*)const_cast<FindReplaceDialog*>(this)->GetDlgItem(IDC_CHECK_CASE))->GetCheck();
	if (checked == BST_CHECKED) {
		ret = TRUE;
	}
	AfxGetApp()->WriteProfileInt("EditSection", "MatchCase", !ret);

	return ret;
}

inline BOOL FindReplaceDialog::WrapAround() const {
	BOOL ret = FALSE;
	UINT checked = ((CButton*)const_cast<FindReplaceDialog*>(this)->GetDlgItem(IDC_CHECK_WRAP))->GetCheck();
	if (checked == BST_CHECKED) {
		ret = TRUE;
	}
	AfxGetApp()->WriteProfileInt("EditSection", "WrapAround", !ret);

	return ret;
}

#endif // _FINDREPLACEDIALOG_H