// Font.cpp
/*
���ϸ�Ī : Font.cpp
��� : �۲� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.07
*/
#include "Font.h"

Font::Font() {
	memset(&this->logFont, 0, sizeof(LOGFONT));
	memset(&this->color, 0, sizeof(COLORREF));
	this->pointSize = 0;
}

Font::Font(LOGFONT logFont, COLORREF color, LONG pointSize) {
	this->logFont = logFont;
	this->logFont.lfCharSet = HANGUL_CHARSET;
	this->color = color;
	this->pointSize = pointSize;
}

Font::Font(const Font& source) {
	this->logFont = source.logFont;
	this->color = source.color;
	this->pointSize = pointSize;
}

Font::~Font() {
}

Font& Font::operator =(const Font& source) {
	this->logFont = source.logFont;
	this->color = source.color;
	this->pointSize = pointSize;

	return* this;
}

LOGFONT Font::FindPrintingLogFont(CDC* printerDC) {
	LOGFONT logFont;
	bool isSame = false;

	// 1. ���Ŀ� ���� �α���Ʈ�� ���Ѵ�. -MulDiv(pointSize, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
	logFont = this->logFont;
	logFont.lfHeight = -MulDiv(this->pointSize, printerDC->GetDeviceCaps(LOGPIXELSY), 72);

	// 2. �μ��� �� ����ġ�ϴ��� Ȯ���Ѵ�.
	CRect rRect;
	CRect printRect(0, 0, printerDC->GetDeviceCaps(HORZRES), printerDC->GetDeviceCaps(VERTRES));

	HFONT hFont = CreateFontIndirect(&logFont);
	HFONT oldFont = (HFONT)printerDC->SelectObject(hFont);

	CSize iSize = printerDC->GetTextExtent("i");

	int iCount = printRect.Width() / iSize.cx;
	int compareICount = 0;

	char characterI[9999];
	int nHeight = iSize.cy;
	int i = 0;
	while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy) { // �μ��� ���� ���� - �� ������ �ʺ�(�� �°� ���� �� ���ڸ� �� ���� �ǹǷ� ���ߵ�)
		rRect.right = printRect.right;
		characterI[i] = 'i';
		characterI[i + 1] = '\0';
		i++;

		compareICount++;

		// �˻��� ���ڿ�, ���� ����, ���� Rect, Flag => ��ȯ�� ����
		nHeight = printerDC->DrawText(characterI, i, rRect, DT_CALCRECT);
	}
	if (compareICount == iCount) {
		isSame = true;
	}

	// 3. ����ġ�ϸ�
	if (isSame != true) {
		// 3.1. �� ġ�� �۰� �α���Ʈ�� ���Ѵ�.
		logFont.lfHeight = -MulDiv(this->pointSize - 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
	}

	// 4. ��ġ�ϸ�
	else {
		// 4.1. �� ġ�� ũ�� �α���Ʈ�� ���Ѵ�.
		logFont.lfHeight = -MulDiv(this->pointSize + 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
	}

	// 5. ��ġ�ϸ� �μ��� �� ����ġ�ϴ��� Ȯ���Ѵ�.
	printerDC->SelectObject(oldFont);
	DeleteObject(hFont);

	hFont = CreateFontIndirect(&logFont);
	oldFont = (HFONT)printerDC->SelectObject(hFont);

	iSize = printerDC->GetTextExtent("i");
	iCount = printRect.Width() / iSize.cx;
	rRect.right = 0;

	compareICount = 0;
	i = 0;
	while (rRect.right < printRect.Width() - iSize.cy && nHeight == iSize.cy && isSame == true) { // �μ��� ���� ���� - �� ������ �ʺ�(�� �°� ���� �� ���ڸ� �� ���� �ǹǷ� ���ߵ�)
		rRect.right = printRect.right;
		characterI[i] = 'i';
		characterI[i + 1] = '\0';
		i++;

		compareICount++;

		// �˻��� ���ڿ�, ���� ����, ���� Rect, Flag => ��ȯ�� ����
		nHeight = printerDC->DrawText(characterI, i, rRect, DT_CALCRECT);
	}
	if (compareICount == iCount) {
		isSame = true;
	}

	// 6. ����ġ�ϸ�
	if (isSame != true) {
		// 6.1. �� ġ�� �۰� �α���Ʈ�� ���Ѵ�.
		logFont.lfHeight = -MulDiv(this->pointSize - 1, printerDC->GetDeviceCaps(LOGPIXELSY), 72);
	}

	printerDC->SelectObject(oldFont);
	DeleteObject(hFont);

	return logFont;
}