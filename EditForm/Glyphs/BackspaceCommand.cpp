// BackspaceCommand.cpp
/*
���ϸ�Ī : BackspaceCommand.cpp
��� : ���� ���� ���� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.25
*/
#include "BackspaceCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "resource.h"

BackspaceCommand::BackspaceCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->id = EditCommand::BACKSPACECOMMAND;
	this->onIsNew = true;
	this->onIsSelected = false;
	this->onIsEnter = false;
}

BackspaceCommand::~BackspaceCommand() {
}

void BackspaceCommand::Execute() {
	LONG noteCurrent;
	LONG noteLength;
	LONG rowCurrent;
	LONG rowLength;

	string text;

	Glyph* previousRow;
	Glyph* currentRow;
	Glyph* glyph;
	// + ���õ� ���� �ִ��� Ȯ���Ѵ�.
	this->onIsSelected = this->glyphSubject->note->IsSelected();

	//1. �ڵ��������̰�, ���õǾ� �ִ��� Ȯ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 1.1. ���õ� �ٵ��� ��ģ��.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 1.2. ���� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
	}

	//2. �������� rowCurrent ���� ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	// + ���õ� ���� ������ ���õ� �κ��� �����.
	if (this->onIsSelected == true) {
		// ���õ� ������ �д´�.
		text = this->glyphSubject->note->GetSelectString();

		// ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
	}
	//3. rowCurrent ���� 0���� �۰ų� ������
	else if (rowCurrent <= 0 && noteCurrent > 0) {
		this->onIsEnter = true;
		text = "\n";
		//3.1. �������� �����´�.
		currentRow = this->glyphSubject->note->GetAt(noteCurrent);
		//3.2 �������� �����´�.
		previousRow = this->glyphSubject->note->GetAt(noteCurrent - 1);
		//3.3. �����ٿ��� �������� ��ģ��.
		previousRow->Combine(currentRow);

		//3.4. ��Ʈ���� noteCurrent��° ���� ���ش�.
		this->glyphSubject->note->Remove(noteCurrent);
		noteCurrent = this->glyphSubject->note->Previous();
	}
	//4. rowCurrent ���� 0���� ũ��
	else if (rowCurrent > 0) {
		glyph = this->glyphSubject->current->GetAt(rowCurrent - 1);
		text = glyph->GetString();
		this->glyphSubject->current->Remove(rowCurrent - 1);
	}

	// ���� ��ġ�� �а� ��ġ�� �̵��Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// ���� ���¸� �����.
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

void BackspaceCommand::UnExecute() {
	Glyph* previousNote;

	Long rowCurrent;
	Long noteCurrent;

	if (this->previousState != 0) {
		// 2.1. ���� ���� ������ �о�´�.
		previousNote = this->previousState->GetNote();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		if (this->glyphSubject->onIsRowWrapping == TRUE) {// �ڵ��������̸� �� �� �ڵ������Ѵ�.(2021.02.09)
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->note->Combine(noteCurrent);
		}

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 2.2. ���� ������ ������ �߰��Ѵ�.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 2.3. �޸������� ���� ���� ���Ѵ�.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->previousState != 0 && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void BackspaceCommand::ReExecute() {
	Glyph* previousNote;
	Glyph* currentRow;
	Glyph* nextRow;

	string text;

	Long rowCurrent;
	Long noteCurrent;
	Long textCount;

	// 1. ���� ���� ������ �о�´�.
	previousNote = this->previousState->GetNote();
	text = previousNote->GetString();
	rowCurrent = this->previousState->GetRowCurrent();
	noteCurrent = this->previousState->GetNoteCurrent();

	// 2. �о�� ��ġ�� �̵��Ѵ�.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	// 3. ���õǾ� ������ ������ �����Ѵ�.
	if (this->onIsSelected == true) {
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);
	}

	// 4. �ڵ��������̰� ���õǾ� �ִ��� Ȯ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 4.1. ���õ� �ٵ��� �����ش�.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 4.2. ���� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	// 5. ���õǾ� ������ ���õ� ������ �����.
	if (this->onIsSelected == true) {
		this->glyphSubject->note->RemoveSelect();
	}
	// 6. ���๮���̸� ���� ��ģ��.
	else if (text == "\n") {
		//6.1. �������� �����´�.
		currentRow = this->glyphSubject->note->GetAt(noteCurrent);

		//6.2 �������� �����´�.
		nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);

		//6.3. �����ٿ��� �������� ��ģ��.
		currentRow->Combine(nextRow);

		//6.4. ��Ʈ���� noteCurrent��° ���� ���ش�.
		this->glyphSubject->note->Remove(noteCurrent + 1);
		this->glyphSubject->note->Previous();
	}
	// 7. ���๮�ڰ� �ƴϸ� ��ġ�� ���ڸ� �����.
	else {
		// 7.2 rowCurrent��° ���ڸ� �����.
		this->glyphSubject->current->Remove(rowCurrent);
	}

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 8. �ڵ��������̸� �ڵ������Ѵ�.
	if (this->previousState != 0 && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

Long BackspaceCommand::GetRowCurrent() {
	Long ret = -1;
	if (this->previousState != 0) {
		ret = this->previousState->GetRowCurrent();
	}

	return ret;
}

Long BackspaceCommand::GetNoteCurrent() {
	Long ret = -1;
	if (this->previousState != 0) {
		ret = this->previousState->GetNoteCurrent();
	}

	return ret;
}

void BackspaceCommand::SetOnIsNew(bool onIsNew) {
	this->onIsNew = onIsNew;
}