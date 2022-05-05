// CharacterCommand.cpp
/*
파일명칭 : CharacterCommand.cpp
기능 : 1바이트 글자 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
*/
#include "CharacterCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "GlyphFactory.h"
#include "resource.h"

CharacterCommand::CharacterCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->onIsEnter = false;
}

CharacterCommand::~CharacterCommand() {
}

void CharacterCommand::Execute() {
	GlyphFactory glyphFactory;
	Glyph* row;
	Glyph* glyph;

	CString title;
	string text;

	Long rowCurrent;
	Long noteCurrent;
	Long rowLength;
	//Long prevRowCurrent;// 현재 줄의 current는 변한적이 없으므로 구할 필요 없음
	Long prevNoteCurrent;

	// 1. 글자를 읽는다.
	TCHAR* letter = this->glyphSubject->GetLetter();

	// 2. 선택된 내용이 있는지 확인한다.
	this->onIsSelected = this->glyphSubject->note->IsSelected();

	// 3. 자동개행모드이면 선택여부를 확인한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 3.1. 선택되어 있으면 선택된 줄들을 합친다.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 3.2. 선택되어 있지 않으면 현재 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 4. 선택된 내용이 있으면
	if (this->onIsSelected == true) {
		// 4.1. 선택된 내용을 읽는다.
		text = this->glyphSubject->note->GetSelectString();

		// 4.2. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		this->previousState = new State(rowCurrent, noteCurrent, text);
		this->onIsMore = false;
	}

	// 5 개행 문자이면
	if (letter[0] == '\n' || letter[0] == '\r') {
		this->onIsEnter = true;
		// 5.1. 줄을 만든다.
		row = glyphFactory.Create((char*)&letter[0]);
		// 5.2. 현재 줄의 length와 current가 다르면 줄을 나눠준다.
		rowCurrent = this->glyphSubject->current->GetCurrent();
		rowLength = this->glyphSubject->current->GetLength();
		if (rowCurrent != rowLength) {
			this->glyphSubject->current->Split(row, rowCurrent);
		}
		// 5.3. 노트에서 새 줄을 끼워넣는다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		noteCurrent = this->glyphSubject->note->Add(noteCurrent + 1, row);

		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	// 6. 개행 문자가 아니면
	else if ((letter[0] >= 32 && letter[0] < 127) || letter[0] == 9) {
		// 6.1. 글자를 만든다.
		glyph = glyphFactory.Create((char*)&letter[0]);
		// 6.2. 현재줄의 current와 current의 length가 같으면 현재 줄에서 추가한다.
		if (this->glyphSubject->current->GetCurrent() == this->glyphSubject->current->GetLength()) {
			this->glyphSubject->current->Add(glyph);
		}
		else {
			//6.3.다르면 글자를 current번째에 끼워준다.
			this->glyphSubject->current->Add(this->glyphSubject->current->GetCurrent(), glyph);
		}
	}

	// 7. 현재 상태를 만든다.
	this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
	if (this->onIsEnter == true) {
		// 8. 개행문자이면
		// 8.1. 현재 위치를 저장한다.
		// prevRowCurrent = this->glyphSubject->current->GetCurrent();// 현재 줄의 current는 변한 적이 없으므로 구할 필요 없음
		prevNoteCurrent = this->glyphSubject->note->GetCurrent();

		// 8.2. 노트에서 이전으로 이동한다.
		noteCurrent = prevNoteCurrent - 1;
		this->glyphSubject->note->Move(noteCurrent);

		// 8.3. 줄에서 마지막 위치로 이동한다.
		row = this->glyphSubject->note->GetAt(noteCurrent);
		row->Last();

		// 8.4. 합쳐진 위치를 미리 구한다.
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		// 8.5. 다시 원래 위치로 돌아온다.
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		// this->glyphSubject->current->Move(prevRowCurrent); // 현재 줄의 current는 변한 적이 없음.
	}
	else {
		rowCurrent--;
	}

	if ((letter[0] == '\n' || letter[0] == '\r') || ((letter[0] >= 32 && letter[0] < 127) || letter[0] == 9)) {
		this->state = new State(rowCurrent, noteCurrent, letter);
	}

	// 8. 자동개행모드이면 자동개행 해준다.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	this->glyphSubject->onIsComposing = FALSE;

	this->glyphSubject->GetWindowText(title);
	if (this->state != 0 && title.GetAt(0) != '*') {
		title = "*" + title;
	}
	this->glyphSubject->SetWindowTextA(title);
}

void CharacterCommand::UnExecute() {
	// 1. 현재 상태 정보를 읽어온다.
	Glyph* previousNote;
	Glyph* currentNote = this->state->GetNote();

	string text = currentNote->GetString();

	Long textCount;
	Long rowCurrent = this->state->GetRowCurrent();
	Long noteCurrent = this->state->GetNoteCurrent();
	Long i = 0;

	// 2. 위치로 이동하고, 선택한다.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
	this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

	// + 자동개행모드이면 선택여부를 확인한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. 선택된 내용을 지운다.
	this->glyphSubject->note->RemoveSelect();
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 4. 이전 상태의 정보가 있으면
	if (this->previousState != 0) {
		// 4.1. 이전 상태 정보를 읽어온다.
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

		// 4.2. 이전 상태의 내용을 추가한다.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 4.3. 메모지에서 현재 줄을 구한다.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void CharacterCommand::ReExecute() {
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
		if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
			this->glyphSubject->note->CombineSelectedRows();
		}
		else if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
		}
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 1.3. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 2. 현재 상태 정보를 읽어온다.
	currentNote = this->state->GetNote();
	rowCurrent = this->state->GetRowCurrent();
	noteCurrent = this->state->GetNoteCurrent();

	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 5.4. 자동개행모드이면 한 줄을 합친다.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 3. 상태의 내용을 추가한다.
	currentNote->SelectAll(true);
	this->glyphSubject->note->Paste(currentNote);

	// 4. 메모지에서 현재 줄을 구한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

Long CharacterCommand::GetRowCurrent() {
	Long ret = -1;
	if (this->state != 0) {
		ret = this->state->GetRowCurrent();
	}

	return ret;
}

Long CharacterCommand::GetNoteCurrent() {
	Long ret = -1;
	if (this->state != 0) {
		ret = this->state->GetNoteCurrent();
	}

	return ret;
}