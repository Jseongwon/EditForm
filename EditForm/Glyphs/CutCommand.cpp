// CutCommand.cpp
/*
���ϸ�Ī : CutCommand.cpp
��� : �߶󳽴� ��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.25
*/
#include "CutCommand.h"
#include "State.h"
#include "GlyphSubject.h"
#include "Clipboard.h"
#include "Glyph.h"
#include "GlyphExtent.h"
#include "resource.h"

CutCommand::CutCommand(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CutCommand::~CutCommand() {
}

void CutCommand::Execute() {
	LONG rowCurrent;
	LONG noteCurrent;

	CString title;
	CString copyString;
	copyString = this->glyphSubject->note->GetSelectString().c_str();

	noteCurrent = this->glyphSubject->note->GetCurrent();
	if (copyString != "" && this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->CombineSelectedRows();
	}

	if (copyString != "") {
		this->glyphSubject->note->RemoveSelect();
		noteCurrent = this->glyphSubject->note->GetCurrent();
		this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);// ���õ� ������ �־�� �����ϰԲ� ����(2021.02.09)
		rowCurrent = this->glyphSubject->current->GetCurrent(); // ����� ���� ���� ��ġ�� �аԲ� ����(2021.02.09)

		this->glyphSubject->note->GetCombineCurrent(&rowCurrent, &noteCurrent);
		this->previousState = new State(rowCurrent, noteCurrent, (LPCTSTR)copyString);
		this->glyphSubject->clipboard->Write(copyString);
		this->onIsCommandSaving = true;
	}

	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAPLINE);
	}

	this->glyphSubject->GetWindowText(title);
	if (title.GetAt(0) != '*' && copyString != "") {
		title = "*" + title;
	}
	this->glyphSubject->SetWindowTextA(title);
}

void CutCommand::UnExecute() {
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

void CutCommand::ReExecute() {
	Glyph* previousNote;

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

	// 3. ������ �����Ѵ�.
	textCount = this->glyphSubject->glyphExtent->GetStringCount((char*)text.c_str());
	this->glyphSubject->note->SelectCount(rowCurrent, noteCurrent, textCount);

	// 4. �ڵ��������̰� ���õǾ� ������ ���õ� �ٵ��� �����ش�.
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->note->CombineSelectedRows();
	}
	
	this->glyphSubject->note->RemoveSelect();

	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
	
	if (this->glyphSubject->onIsRowWrapping == TRUE) {
		this->glyphSubject->SendMessage(WM_COMMAND, IDM_FORMAT_WRAP);
	}
}