// FindPreviousCommand.h
#ifndef _FINDPREVIOUSCOMMAND_H
#define _FINDPREVIOUSCOMMAND_H
#include "EditCommand.h"

class FindPreviousCommand : public EditCommand {
public:
	FindPreviousCommand(GlyphSubject* glyphSubject = 0);
	virtual ~FindPreviousCommand();
	virtual void Execute();
};

#endif // _FINDPREVIOUSCOMMAND_H