//CommandFactory.cpp
#include"Command.h"
#include"CommandFactory.h"
#include"GlyphSubject.h"
#include"resource.h"

// EditCommand
#include"RedoCommand.h"
#include"UndoCommand.h"
#include"CutCommand.h"
#include"CopyCommand.h"
#include"PasteCommand.h"
#include"CharacterCommand.h"
#include"ImeCharacterCommand.h"
#include"BackspaceCommand.h"
#include"DeleteCommand.h"
#include"BingSearchCommand.h"
#include"FindCommand.h"
#include"FindNextCommand.h"
#include"FindPreviousCommand.h"
#include"ReplaceDialogCommand.h"
#include"ReplaceCommand.h"
#include"GoToLineCommand.h"
#include"SelectAllCommand.h"
#include"DateCommand.h"

// FormatCommand
#include"WrapCommand.h"
#include"WrapLineCommand.h"

CommandFactory::CommandFactory(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

CommandFactory::~CommandFactory() {

}

Command* CommandFactory::Create(Long id) {
	Command* command = 0;

	switch (id) {
	case IDM_EDIT_REDO:
		command = new RedoCommand(this->glyphSubject);
		break;

	case IDM_EDIT_UNDO:
		command = new UndoCommand(this->glyphSubject);
		break;

	case IDM_EDIT_CUT:
		command = new CutCommand(this->glyphSubject);
		break;

	case IDM_EDIT_COPY:
		command = new CopyCommand(this->glyphSubject);
		break;

	case IDM_EDIT_PASTE:
		command = new PasteCommand(this->glyphSubject);
		break;

	case IDM_EDIT_CHARACTER:
		command = new CharacterCommand(this->glyphSubject);
		break;

	case IDM_EDIT_IMECHARACTER:
		command = new ImeCharacterCommand(this->glyphSubject);
		break;

	case IDM_EDIT_BACKSPACE:
		command = new BackspaceCommand(this->glyphSubject);
		break;

	case IDM_EDIT_DELETE:
		command = new DeleteCommand(this->glyphSubject);
		break;

	case IDM_EDIT_BING:
		command = new BingSearchCommand(this->glyphSubject);
		break;

	case IDM_EDIT_FIND:
		command = new FindCommand(this->glyphSubject);
		break;

	case IDM_EDIT_FINDNEXT:
		command = new FindNextCommand(this->glyphSubject);
		break;

	case IDM_EDIT_FINDPREV:
		command = new FindPreviousCommand(this->glyphSubject);
		break;

	case IDM_EDIT_REPLACEDIALOG:
		command = new ReplaceDialogCommand(this->glyphSubject);
		break;

	case IDM_EDIT_REPLACE:
		command = new ReplaceCommand(this->glyphSubject);
		break;

	case IDM_EDIT_MOVE:
		command = new GoToLineCommand(this->glyphSubject);
		break;

	case IDM_EDIT_SELECT_ALL:
		command = new SelectAllCommand(this->glyphSubject);
		break;

	case IDM_EDIT_DATE:
		command = new DateCommand(this->glyphSubject);
		break;

	case IDM_FORMAT_WRAP:
		command = new WrapCommand(this->glyphSubject);
		break;

	case IDM_FORMAT_WRAPLINE:
		command = new WrapLineCommand(this->glyphSubject);
		break;

	default:
		break;
	
	}

	return command;
}