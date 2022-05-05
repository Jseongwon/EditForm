// CharacterCommand.cpp
/*
���ϸ�Ī : CharacterCommand.cpp
��� : 1����Ʈ ���� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
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
	//Long prevRowCurrent;// ���� ���� current�� �������� �����Ƿ� ���� �ʿ� ����
	Long prevNoteCurrent;

	// 1. ���ڸ� �д´�.
	TCHAR* letter = this->glyphSubject->GetLetter();

	// 2. ���õ� ������ �ִ��� Ȯ���Ѵ�.
	this->onIsSelected = this->glyphSubject->note->IsSelected();

	// 3. �ڵ��������̸� ���ÿ��θ� Ȯ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		// 3.1. ���õǾ� ������ ���õ� �ٵ��� ��ģ��.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 3.2. ���õǾ� ���� ������ ���� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 4. ���õ� ������ ������
	if (this->onIsSelected == true) {
		// 4.1. ���õ� ������ �д´�.
		text = this->glyphSubject->note->GetSelectString();

		// 4.2. ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		this->previousState = new State(rowCurrent, noteCurrent, text);
		this->onIsMore = false;
	}

	// 5 ���� �����̸�
	if (letter[0] == '\n' || letter[0] == '\r') {
		this->onIsEnter = true;
		// 5.1. ���� �����.
		row = glyphFactory.Create((char*)&letter[0]);
		// 5.2. ���� ���� length�� current�� �ٸ��� ���� �����ش�.
		rowCurrent = this->glyphSubject->current->GetCurrent();
		rowLength = this->glyphSubject->current->GetLength();
		if (rowCurrent != rowLength) {
			this->glyphSubject->current->Split(row, rowCurrent);
		}
		// 5.3. ��Ʈ���� �� ���� �����ִ´�.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		noteCurrent = this->glyphSubject->note->Add(noteCurrent + 1, row);

		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	// 6. ���� ���ڰ� �ƴϸ�
	else if ((letter[0] >= 32 && letter[0] < 127) || letter[0] == 9) {
		// 6.1. ���ڸ� �����.
		glyph = glyphFactory.Create((char*)&letter[0]);
		// 6.2. �������� current�� current�� length�� ������ ���� �ٿ��� �߰��Ѵ�.
		if (this->glyphSubject->current->GetCurrent() == this->glyphSubject->current->GetLength()) {
			this->glyphSubject->current->Add(glyph);
		}
		else {
			//6.3.�ٸ��� ���ڸ� current��°�� �����ش�.
			this->glyphSubject->current->Add(this->glyphSubject->current->GetCurrent(), glyph);
		}
	}

	// 7. ���� ���¸� �����.
	this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
	if (this->onIsEnter == true) {
		// 8. ���๮���̸�
		// 8.1. ���� ��ġ�� �����Ѵ�.
		// prevRowCurrent = this->glyphSubject->current->GetCurrent();// ���� ���� current�� ���� ���� �����Ƿ� ���� �ʿ� ����
		prevNoteCurrent = this->glyphSubject->note->GetCurrent();

		// 8.2. ��Ʈ���� �������� �̵��Ѵ�.
		noteCurrent = prevNoteCurrent - 1;
		this->glyphSubject->note->Move(noteCurrent);

		// 8.3. �ٿ��� ������ ��ġ�� �̵��Ѵ�.
		row = this->glyphSubject->note->GetAt(noteCurrent);
		row->Last();

		// 8.4. ������ ��ġ�� �̸� ���Ѵ�.
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		// 8.5. �ٽ� ���� ��ġ�� ���ƿ´�.
		this->glyphSubject->note->Move(prevNoteCurrent);
		this->glyphSubject->current = this->glyphSubject->note->GetAt(prevNoteCurrent);
		// this->glyphSubject->current->Move(prevRowCurrent); // ���� ���� current�� ���� ���� ����.
	}
	else {
		rowCurrent--;
	}

	if ((letter[0] == '\n' || letter[0] == '\r') || ((letter[0] >= 32 && letter[0] < 127) || letter[0] == 9)) {
		this->state = new State(rowCurrent, noteCurrent, letter);
	}

	// 8. �ڵ��������̸� �ڵ����� ���ش�.
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
	// 1. ���� ���� ������ �о�´�.
	Glyph* previousNote;
	Glyph* currentNote = this->state->GetNote();

	string text = currentNote->GetString();

	Long textCount;
	Long rowCurrent = this->state->GetRowCurrent();
	Long noteCurrent = this->state->GetNoteCurrent();
	Long i = 0;

	// 2. ��ġ�� �̵��ϰ�, �����Ѵ�.
	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	rowCurrent = this->glyphSubject->current->GetCurrent();

	textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
	this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

	// + �ڵ��������̸� ���ÿ��θ� Ȯ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. ���õ� ������ �����.
	this->glyphSubject->note->RemoveSelect();
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 4. ���� ������ ������ ������
	if (this->previousState != 0) {
		// 4.1. ���� ���� ������ �о�´�.
		previousNote = this->previousState->GetNote();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// �ڵ��������̸� �� ���� ��ģ��.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 4.2. ���� ������ ������ �߰��Ѵ�.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 4.3. �޸������� ���� ���� ���Ѵ�.
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
	// 1. ���� ���� ������ ������
	if (this->previousState != 0) {
		// 1.1.  ���� ���� ������ �о�´�.
		previousNote = this->previousState->GetNote();
		text = previousNote->GetString();
		rowCurrent = this->previousState->GetRowCurrent();
		noteCurrent = this->previousState->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		rowCurrent = this->glyphSubject->current->GetCurrent();

		// 1.2. ��Ī�Ǵ� ���ڸ� �����Ѵ�.
		textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
		this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

		// + �ڵ��������̸� ���ÿ��θ� Ȯ���Ѵ�.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		if (this->glyphSubject->onIsRowWrapping == TRUE && this->onIsSelected == true) {
			this->glyphSubject->note->CombineSelectedRows();
		}
		else if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
		}
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 1.3. ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 2. ���� ���� ������ �о�´�.
	currentNote = this->state->GetNote();
	rowCurrent = this->state->GetRowCurrent();
	noteCurrent = this->state->GetNoteCurrent();

	this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 5.4. �ڵ��������̸� �� ���� ��ģ��.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->Combine(noteCurrent);
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 3. ������ ������ �߰��Ѵ�.
	currentNote->SelectAll(true);
	this->glyphSubject->note->Paste(currentNote);

	// 4. �޸������� ���� ���� ���Ѵ�.
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