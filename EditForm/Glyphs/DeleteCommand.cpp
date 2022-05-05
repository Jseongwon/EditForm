// DeleteCommand.cpp
/*
���ϸ�Ī : DeleteCommand.cpp
��� : ������ ���� ���� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.25
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

	//2. �޸����� ���� ��ġ�� ��뷮 ���� ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	noteLength = this->glyphSubject->note->GetLength();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	rowCurrent = this->glyphSubject->current->GetCurrent();
	rowLength = this->glyphSubject->current->GetLength();

	// + ���õ� ���� ������ ���õ� �κ��� �����.
	if (this->onIsSelected == true) {
		text = this->glyphSubject->note->GetSelectString();
		this->glyphSubject->note->RemoveSelect();
	}
	//3. rowCurrent ���� row�� Length���� ũ�ų� ���� noteCurrent ���� noteLength-1 ���� ũ��
	else if (rowCurrent >= rowLength && noteCurrent < noteLength - 1) {
		this->onIsEnter = true;
		text = "\n";
		//3.1. �������� �����´�.
		currentRow = this->glyphSubject->note->GetAt(noteCurrent);

		//3.2 �������� �����´�.
		nextRow = this->glyphSubject->note->GetAt(noteCurrent + 1);

		//3.3. �����ٿ��� �������� ��ģ��.
		currentRow->Combine(nextRow);

		//3.4. ��Ʈ���� noteCurrent+1��° ���� ���ش�.
		this->glyphSubject->note->Remove(noteCurrent + 1);
		noteCurrent = this->glyphSubject->note->Previous();

		// 3.5. ���� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	//4. rowCurrent ���� row�� Length���� ������ 
	else if (rowCurrent < rowLength) {
		// 4.1. ���� ���ڸ� �д´�.
		glyph = this->glyphSubject->current->GetAt(rowCurrent);
		text = glyph->GetString();

		// 4.2 rowCurrent��° ���ڸ� �����.
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

void DeleteCommand::ReExecute() {
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

		//6.4. ��Ʈ���� noteCurrent+1��° ���� ���ش�.
		this->glyphSubject->note->Remove(noteCurrent + 1);
		noteCurrent = this->glyphSubject->note->Previous();

		// 6.5. ���� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
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