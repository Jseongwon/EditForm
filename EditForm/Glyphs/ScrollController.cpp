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

	//1. 스크롤바 박스의 길이를 구한다.
	box = GetSystemMetrics(SM_CYHSCROLL);

	//2. 윈도우 화면 크기를 구한다.
	this->glyphSubject->GetClientRect(&rect);

	//3. 스크롤의 가로, 세로 최대 길이를 구한다.
	horizontalMax = this->glyphSubject->glyphExtent->GetMaxX();
	verticalMax = this->glyphSubject->glyphExtent->GetMaxY();
	// 높이는 캐럿의 위 기준이 아니라 아래의 기준으로 스크롤바가 생기므로 높이를 한 번 더함(2021.01.13)

	//4. 페이지의 가로, 세로 길이를 구한다.
	horizontalPage = rect.right;
	verticalPage = rect.bottom;

	//5. 가로, 세로 current 값을 읽는다.
	horizontalCurrent = this->horizontalScroll->GetPosition();
	verticalCurrent = this->verticalScroll->GetPosition();

	// + 상태바가 있으면 수직 페이지 크기를 줄인다.
	if (this->glyphSubject->onIsStatusBar == TRUE) {
		verticalPage -= box;
		statusBarHeight = box;
	}

	//6. 수평스크롤바가 생기면(가로스크롤의 최대 길이가 페이지의 가로길이보다 크면)
	if (horizontalMax > horizontalPage && this->glyphSubject->onIsRowWrapping != TRUE) {
		//6.1 수평스크롤바의 플래그를 설정한다.
		this->glyphSubject->onIsHorizontalScrollBar = TRUE;
		//6.1. 페이지의 세로길이에서 박스의 길이를 뺀다.
		verticalPage -= box;
	}
	//7. 수직스크롤바가 생기면(세로스크롤의 최대 길이가 페이지의 세로길이보다 크면)
	if (verticalMax > verticalPage) {
		//7.1 수평스크롤바의 플래그를 설정한다.
		this->glyphSubject->onIsVerticalScrollBar = TRUE;
		//7.2. 페이지의 가로길이에서 박스의 길이를 뺀다.
		horizontalPage -= box;
	}
	// 8. 수평 스크롤바가 생기면(한번 더 확인) (2021.01.13)
	if (horizontalMax > horizontalPage && this->glyphSubject->onIsHorizontalScrollBar != TRUE &&
		this->glyphSubject->onIsRowWrapping != TRUE) {
		//6.1 수평스크롤바의 플래그를 설정한다.
		this->glyphSubject->onIsHorizontalScrollBar = TRUE;
		//6.1. 페이지의 세로길이에서 박스의 길이를 뺀다.
		verticalPage -= box;
	}

	//8. 수평스크롤바가 안생겼으면 수평스크롤의 현재 위치를 0으로 설정한다.
	if (this->glyphSubject->onIsHorizontalScrollBar != TRUE) {
		horizontalCurrent = 0;
	}
	
	// + 자동개행모드이면 수평 스크롤바 0으로 설정.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		horizontalCurrent = 0;
	}

	//9. 수직스크롤바가 안생겼으면 수직스크롤의 현재위치를 0으로 설정한다.
	if (this->glyphSubject->onIsVerticalScrollBar != TRUE) {
		verticalCurrent = 0;
	}

	//10. 수평, 수직 스크롤의 유닛을 구한다.
	horizontalUnit = this->glyphSubject->glyphExtent->GetWidth(128) / 2;
	verticalUnit = this->glyphSubject->glyphExtent->GetHeight();

	//11. 수평스크롤바가 있으면 스크롤바의 위치를 정해준다.
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE) {
		horizontalScrollBarX = 0;
		horizontalScrollBarY = rect.bottom - statusBarHeight - box;
		horizontalScrollBarWidth = horizontalPage;
		horizontalScrollBarHeight = box;
	}

	//12. 수직스크롤바가 있으면 스크롤바의 위치를 정해준다.
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		verticalScrollBarX = rect.right - box;
		verticalScrollBarY = 0;
		verticalScrollBarWidth = box;
		verticalScrollBarHeight = verticalPage;
	}

	//14. 기존의 스크롤들을 지운다.
	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
	}

	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
	}

	// + 스크롤의 현재 위치 값을 보정한다.(2021.02.03)
	if (horizontalCurrent >= horizontalMax - horizontalPage && horizontalMax > horizontalPage) {
		horizontalCurrent = horizontalMax - horizontalPage + 2;
	}

	if (verticalCurrent >= verticalMax - verticalPage && verticalMax > verticalPage) {
		verticalCurrent = verticalMax - verticalPage + 2;
	}

	//15. 스크롤의 값들을 새로 저장한다.
	this->horizontalScroll = new HorizontalScroll(0,horizontalMax,horizontalUnit,horizontalPage,horizontalCurrent);
	this->verticalScroll = new VerticalScroll(0,verticalMax,verticalUnit,verticalPage,verticalCurrent);

	//16. 스크롤에 정보를 저장한다
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

	//17. 수평 스크롤바를 보여준다.
	this->glyphSubject->horizontalScrollBar->SetScrollInfo(&horizontalScrollInfo, TRUE); // 어차피 설정하니 밖으로 빼냄 (2021.01.13)
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE) {
		this->glyphSubject->horizontalScrollBar->SetWindowPos(NULL, horizontalScrollBarX, horizontalScrollBarY, horizontalScrollBarWidth, horizontalScrollBarHeight, SWP_DRAWFRAME);
		this->glyphSubject->horizontalScrollBar->ShowScrollBar(TRUE);
	}
	else {
		this->glyphSubject->horizontalScrollBar->ShowScrollBar(FALSE);
	}
	//18. 수직 스크롤바를 보여준다.
	this->glyphSubject->verticalScrollBar->SetScrollInfo(&verticalScrollInfo, TRUE); // 어차피 설정하니 밖으로 빼냄 (2021.01.13)
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		this->glyphSubject->verticalScrollBar->SetWindowPos(NULL, verticalScrollBarX,verticalScrollBarY, verticalScrollBarWidth,
			verticalScrollBarHeight, SWP_DRAWFRAME);
		this->glyphSubject->verticalScrollBar->ShowScrollBar(TRUE);
	}
	else {
		this->glyphSubject->verticalScrollBar->ShowScrollBar(FALSE);
	}

	//19. 수평, 수직스크롤바가 동시에 있으면 사이즈 박스를 끼워넣는다.
	if (this->glyphSubject->onIsHorizontalScrollBar == TRUE && this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		// X = VerticalScrollBar의 X, Y = HorizontalScrollBar의 Y축으로 변경(2021.01.13)
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

// Scroll들을 읽는 연산 추가 (2021.01.13)
Scroll* ScrollController::GetHorizontalScroll() {
	return this->horizontalScroll;
}

Scroll* ScrollController::GetVerticalScroll() {
	return this->verticalScroll;
}