// EditCommand.h
#ifndef _EDITCOMMAND_H
#define _EDITCOMMAND_H
#include "Command.h"
typedef signed long int Long;

class State;
class EditCommand : public Command {
public:
	enum { ETCCOMMAND = 0, WRITECOMMAND = 1, BACKSPACECOMMAND = 2, DELETECOMMAND = 3 };

public:
	EditCommand(GlyphSubject* glyphSubject = 0);
	virtual ~EditCommand() = 0;
	virtual void Execute() {  }
	virtual void UnExecute() {  };
	virtual void ReExecute() {  };
	virtual Long GetID() const;
	virtual bool GetOnIsCommandSaving();
	virtual bool GetOnIsNew() { return true; }
	virtual bool GetOnIsMore() { return true; }
	virtual bool GetOnIsEnter() { return false; }
	virtual Long GetRowCurrent() { return -1; }
	virtual Long GetNoteCurrent() { return -1; }
	virtual void SetOnIsNew(bool onIsNew = true) {  }

protected:
	State* previousState;
	Long id;
	bool onIsCommandSaving;
};

inline Long EditCommand::GetID() const {
	return this->id;
}

inline bool EditCommand::GetOnIsCommandSaving() {
	return this->onIsCommandSaving;
}

#endif // _EDITCOMMAND_H