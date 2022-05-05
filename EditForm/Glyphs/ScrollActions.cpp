// ScrollActions.cpp
/*
파일명칭 : ScrollActions.cpp
기능 : 이동 스크롤 행위들의 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.08.25
*/
#include "ScrollActions.h"
#include "Subject.h"
#include "ScrollController.h"
#include "Scrolls.h"

#define SB_LINEPREVIOUS	0 // SB_LINEUP, SB_LINELEFT
#define SB_LINENEXT		1 // SB_LINEDOWN, SB_LINERIGHT
#define SB_PAGEPREVIOUS	2 // SB_PAGEUP, SB_PAGELEFT
#define SB_PAGENEXT		3 // SB_PAGEDOWN, SB_PAGERIGHT
#define SB_FIRST		6 // SB_TOP, SB_LEFT
#define SB_LAST			7 // SB_BOTTOM, SB_RIGHT

// ScrollAction
ScrollAction::ScrollAction(Subject* pParentSubject) {
	this->pParentSubject = pParentSubject;
}

ScrollAction::~ScrollAction() {
}

// RowPreviousScrollAction
RowPreviousScrollAction::RowPreviousScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

RowPreviousScrollAction::~RowPreviousScrollAction() {
}

void RowPreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->PreviousRow();
	}
}

void RowPreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->PreviousRow();
	}
}

// RowNextScrollAction
RowNextScrollAction::RowNextScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

RowNextScrollAction::~RowNextScrollAction() {
}

void RowNextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->NextRow();
	}
}

void RowNextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->NextRow();
	}
}

// PagePreviousScrollAction
PagePreviousScrollAction::PagePreviousScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

PagePreviousScrollAction::~PagePreviousScrollAction() {
}

void PagePreviousScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->PreviousPage();
	}
}

void PagePreviousScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->PreviousPage();
	}
}

// PageNextScrollAction
PageNextScrollAction::PageNextScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

PageNextScrollAction::~PageNextScrollAction() {
}

void PageNextScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->NextPage();
	}
}

void PageNextScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->NextPage();
	}
}

// ThumbTrackScrollAction
ThumbTrackScrollAction::ThumbTrackScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

ThumbTrackScrollAction::~ThumbTrackScrollAction() {
}

void ThumbTrackScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->Move(nPos);
	}
}

void ThumbTrackScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->Move(nPos);
	}
}

// FirstScrollAction
FirstScrollAction::FirstScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

FirstScrollAction::~FirstScrollAction() {
}

void FirstScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->First();
	}
}

void FirstScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->First();
	}
}

// LastScrollAction
LastScrollAction::LastScrollAction(Subject* pParentSubject)
	: ScrollAction(pParentSubject) {
}

LastScrollAction::~LastScrollAction() {
}

void LastScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetVerticalScroll()->Last();
	}
}

void LastScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollController* scrollController;
	Long index;
	index = this->pParentSubject->Find(Observer::SCROLL);
	if (index >= 0) {
		scrollController = (ScrollController*)this->pParentSubject->GetAt(index);
		scrollController->GetHorizontalScroll()->Last();
	}
}

// ScrollActionFactory
ScrollActionFactory::ScrollActionFactory(Subject* pParentSubject) {
	this->pParentSubject = dynamic_cast<Subject*>(pParentSubject);
}

ScrollActionFactory::~ScrollActionFactory() {
}

ScrollAction* ScrollActionFactory::Create(UINT nSBCode) {
	ScrollAction* scrollAction = 0;

	switch (nSBCode) {
	case SB_LINEPREVIOUS:
		scrollAction = new RowPreviousScrollAction(this->pParentSubject);
		break;

	case SB_LINENEXT:
		scrollAction = new RowNextScrollAction(this->pParentSubject);
		break;

	case SB_PAGEPREVIOUS:
		scrollAction = new PagePreviousScrollAction(this->pParentSubject);
		break;

	case SB_PAGENEXT:
		scrollAction = new PageNextScrollAction(this->pParentSubject);
		break;

	case SB_THUMBTRACK:
		scrollAction = new ThumbTrackScrollAction(this->pParentSubject);
		break;

	case SB_FIRST:
		scrollAction = new FirstScrollAction(this->pParentSubject);
		break;

	case SB_LAST:
		scrollAction = new LastScrollAction(this->pParentSubject);
		break;

	default:
		break;
	}

	return scrollAction;
}