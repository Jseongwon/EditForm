// PasteCommand.cpp
/*
파일명칭 : PasteCommand.cpp
기능 : 잘라낸다 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.25
*/
#include "PasteCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Clipboard.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "GlyphFactory.h"
#include "resource.h"

PasteCommand::PasteCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

PasteCommand::~PasteCommand() {
}

void PasteCommand::Execute() {
	GlyphFactory glyphFactory;

	string text;

	bool onIsSelected;

	CString pasteString;
	CString title;

	LONG noteCurrent;
	LONG rowCurrent;
	LONG i = 0;

	// 1. 선택된 내용이 있는지 확인한다.
	onIsSelected = this->glyphSubject->note->IsSelected();

	// 2. 자동개행모드이면 선택된 내용이 있는지 확인한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && onIsSelected == true) {
		// 2.1. 선택된 내용이 있으면 선택된 줄들을 합친다.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 2.2. 선택된 내용이 없으면 한 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. 선택된 내용이 있으면
	if (onIsSelected == true) {
		// 3.1. 선택된 내용을 읽는다.
		text = this->glyphSubject->note->GetSelectString();

		// 3.2. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		this->previousState = new State(rowCurrent, noteCurrent, text);
	}

	// 2. 클립보드에서 내용을 읽어온다.
	pasteString = this->glyphSubject->clipboard->Read();

	// 3. 상태를 만든다.
	if (pasteString != "") {
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
		
		this->state = new State(rowCurrent, noteCurrent, (LPCTSTR)pasteString);

		this->onIsCommandSaving = true;
	}

	// 4. 메모지에서 내용을 추가한다.
	this->glyphSubject->note->Paste((LPCTSTR)pasteString);
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 5. 자동개행 모드이면 전체 자동개행한다.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	// 6. 별을 추가한다.
	this->glyphSubject->GetWindowText(title);
	if (title.GetAt(0) != '*') {
		title = "*" + title;
	}
	this->glyphSubject->SetWindowTextA(title);
}

void PasteCommand::UnExecute() {
	Glyph* previousNote;
	Glyph* currentNote;

	string text;

	Long textCount;
	Long rowCurrent;
	Long noteCurrent;
	Long i = 0;

	// 1. 현재 상태 정보가 있으면
	if (this->state != 0) {
		// 1.1. 현재 상태 정보를 읽어온다.
		currentNote = this->state->GetNote();
		text = currentNote->GetString();
		rowCurrent = this->state->GetRowCurrent();
		noteCurrent = this->state->GetNoteCurrent();

		// 1.2. 위치로 이동하고, 선택한다.
		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();

		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// + 자동개행모드이면 선택여부를 확인한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->CombineSelectedRows();
		}
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 1.3. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 2. 이전 상태의 정보가 있으면
	if (this->previousState != 0) {
		// 2.1. 이전 상태 정보를 읽어온다.
		previousNote = this->previousState->GetNote();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 자동개행모드이면 한 줄을 합친다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 2.2. 이전 상태의 내용을 추가한다.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 2.3. 메모지에서 현재 줄을 구한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void PasteCommand::ReExecute() {
	Glyph* previousNote;
	Glyph* currentNote;

	string text;

	Long textCount;
	Long rowCurrent;
	Long noteCurrent;
	Long i = 0;
	// 1. 이전 상태 정보가 있으면
	if (this->previousState != 0) {
		// 1.1.  이전 상태 정보를 읽어온다.
		previousNote = this->previousState->GetNote();
		text = previousNote->GetString();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();

		// 1.2. 매칭되는 글자를 선택한다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// + 자동개행모드이면 선택여부를 확인한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->CombineSelectedRows();
		}
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 1.3. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 2. 현재 상태 정보가 있으면
	if (this->state != 0) {
		// 2.1. 현재 상태 정보를 읽어온다.
		currentNote = this->state->GetNote();
		rowCurrent = this->state->GetRowCurrent();
		noteCurrent = this->state->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 2.2. 자동개행모드이면 한 줄을 합친다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 2.3. 상태의 내용을 추가한다.
		currentNote->SelectAll(true);
		this->glyphSubject->note->Paste(currentNote);

		// 2.4. 메모지에서 현재 줄을 구한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}