#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H
#include <afxwin.h>

// <<Abstract(Inteface)>>
// KeyAction
class GlyphSubject;
class KeyAction {
public:
	KeyAction();
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	KeyAction& operator =(const KeyAction& source);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {  }

protected:
	GlyphSubject* glyphSubject;
};

// <<Factory Method Pattern>>
// KeyActionFactory
class KeyActionFactory {
public:
	KeyActionFactory();
	KeyActionFactory(GlyphSubject* glyphSubject);
	~KeyActionFactory();
	KeyAction* Create(UINT nChar);

private:
	GlyphSubject* glyphSubject;
};

// PageUpKeyAction
class PageUpKeyAction : public KeyAction {
public:
	PageUpKeyAction(GlyphSubject* glyphSubject = 0);
	virtual ~PageUpKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// PageDownKeyAction
class PageDownKeyAction : public KeyAction {
public:
	PageDownKeyAction(GlyphSubject* glyphSubject = 0);
	virtual ~PageDownKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// EndKeyAction
class EndKeyAction : public KeyAction {
public:
	EndKeyAction(GlyphSubject* glyphSubject);
	virtual ~EndKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// CtrlEndKeyAction
class CtrlEndKeyAction : public KeyAction {
public:
	CtrlEndKeyAction(GlyphSubject* glyphSubject);
	virtual ~CtrlEndKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// HomeKeyAction
class HomeKeyAction : public KeyAction {
public:
	HomeKeyAction(GlyphSubject* glyphSubject);
	virtual ~HomeKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// CtrlHomeKeyAction
class CtrlHomeKeyAction : public KeyAction {
public:
	CtrlHomeKeyAction(GlyphSubject* glyphSubject);
	virtual ~CtrlHomeKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// LeftKeyAction
class LeftKeyAction : public KeyAction {
public:
	LeftKeyAction(GlyphSubject* glyphSubject);
	virtual ~LeftKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// CtrlLeftKeyAction
class CtrlLeftKeyAction : public KeyAction {
public:
	CtrlLeftKeyAction(GlyphSubject* glyphSubject);
	virtual ~CtrlLeftKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// ShiftLeftKeyAction
class ShiftLeftKeyAction : public KeyAction {
public:
	ShiftLeftKeyAction(GlyphSubject* glyphSubject);
	virtual ~ShiftLeftKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// UpKeyAction
class UpKeyAction : public KeyAction {
public:
	UpKeyAction(GlyphSubject* glyphSubject);
	virtual ~UpKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// RightKeyAction
class RightKeyAction : public KeyAction {
public:
	RightKeyAction(GlyphSubject* glyphSubject);
	virtual ~RightKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// CtrlRightKeyAction
class CtrlRightKeyAction : public KeyAction {
public:
	CtrlRightKeyAction(GlyphSubject* glyphSubject);
	virtual ~CtrlRightKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// ShiftRightKeyAction
class ShiftRightKeyAction : public KeyAction {
public:
	ShiftRightKeyAction(GlyphSubject* glyphSubject);
	virtual ~ShiftRightKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

// DownKeyAction
class DownKeyAction : public KeyAction {
public:
	DownKeyAction(GlyphSubject* glyphSubject);
	virtual ~DownKeyAction();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif // _KEYACTIONS_H