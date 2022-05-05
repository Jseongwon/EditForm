//CommandHistory.cpp
#include"CommandHistory.h"
#include"CommandStack.h"
#include"EditCommand.h"
#include"GlyphSubject.h"
#include"Glyph.h"

CommandHistory::CommandHistory(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
	this->undoStack = new CommandStack;
	this->redoStack = new CommandStack;
}

CommandHistory::~CommandHistory() {//������ ����Ǿ��ִ� command�� ����
	if (this->undoStack != 0) {
		delete this->undoStack;
		this->undoStack = 0;
	}
	if (this->redoStack != 0) {
		delete this->redoStack;
		this->redoStack = 0;
	}
}

void CommandHistory::Save(Command* command) {
	// 1. ���� ��ġ�� �д´�.
	EditCommand* currentCommand = (EditCommand*)command;
	EditCommand* previousCommand = 0;
	Long currentNoteCurrent = currentCommand->GetNoteCurrent();
	Long currentRowCurrent = currentCommand->GetRowCurrent();
	Long previousNoteCurrent = -1;
	Long previousRowCurrent = -1;
	Long currentID = currentCommand->GetID();
	Long previousID = EditCommand::ETCCOMMAND;

	// 2. ���� ���� ��ġ�� �д´�.
	if (this->undoStack->GetLength() > 0) {
		previousCommand = (EditCommand*)this->undoStack->Peek();
		previousNoteCurrent = previousCommand->GetNoteCurrent();
		previousRowCurrent = previousCommand->GetRowCurrent();
		previousID = previousCommand->GetID();
	}

	// 3. ���� ���ɵ��̸�// �����ؾ� �� ���� ���� ���� ����, StartImeComposition����
	if (previousID == currentID && currentID == EditCommand::WRITECOMMAND &&
		currentNoteCurrent == previousNoteCurrent && currentRowCurrent - 1 == previousRowCurrent) {
		currentCommand->SetOnIsNew(false);
	}

	// 4. ���� ���� ���ɵ��̸�
	if (previousID == currentID && currentID == EditCommand::DELETECOMMAND &&
		currentNoteCurrent == previousNoteCurrent && currentRowCurrent == previousRowCurrent) {
		currentCommand->SetOnIsNew(false);
	}

	// 5. ������ ���� ���ɵ��̸�
	if (previousID == currentID && currentID == EditCommand::BACKSPACECOMMAND &&
		currentNoteCurrent == previousNoteCurrent && currentRowCurrent == previousRowCurrent - 1) {
		currentCommand->SetOnIsNew(false);
	}

	if (this->redoStack != 0) {
		delete this->redoStack;
	}
	this->redoStack = new CommandStack;
	this->undoStack->Push(command);
}

void CommandHistory::Undo() {//�������
	Long firstID = EditCommand::ETCCOMMAND;
	Long currentID = EditCommand::ETCCOMMAND;
	Long itID = EditCommand::ETCCOMMAND;
	Long noteCurrent;

	bool onIsNew = true;
	bool onIsEnter = false;
	bool onIsMore = false;
	bool onIsStop = false;

	EditCommand* editCommand = 0;
	if (this->undoStack->GetLength() > 0) {
		this->glyphSubject->note->SelectAll(false);
		editCommand = (EditCommand*)this->undoStack->Pop();
		editCommand->UnExecute();
		this->redoStack->Push(editCommand);
		firstID = editCommand->GetID();
		currentID = firstID;
		onIsNew = editCommand->GetOnIsNew();
		if (onIsNew == true) {
			onIsStop = true;
		}
		onIsEnter = editCommand->GetOnIsEnter();
		onIsMore = !editCommand->GetOnIsMore();
		if (onIsEnter == true && onIsMore != true) {
			onIsStop = true;
		}
	}

	// ���ڸ� ���鼭 ���õ� ������ ����� �� ��츸 �˾Ƴ� ���. 1. �÷��׸� �ְ� WriteCommand���� üũ�صд�.
	if (this->undoStack->GetLength() > 0) {
		editCommand = (EditCommand*)this->undoStack->Peek();
		itID = editCommand->GetID();
		onIsEnter = editCommand->GetOnIsEnter();
		onIsMore = !editCommand->GetOnIsMore();
		if (onIsStop != true && onIsEnter == true && onIsMore != true) {
			onIsStop = true;
		}
	}
	while (itID == currentID && itID != EditCommand::ETCCOMMAND && onIsStop != true) {
		onIsNew = editCommand->GetOnIsNew();
		if (onIsNew == true) {
			onIsStop = true;
		}
		editCommand = (EditCommand*)this->undoStack->Pop();
		editCommand->UnExecute();
		this->redoStack->Push(editCommand);
		if (this->undoStack->GetLength() > 0) {
			editCommand = (EditCommand*)this->undoStack->Peek();
			itID = editCommand->GetID();
			onIsEnter = editCommand->GetOnIsEnter();
			onIsMore = !editCommand->GetOnIsMore();
			if (onIsStop != true && onIsEnter == true && onIsMore != true) {
				onIsStop = true;
			}
		}
		else {
			itID = EditCommand::ETCCOMMAND;
		}
	}
	if (firstID != EditCommand::DELETECOMMAND) {
		this->glyphSubject->note->MoveToLastSelectedCurrent();
	}
	else {
		this->glyphSubject->note->MoveToFirstSelectedCurrent();
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}

void CommandHistory::Redo() {//�ٽý���
	Long firstID = EditCommand::ETCCOMMAND;
	Long currentID = EditCommand::ETCCOMMAND;
	Long itID = EditCommand::ETCCOMMAND;
	Long noteCurrent;

	bool onIsNew = false;
	bool onIsEnter = false;
	bool onIsMore = false;
	bool onIsStop = false;

	EditCommand* editCommand = 0;
	if (this->redoStack->GetLength() > 0) {
		this->glyphSubject->note->SelectAll(false);
		editCommand = (EditCommand*)this->redoStack->Pop();
		editCommand->ReExecute();
		this->undoStack->Push(editCommand);
		firstID = editCommand->GetID();
		currentID = firstID;
		onIsEnter = editCommand->GetOnIsEnter();
		onIsMore = !editCommand->GetOnIsMore();
		if (onIsEnter == true && onIsMore != true) {
			onIsStop = true;
		}
	}

	if (this->redoStack->GetLength() > 0) {
		editCommand = (EditCommand*)this->redoStack->Peek();
		itID = editCommand->GetID();
		onIsNew = editCommand->GetOnIsNew();
		if (onIsNew == true) {
			onIsStop = true;
		}
		onIsEnter = editCommand->GetOnIsEnter();
		onIsMore = !editCommand->GetOnIsMore();
		if (onIsStop != true && onIsEnter == true && onIsMore != true) {
			onIsStop = true;
		}
	}
	while (itID == currentID && itID != EditCommand::ETCCOMMAND && onIsStop != true) {
		editCommand = (EditCommand*)this->redoStack->Pop();
		editCommand->ReExecute();
		this->undoStack->Push(editCommand);
		if (this->redoStack->GetLength() > 0) {
			editCommand = (EditCommand*)this->redoStack->Peek();
			itID = editCommand->GetID();
			onIsNew = editCommand->GetOnIsNew();
			if (onIsNew == true) {
				onIsStop = true;
			}
			onIsEnter = editCommand->GetOnIsEnter();
			onIsMore = !editCommand->GetOnIsMore();
			if (onIsStop != true && onIsEnter == true && onIsMore != true) {
				onIsStop = true;
			}
		}
		else {
			itID = EditCommand::ETCCOMMAND;
		}
	}
	if (firstID != EditCommand::DELETECOMMAND) {
		this->glyphSubject->note->MoveToLastSelectedCurrent();
	}
	else {
		this->glyphSubject->note->MoveToFirstSelectedCurrent();
	}
	noteCurrent = this->glyphSubject->note->GetCurrent();
	this->glyphSubject->current = this->glyphSubject->note->GetAt(noteCurrent);
}