// CaretController.cpp
/*
���ϸ�Ī : CaretController.cpp
��� : ������(������)�� ĳ�� ������ Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.07
*/
#include "CaretController.h"
#include "Caret.h"
#include "GlyphSubject.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "Scrolls.h"
#include "ScrollController.h"

CaretController::CaretController(GlyphSubject* glyphSubject) {
	this->type = Observer::CARET;
	this->glyphSubject = glyphSubject;
	this->caret = new Caret(this);
	dynamic_cast<Subject*>(this->glyphSubject)->Attach(this);
}

CaretController::~CaretController() {
	if (this->caret != 0) {
		delete this->caret;
	}
}

void CaretController::Update() {
	ScrollController* scrollController;

	Long index;
	Long vScrollBarWidth = 0;
	Long hScrollBarHeight = 0;
	Long statusHeight = 0;
	Long hideCaretRight;
	Long hideCaretBottom;
	Long x;
	Long y;
	Long textWidth = 2;
	if (this->glyphSubject->onIsComposing == TRUE) {
		textWidth = this->glyphSubject->glyphExtent->GetWidth(128);
	}

	x = this->glyphSubject->glyphExtent->GetX(this->glyphSubject->current->GetCurrent());
	y = this->glyphSubject->glyphExtent->GetY(this->glyphSubject->note->GetCurrent());

	// ��ũ�ѹ� ��ġ�� ĳ���� ���� �� ������ ���� �߰� (2021.01.13)
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		vScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	}

	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE) {
		hScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);
	}

	if (this->glyphSubject->onIsStatusBar == TRUE) {
		statusHeight = GetSystemMetrics(SM_CYHSCROLL);
	}

	index = this->glyphSubject->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->glyphSubject->GetAt(index));

	hideCaretRight = scrollController->GetHorizontalScroll()->GetPosition() +
		scrollController->GetHorizontalScroll()->GetPageLength();
	hideCaretBottom = scrollController->GetVerticalScroll()->GetPosition() +
		scrollController->GetVerticalScroll()->GetPageLength() - this->glyphSubject->glyphExtent->GetHeight();

	this->onIsCaret = true;
	if (this->glyphSubject->onIsScrolling == TRUE && ((statusHeight > 0 || hScrollBarHeight > 0) && y > hideCaretBottom)) {
		this->onIsCaret = false;
	}
	else if (this->glyphSubject->onIsScrolling == TRUE && vScrollBarWidth > 0 && x > hideCaretRight) {
		this->onIsCaret = false;
	}

	if (this->glyphSubject->onIsScrolling != TRUE && (this->glyphSubject->onIsMoving == TRUE || !(GetKeyState(VK_CONTROL) & 0x8000)) &&
		(this->glyphSubject->onIsMoving == TRUE || !(GetKeyState(VK_SHIFT) & 0x8000))) {
		if (y < scrollController->GetVerticalScroll()->GetPosition()) {// 1. ĳ���� ���� ������
			scrollController->GetVerticalScroll()->Move(y);
			this->glyphSubject->verticalScrollBar->SetScrollPos(scrollController->GetVerticalScroll()->GetPosition());
		}// 2. ĳ���� �Ʒ��� ������
		else if (y + this->glyphSubject->glyphExtent->GetHeight() > scrollController->GetVerticalScroll()->GetPosition() + scrollController->GetVerticalScroll()->GetPageLength()) {
			scrollController->GetVerticalScroll()->Move(
				y - scrollController->GetVerticalScroll()->GetPageLength() + this->glyphSubject->glyphExtent->GetHeight()
			);
			this->glyphSubject->verticalScrollBar->SetScrollPos(scrollController->GetVerticalScroll()->GetPosition());
		}// 3. ĳ���� ���ʿ� ������
		if (x < scrollController->GetHorizontalScroll()->GetPosition() &&	this->glyphSubject->onIsRowWrapping != TRUE) {
			scrollController->GetHorizontalScroll()->Move(
				x - scrollController->GetHorizontalScroll()->GetPageLength() / 5
			);
			this->glyphSubject->horizontalScrollBar->SetScrollPos(scrollController->GetHorizontalScroll()->GetPosition());
		}// 4. ĳ���� �����ʿ� ������
		else if (x > scrollController->GetHorizontalScroll()->GetPosition() + scrollController->GetHorizontalScroll()->GetPageLength() &&
			this->glyphSubject->onIsRowWrapping != TRUE) {
			scrollController->GetHorizontalScroll()->Move(
				x - scrollController->GetHorizontalScroll()->GetPageLength() + scrollController->GetHorizontalScroll()->GetPageLength() / 5
			);
			this->glyphSubject->horizontalScrollBar->SetScrollPos(scrollController->GetHorizontalScroll()->GetPosition());
		}
	}
	this->glyphSubject->onIsMoving = FALSE;

	if (this->glyphSubject->onIsComposing == TRUE) {
		x -= textWidth;
	}

	if (this->onIsCaret == true) {
		this->caret->Create(textWidth, this->glyphSubject->glyphExtent->GetHeight());
		this->caret->Move(x - scrollController->GetHorizontalScroll()->GetPosition(),
			y - scrollController->GetVerticalScroll()->GetPosition());
	}
	else {
		this->caret->Hide();
	}
}

Observer* CaretController::Clone() {
	return new CaretController(*this);
}