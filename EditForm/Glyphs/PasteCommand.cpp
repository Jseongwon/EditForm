// PasteCommand.cpp
/*
���ϸ�Ī : PasteCommand.cpp
��� : �߶󳽴� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.25
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

	// 1. ���õ� ������ �ִ��� Ȯ���Ѵ�.
	onIsSelected = this->glyphSubject->note->IsSelected();

	// 2. �ڵ��������̸� ���õ� ������ �ִ��� Ȯ���Ѵ�.
	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (this->glyphSubject->onIsRowWrapping == TRUE && onIsSelected == true) {
		// 2.1. ���õ� ������ ������ ���õ� �ٵ��� ��ģ��.
		this->glyphSubject->note->CombineSelectedRows();
	}
	else if (this->glyphSubject->onIsRowWrapping == TRUE) {
		// 2.2. ���õ� ������ ������ �� ���� ��ģ��.
		this->glyphSubject->note->Combine(noteCurrent);
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 3. ���õ� ������ ������
	if (onIsSelected == true) {
		// 3.1. ���õ� ������ �д´�.
		text = this->glyphSubject->note->GetSelectString();

		// 3.2. ���õ� ������ �����.
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);

		this->previousState = new State(rowCurrent, noteCurrent, text);
	}

	// 2. Ŭ�����忡�� ������ �о�´�.
	pasteString = this->glyphSubject->clipboard->Read();

	// 3. ���¸� �����.
	if (pasteString != "") {
		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
		
		this->state = new State(rowCurrent, noteCurrent, (LPCTSTR)pasteString);

		this->onIsCommandSaving = true;
	}

	// 4. �޸������� ������ �߰��Ѵ�.
	this->glyphSubject->note->Paste((LPCTSTR)pasteString);
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);

	// 5. �ڵ����� ����̸� ��ü �ڵ������Ѵ�.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}

	// 6. ���� �߰��Ѵ�.
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
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->CombineSelectedRows();
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

void PasteCommand::ReExecute() {
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
		if (this->glyphSubject->onIsRowWrapping == TRUE) {
			this->glyphSubject->note->CombineSelectedRows();
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