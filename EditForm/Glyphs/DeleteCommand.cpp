// DeleteCommand.cpp
/*
파일명칭 : DeleteCommand.cpp
기능 : 오른쪽 문자 삭제 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.25
*/
#include "DeleteCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "resource.h"

DeleteCommand::DeleteCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->id = EditCommand::DELETECOMMAND;
	this->onIsNew = true;
	this->onIsSelected = false;
	this->onIsEnter = false;
}

DeleteCommand::~DeleteCommand() {
}

void DeleteCommand::Execute() {
	LONG noteCurrent;
	LONG noteLength;
	LONG rowCurrent;
	LONG rowLength;

	string text;

	Glyph* nextRow;
	Glyph* currentRow;
	Glyph* glyph;
	// + 선택된 곳이 있는지 확인한다.
	this->onIsSelected = this->glyphSubject->note->IsSelected();

	//1. 자동개행모드이고, 선택되어 있는지 확인한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 1.1. 선택된 줄들을 합친다.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 1.2. 현재 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}

	//2. 메모지와 줄의 위치와 사용량 값을 구한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	noteLength = this->glyphSubject->note->GetLength();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	rowCurrent = this->glyphSubject->current->GetCurrent();
	rowLength = this->glyphSubject->current->GetLength();

	// + 선택된 곳이 있으면 선택된 부분을 지운다.
	if (this->onIsSelected == true) {
		text = this->glyphSubject->note->GetSelectString();
		this->glyphSubject->note->RemoveSelect();
	}
	//3. rowCurrent 값이 row의 Length보다 크거나 같고 noteCurrent 값이 noteLength-1 보다 크면
	else if (rowCurrent >= rowLength && noteCurrent < noteLength - 1) {
		this->onIsEnter = true;
		text = "\n";
		//3.1. 현재줄을 가져온다.
		currentRow = this->glyphSubject->note->GetAt(noteCurrent);

		//3.2 다음줄을 가져온다.
		nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);

		//3.3. 현재줄에서 다음줄을 합친다.
		currentRow->Combine(nextRow);

		//3.4. 노트에서 noteCurrent+1번째 줄을 없앤다.
		this->glyphSubject->note->Remove(noteCurrent + 1);
		noteCurrent = this->glyphSubject->note->Previous();

		// 3.5. 현재 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	//4. rowCurrent 값이 row의 Length보다 작으면 
	else if (rowCurrent < rowLength) {
		// 4.1. 지울 글자를 읽는다.
		glyph = this->glyphSubject->current->GetAt(rowCurrent);
		text = glyph->GetString();

		// 4.2 rowCurrent번째 글자를 지운다.
		this->glyphSubject->current->Remove(rowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	if (text != "") {
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
		this->previousState = new State(rowCurrent, noteCurrent, text);
		this->onIsCommandSaving = true;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	CString title;
	this->glyphSubject->GetWindowText(title);
	if (title.GetAt(0) != '*') {
		title = "*" + title;
	}
	noteLength = this->glyphSubject->note->GetLength();
	rowLength = this->glyphSubject->current->GetLength();
	if ((noteLength > 1 || (noteLength <= 1 && rowLength > 0)) || this->onIsCommandSaving == true) {
		this->glyphSubject->SetWindowTextA(title);
	}
}

void DeleteCommand::UnExecute() {
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

void DeleteCommand::ReExecute() {
	Glyph* previousNote;
	Glyph* currentRow;
	Glyph* nextRow;

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

	// 3. 선택되어 있으면 내용을 선택한다.
	if (this->onIsSelected == true) {
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);
	}

	// 4. 자동개행모드이고 선택되어 있는지 확인한다.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 4.1. 선택된 줄들을 합쳐준다.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 4.2. 현재 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	// 5. 선택되어 있으면 선택된 내용을 지운다.
	if (this->onIsSelected == true) {
		this->glyphSubject->note->RemoveSelect();
	}
	// 6. 개행문자이면 줄을 합친다.
	else if (text == "\n") {
		//6.1. 현재줄을 가져온다.
		currentRow = this->glyphSubject->note->GetAt(noteCurrent);

		//6.2 다음줄을 가져온다.
		nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);

		//6.3. 현재줄에서 다음줄을 합친다.
		currentRow->Combine(nextRow);

		//6.4. 노트에서 noteCurrent+1번째 줄을 없앤다.
		this->glyphSubject->note->Remove(noteCurrent + 1);
		noteCurrent = this->glyphSubject->note->Previous();

		// 6.5. 현재 줄을 합친다.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	// 7. 개행문자가 아니면 위치의 글자를 지운다.
	else {
		// 7.2 rowCurrent번째 글자를 지운다.
		this->glyphSubject->current->Remove(rowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 8. 자동개행모드이면 자동개행한다.
	if (this->previousState != 0 && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

Long DeleteCommand::GetRowCurrent() {
	Long ret = -1;
	if (this->previousState != 0) {
		ret = this->previousState->GetRowCurrent();
	}

	return ret;
}

Long DeleteCommand::GetNoteCurrent() {
	Long ret = -1;
	if (this->previousState != 0) {
		ret = this->previousState->GetNoteCurrent();
	}

	return ret;
}

void DeleteCommand::SetOnIsNew(bool onIsNew) {
	this->onIsNew = onIsNew;
}