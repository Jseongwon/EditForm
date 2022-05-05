// KeyActions.cpp
/*
���ϸ�Ī : KeyActions.cpp
��    �� : KeyAction�� ���õ� Ŭ�������� �����Ѵ�.
�� �� �� : ������
�ۼ����� : 2021.08.09
*/
#include "KeyActions.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h" // GlyphExtent �� ���õ� ���� 1. Mediator
#include "ScrollController.h"
#include "Scrolls.h"
#include "resource.h"

/*
Ŭ������ : KeyAction
��    �� : KeyAction Interface
*/
KeyAction::KeyAction() {
	this->glyphSubject = 0;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->glyphSubject = source.glyphSubject;
}

KeyAction::~KeyAction() {
}

KeyAction& KeyAction::operator =(const KeyAction& source) {
	this->glyphSubject = source.glyphSubject;

	return* this;
}

/*
Ŭ������ : KeyActionFactory
��    �� : KeyAction Factory Method Pattern
*/
KeyActionFactory::KeyActionFactory() {
	this->glyphSubject = 0;
}

KeyActionFactory::KeyActionFactory(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

KeyActionFactory::~KeyActionFactory() {
}

KeyAction* KeyActionFactory::Create(UINT nChar) {
	KeyAction* keyAction = 0;
	bool onIsSelected = this->glyphSubject->note->IsSelected();
	switch (nChar) {
	case VK_PRIOR:
		keyAction = new PageUpKeyAction(this->glyphSubject);
		break;

	case VK_NEXT:
		keyAction = new PageDownKeyAction(this->glyphSubject);
		break;

	case VK_END:
		if (!(GetKeyState(VK_CONTROL) & 0x8000)) {
			keyAction = new EndKeyAction(this->glyphSubject);
		}
		else {
			keyAction = new CtrlEndKeyAction(this->glyphSubject);
		}
		break;

	case VK_HOME:
		if (!(GetKeyState(VK_CONTROL) & 0x8000)) {
			keyAction = new HomeKeyAction(this->glyphSubject);
		}
		else {
			keyAction = new CtrlHomeKeyAction(this->glyphSubject);
		}
		break;

	case VK_LEFT:
		if (!(GetKeyState(VK_CONTROL) & 0x8000) && ((GetKeyState(VK_SHIFT) & 0x8000) || onIsSelected != true)) {
			keyAction = new LeftKeyAction(this->glyphSubject);
		}
		else if ((GetKeyState(VK_CONTROL) & 0x8000) && ((GetKeyState(VK_SHIFT) & 0x8000) || onIsSelected != true)) {
			keyAction = new CtrlLeftKeyAction(this->glyphSubject);
		}
		else {
			keyAction = new ShiftLeftKeyAction(this->glyphSubject);
		}
		break;

	case VK_UP:
		keyAction = new UpKeyAction(this->glyphSubject);
		break;

	case VK_RIGHT:
		if (!(GetKeyState(VK_CONTROL) & 0x8000) && ((GetKeyState(VK_SHIFT) & 0x8000) || onIsSelected != true)) {
			keyAction = new RightKeyAction(this->glyphSubject);
		}
		else if ((GetKeyState(VK_CONTROL) & 0x8000) && ((GetKeyState(VK_SHIFT) & 0x8000) || onIsSelected != true)) {
			keyAction = new CtrlRightKeyAction(this->glyphSubject);
		}
		else {
			keyAction = new ShiftRightKeyAction(this->glyphSubject);
		}
		break;

	case VK_DOWN:
		keyAction = new DownKeyAction(this->glyphSubject);
		break;

	default:
		break;
	}

	return keyAction;
}

/*
Ŭ������ : PageUpKeyAction
��    �� : �������� Ű�� ������ �� ó���Ѵ�.
*/
PageUpKeyAction::PageUpKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

PageUpKeyAction::~PageUpKeyAction() {
}

void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	ScrollController* scrollController;

	Long index;
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long x = this->glyphSubject->glyphExtent->GetX(rowCurrent);
	Long y = this->glyphSubject->glyphExtent->GetY(noteCurrent);

	Long scrollCurrent;
	Long height;
	Long count = 0;
	Long max;
	Long i = 0;

	index = this->glyphSubject->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->glyphSubject->GetAt(index));

	scrollCurrent = scrollController->GetVerticalScroll()->GetPosition();
	height = this->glyphSubject->glyphExtent->GetHeight();
	max = y - scrollCurrent + 2;

	while (i < max) {
		count++;
		i += height;
	}

	this->glyphSubject->SendMessage(WM_VSCROLL, SB_PAGEUP);
	scrollCurrent = scrollController->GetVerticalScroll()->GetPosition();
	noteCurrent = this->glyphSubject->glyphExtent->GetNoteCurrent(height*  count + scrollCurrent);
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->glyphExtent->GetRowCurrent(x);
	this->glyphSubject->current->Move(rowCurrent);
}

/*
Ŭ������ : PageDownKeyAction
��    �� : �������ٿ� Ű�� ������ �� ó���Ѵ�.
*/
PageDownKeyAction::PageDownKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

PageDownKeyAction::~PageDownKeyAction() {
}

void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	ScrollController* scrollController;

	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long x = this->glyphSubject->glyphExtent->GetX(rowCurrent);
	Long y = this->glyphSubject->glyphExtent->GetY(noteCurrent);
	Long index;
	Long scrollCurrent;
	Long height = this->glyphSubject->glyphExtent->GetHeight();
	Long count = 0;
	Long max;
	Long i = 0;
	
	index = this->glyphSubject->Find(Observer::SCROLL);

	scrollController = dynamic_cast<ScrollController*>(this->glyphSubject->GetAt(index));

	scrollCurrent = scrollController->GetVerticalScroll()->GetPosition();
	max = y - scrollCurrent + 2;
	while (i < max) {
		count++;
		i += height;
	}

	this->glyphSubject->SendMessage(WM_VSCROLL, SB_PAGEDOWN);
	scrollCurrent = scrollController->GetVerticalScroll()->GetPosition();
	noteCurrent = this->glyphSubject->glyphExtent->GetNoteCurrent(height*  count + scrollCurrent);
	this->glyphSubject->note->Move(noteCurrent);
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->glyphExtent->GetRowCurrent(x);
	this->glyphSubject->current->Move(rowCurrent);
}

/*
Ŭ������ : EndKeyAction
��    �� : ���� Ű�� ������ �� ó���Ѵ�.
*/
EndKeyAction::EndKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

EndKeyAction::~EndKeyAction() {
}

void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long noteLength = this->glyphSubject->note->GetLength();
	Long rowLength = this->glyphSubject->current->GetLength();

	Glyph* nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);

	if (this->glyphSubject->onIsRowWrapping == TRUE && noteCurrent + 1 < noteLength && rowCurrent >= rowLength &&
		nextRow->GetOnIsRow() != true) {
		noteCurrent = this->glyphSubject->note->Next();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	this->glyphSubject->current->Last();
}

/*
Ŭ������ : CtrlEndKeyAction
��    �� : ��Ʈ�� ���� Ű�� ������ �� ó���Ѵ�.
*/
CtrlEndKeyAction::CtrlEndKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CtrlEndKeyAction::~CtrlEndKeyAction() {
}

void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->glyphSubject->note->Last();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(index);
	this->glyphSubject->current->Last();
}

/*
Ŭ������ : HomeKeyAction
��    �� : Ȩ Ű�� ������ �� ó���Ѵ�.
*/
HomeKeyAction::HomeKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

HomeKeyAction::~HomeKeyAction() {
}

void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent;
	Long rowCurrent = this->glyphSubject->current->GetCurrent();

	if (this->glyphSubject->onIsRowWrapping == TRUE && rowCurrent <= 0 && this->glyphSubject->current->GetOnIsRow() != true) {
		noteCurrent = this->glyphSubject->note->Previous();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	this->glyphSubject->current->First();
}

/*
Ŭ������ : CtrlHomeKeyAction
��    �� : ��Ʈ�� Ȩ Ű�� ������ �� ó���Ѵ�.
*/
CtrlHomeKeyAction::CtrlHomeKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {
}

void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->glyphSubject->note->First();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
	;	this->glyphSubject->current->First();
}

/*
Ŭ������ : LeftKeyAction
��    �� : ���� Ű�� ������ �� ó���Ѵ�.
*/
LeftKeyAction::LeftKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

LeftKeyAction::~LeftKeyAction() {
}

void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();

	if (rowCurrent > 0 || noteCurrent == 0) {
		this->glyphSubject->current->Previous();
	}
	else if (this->glyphSubject->current->GetOnIsRow() == true) {
		noteCurrent = this->glyphSubject->note->Previous();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Last();
	}
	else {
		noteCurrent = this->glyphSubject->note->Previous();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Last();
		this->glyphSubject->current->Previous();
	}
}

/*
Ŭ������ : CtrlLeftKeyAction
��    �� : ��Ʈ�� ���� Ű�� ������ �� ó���Ѵ�.
*/
CtrlLeftKeyAction::CtrlLeftKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CtrlLeftKeyAction::~CtrlLeftKeyAction() {
}

void CtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long rowLength = this->glyphSubject->current->GetLength();

	string text = "\0";
	bool onIsFirst = false;
	bool onIsSpaceOrTab = false;
	if (rowCurrent == 0 && noteCurrent > 0) {
		onIsFirst = true;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	this->glyphSubject->current->PreviousWord();

	rowCurrent = this->glyphSubject->current->GetCurrent();
	if (rowCurrent < rowLength) {
		text = this->glyphSubject->current->GetAt(rowCurrent)->GetString();
	}

	if (text == " " || text == "\t") {
		onIsSpaceOrTab = true;
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	if ((onIsSpaceOrTab == true && noteCurrent > 0) || onIsFirst == true) {
		noteCurrent = this->glyphSubject->note->Previous();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Last();
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

/*
Ŭ������ : ShiftLeftKeyAction
��    �� : ����Ʈ ���� Ű�� ������ �� ó���Ѵ�.
*/
ShiftLeftKeyAction::ShiftLeftKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

ShiftLeftKeyAction::~ShiftLeftKeyAction() {
}

void ShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent;

	this->glyphSubject->note->MoveToFirstSelectedCurrent();
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}

/*
Ŭ������ : UpKeyAction
��    �� : ���� Ű�� ������ �� ó���Ѵ�.
*/
UpKeyAction::UpKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

UpKeyAction::~UpKeyAction() {
}

void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long x;
	x = this->glyphSubject->glyphExtent->GetX(this->glyphSubject->current->GetCurrent());

	Long noteCurrent;
	noteCurrent = this->glyphSubject->note->Previous();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	Long rowCurrent;
	rowCurrent = this->glyphSubject->glyphExtent->GetRowCurrent(x);
	this->glyphSubject->current->Move(rowCurrent);
}

/*
Ŭ������ : RightKeyAction
��    �� : ������ Ű�� ������ �� ó���Ѵ�.
*/
RightKeyAction::RightKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

RightKeyAction::~RightKeyAction() {
}

void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	bool onIsRow = true;

	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long noteLength = this->glyphSubject->note->GetLength();
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long rowLength = this->glyphSubject->current->GetLength();

	Glyph* nextRow = 0;
	// 1. �ٿ��� �������� �̵��Ѵ�.

	// 2. ���� �ٷ� �̵��ؾ� �Ǹ�
	// 2.1. 
	if (rowCurrent < rowLength || noteCurrent >= noteLength - 1) {
		rowCurrent = this->glyphSubject->current->Next();

		if (noteCurrent < noteLength - 1) {
			nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);
			onIsRow = nextRow->GetOnIsRow();
		}

		if (this->glyphSubject->onIsRowWrapping == TRUE && rowCurrent >= rowLength && onIsRow != true) {
			noteCurrent = this->glyphSubject->note->Next();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			this->glyphSubject->current->First();
		}
	}
	else {
		noteCurrent = this->glyphSubject->note->Next();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->First();
		onIsRow = this->glyphSubject->current->GetOnIsRow();
		if (this->glyphSubject->onIsRowWrapping == TRUE && noteCurrent < noteLength && onIsRow != true) {
			this->glyphSubject->current->Next();
		}
	}
}

/*
Ŭ������ : CtrlRightKeyAction
��    �� : ��Ʈ�� ������ Ű�� ������ �� ó���Ѵ�.
*/
CtrlRightKeyAction::CtrlRightKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CtrlRightKeyAction::~CtrlRightKeyAction() {
}

void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->glyphSubject->note->GetCurrent();
	Long noteLength;
	Long rowCurrent = this->glyphSubject->current->GetCurrent();
	Long rowLength = this->glyphSubject->current->GetLength();


	string text = "\0";
	bool onIsNext = true;
	bool onIsSpaceOrTab = false;
	if (rowCurrent >= rowLength) {
		onIsNext = false;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE && onIsNext == true) {
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	noteLength = this->glyphSubject->note->GetLength();
	rowCurrent = this->glyphSubject->current->GetCurrent();
	rowLength = this->glyphSubject->current->GetLength();

	if (rowCurrent == rowLength && noteCurrent < noteLength - 1) {
		noteCurrent = this->glyphSubject->note->Next();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->First();
		rowLength = this->glyphSubject->current->GetLength();
		if (rowCurrent < rowLength) {
			text = this->glyphSubject->current->GetAt(rowCurrent)->GetString();
		}
	}

	if (text == " " || text == "\t") {
		onIsSpaceOrTab = true;
	}

	if (onIsSpaceOrTab == true || onIsNext == true) {
		this->glyphSubject->current->NextWord();
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE && onIsNext == true) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAPLINE);
	}
}

/*
Ŭ������ : ShiftRightKeyAction
��    �� : ����Ʈ ������ Ű�� ������ �� ó���Ѵ�.
*/
ShiftRightKeyAction::ShiftRightKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

ShiftRightKeyAction::~ShiftRightKeyAction() {
}

void ShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent;

	this->glyphSubject->note->MoveToLastSelectedCurrent();
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}

/*
Ŭ������ : DownKeyAction
��    �� : �Ʒ��� Ű�� ������ �� ó���Ѵ�.
*/
DownKeyAction::DownKeyAction(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

DownKeyAction::~DownKeyAction() {
}

void DownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long x = this->glyphSubject->glyphExtent->GetX(this->glyphSubject->current->GetCurrent());

	Long noteCurrent;
	noteCurrent = this->glyphSubject->note->Next();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	Long rowCurrent;
	rowCurrent = this->glyphSubject->glyphExtent->GetRowCurrent(x);
	this->glyphSubject->current->Move(rowCurrent);
}