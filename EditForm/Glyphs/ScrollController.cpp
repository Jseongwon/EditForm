//ScrollController.cpp
#include"ScrollController.h"
#include"Scrolls.h"
#include"GlyphSubject.h"
#include"GlyphExtent.h"
#include"Glyph.h"
#include<afxext.h>


ScrollController::ScrollController(GlyphSubject* glyphSubject) {
	this->type = Observer::SCROLL;

	this->glyphSubject = glyphSubject;
	
	Long horizontalMax;
	Long verticalMax;
	Long horizontalPage;
	Long verticalPage;
	Long horizontalUnit;
	Long verticalUnit;
	RECT rect = { 0, };
	this->glyphSubject->GetClientRect(&rect);

	horizontalMax = this->glyphSubject->glyphExtent->GetMaxX();
	verticalMax = this->glyphSubject->glyphExtent->GetMaxY();
	horizontalPage = rect.right;
	verticalPage = rect.bottom;
	horizontalUnit = this->glyphSubject->glyphExtent->GetWidth(128) / 2;
	verticalUnit = this->glyphSubject->glyphExtent->GetHeight();

	this->horizontalScroll = new HorizontalScroll(0, horizontalMax, horizontalUnit, horizontalPage, 0);
	this->verticalScroll = new VerticalScroll(0, verticalMax, verticalUnit, verticalPage, 0);

	dynamic_cast<Subject*>(this->glyphSubject)->Attach(this);
}

ScrollController::~ScrollController() {
	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
	}
	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
	}
}

void ScrollController::Update() {
	CRect rect = { 0, };

	Long box;
	Long verticalMax;
	Long verticalPage;
	Long verticalUnit;
	Long verticalCurrent;
	Long horizontalMax;
	Long horizontalPage;
	Long horizontalUnit;
	Long horizontalCurrent;

	Long horizontalScrollBarX;
	Long horizontalScrollBarY;
	Long horizontalScrollBarWidth;
	Long horizontalScrollBarHeight;

	Long verticalScrollBarX;
	Long verticalScrollBarY;
	Long verticalScrollBarWidth;
	Long verticalScrollBarHeight;

	Long statusBarHeight = 0;

	SCROLLINFO horizontalScrollInfo;
	SCROLLINFO verticalScrollInfo;

	this->glyphSubject->onIsHorizontalScrollBar = FALSE;
	this->glyphSubject->onIsVerticalScrollBar = FALSE;

	//1. ��ũ�ѹ� �ڽ��� ���̸� ���Ѵ�.
	box = GetSystemMetrics(SM_CYHSCROLL);

	//2. ������ ȭ�� ũ�⸦ ���Ѵ�.
	this->glyphSubject->GetClientRect(&rect);

	//3. ��ũ���� ����, ���� �ִ� ���̸� ���Ѵ�.
	horizontalMax = this->glyphSubject->glyphExtent->GetMaxX();
	verticalMax = this->glyphSubject->glyphExtent->GetMaxY();
	// ���̴� ĳ���� �� ������ �ƴ϶� �Ʒ��� �������� ��ũ�ѹٰ� ����Ƿ� ���̸� �� �� ����(2021.01.13)

	//4. �������� ����, ���� ���̸� ���Ѵ�.
	horizontalPage = rect.right;
	verticalPage = rect.bottom;

	//5. ����, ���� current ���� �д´�.
	horizontalCurrent = this->horizontalScroll->GetPosition();
	verticalCurrent = this->verticalScroll->GetPosition();

	// + ���¹ٰ� ������ ���� ������ ũ�⸦ ���δ�.
	if (this->glyphSubject->onIsStatusBar == TRUE) {
		verticalPage -= box;
		statusBarHeight = box;
	}

	//6. ����ũ�ѹٰ� �����(���ν�ũ���� �ִ� ���̰� �������� ���α��̺��� ũ��)
	if (horizontalMax > horizontalPage && this->glyphSubject->onIsRowWrapping != TRUE) {
		//6.1 ����ũ�ѹ��� �÷��׸� �����Ѵ�.
		this->glyphSubject->onIsHorizontalScrollBar = TRUE;
		//6.1. �������� ���α��̿��� �ڽ��� ���̸� ����.
		verticalPage -= box;
	}
	//7. ������ũ�ѹٰ� �����(���ν�ũ���� �ִ� ���̰� �������� ���α��̺��� ũ��)
	if (verticalMax > verticalPage) {
		//7.1 ����ũ�ѹ��� �÷��׸� �����Ѵ�.
		this->glyphSubject->onIsVerticalScrollBar = TRUE;
		//7.2. �������� ���α��̿��� �ڽ��� ���̸� ����.
		horizontalPage -= box;
	}
	// 8. ���� ��ũ�ѹٰ� �����(�ѹ� �� Ȯ��) (2021.01.13)
	if (horizontalMax > horizontalPage && this->glyphSubject->onIsHorizontalScrollBar != TRUE &&
		this->glyphSubject->onIsRowWrapping != TRUE) {
		//6.1 ����ũ�ѹ��� �÷��׸� �����Ѵ�.
		this->glyphSubject->onIsHorizontalScrollBar = TRUE;
		//6.1. �������� ���α��̿��� �ڽ��� ���̸� ����.
		verticalPage -= box;
	}

	//8. ����ũ�ѹٰ� �Ȼ������� ����ũ���� ���� ��ġ�� 0���� �����Ѵ�.
	if (this->glyphSubject->onIsHorizontalScrollBar != TRUE) {
		horizontalCurrent = 0;
	}
	
	// + �ڵ��������̸� ���� ��ũ�ѹ� 0���� ����.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		horizontalCurrent = 0;
	}

	//9. ������ũ�ѹٰ� �Ȼ������� ������ũ���� ������ġ�� 0���� �����Ѵ�.
	if (this->glyphSubject->onIsVerticalScrollBar != TRUE) {
		verticalCurrent = 0;
	}

	//10. ����, ���� ��ũ���� ������ ���Ѵ�.
	horizontalUnit = this->glyphSubject->glyphExtent->GetWidth(128) / 2;
	verticalUnit = this->glyphSubject->glyphExtent->GetHeight();

	//11. ����ũ�ѹٰ� ������ ��ũ�ѹ��� ��ġ�� �����ش�.
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE) {
		horizontalScrollBarX = 0;
		horizontalScrollBarY = rect.bottom - statusBarHeight - box;
		horizontalScrollBarWidth = horizontalPage;
		horizontalScrollBarHeight = box;
	}

	//12. ������ũ�ѹٰ� ������ ��ũ�ѹ��� ��ġ�� �����ش�.
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		verticalScrollBarX = rect.right - box;
		verticalScrollBarY = 0;
		verticalScrollBarWidth = box;
		verticalScrollBarHeight = verticalPage;
	}

	//14. ������ ��ũ�ѵ��� �����.
	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
	}

	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
	}

	// + ��ũ���� ���� ��ġ ���� �����Ѵ�.(2021.02.03)
	if (horizontalCurrent >= horizontalMax - horizontalPage && horizontalMax > horizontalPage) {
		horizontalCurrent = horizontalMax - horizontalPage + 2;
	}

	if (verticalCurrent >= verticalMax - verticalPage && verticalMax > verticalPage) {
		verticalCurrent = verticalMax - verticalPage + 2;
	}

	//15. ��ũ���� ������ ���� �����Ѵ�.
	this->horizontalScroll = new HorizontalScroll(0,horizontalMax,horizontalUnit,horizontalPage,horizontalCurrent);
	this->verticalScroll = new VerticalScroll(0,verticalMax,verticalUnit,verticalPage,verticalCurrent);

	//16. ��ũ�ѿ� ������ �����Ѵ�
	horizontalScrollInfo.cbSize = sizeof(SCROLLINFO);
	horizontalScrollInfo.fMask = SIF_ALL;
	horizontalScrollInfo.nMin = this->horizontalScroll->GetMinimum();
	horizontalScrollInfo.nMax = this->horizontalScroll->GetMaximum();
	horizontalScrollInfo.nPage = this->horizontalScroll->GetPageLength();
	horizontalScrollInfo.nPos = this->horizontalScroll->GetPosition();
	horizontalScrollInfo.nTrackPos = 2;

	verticalScrollInfo.cbSize = sizeof(SCROLLINFO);
	verticalScrollInfo.fMask = SIF_ALL;
	verticalScrollInfo.nMin = this->verticalScroll->GetMinimum();
	verticalScrollInfo.nMax = this->verticalScroll->GetMaximum();
	verticalScrollInfo.nPage = this->verticalScroll->GetPageLength();
	verticalScrollInfo.nPos = this->verticalScroll->GetPosition();
	verticalScrollInfo.nTrackPos = 2;

	//17. ���� ��ũ�ѹٸ� �����ش�.
	this->glyphSubject->horizontalScrollBar->SetScrollInfo(&horizontalScrollInfo, TRUE); // ������ �����ϴ� ������ ���� (2021.01.13)
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE) {
		this->glyphSubject->horizontalScrollBar->SetWindowPos(NULL, horizontalScrollBarX, horizontalScrollBarY, horizontalScrollBarWidth, horizontalScrollBarHeight, SWP_DRAWFRAME);
		this->glyphSubject->horizontalScrollBar->ShowScrollBar(TRUE);
	}
	else {
		this->glyphSubject->horizontalScrollBar->ShowScrollBar(FALSE);
	}
	//18. ���� ��ũ�ѹٸ� �����ش�.
	this->glyphSubject->verticalScrollBar->SetScrollInfo(&verticalScrollInfo, TRUE); // ������ �����ϴ� ������ ���� (2021.01.13)
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		this->glyphSubject->verticalScrollBar->SetWindowPos(NULL, verticalScrollBarX,verticalScrollBarY, verticalScrollBarWidth,
			verticalScrollBarHeight, SWP_DRAWFRAME);
		this->glyphSubject->verticalScrollBar->ShowScrollBar(TRUE);
	}
	else {
		this->glyphSubject->verticalScrollBar->ShowScrollBar(FALSE);
	}

	//19. ����, ������ũ�ѹٰ� ���ÿ� ������ ������ �ڽ��� �����ִ´�.
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE && this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		// X = VerticalScrollBar�� X, Y = HorizontalScrollBar�� Y������ ����(2021.01.13)
		this->glyphSubject->sizeBox->SetWindowPos(NULL, verticalScrollBarX, horizontalScrollBarY, box, box, SWP_DRAWFRAME);
		this->glyphSubject->sizeBox->ShowScrollBar(TRUE);
	}
	else {
		this->glyphSubject->sizeBox->ShowScrollBar(FALSE);
	}
}

Observer* ScrollController::Clone() {
	return new ScrollController(*this);
}

// Scroll���� �д� ���� �߰� (2021.01.13)
Scroll* ScrollController::GetHorizontalScroll() {
	return this->horizontalScroll;
}

Scroll* ScrollController::GetVerticalScroll() {
	return this->verticalScroll;
}