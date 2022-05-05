// UndoCommand.h
#ifndef _UNDOCOMMAND_H
#define _UNDOCOMMAND_H
#include "Command.h"

class UndoCommand : public Command {
public:
	UndoCommand(GlyphSubject* glyphSubject);
	virtual ~UndoCommand();
	virtual void Execute();
};

#endif // _UNDOCOMMAND_H