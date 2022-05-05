// BackspaceCommand.h
#ifndef _BACKSPACECOMMAND_H
#define _BACKSPACECOMMAND_H
#include "EditCommand.h"

class BackspaceCommand : public EditCommand {
public:
	BackspaceCommand(GlyphSubject* glyphSubject = 0);
	virtual ~BackspaceCommand();
	virtual void Execute();
	virtual void UnExecute();
	virtual void ReExecute();
	virtual bool GetOnIsNew();
	virtual bool GetOnIsEnter();
	virtual Long GetRowCurrent();
	virtual Long GetNoteCurrent();
	virtual void SetOnIsNew(bool onIsNew = true);

private:
	bool onIsNew;
	bool onIsSelected;
	bool onIsEnter;
};

inline bool BackspaceCommand::GetOnIsNew() {
	return this->onIsNew;
}

inline bool BackspaceCommand::GetOnIsEnter() {
	return this->onIsEnter;
}

#endif // _BACKSPACECOMMAND_H