// GlyphDrawingVisitor.cpp
/*
파일명칭 : GlyphDrawingVisitor.cpp
기    능 : 텍스트 그리기 방문자 클래스를 만든다.
작성일자 : 2022.01.11
*/
#include "GlyphVisitors.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include "ScrollController.h"
#include "Scrolls.h"
#include "Font.h"
#include "GlyphExtent.h"

// GlyphVisitor
GlyphVisitor::GlyphVisitor() {

}

GlyphVisitor::~GlyphVisitor() {

}

// GlyphDrawingVisitor
GlyphDrawingVisitor::GlyphDrawingVisitor(GlyphSubject* glyphSubject, CDC* pDC) {
	this->glyphSubject = glyphSubject;
	this->pDC = pDC;
	this->tempDC = 0;
	this->x = 0;
	this->y = 0;
}

GlyphDrawingVisitor::~GlyphDrawingVisitor() {

}

void GlyphDrawingVisitor::Visit(Note* note) {
	CString text;
	Glyph* row;

	CRect rect;

	ScrollController* scrollController;

	HBITMAP hbmp;
	HBITMAP oldBMP;

	HFONT hFont;
	HFONT oldFont;

	CDC tempDC;

	LONG index;
	LONG criticalIndex;
	Long height;
	Long i;

	index = this->glyphSubject->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->glyphSubject->GetAt(index));

	rect.SetRect(0, 0, scrollController->GetHorizontalScroll()->GetPageLength(), scrollController->GetVerticalScroll()->GetPageLength());

	this->tempDC = &tempDC;
	tempDC.CreateCompatibleDC(this->pDC);

	hbmp = ::CreateCompatibleBitmap(*this->pDC, rect.right, rect.bottom);
	oldBMP = (HBITMAP)tempDC.SelectObject(hbmp);

	tempDC.FillRect(&rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)tempDC.SelectObject(hFont);
	tempDC.SetTextColor(this->glyphSubject->font->GetColorRef());

	height = this->glyphSubject->glyphExtent->GetHeight();

	criticalIndex = (scrollController->GetVerticalScroll()->GetPosition() + scrollController->GetVerticalScroll()->GetPageLength()) / height;
	this->y = scrollController->GetVerticalScroll()->GetPosition();
	i = scrollController->GetVerticalScroll()->GetPosition() / height - 1;
	if (i < 0) {
		i = 0;
	}
	while (i < note->GetLength() && i <= criticalIndex) {
		this->x = -scrollController->GetHorizontalScroll()->GetPosition();
		this->y = i*  height - scrollController->GetVerticalScroll()->GetPosition();

		row = note->GetAt(i);
		row->Accept(this);
		i++;
	}

	this->pDC->BitBlt(0, 0, rect.right, rect.bottom, &tempDC, 0, 0, SRCCOPY);

	SelectObject(tempDC, oldFont);
	DeleteObject(hFont);

	tempDC.SelectObject(oldBMP);
	DeleteObject(hbmp);
	tempDC.DeleteDC();
}

void GlyphDrawingVisitor::Visit(Row* row) {
	Glyph* letter;

	ScrollController* scrollController;

	Long index;
	Long pageLength;
	Long width;
	Long i = 0;

	index = this->glyphSubject->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->glyphSubject->GetAt(index));
	pageLength = scrollController->GetHorizontalScroll()->GetPageLength();

	while (i < row->GetLength()) {
		letter = row->GetAt(i);

		width = this->glyphSubject->glyphExtent->GetWidth((char*)letter->GetString().c_str());
		if (this->x + width > 0 && this->x < pageLength) {
			letter->Accept(this);
		}
		this->x += width;

		i++;
	}
}

void GlyphDrawingVisitor::Visit(SingleByteLetter* singleByteLetter) {
	// 글자를 출력한다.
	string text = singleByteLetter->GetString();
	if (text == "\t") {
		text = "        ";
	}

	if (singleByteLetter->IsSelected() == true) {
		this->tempDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		this->tempDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else {
		this->tempDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		this->tempDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}
	this->tempDC->TextOut(this->x, this->y, text.c_str());
}

void GlyphDrawingVisitor::Visit(DoubleByteLetter* doubleByteLetter) {
	// 글자를 출력한다.
	if (doubleByteLetter->IsSelected() == true) {
		this->tempDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		this->tempDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else {
		this->tempDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		this->tempDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	}
	this->tempDC->TextOut(this->x, this->y, doubleByteLetter->GetString().c_str());
}