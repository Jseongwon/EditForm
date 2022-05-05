//ScrollActions.h
#ifndef _SCROLLACTIONS_H
#define _SCROLLACTIONS_H
#include <afxwin.h>

class Subject;
class ScrollAction {
public:
	Subject* pParentSubject;

public:
	ScrollAction(Subject* pParentSubject);
	virtual ~ScrollAction() = 0;
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) = 0;
};

// RowPreviousScrollAction
class RowPreviousScrollAction : public ScrollAction {
public:
	RowPreviousScrollAction(Subject* pParentSubject);
	virtual ~RowPreviousScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// RowNextScrollAction
class RowNextScrollAction : public ScrollAction {
public:
	RowNextScrollAction(Subject* pParentSubject);
	virtual ~RowNextScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// PagePreviousScrollAction
class PagePreviousScrollAction : public ScrollAction {
public:
	PagePreviousScrollAction(Subject* pParentSubject);
	virtual ~PagePreviousScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// PageNextScrollAction
class PageNextScrollAction : public ScrollAction {
public:
	PageNextScrollAction(Subject* pParentSubject);
	virtual ~PageNextScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// ThumbTrackScrollAction
class ThumbTrackScrollAction : public ScrollAction {
public:
	ThumbTrackScrollAction(Subject* pParentSubject);
	virtual ~ThumbTrackScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// FirstScrollAction
class FirstScrollAction : public ScrollAction {
public:
	FirstScrollAction(Subject* pParentSubject);
	virtual ~FirstScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// LastScrollAction
class LastScrollAction : public ScrollAction {
public:
	LastScrollAction(Subject* pParentSubject);
	virtual ~LastScrollAction();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

// ScrollActionFactory
class ScrollActionFactory {
public:
	Subject* pParentSubject;

public:
	ScrollActionFactory(Subject* pParentSubject);
	~ScrollActionFactory();
	ScrollAction* Create(UINT nSBCode);
};

#endif // _SCROLLACTIONS_H