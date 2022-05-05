// CutCommand.cpp
/*
파일명칭 : CutCommand.cpp
기능 : 잘라낸다 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.25
*/
#include "CutCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Clipboard.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "resource.h"

CutCommand::CutCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CutCommand::~CutCommand() {
}

void CutCommand::Execute() {
	LONG rowCurrent;
	LONG noteCurrent;

	CString title;
	CString copyString;
	copyString = this->glyphSubject->note->GetSelectString().c_str();

	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (copyString != "" && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->CombineSelectedRows();
	}

	if (copyString != "") {
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);// 선택된 영역이 있어야 삭제하게끔 수정(2021.02.09)
		rowCurrent = this->glyphSubject->current->GetCurrent(); // 지우고 난뒤 현재 위치를 읽게끔 수정(2021.02.09)

		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
		this->previousState = new State(rowCurrent, noteCurrent, (LPCTSTR)copyString);
		this->glyphSubject->clipboard->Write(copyString);
		this->onIsCommandSaving = true;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAPLINE);
	}

	this->glyphSubject->GetWindowText(title);
	if (title.GetAt(0) != '*' && copyString != "") {
		title = "*" + title;
	}
	this->glyphSubject->SetWindowTextA(title);
}

void CutCommand::UnExecute() {
	Glyph* previousNote;

	Long rowCurrent;
	Long noteCurrent;

	if (this->previousState != 0) {
		// 2.1. 이전 상태 정보를 읽어온다.
		previousNote = this->previousState->GetNote();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		if (this->glyphSubject->onIsRowWrapping == TRUE) {// 자동개행모드이면 한 줄 자동개행한다.(2021.02.09)
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->note->Combine(noteCurrent);
		}

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 2.2. 이전 상태의 내용을 추가한다.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 2.3. 메모지에서 현재 줄을 구한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->previousState != 0 && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void CutCommand::ReExecute() {
	Glyph* previousNote;

	string text;

	Long rowCurrent;
	Long noteCurrent;
	Long textCount;

	// 1. 이전 상태 정보를 읽어온다.
	previousNote = this->previousState->GetNote();
	text = previousNote->GetString();
	rowCurrent = this->previousState->GetRowCurrent();
	noteCurrent = this->previousState->GetNoteCurrent();

	// 2. 읽어온 위치로 이동한다.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	// 3. 내용을 선택한다.
	textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
	this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

	// 4. 자동개행모드이고 선택되어 있으면 선택된 줄들을 합쳐준다.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->CombineSelectedRows();
	}
	
	this->glyphSubject->note->RemoveSelect();

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}