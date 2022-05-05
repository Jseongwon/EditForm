// GlyphExtent.cpp
/*
ÆÄÀÏ¸íÄª : GlyphExtent.cpp
±â´É : ±Û¸®ÇÁ Å©±â Å¬·¡½º¸¦ ¸¸µç´Ù.
ÀÛ¼ºÀÚ : Á¤¼º¿ø
ÀÛ¼ºÀÏÀÚ : 2021.08.09
*/
#include "GlyphExtent.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "Font.h"
#define ASCII 129

GlyphExtent::GlyphExtent() {
	this->glyphSubject = 0;
	this->widths = 0;
	this->height = 0;
}

GlyphExtent::GlyphExtent(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	CSize cSize;
	this->widths = new Long[ASCII];

	Long i = 0;
	this->widths[0] = 0;
	i++;
	while (i < 9) {
		this->widths[i] = 0;
		i++;
	}

	cSize = dc->GetTextExtent(CString("        "));
	this->widths[i] = cSize.cx;
	i++;
	while (i < 32) {
		this->widths[i] = 0;
		i++;
	}
	while (i < ASCII - 2) {
		cSize = dc->GetTextExtent(CString((char)i));
		this->widths[i] = cSize.cx;
		i++;
	}
	this->widths[i] = 0;
	i++;

	cSize = dc->GetTextExtent(CString("¤¡"));
	this->widths[i] = cSize.cx;
	this->height = cSize.cy;

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);
}

GlyphExtent::GlyphExtent(const GlyphExtent& source) {
	this->glyphSubject = source.glyphSubject;
	this->widths = new Long[ASCII];
	Long i = 0;
	while (i < ASCII) {
		this->widths[i] = source.widths[i];
		i++;
	}
	this->height = source.height;
}

GlyphExtent::~GlyphExtent() {
	if (this->widths != 0) {
		delete[] this->widths;
	}
}

Long GlyphExtent::GetWidth(Long index) {
	return this->widths[index];
}

Long GlyphExtent::GetWidth(char(*text)) {
	Long width;

	CDC* dc;
	HFONT hFont;
	HFONT oldFont;

	string str;

	CSize cSize;

	dc = this->glyphSubject->GetDC();

	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	str = text;
	if (!(text[0] & 0x80)) {
		width = this->widths[(int)text[0]];
	}
	else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
		width = this->widths[ASCII - 1];
	}
	else {
		cSize = dc->GetTextExtent(text);
		width = cSize.cx;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return width;
}

Long GlyphExtent::GetX(Long current) { // ¹®Á¦ ÀÖÀ½ ¿µ¹®ÀÌ
	const char(*text);
	string str;

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	CSize cSize;

	Long x = 0;
	Long width = 0;
	Long i = 0;
	while (i < this->glyphSubject->current->GetCurrent()) {
		str = this->glyphSubject->current->GetAt(i)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			width = this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			width = this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			width = cSize.cx;
		}
		x += width;
		i++;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return x;
}

Long GlyphExtent::GetMaxX() {
	Long maxX;

	Glyph* row;// ¿©±â.

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	CSize cSize;

	const char(*text);
	string str;

	Long currentMaxX = 0;
	Long width;
	Long i = 0;
	row = this->glyphSubject->note->GetAt(i);

	Long j = 0;
	while (j < row->GetLength()) {
		str = row->GetAt(j)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			width = this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			width = this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			width = cSize.cx;
		}
		currentMaxX += width;
		j++;
	}
	maxX = currentMaxX;
	i++;
	while (i < this->glyphSubject->note->GetLength()) {
		row = this->glyphSubject->note->GetAt(i);
		currentMaxX = 0;
		j = 0;
		while (j < row->GetLength()) {
			str = row->GetAt(j)->GetString();
			text = str.c_str();
			if (!(text[0] & 0x80)) {
				width = this->widths[(int)text[0]];
			}
			else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
				width = this->widths[ASCII - 1];
			}
			else {
				cSize = dc->GetTextExtent(text);
				width = cSize.cx;
			}
			currentMaxX += width;
			j++;
		}
		if (maxX < currentMaxX) {
			maxX = currentMaxX;
		}

		i++;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return maxX;
}

Long GlyphExtent::GetY(Long current) {
	return this->height * current;
}

Long GlyphExtent::GetMaxY() {
	return this->height * this->glyphSubject->note->GetLength();
}

Long GlyphExtent::GetDistance(Glyph* row, Long startXPos, Long endXPos) {
	const char(*text);
	string str;

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	CSize cSize;

	Long width;
	Long distance = 0;
	Long i = startXPos;
	while (i < endXPos && i < row->GetLength()) {
		str = row->GetAt(i)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			width = this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			width = this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			width = cSize.cx;
		}
		distance += width;
		i++;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return distance;
}

Long GlyphExtent::GetRowCurrent(Long x) {
	Long rowCurrent = 0;
	Long width;
	Long totalWidth = 0;
	Long criticalPoint = 0;
	Long i = 0;

	CSize cSize;

	string str;
	const char(*text);

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	if (this->glyphSubject->current->GetLength() > 0) {
		str = this->glyphSubject->current->GetAt(i)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			width = this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			width = this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			width = cSize.cx;
		}
		criticalPoint = totalWidth + width / 2 + 1;
		i++;
	}
	while (i < this->glyphSubject->current->GetLength() && criticalPoint < x) {
		rowCurrent++;
		totalWidth += width;
		str = this->glyphSubject->current->GetAt(i)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			width = this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			width = this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			width = cSize.cx;
		}
		criticalPoint = totalWidth + width / 2 + 1;
		i++;
	}
	if (i > 0 && criticalPoint < x) {
		rowCurrent++;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return rowCurrent;
}

Long GlyphExtent::GetNoteCurrent(Long y) {
	Long noteCurrent = 0;
	Long i = 0;
	Long cirticalPoint = this->height;
	i++;
	while (i < this->glyphSubject->note->GetLength() && cirticalPoint < y) {
		noteCurrent++;
		cirticalPoint += this->height;
		i++;
	}
	
	return noteCurrent;
}

Long GlyphExtent::GetSplitCurrent(Glyph* row, Long width) {
	Long index = 0;

	CSize cSize;

	string str;
	const char(*text);

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	Long totalWidth = 0;
	Long i = 0;
	while (i < row->GetLength() && totalWidth <= width) {
		str = row->GetAt(i)->GetString();
		text = str.c_str();
		if (!(text[0] & 0x80)) {
			totalWidth += this->widths[(int)text[0]];
		}
		else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
			totalWidth += this->widths[ASCII - 1];
		}
		else {
			cSize = dc->GetTextExtent(text);
			totalWidth += cSize.cx;
		}
		index = i;
		i++;
	}
	if (totalWidth <= width) {
		index = -1;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return index;
}

Long GlyphExtent::GetSplitCount(Glyph* row) {
	Long count = 0;

	bool onIsStop = false;

	RECT rect;
	this->glyphSubject->GetClientRect(&rect);
	if (this->glyphSubject->onIsVerticalScrollBar == TRUE) {
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}

	CSize cSize;

	string str;
	const char(*text);

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	Long width;
	Long j;
	Long i = 0;
	while (i < row->GetLength() && onIsStop != true) {
		width = 0;
		j = i;
		while (j < row->GetLength() && width <= rect.right) {
			str = row->GetAt(j)->GetString();
			text = str.c_str();
			if (!(text[0] & 0x80)) {
				width += this->widths[(int)text[0]];
			}
			else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
				width += this->widths[ASCII - 1];
			}
			else {
				cSize = dc->GetTextExtent(text);
				width += cSize.cx;
			}
			j++;
		}
		if (width > rect.right) {
			count++;
			i = j - 2;
		}
		else {
			onIsStop = true;
		}
		i++;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return count;
}

Long GlyphExtent::GetStringCount(char(*text)) {
	string tempText = text;
	Long length = (Long)tempText.length();
	Long textCount = 0;
	Long i = 0;
	while (i < length) {
		if (text[i] == '\r' || text[i] == '\n') {
			textCount++;
			if (i + 1 < length && text[i] == '\r' && text[i + 1] == '\n') {
				i++;
			}
		}
		else if (text[i] == 0x09 || text[i] >= 0x20 && text[i] < 0x7F) {
			textCount++;
		}
		else if (text[i] & 0x80) {
			textCount++;
			i++;
		}
		i++;
	}

	return textCount;
}

bool GlyphExtent::GetOnIsGreaterThanScreen() {// (2020.01.22)
	bool ret = false;

	Long count = 0;

	Glyph* row;

	RECT rect;
	this->glyphSubject->GetClientRect(&rect);
	if (this->glyphSubject->onIsStatusBar == TRUE) {
		rect.bottom -= GetSystemMetrics(SM_CYHSCROLL);
	}

	CSize cSize;

	string str;
	const char(*text);

	CDC* dc = this->glyphSubject->GetDC();
	HFONT hFont;
	HFONT oldFont;
	hFont = CreateFontIndirect(&this->glyphSubject->font->GetLogFont());
	oldFont = (HFONT)SelectObject(*dc, hFont);

	Long width;
	Long k;
	Long j;
	Long i = 0;
	while (i < this->glyphSubject->note->GetLength() && this->height*  count < rect.bottom) {
		row = this->glyphSubject->note->GetAt(i);
		j = 0;
		while (j < row->GetLength()) {
			width = 0;
			k = j;
			while (k < row->GetLength() && width <= rect.right) {
				str = row->GetAt(k)->GetString();
				text = str.c_str();
				if (!(text[0] & 0x80)) {
					width += this->widths[(int)text[0]];
				}
				else if ((str >= "¤¡" && str <= "¤Ó") || (str >= "°¡" && str <= "ÆR")) {
					width += this->widths[ASCII - 1];
				}
				else {
					cSize = dc->GetTextExtent(text);
					width += cSize.cx;
				}
				k++;
			}
			if (width > rect.right) {
				count++;
				if (j != k - 1) {
					j = k - 2;
				}
				else {
					j = k - 1;
				}
			}
			j++;
		}
		count++;
		i++;
	}
	if (this->height*  count > rect.bottom) {
		ret = true;
	}

	SelectObject(*dc, oldFont);
	DeleteObject(hFont);

	this->glyphSubject->ReleaseDC(dc);

	return ret;
}

GlyphExtent& GlyphExtent::operator =(const GlyphExtent& source) {
	this->glyphSubject = source.glyphSubject;

	if (this->widths != 0) {
		delete this->widths;
	}

	this->widths = new Long[ASCII];
	Long i = 0;
	while (i < ASCII) {
		this->widths[i] = source.widths[i];
		i++;
	}
	this->height = source.height;

	return* this;
}