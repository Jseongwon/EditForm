// SelectAllCommand.h
#ifndef _SELECTALLCOMMAND_H
#define _SELECTALLCOMMAND_H
#include "EditCommand.h"

class SelectAllCommand : public EditCommand {
public:
	SelectAllCommand(GlyphSubject* glyphSubject = 0);
	virtual ~SelectAllCommand();
	virtual void Execute();
};

#endif // _SELECTALLCOMMAND_H