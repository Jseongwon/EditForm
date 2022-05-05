// ImeCharacterCommand.cpp
/*
���ϸ�Ī : ImeCharacterCommand.cpp
��� : 1����Ʈ ���� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.26
*/
#include "ImeCharacterCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "GlyphFactory.h"
#include "resource.h"

ImeCharacterCommand::ImeCharacterCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->onIsCommandSaving = false;
}

ImeCharacterCommand::~ImeCharacterCommand() {
}

void ImeCharacterCommand::Execute() {
	// 1. ���ڸ� �����.
	GlyphFactory glyphFactory;
	Glyph* glyph;
	TCHAR letter[3];

	string text;

	Long rowCurrent;
	Long noteCurrent;

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
		this->onIsCommandSaving = true;
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

	letter[0] = this->glyphSubject->GetLetter()[0];
	letter[1] = this->glyphSubject->GetLetter()[1];
	letter[2] = '\0';
	if (letter[0] != '\0') {
		this->onIsCommandSaving = true;

		glyph = glyphFactory.Create(letter);

		// 5. �޸����� �ٿ��� ���ش�.
		this->glyphSubject->current->Remove(this->glyphSubject->current->GetCurrent() - 1);

		// 6. �޸����� �ٿ��� �߰��Ѵ�.
		if (this->glyphSubject->current->GetCurrent() == this->glyphSubject->current->GetLength()) {
			this->glyphSubject->current->Add(glyph);
		}
		else {
			this->glyphSubject->current->Add(this->glyphSubject->current->GetCurrent(), glyph);
		}

		// 7. ���¸� �����.
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		this->state = new State(rowCurrent - 1, noteCurrent, letter);
	}
	// 8. �ڵ��������̸� �ڵ������Ѵ�.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	this->glyphSubject->onIsComposing = FALSE;
}

void ImeCharacterCommand::UnExecute() {
	Glyph* previousNote;
	Glyph* currentNote;

	string text;

	Long textCount;
	Long rowCurrent;
	Long noteCurrent;
	Long i = 0;

	// 1. ���� ���� ������ ������
	if (this->state != 0) {
		// 1.1. ���� ���� ������ �о�´�.
		currentNote = this->state->GetNote();
		text = currentNote->GetString();
		rowCurrent = this->state->GetRowCurrent();
		noteCurrent = this->state->GetNoteCurrent();

		// 1.2. ��ġ�� �̵��ϰ�, �����Ѵ�.
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

		// 1.3. ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	// 2. ���� ������ ������ ������
	if (this->previousState != 0) {
		// 2.1. ���� ���� ������ �о�´�.
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

		// 2.2. ���� ������ ������ �߰��Ѵ�.
		previousNote->SelectAll(true);
		this->glyphSubject->note->Paste(previousNote);

		// 2.3. �޸������� ���� ���� ���Ѵ�.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

void ImeCharacterCommand::ReExecute() {
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

	// 2. ���� ���� ������ ������
	if (this->state != 0) {
		// 2.1. ���� ���� ������ �о�´�.
		currentNote = this->state->GetNote();
		rowCurrent = this->state->GetRowCurrent();
		noteCurrent = this->state->GetNoteCurrent();

		this->glyphSubject->note->MoveActualCurrent(rowCurrent, noteCurrent);

		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		// 2.2. �ڵ��������̸� �� ���� ��ģ��.
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->Combine(noteCurrent);
			noteCurrent = this->glyphSubject->note->GetCurrent();
			this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
		}

		// 2.3. ������ ������ �߰��Ѵ�.
		currentNote->SelectAll(true);
		this->glyphSubject->note->Paste(currentNote);

		// 2.4. �޸������� ���� ���� ���Ѵ�.
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	}
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}

Long ImeCharacterCommand::GetRowCurrent() {
	Long ret = -1;
	if (this->state != 0) {
		ret = this->state->GetRowCurrent();
	}

	return ret;
}

Long ImeCharacterCommand::GetNoteCurrent() {
	Long ret = -1;
	if (this->state != 0) {
		ret = this->state->GetNoteCurrent();
	}

	return ret;
}