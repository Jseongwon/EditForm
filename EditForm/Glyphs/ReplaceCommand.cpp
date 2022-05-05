// ReplaceCommand.cpp
/*
���ϸ�Ī : ReplaceCommand.cpp
��� : �ٲٱ� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.22
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
	bool onIsFindingNext = true; // ���� ã���� ����.

	// 1. ���� ���õ� ���� �ٲٴ���, ��� �ٲٴ��� ���θ� Ȯ���Ѵ�.
	if (this->glyphSubject->pFindReplaceDialog != NULL) {
		this->onIsReplacedCurrent = true;
		if (this->glyphSubject->pFindReplaceDialog->ReplaceAll() != 0) {
			this->onIsReplacedCurrent = false;
		}
		this->onIsMatchCase = ((FindReplaceDialog*)this->glyphSubject->pFindReplaceDialog)->MatchCase();
		this->onIsWrapAround = !((FindReplaceDialog*)this->glyphSubject->pFindReplaceDialog)->WrapAround();
	}

	// 2. ���õ� ������ �д´�.
	previousreplaceString = this->glyphSubject->note->GetSelectString().c_str();

	// 3. ã�� ����� ���õ� ������ ����, ���� ���õ� ���� �ٲ���ϸ�
	if (findString == previousreplaceString && findString != "" && this->onIsReplacedCurrent == true) {
		onIsReplaced = true; // �ٲ� ���θ� ������ �Ѵ�.

		// 3.1. ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.2. �ڵ��������̸� �޸������� ���� ���� �ڵ����� Ǭ��.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.3. ã�� ������ �߰��Ѵ�.
		// + ������ ��ġ�� ���Ѵ� (2021.01.28)
		startNoteCurrent = noteCurrent;
		startRowCurrent = this->glyphSubject->current->GetCurrent();
		this->glyphSubject->note->Paste((LPCTSTR)replaceString);
		this->glyphSubject->note->SelectCount(startRowCurrent, startNoteCurrent, replaceStringLen);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.4. �ڵ��������̸� �� �� �ڵ����� �Ѵ�.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAPLINE);
		}
	}
	// 4. ��ü �ٲٱ��̸�
	if (this->onIsReplacedCurrent != true) {
		// 4.1. ó�� ��ġ�� �̵��Ѵ�.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();

		// 4.2. �޸������� �������� ã�´�.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 4.3. ã�� ������ ���� ���� �ݺ��Ѵ�.
		while (this->glyphSubject->onIsFound == TRUE) {
			onIsReplaced = true;
			onIsFindingNext = false;

			// 4.3.1. ���õ� ������ �����.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			rowCurrent = this->glyphSubject->current->GetCurrent();

			// 4.3.2. �ڵ��������̸� �޸������� ���� ���� �ڵ����� Ǭ��.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. ã�� ������ �߰��Ѵ�.
			this->glyphSubject->note->Paste((LPCTSTR)replaceString);
			this->glyphSubject->current->Move(rowCurrent);
			this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, replaceStringLen);

			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.4. �޸������� �������� ã�´�.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}
		// 4.4. �ٲ� ���ΰ� ������
		if (onIsReplaced == true) {
			// 4.4.1. �ڵ����� ����̸� ��ü �ڵ����� �Ѵ�.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
			}
			// 4.4.2. ��ü ���� �����Ѵ�.
			this->glyphSubject->note->SelectAll(false);
			// 4.4.3. ã�� ���θ� ������ �Ѵ�.
			this->glyphSubject->onIsFound = TRUE;
		}
		// 4.5. ó�� ��ġ�� �̵��Ѵ�.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();
	}

	// 5. �������� ã���� ���ΰ� ���̸� �޸������� �������� ã�´�.
	if (findString != "" && onIsFindingNext == true) {
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	// 6. ���� ��ġ�� �����Ѵ�.
	prevRowCurrent = this->glyphSubject->current->GetCurrent();
	prevNoteCurrent = this->glyphSubject->note->GetCurrent();
	// 7. ã�� ���ϰ� (��ü �ٲٱⰡ �ƴϰ�) ������ ��ġ ���ΰ� ���̸� (2021.02.18)
	if (findString != "" && this->glyphSubject->onIsFound != TRUE && this->onIsReplacedCurrent == true && this->onIsWrapAround == 0) {
		// 7.1. ó�� ��ġ�� ù ��° ĭ���� �̵��Ѵ�.
		noteCurrent = this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		this->glyphSubject->current->First();
		// 7.2. �޸������� �������� ã�´�.
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext((LPCTSTR)findString, findStringLen, (bool)!this->onIsMatchCase);
	}
	// 8. ã�� ���ϸ� ���� ��ġ�� ���ư���.
	if (this->glyphSubject->onIsFound != TRUE) {
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		this->glyphSubject->current->Move(prevRowCurrent);
	}

	// 9. �ٲ� ���ΰ� ������ ���� ���¿� ���� ���¸� �����.
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

	// 1. ���� ���¸� �д´�.
	currentString = this->state->GetNote()->GetString();
	noteCurrent = this->state->GetNoteCurrent();
	rowCurrent = this->state->GetRowCurrent();

	// 2. ���� ���¸� �д´�.
	previousString = this->previousState->GetNote()->GetString();

	// + ��ġ�� �̵��Ѵ�.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. �� �ܾ �ٲ�����
	if (this->onIsReplacedCurrent == true) {
		// 3.1. �ڵ����� ����̸� �� ���� ��ģ��.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.2. ���� ������ ���� ���� ���Ѵ�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());

		// 3.3. ���ڸ�ŭ �����Ѵ�.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.4. ������ ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();
		
		// 3.5. �޸����� �߰��Ѵ�.
		this->glyphSubject->note->Paste(previousString);

		// 3.6. ���ڸ�ŭ �����Ѵ�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)previousString.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 3.7. �ڵ��������̸� �����ڵ������Ѵ�.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
	}

	// 4. ��ü �ٲٱ��̸�
	if (this->onIsReplacedCurrent != true) {
		// 4.2. �޸������� �������� ã�´�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);

		// 4.3. ã�� ������ ���� ���� �ݺ��Ѵ�.
		while (this->glyphSubject->onIsFound == TRUE) {
			// 4.3.1. ���õ� ������ �����.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.2. �ڵ��������̸� �޸������� ���� ���� �ڵ����� Ǭ��.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. ã�� ������ �߰��Ѵ�.
			this->glyphSubject->note->Paste(previousString);

			// 4.3.4. �޸������� �������� ã�´�.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);
		}
		
		// 4.4. �ڵ����� ����̸� ��ü �ڵ����� �Ѵ�.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
		// 4.5. ��ü ���� �����Ѵ�.
		this->glyphSubject->note->SelectAll(false);
		
		// 4.6. ó�� ��ġ�� �̵��Ѵ�.
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

	// 1. ���� ���¸� �д´�.
	currentString = this->previousState->GetNote()->GetString();
	noteCurrent = this->previousState->GetNoteCurrent();
	rowCurrent = this->previousState->GetRowCurrent();

	// 2. ���� ���¸� �д´�.
	previousString = this->state->GetNote()->GetString();

	// + ��ġ�� �̵��Ѵ�.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. �� �ܾ �ٲ�����
	if (this->onIsReplacedCurrent == true) {
		// 3.1. �ڵ����� ����̸� �� ���� ��ģ��.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 3.2. ���� ������ ���� ���� ���Ѵ�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());

		// 3.3. ���ڸ�ŭ �����Ѵ�.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.4. ������ ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();

		// 3.5. �޸����� �߰��Ѵ�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)previousString.c_str());
		this->glyphSubject->note->Paste(previousString);

		// 3.6. ���ڸ�ŭ �����Ѵ�.
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// 3.7. �ڵ��������̸� �����ڵ������Ѵ�.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
	}

	// 4. ��ü �ٲٱ��̸�
	if (this->onIsReplacedCurrent != true) {
		// 4.2. �޸������� �������� ã�´�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)currentString.c_str());
		this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);

		// 4.3. ã�� ������ ���� ���� �ݺ��Ѵ�.
		while (this->glyphSubject->onIsFound == TRUE) {
			// 4.3.1. ���õ� ������ �����.
			this->glyphSubject->note->RemoveSelect();
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

			// 4.3.2. �ڵ��������̸� �޸������� ���� ���� �ڵ����� Ǭ��.
			if (this->glyphSubject->onIsRowWrapping == TRUE) {
				this->glyphSubject->note->Combine(noteCurrent);
				noteCurrent = this->glyphSubject->note->GetCurrent();
				this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
			}

			// 4.3.3. ã�� ������ �߰��Ѵ�.
			this->glyphSubject->note->Paste(previousString);

			// 4.3.4. �޸������� �������� ã�´�.
			this->glyphSubject->onIsFound = (BOOL)this->glyphSubject->note->FindNext(currentString, textCount, (bool)!this->onIsMatchCase);
		}

		// 4.4. �ڵ����� ����̸� ��ü �ڵ����� �Ѵ�.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
		}
		// 4.5. ��ü ���� �����Ѵ�.
		this->glyphSubject->note->SelectAll(false);

		// 4.6. ó�� ��ġ�� �̵��Ѵ�.
		this->glyphSubject->note->First();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(0);
		this->glyphSubject->current->First();
	}
}