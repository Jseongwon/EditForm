// ReplaceCommand.cpp
/*
파일명칭 : ReplaceCommand.cpp
기능 : 바꾸기 명령 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.22
*/
#include "ReplaceCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "GlyphFactory.h"
#include "FindReplaceDialog.h"
//#include <FindreplaceString.dlg>

ReplaceCommand::ReplaceCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->state = 0;
}

ReplaceCommand::~ReplaceCommand() {
}

void ReplaceCommand::Execute() {
	CString findString = AfxGetApp()->GetProfileString("EditSection", "FindString", "");
	CString replaceString = AfxGetApp()->GetProfileString("EditSection", "ReplaceString", "");
	CString findMessage;
	CString previousreplaceString;
	CString title;

	LONG findStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)findString);
	LONG replaceStringLen = this->glyphSubject->glyphExtent->GetStringCount((LPSTR)(LPCTSTR)replaceString);
	LONG prevRowCurrent = 0;
	LONG prevNoteCurrent = 0;
	LONG stateRowCurrent = 0;
	LONG stateNoteCurrent = 0;
	LONG startRowCurrent = 0;
	LONG startNoteCurrent = 0;
	LONG rowCurrent = 0;
	LONG noteCurrent = 0;

	Glyph* line = 0;

	this->onIsMatchCase = AfxGetApp()->GetProfileInt("EditSection", "MatchCase", -1);
	this->onIsWrapAround = AfxGetApp()->GetProfileInt("EditSection", "WrapAround", -1);

	bool onIsReplaced = false;
	bool onIsFindingNext = true; // 다음 찾을지 여부.

	// 1. 현재 선택된 것을 바꾸는지, 모두 바꾸는지 여부를 확인한다.
	if (this->glyphSubject->pFindReplaceDialog != NULL) {
		this->onIsReplacedCurrent = true;
		if (this->glyphSubject->pFindReplaceDialog->ReplaceAll() != 0) {
			this->onIsReplacedCurrent = false;
		}
		this->onIsMatchCase = ((FindReplaceDialog*)this->glyphSubject->pFindReplaceDialog)->MatchCase();
		this->onIsWrapAround = !((FindReplaceDialog*)this->glyphSubject->pFindReplaceDialog)->WrapAround();
	}

	// 2. 선택된 내용을 읽는다.
	previousreplaceString = this->glyphSubject->note->GetSelectString().c_str();

	// 3. 찾는 내용과 선택된 내용이 같고, 현재 선택된 것을 바꿔야하면
	if (findString == previousreplaceString && findString != "" && this->onIsReplacedCurrent == true) {
		onIsReplaced = true; // 바뀐 여부를 참으로 한다.

		// 3.1. 선택된 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.2. 자동개행모드이면 메모지에서 현재 줄을 자동개행 푼다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.3. 찾는 내용을 추가한다.
		// + 상태의 위치를 구한다 (2021.01.28)
		startNoteCurrent = noteCurrent;
		startRowCurrent = this->glyphSubject->current->GetCurrent();
		this->glyphSubject->note->Paste((LPCTSTR)replaceString);
		this->glyphSubject->note->SelectCount(startRowCurrent, startNoteCurrent, replaceStringLen);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.4. 자동개행모드이면 한 줄 자동개행 한다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAPLINE);
		}
	}
	// 4. 전체 바꾸기이면
	if (this->onIsReplacedCurrent != true) {
		// 4.1. 처음 위치로 이동한다.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();

		// 4.2. 메모지에서 다음으로 찾는다.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 4.3. 찾은 내용이 있을 동안 반복한다.
		while (this->glyphSubject->onIsFound == TRUE) {
			onIsReplaced = true;
			onIsFindingNext = false;

			// 4.3.1. 선택된 내용을 지운다.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			rowCurrent = this->glyphSubject->current->GetCurrent();

			// 4.3.2. 자동개행모드이면 메모지에서 현재 줄을 자동개행 푼다.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. 찾는 내용을 추가한다.
			this->glyphSubject->note->Paste((LPCTSTR)replaceString);
			this->glyphSubject->current->Move(rowCurrent);
			this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, replaceStringLen);

			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.4. 메모지에서 다음으로 찾는다.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}
		// 4.4. 바꾼 여부가 있으면
		if (onIsReplaced == true) {
			// 4.4.1. 자동개행 모드이면 전체 자동개행 한다.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
			}
			// 4.4.2. 전체 선택 해제한다.
			this->glyphSubject->note->SelectAll(false);
			// 4.4.3. 찾은 여부를 참으로 한다.
			this->glyphSubject->onIsFound = TRUE;
		}
		// 4.5. 처음 위치로 이동한다.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();
	}

	// 5. 다음으로 찾을지 여부가 참이면 메모지에서 다음으로 찾는다.
	if (findString != "" && onIsFindingNext == true) {
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	// 6. 현재 위치를 저장한다.
	prevRowCurrent = this->glyphSubject->current->GetCurrent();
	prevNoteCurrent = this->glyphSubject->note->GetCurrent();
	// 7. 찾지 못하고 (전체 바꾸기가 아니고) 주위의 배치 여부가 참이면 (2021.02.18)
	if (findString != "" && this->glyphSubject->onIsFound != TRUE && this->onIsReplacedCurrent == true && this->onIsWrapAround == 0) {
		// 7.1. 처음 위치의 첫 번째 칸으로 이동한다.
		noteCurrent = this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->First();
		// 7.2. 메모지에서 다음으로 찾는다.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
	}
	// 8. 찾지 못하면 원래 위치로 돌아간다.
	if (this->glyphSubject->onIsFound != TRUE) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	// 9. 바뀐 여부가 있으면 이전 상태와 현재 상태를 만든다.
	if (onIsReplaced == true) {
		stateRowCurrent = 0;
		stateNoteCurrent = 0;
		if (this->onIsReplacedCurrent == true) {
			prevRowCurrent = this->glyphSubject->current->GetCurrent();
			prevNoteCurrent = this->glyphSubject->note->GetCurrent();

			this->glyphSubject->note->Move(startNoteCurrent);
			line = this->glyphSubject->note->GetAt(startNoteCurrent);
			line->Move(startRowCurrent);

			this->glyphSubject->note->GetCombineCurrent(&stateRowCurrent, &stateNoteCurrent);

			this->glyphSubject->note->Move(prevNoteCurrent);
			this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
			this->glyphSubject->current->Move(prevRowCurrent);
		}

		this->previousState = new State(stateRowCurrent, stateNoteCurrent, (LPCTSTR)findString);
		this->state = new State(stateRowCurrent, stateNoteCurrent, (LPCTSTR)replaceString);
		this->onIsCommandSaving = true;

		this->glyphSubject->GetWindowText(title);
		if (title.GetAt(0) != '*') {
			title = "*" + title;
		}
		this->glyphSubject->SetWindowTextA(title);
	}
}

void ReplaceCommand::UnExecute() {
	string previousString;
	string currentString;

	LONG textCount;
	LONG noteCurrent;
	LONG rowCurrent;

	// 1. 현재 상태를 읽는다.
	currentString = this->state->GetNote()->GetString();
	noteCurrent = this->state->GetNoteCurrent();
	rowCurrent = this->state->GetRowCurrent();

	// 2. 이전 상태를 읽는다.
	previousString = this->previousState->GetNote()->GetString();

	// + 위치로 이동한다.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. 한 단어를 바꿨으면
	if (this->onIsReplacedCurrent == true) {
		// 3.1. 자동개행 모드이면 한 줄을 합친다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.2. 현재 내용의 글자 수를 구한다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());

		// 3.3. 글자만큼 선택한다.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.4. 선택한 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();
		
		// 3.5. 메모지에 추가한다.
		this->glyphSubject->note->Paste(previousString);

		// 3.6. 글자만큼 선택한다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)previousString.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.7. 자동개행모드이면 한줄자동개행한다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
	}

	// 4. 전체 바꾸기이면
	if (this->onIsReplacedCurrent != true) {
		// 4.2. 메모지에서 다음으로 찾는다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);

		// 4.3. 찾은 내용이 있을 동안 반복한다.
		while (this->glyphSubject->onIsFound == TRUE) {
			// 4.3.1. 선택된 내용을 지운다.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.2. 자동개행모드이면 메모지에서 현재 줄을 자동개행 푼다.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. 찾는 내용을 추가한다.
			this->glyphSubject->note->Paste(previousString);

			// 4.3.4. 메모지에서 다음으로 찾는다.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);
		}
		
		// 4.4. 자동개행 모드이면 전체 자동개행 한다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
		// 4.5. 전체 선택 해제한다.
		this->glyphSubject->note->SelectAll(false);
		
		// 4.6. 처음 위치로 이동한다.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();
	}
}

void ReplaceCommand::ReExecute() {
	string previousString;
	string currentString;

	LONG textCount;
	LONG noteCurrent;
	LONG rowCurrent;

	// 1. 현재 상태를 읽는다.
	currentString = this->previousState->GetNote()->GetString();
	noteCurrent = this->previousState->GetNoteCurrent();
	rowCurrent = this->previousState->GetRowCurrent();

	// 2. 이전 상태를 읽는다.
	previousString = this->state->GetNote()->GetString();

	// + 위치로 이동한다.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. 한 단어를 바꿨으면
	if (this->onIsReplacedCurrent == true) {
		// 3.1. 자동개행 모드이면 한 줄을 합친다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.2. 현재 내용의 글자 수를 구한다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());

		// 3.3. 글자만큼 선택한다.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.4. 선택한 내용을 지운다.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();

		// 3.5. 메모지에 추가한다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)previousString.c_str());
		this->glyphSubject->note->Paste(previousString);

		// 3.6. 글자만큼 선택한다.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.7. 자동개행모드이면 한줄자동개행한다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
	}

	// 4. 전체 바꾸기이면
	if (this->onIsReplacedCurrent != true) {
		// 4.2. 메모지에서 다음으로 찾는다.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);

		// 4.3. 찾은 내용이 있을 동안 반복한다.
		while (this->glyphSubject->onIsFound == TRUE) {
			// 4.3.1. 선택된 내용을 지운다.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.2. 자동개행모드이면 메모지에서 현재 줄을 자동개행 푼다.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. 찾는 내용을 추가한다.
			this->glyphSubject->note->Paste(previousString);

			// 4.3.4. 메모지에서 다음으로 찾는다.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);
		}

		// 4.4. 자동개행 모드이면 전체 자동개행 한다.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
		// 4.5. 전체 선택 해제한다.
		this->glyphSubject->note->SelectAll(false);

		// 4.6. 처음 위치로 이동한다.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();
	}
}