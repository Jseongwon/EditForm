// GlyphSubject.cpp
/*
���ϸ�Ī : GlyphSubject.cpp
��� : �޸���ö ������ Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.07
��ü���� ���� ���� :
�������̽� ���� ���� :
�����ؾ� �� ���� : ������ ���� �̵�, State �����, ���� ���� Command �߰��ϱ�
������ ���� : -
*/
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "GlyphVisitors.h"
#include "Command.h"
#include "EditCommand.h"
#include "CommandFactory.h"
#include "KeyActions.h"
#include "File.h"
#include "GlyphExtent.h"
#include "Font.h"
#include "CommandHistory.h"
#include "Clipboard.h"
#include "Scrolls.h"
#include "ScrollActions.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "resource.h"
#include <afxdlgs.h>
#include <afxext.h>

HHOOK hSaveMessageBoxHook;
static UINT NEAR WM_FIND = ::RegisterWindowMessage(FINDMSGSTRING);

GlyphSubject::GlyphSubject(CWnd* pParentWnd) {
	this->pFindReplaceDialog = NULL;
	this->note = NULL;
	this->current = NULL;
	this->font = NULL;
	this->glyphExtent = NULL;
	this->commandHistory = NULL;
	this->clipboard = NULL;
	this->contextMenu = NULL;
	this->onIsComposing = FALSE;
	this->onIsSaving = FALSE;
	this->onIsMoving = FALSE;
	this->onIsScrolling = FALSE;
	this->onIsRowWrapping = FALSE;
	this->onIsSaveCanceled = FALSE;
	this->onIsFound = FALSE;
	this->onIsOpeningFindReplace = FALSE;
	this->onIsIBeam = FALSE;
	this->onIsDraging = FALSE;
	this->onIsDoubleClicked = FALSE;
}

BOOL GlyphSubject::SetWrapStyle(BOOL isWrapped) {
	ScrollController* scrollController;

	LONG index;
	this->onIsRowWrapping = isWrapped;
	if (this->onIsRowWrapping == TRUE) {
		this->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
	else {
		this->note->CombineAll();
	}

	if (this->commandHistory != NULL) {
		delete this->commandHistory;
	}
	this->commandHistory = new CommandHistory(this);

	this->note->First();
	this->current = this->note->GetAt(0);
	this->current->First();

	index = this->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->GetAt(index));

	scrollController->GetVerticalScroll()->First();
	scrollController->GetHorizontalScroll()->First();

	this->onIsComposing = FALSE;

	return this->onIsRowWrapping;
}

BEGIN_MESSAGE_MAP(GlyphSubject, CWnd)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
	ON_COMMAND_RANGE(40001, 40037, OnRangeCmds)
	ON_UPDATE_COMMAND_UI(IDS_INDICATOR_POINT, OnUpdatePoint)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_REGISTERED_MESSAGE(WM_FIND, OnFindReplace)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL GlyphSubject::PreTranslateMessage(MSG* pMsg) {// �׼��������� �߰�(2021.01.22)
	// Ű���� �޽����� WM_COMMAND �޽����� �����Ͽ� �׼��������Ͱ� ������ �� �ֵ��� �Ѵ�.
	// ���� ���, ���� �׸��� ���� �׼��������Ϳ� Ctrl+A �� �ԷµǾ��� �� Ctrl+A �� �׼����������̱�
	// ������ Ű����κ��� �Է��� ���� �޾ұ� ������ WM_KEYDOWN �޽����� �߻��� ���̴�.
	// �׷��� WndProc �� WM_KEYDOWN �޽��� ó����ƾ���� ���� �� Ű���� ó���� ���� ���̴�.
	// TranslateAccelerator �Լ��� pMsg �� Ű���� �Է°��� �о� �� Ű���� hAccTable �� �ִ���
	// ���� ���캸�� ���̺� ���� ��� �� Ű�� �ش��ϴ� WM_COMMAND �޽����� hWnd �������
	// �����ϰ� TRUE �� ��ȯ�ϴ�.
	if (this->m_hAccelTable != NULL) {
		if (::TranslateAccelerator(this->m_hWnd, this->m_hAccelTable, pMsg)) {
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

int GlyphSubject::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	// �׼��������� �߰�(2021.01.22)
	this->m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	// ���ؽ�Ʈ �޴� �߰�(2021.02.02)
	this->contextMenu = new CMenu;
	this->contextMenu->CreatePopupMenu();
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_UNDO, "�������(&U)");
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_REDO, "�ٽý���(&R)");
	this->contextMenu->AppendMenu(MF_SEPARATOR);
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_CUT, "�߶󳻱�(&T)");
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_COPY), "����(&C)";
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_PASTE, "�ٿ��ֱ�(&P)");
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_DELETE, "����(&D)");
	this->contextMenu->AppendMenu(MF_SEPARATOR);
	this->contextMenu->AppendMenu(MF_STRING, IDM_EDIT_SELECT_ALL, "��� ����(&A)");

	// 1. ���̸� �����.
	GlyphFactory glyphFactory;
	this->note = glyphFactory.Create((char*)0);

	// 2. ���� �����.
	this->current = glyphFactory.Create((char*)"\r");

	// 3. ���̿��� ���� �߰��Ѵ�.
	this->note->Add(this->current);
	this->note->First();

	// 5. ���� ����� ���Ѵ�. 
	// + ������Ʈ���� ������ �������� ������ �⺻ ������ �����. (2021.01.08)
	this->onIsRowWrapping = !(BOOL)AfxGetApp()->GetProfileInt("EditSection", "OnIsWrap", 1);
	this->onIsStatusBar = !(BOOL)AfxGetApp()->GetProfileInt("EditSection", "OnIsStatusBar", 1);

	CString faceName;
	faceName = AfxGetApp()->GetProfileString("EditSection", "FaceName", "���� ���");

	LONG pointSize;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	strcpy_s(lf.lfFaceName, (LPCTSTR)faceName);
	lf.lfHeight = AfxGetApp()->GetProfileInt("EditSection", "LogFontHeight", -27); // -27�� 16��Ʈ
	lf.lfWidth = AfxGetApp()->GetProfileInt("EditSection", "LogFontWidth", 0);
	pointSize = AfxGetApp()->GetProfileInt("EditSection", "PointSize", 16);

	COLORREF color;
	memset(&color, 0, sizeof(COLORREF));
	color = GetSysColor(COLOR_DESKTOP);

	this->font = new Font(lf, color, pointSize);

	this->glyphExtent = new GlyphExtent(this);

	// + ��� ����� �����.
	this->commandHistory = new CommandHistory(this);

	// + �ʱ����� �����. (20201.01.19) Clipboard
	this->clipboard = new Clipboard(this);

	// 6. ��ũ�ѹٸ� �����.(2021.01.21)
	this->verticalScrollBar = new CScrollBar;
	this->verticalScrollBar->Create(SBS_VERT | SBPS_NORMAL | WS_CHILD, CRect(0, 0, 0, 0), this, NULL);
	this->horizontalScrollBar = new CScrollBar;
	this->horizontalScrollBar->Create(SBS_HORZ | SBPS_NORMAL | WS_CHILD, CRect(0, 0, 0, 0), this, NULL);
	this->sizeBox = new CScrollBar;
	this->sizeBox->Create(SBS_SIZEBOX | WS_CHILD, CRect(0, 0, 0, 0), this, NULL);

	// ��ũ�� ��Ʈ�ѷ��� �����.
	new ScrollController(this);

	// 7. ���¹ٸ� �����.
	static UINT indicators[] =
	{
		IDS_INDICATOR_VOID,
		IDS_INDICATOR_POINT,
		IDS_INDICATOR_ZOOM,
		IDS_INDICATOR_EOL,
		IDS_INDICATOR_ENCODING,
	};

	// 8. �ڵ������̸� �޴��� üũ���ش�.
	if (this->onIsRowWrapping == TRUE) {
		SetWrapStyle(this->onIsRowWrapping);
	}

	// �����Ѵ�. (2021.01.08), 
	this->Notify();

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return 0;
}

void GlyphSubject::OnContextMenu(CWnd* pCWnd, CPoint point) {
	// TODO: Add your message handler code here
	this->contextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd());
}

void GlyphSubject::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);
	LONG current;

	if (this->note != NULL && nType != SIZE_MINIMIZED) {
		if (this->onIsRowWrapping == TRUE) {
			this->note->CombineAll();
			current = this->note->GetCurrent();
			this->current = this->note->GetAt(current);
			this->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
		this->Notify();
	}

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnSetFocus(CWnd* pCWnd) {
	CWnd::OnSetFocus(pCWnd);
	LONG index = this->Find(Observer::CARET);
	if (this->glyphExtent != 0 && index <= -1) {
		new CaretController(this);

		this->Notify();
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void GlyphSubject::OnKillFocus(CWnd* pCWnd) {
	CWnd::OnKillFocus(pCWnd);
	CaretController* caretController;

	LONG index = this->Find(Observer::CARET);
	if (index > -1) {
		caretController = dynamic_cast<CaretController*>(this->GetAt(index));
		dynamic_cast<Subject*>(this)->Detach(caretController);
	}
}

BOOL GlyphSubject::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	BOOL ret = TRUE;
	if (this->onIsIBeam == TRUE) {
		SetCursor(LoadCursor(NULL, IDC_IBEAM));
	}
	else {
		ret = CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

	return ret;
}

void GlyphSubject::OnLButtonDown(UINT nFlags, CPoint point) {
	ScrollController* scrollController;

	LONG index;
	LONG rowCurrent = this->current->GetCurrent();
	LONG noteCurrent = this->note->GetCurrent();
	LONG startRowCurrent;
	LONG startNoteCurrent;
	LONG endRowCurrent;
	LONG endNoteCurrent;
	LONG firstRowCurrent = rowCurrent;
	LONG firstNoteCurrent = noteCurrent;

	this->prevPoint = point;

	// 1. ���õ� ù��° ��ġ�� ���Ѵ�.
	if ((GetKeyState(VK_SHIFT) & 0x8000)) {
		this->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);
		firstRowCurrent = startRowCurrent;
		firstNoteCurrent = startNoteCurrent;
		if (firstRowCurrent == rowCurrent && firstNoteCurrent == noteCurrent) {
			firstRowCurrent = endRowCurrent;
			firstNoteCurrent = endNoteCurrent;
		}
	}

	index = this->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->GetAt(index));

	noteCurrent = this->glyphExtent->GetNoteCurrent(point.y + scrollController->GetVerticalScroll()->GetPosition());
	this->note->Move(noteCurrent);
	this->current = this->note->GetAt(noteCurrent);

	rowCurrent = this->glyphExtent->GetRowCurrent(point.x + scrollController->GetHorizontalScroll()->GetPosition());
	this->current->Move(rowCurrent);

	this->onIsComposing = FALSE;

	this->note->SelectAll(false);
	if ((GetKeyState(VK_SHIFT) & 0x8000)) {
		this->note->Select(firstRowCurrent, firstNoteCurrent, rowCurrent, noteCurrent, true);
	}

	this->onIsDraging = TRUE;

	this->SetCapture();

	this->Notify();

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnLButtonDblClk(UINT nFlags, CPoint point) {
	LONG startRowCurrent;
	LONG startNoteCurrent;
	LONG rowCurrent;
	LONG rowLength;
	LONG noteCurrent;

	this->onIsComposing = FALSE;

	if (this->prevPoint == point) {
		rowLength = this->current->GetLength();
		if (this->onIsRowWrapping == TRUE) {
			noteCurrent = this->note->GetCurrent();
			this->note->Combine(noteCurrent);
			noteCurrent = this->note->GetCurrent();
			this->current = this->note->GetAt(noteCurrent);
		}
		if (rowLength > 0) {
			this->current->PreviousWord();

			startNoteCurrent = this->note->GetCurrent();
			startRowCurrent = this->current->GetCurrent();

			this->current->NextWord();

			noteCurrent = this->note->GetCurrent();
			rowCurrent = this->current->GetCurrent();

			this->note->SelectAll(false);
			this->note->Select(startRowCurrent, startNoteCurrent, rowCurrent, noteCurrent, true);
		}
		if (this->onIsRowWrapping == TRUE) {
			this->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}

		this->onIsComposing = FALSE;
		this->onIsDoubleClicked = TRUE;

		this->Notify();

		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void GlyphSubject::OnMouseMove(UINT nFlags, CPoint point) { // ��ũ�ѹٿ� ���¹ٸ� �� ������ ƴ���� �巡���ϸ� 1�پ� �̵��ϸ� ����
	CRect rect;
	CRect tempRect;

	ScrollController* scrollController;

	LONG index;
	LONG rowCurrent = this->current->GetCurrent();
	LONG noteCurrent = this->note->GetCurrent();
	LONG startRowCurrent;
	LONG startNoteCurrent;
	LONG endRowCurrent;
	LONG endNoteCurrent;
	LONG firstRowCurrent = rowCurrent;
	LONG firstNoteCurrent = noteCurrent;
	LONG x;

	UINT scrollCurrent;

	this->GetClientRect(&rect);

	if (this->onIsVerticalScrollBar == TRUE) {
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}
	if (this->onIsHorizontalScrollBar == TRUE && this->onIsRowWrapping != TRUE) {
		rect.bottom -= GetSystemMetrics(SM_CYHSCROLL);
	}
	if (this->onIsStatusBar == TRUE) {
		rect.bottom -= GetSystemMetrics(SM_CYHSCROLL);
	}

	tempRect.left = rect.left;
	tempRect.top = rect.top;
	tempRect.right = rect.right - 2;
	tempRect.bottom = rect.bottom - 2;

	if (tempRect.PtInRect(point) != 0) {
		this->onIsIBeam = TRUE;
	}
	else {
		this->onIsIBeam = FALSE;
	}

	this->SendMessage(WM_SETCURSOR, 0, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));

	if (this->onIsDraging == TRUE) { // ĳ���� �̵���Ű�� �� ĳ�� ��ġ�� �����ϸ� ��ũ�ѵǴ� ����.
		this->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);
		firstRowCurrent = startRowCurrent;
		firstNoteCurrent = startNoteCurrent;
		if (firstRowCurrent == rowCurrent && firstNoteCurrent == noteCurrent) {
			firstRowCurrent = endRowCurrent;
			firstNoteCurrent = endNoteCurrent;
		}

		index = this->Find(Observer::SCROLL);
		scrollController = dynamic_cast<ScrollController*>(this->GetAt(index));

		x = this->glyphExtent->GetX(rowCurrent) - scrollController->GetHorizontalScroll()->GetPosition();
		if (x > point.x && (rect.left > point.x || rect.right < point.x)) {
			scrollCurrent = scrollController->GetHorizontalScroll()->GetPosition() - this->glyphExtent->GetWidth(128) / 2*  3;
			if (rect.right < point.x) {
				scrollCurrent = scrollController->GetHorizontalScroll()->GetPosition() + this->glyphExtent->GetWidth(128) / 2*  3;
			}
			this->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, scrollCurrent));
		}
		if (rect.top > point.y || rect.bottom < point.y) {
			scrollCurrent = scrollController->GetVerticalScroll()->GetPosition() - this->glyphExtent->GetHeight()*  2;
			if (rect.bottom < point.y) {
				scrollCurrent = scrollController->GetVerticalScroll()->GetPosition() + this->glyphExtent->GetHeight()*  2;
			}
			this->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, scrollCurrent));
		}

		noteCurrent = this->glyphExtent->GetNoteCurrent(point.y + scrollController->GetVerticalScroll()->GetPosition());
		this->note->Move(noteCurrent);
		this->current = this->note->GetAt(noteCurrent);

		rowCurrent = this->glyphExtent->GetRowCurrent(point.x + scrollController->GetHorizontalScroll()->GetPosition());
		this->current->Move(rowCurrent);

		this->note->SelectAll(false);
		this->note->Select(firstRowCurrent, firstNoteCurrent, rowCurrent, noteCurrent, true);

		this->Notify();

		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void GlyphSubject::OnNcMouseMove(UINT nHitTest, CPoint point) {
	CWnd::OnNcMouseMove(nHitTest, point);
	this->onIsIBeam = FALSE;
	this->SendMessage(WM_SETCURSOR, 0, MAKELPARAM(nHitTest, WM_NCMOUSEMOVE));
}

void GlyphSubject::OnLButtonUp(UINT nFlags, CPoint point) {
	ReleaseCapture();

	this->onIsDraging = FALSE;
	this->onIsDoubleClicked = FALSE;
}

BOOL GlyphSubject::OnMouseWheel(UINT nFlags, short zDelta, CPoint point) {
	SCROLLINFO scrollInfo;

	ScrollController* scrollController;

	LONG index;
	LONG scrollCurrent;

	index = this->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->GetAt(index));

	scrollCurrent = scrollController->GetVerticalScroll()->GetPosition() - zDelta;

	this->verticalScrollBar->GetScrollInfo(&scrollInfo);
	scrollInfo.nPos = scrollCurrent;
	this->verticalScrollBar->SetScrollInfo(&scrollInfo);

	this->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, scrollCurrent));

	this->onIsScrolling = TRUE;
	this->Notify();
	this->onIsScrolling = FALSE;

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return TRUE;
}

void GlyphSubject::OnMouseHWheel(UINT nFlags, short zDelta, CPoint point) {
	SCROLLINFO scrollInfo;

	ScrollController* scrollController;

	LONG index;
	LONG scrollCurrent;

	index = this->Find(Observer::SCROLL);
	scrollController = dynamic_cast<ScrollController*>(this->GetAt(index));

	scrollCurrent = scrollController->GetHorizontalScroll()->GetPosition() + zDelta;

	this->horizontalScrollBar->GetScrollInfo(&scrollInfo);
	scrollInfo.nPos = scrollCurrent;
	this->horizontalScrollBar->SetScrollInfo(&scrollInfo);

	this->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, scrollCurrent));

	this->onIsScrolling = TRUE;
	this->Notify();
	this->onIsScrolling = FALSE;

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnRangeCmds(UINT nId) {
	CommandFactory commandFactory(this);
	Command* command;

	if ((nId == IDM_EDIT_FINDNEXT || nId == IDM_EDIT_FINDPREV) && this->onIsOpeningFindReplace != TRUE) {
		nId = IDM_EDIT_FIND;
		AfxGetApp()->WriteProfileString("EditSection", "FindString", "");
		this->onIsOpeningFindReplace = TRUE;
	}

	command = commandFactory.Create(nId);
	command->Execute();
	if (dynamic_cast<EditCommand*>(command) &&
		dynamic_cast<EditCommand*>(command)->GetOnIsCommandSaving() == true) {
		this->commandHistory->Save(command);
	}
	else if (command != NULL) {
		delete command;
	}

	this->onIsMoving = TRUE;
	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnUpdatePoint(CCmdUI* pCmdUI) {
	pCmdUI->Enable();

	Glyph* it;

	Long noteCurrent = this->note->GetCurrent();
	Long rowCurrent = 0;
	Long k = 0;
	Long j = 0;
	Long i = 0;
	while (i <= this->note->GetCurrent() && this->onIsRowWrapping == TRUE) {
		it = this->note->GetAt(i);
		if (it->GetOnIsRow() == true) {
			noteCurrent = j;
			j++;
			k = i;
		}
		i++;
	}

	while (k < this->note->GetCurrent() && this->onIsRowWrapping == TRUE) {
		it = this->note->GetAt(k);
		rowCurrent += it->GetLength();
		k++;
	}
	rowCurrent += this->current->GetCurrent();

	CString strPage;
	strPage.Format("Ln %d, Col %d", noteCurrent + 1, rowCurrent + 1);
	pCmdUI->SetText(strPage);
}

void GlyphSubject::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollActionFactory scrollActionFactory(this);
	ScrollAction* scrollAction;
	scrollAction = scrollActionFactory.Create(nSBCode);

	SCROLLINFO scrollInfo;
	this->verticalScrollBar->GetScrollInfo(&scrollInfo);
	nPos = scrollInfo.nTrackPos;
	if (scrollAction != 0) {
		scrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		delete scrollAction;
	}

	this->onIsScrolling = TRUE;
	this->Notify();
	this->onIsScrolling = FALSE;

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	ScrollActionFactory scrollActionFactory(this);
	ScrollAction* scrollAction;
	scrollAction = scrollActionFactory.Create(nSBCode);

	SCROLLINFO scrollInfo;
	this->horizontalScrollBar->GetScrollInfo(&scrollInfo);
	nPos = scrollInfo.nTrackPos;
	if (scrollAction != 0) {
		scrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		delete scrollAction;
	}

	this->onIsScrolling = TRUE;
	this->Notify();
	this->onIsScrolling = FALSE;

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory(this);
	KeyAction* keyAction;
	keyAction = keyActionFactory.Create(nChar);

	Long startRowCurrent;
	Long startNoteCurrent;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long firstRowCurrent;
	Long firstNoteCurrent;
	Long rowCurrent = this->current->GetCurrent();
	Long noteCurrent = this->note->GetCurrent();

	// 1. ���õ� ù��° ��ġ�� ���Ѵ�.
	if ((GetKeyState(VK_SHIFT) & 0x8000) && !(GetKeyState(VK_CONTROL) & 0x8000) ||
		((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000) && (nChar >= 0x21 && nChar <= 0x28))) {
		this->note->GetSelectRange(&startRowCurrent, &startNoteCurrent, &endRowCurrent, &endNoteCurrent);
		firstRowCurrent = startRowCurrent;
		firstNoteCurrent = startNoteCurrent;
		if (firstRowCurrent == rowCurrent && firstNoteCurrent == noteCurrent) {
			firstRowCurrent = endRowCurrent;
			firstNoteCurrent = endNoteCurrent;
		}
	}

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		delete keyAction;
	}

	// 2. ������ �����ϰ� 3. ShiftŰ�� ������ ������ ù��° ��ġ�� �̵��� ��ġ���� �����Ѵ�.
	if ((nChar >= 0x21 && nChar <= 0x28) || !(GetKeyState(VK_CONTROL) & 0x8000) && !(nChar == '\t' || nChar == '\n' || nChar == '\r' ||
		(nChar >= 32 && nChar < 127) || nChar & 0x80)) {
		this->note->SelectAll(false);
		this->onIsMoving = TRUE;
	}
	if ((GetKeyState(VK_SHIFT) & 0x8000) && !(GetKeyState(VK_CONTROL) & 0x8000) ||
		((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000) && (nChar >= 0x21 && nChar <= 0x28))) {
		rowCurrent = this->current->GetCurrent();
		noteCurrent = this->note->GetCurrent();
		this->note->Select(firstRowCurrent, firstNoteCurrent, rowCurrent, noteCurrent, true);
	}

	this->Notify();
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void GlyphSubject::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->letter[0] = nChar;
	this->letter[1] = '\0';

	this->SendMessage(WM_COMMAND, IDM_EDIT_CHARACTER);

	this->Notify();

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

LRESULT GlyphSubject::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	CString title;

	this->letter[0] = '\0';
	this->letter[1] = '\0';

	this->SendMessage(WM_COMMAND, IDM_EDIT_IMECHARACTER);

	//1. ���ڰ� �ۼ��Ǹ� Ÿ��Ʋ �տ�* �� �ٿ��ش�.(2020.01.07 �߰�)
	this->GetWindowText(title);
	if (title.GetAt(0) != '*') {
		title = "*" + title;
	}
	this->SetWindowTextA(title);

	return 0;
}

LRESULT GlyphSubject::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	// 1. ���ڸ� �����.
	GlyphFactory glyphFactory;
	Glyph* letter;
	TCHAR text[3];

	Long current;

	bool onIsAllWrapping = false;

	if (this->onIsRowWrapping == TRUE) {
		current = this->note->GetCurrent();
		this->note->Combine(current);
		current = this->note->GetCurrent();
		this->current = this->note->GetAt(current);
	}

	text[0] = HIBYTE(wParam);
	text[1] = LOBYTE(wParam);
	text[2] = 0x00;

	// 2. ���� ���̸� ���� �ٿ��� �����.
	if (this->onIsComposing == TRUE) {
		this->current->Remove(this->current->GetCurrent() - 1);
	}
	// 3. �޸����� �ٿ��� �߰��Ѵ�.
	if (text[0] != '\0') {
		letter = glyphFactory.Create(text);
		// 3.1. �������� current�� current�� length�� ������ ���� �ٿ��� �߰��Ѵ�.(2021.01.10 �߰�)
		if (this->current->GetCurrent() == this->current->GetLength()) {
			this->current->Add(letter);
		}
		else {
			//3.2. �ٸ��� ���ڸ� current��°�� �����ش�.
			this->current->Add(this->current->GetCurrent(), letter);
		}

		this->onIsComposing = TRUE;
	}
	else {
		this->onIsComposing = FALSE;
	}

	if (this->onIsRowWrapping == TRUE) {
		this->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	this->Notify();

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT GlyphSubject::OnImeChar(WPARAM wParam, LPARAM lParam) {
	// 1. ���ڸ� �д´�.
	this->letter[0] = HIBYTE(wParam);
	this->letter[1] = LOBYTE(wParam);

	this->SendMessage(WM_COMMAND, IDM_EDIT_IMECHARACTER);

	// 2. �����Ѵ�.
	this->Notify();

	// 3. �ϼ��� ���ڸ� �����ش�.
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	return 0;
}

LRESULT GlyphSubject::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);

	//CFindReplaceDialog* pDlg = CFindReplaceDialog::GetNotifier(lParam);
	this->pFindReplaceDialog = CFindReplaceDialog::GetNotifier(lParam);
	CString csFindString;
	CString csReplaceString;

	BOOL onIsDown;

	if (this->pFindReplaceDialog != NULL) {
		// Use pDlg as a pointer to the existing FindReplace dlg to
		// call CFindReplaceDialog member functions
		csFindString = this->pFindReplaceDialog->GetFindString();
		csReplaceString = this->pFindReplaceDialog->GetReplaceString();

		AfxGetApp()->WriteProfileString("EditSection", "FindString", csFindString);
		AfxGetApp()->WriteProfileString("EditSection", "ReplaceString", csReplaceString);
		if (this->pFindReplaceDialog->IsTerminating() == 0) {
			if (this->pFindReplaceDialog->ReplaceCurrent() == 0 && this->pFindReplaceDialog->ReplaceAll() == 0) {
				onIsDown = this->pFindReplaceDialog->SearchDown();
				if (onIsDown != 0) {
					this->SendMessage(WM_COMMAND, IDM_EDIT_FINDNEXT);
				}
				else {
					this->SendMessage(WM_COMMAND, IDM_EDIT_FINDPREV);
				}
			}
			else {
				this->SendMessage(WM_COMMAND, IDM_EDIT_REPLACE);
			}
			this->pFindReplaceDialog->ShowWindow(SW_HIDE);
			this->pFindReplaceDialog->ShowWindow(SW_SHOW);
		}
		else {
			this->pFindReplaceDialog = NULL;
			this->onIsFound = TRUE;
		}
	}
	else {
		this->onIsFound = TRUE;
	}

	this->Notify();

	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	CString findMessage;
	if (this->onIsFound != TRUE) {
		findMessage.Format("\"%s\"��(��) ã�� �� �����ϴ�.", (LPCTSTR)csFindString);
		this->MessageBox((LPCTSTR)findMessage, "�޸���", MB_OK);
		::SetFocus(this->pFindReplaceDialog->GetSafeHwnd());
	}

	return 0;
}

void GlyphSubject::OnPaint() {
	CPaintDC dc(this);

	GlyphDrawingVisitor glyphDrawingVisitor(this, &dc);

	this->note->Accept(&glyphDrawingVisitor);
}

void GlyphSubject::OnClose() {
	// ������Ʈ���� �������� �����Ѵ�. (2021.02.01)
	AfxGetApp()->WriteProfileString("EditSection", "FaceName", this->font->GetLogFont().lfFaceName);
	AfxGetApp()->WriteProfileInt("EditSection", "LogFontHeight", this->font->GetLogFont().lfHeight);
	AfxGetApp()->WriteProfileInt("EditSection", "LogFontWidth", this->font->GetLogFont().lfWidth);
	AfxGetApp()->WriteProfileInt("EditSection", "PointSize", this->font->GetPointSize());
	AfxGetApp()->WriteProfileInt("EditSection", "OnIsWrap", !this->onIsRowWrapping);
	AfxGetApp()->WriteProfileInt("EditSection", "OnIsStatusBar", !this->onIsStatusBar);

	if (this->m_hAccelTable != NULL) {
		DestroyAcceleratorTable(this->m_hAccelTable);
	}
	if (this->contextMenu != NULL) {
		delete this->contextMenu;
	}
	if (this->verticalScrollBar != NULL) {
		delete this->verticalScrollBar;
	}
	if (this->horizontalScrollBar != NULL) {
		delete this->horizontalScrollBar;
	}
	if (this->sizeBox != NULL) {
		delete this->sizeBox;
	}
	if (this->note != NULL) {
		delete this->note;
	}
	if (this->font != NULL) {
		delete this->font;
	}
	if (this->glyphExtent != NULL) {
		delete this->glyphExtent;
	}
	if (this->commandHistory != NULL) {
		delete this->commandHistory;
	}
	if (this->clipboard != NULL) {
		delete this->clipboard;
	}

	CWnd::OnClose();
	DestroyWindow();
	delete this;
}