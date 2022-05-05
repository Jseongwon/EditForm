// DateCommand.cpp
/*
파일명칭 : DateCommand.cpp
기능 : 날짜 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.02.01
*/
#include"DateCommand.h"
#include"State.h"
#include"GlyphSubject.h"
#include"Glyph.h"
#include"GlyphExtent.h"
#include"resource.h"

DateCommand::DateCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

DateCommand::~DateCommand() {
}
void DateCommand::Execute() { //삭제할 때
	LONG noteCurrent;
	LONG rowCurrent;

	char* charData = 0;

	CString dateTime;
	CString title;

	CTime time;

	//자동개행부분 수정!
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}


	noteCurrent = this->glyphSubject->note->GetCurrent();
	rowCurrent = this->glyphSubject->current->GetCurrent();

	time = CTime::GetCurrentTime();
	dateTime = time.Format("[%Y-%m-%d, %H:%M]");
	charData = dateTime.GetBuffer();

	//불러온 문자열 데이터를 이제 current 위치에 집어넣는다.
	if (charData[0] != '\0') {
		this->previousState = new State(rowCurrent, noteCurrent, charData);
		this->glyphSubject->note->Paste(charData);
		this->onIsCommandSaving = true;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	this->glyphSubject->GetWindowText(title);
	if (title.GetAt(0) != '*') {
		title = "*" + title;
	}
	this->glyphSubject->SetWindowTextA(title);
}

void DateCommand::UnExecute() {//삭제한 것을 실행취소할 때
	Glyph* previousNote;

	LONG noteCurrent;
	LONG rowCurrent;
	LONG textCount;

	CString text;

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->previousState != 0) {
		noteCurrent = this->previousState->GetNoteCurrent();
		rowCurrent = this->previousState->GetRowCurrent();
		previousNote = this->previousState->GetNote();
		text = CString(previousNote->GetString().c_str());

		this->glyphSubject->note->Move(noteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Move(rowCurrent);

		textCount = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)text);

		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);
		this->glyphSubject->note->RemoveSelect();

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Move(rowCurrent);
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void DateCommand::ReExecute() {//되돌리기 할 때
	Glyph* previousNote;
	Long noteCurrent;
	Long rowCurrent;

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}


	if (this->previousState != 0) {
		noteCurrent = this->previousState->GetNoteCurrent();
		rowCurrent = this->previousState->GetRowCurrent();
		previousNote = this->previousState->GetNote();

		this->glyphSubject->note->Move(noteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->Move(rowCurrent);

		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}