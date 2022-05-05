// Clipboard.h
#ifndef _CLIPBOARD_H
#define _CLIPBOARD_H
#include <afxwin.h>

class GlyphSubject;
class Clipboard {
public:
	GlyphSubject* glyphSubject;

public:
	Clipboard(GlyphSubject* glyphSubject);
	~Clipboard();
	void Write(CString copyString);
	CString Read();
};

#endif // _CLIPBOARD_H