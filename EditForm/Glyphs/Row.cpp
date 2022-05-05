// Row.cpp
/*
파일명칭 : Row.cpp
기능 : 줄 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.06
*/
#include "Row.h"
#include "GlyphVisitors.h"

Row::Row(Long capacity, bool onIsRow)
	: Composite(capacity) {
	this->onIsRow = onIsRow;
	this->onIsSelected = false;
}

Row::Row(const Row& source)
	: Composite(source) {
	this->onIsRow = source.onIsRow;
	this->onIsSelected = source.onIsSelected;
}

Row::~Row() {

}

Row& Row::operator =(const Row& source) {
	Composite::operator =(source);
	this->onIsRow = source.onIsRow;
	this->onIsSelected = source.onIsSelected;

	return* this;
}

Long Row::Add(Long current, Glyph* glyph) {
	Long index;
	index = this->glyphs.Insert(current, glyph);
	this->capacity++;
	this->length++;
	this->current = index + 1;

	return this->current;
}

bool Row::RemoveSelect() {
	bool ret = false;
	bool onIsStop = false;

	Glyph* character;

	Long startIndex = 0;
	Long i = 0;
	// 1. 마지막 칸까지 반복한다.
	while (i < this->length) {
		// 1.1. 글자를 읽는다.
		character = this->glyphs.GetAt(i);
		// 1.2. 마지막 칸보다 작고, 글자가 선택되어 있는 동안 반복한다.
		while (i < this->length && character->IsSelected() == true) {
			ret = true;
			// 1.2.1. 지우기 시작한 위치를 저장한다.
			startIndex = i;
			// 1.2.2. 글자를 지운다.
			delete this->glyphs.GetAt(i);
			this->glyphs.Delete(i);
			// 1.2.3. 글자를 읽는다.
			character = this->glyphs.GetAt(i);
			this->capacity--;
			this->length--;
		}
		
		i++;
	}
	// 2. 지우기 시작한 위치로 이동한다.(2021.01.25)
	this->current = startIndex;
	if (this->onIsSelected == true) {
		this->current = this->length;
		ret = true;
	}

	return ret;
}

void  Row::Split(Glyph* row, Long current) {
	Glyph* letter;
	Glyph* firstLetter = 0;
	Glyph* lastLetter = 0;

	bool onIsFirstLetterSelected = false;
	bool onIsLastLetterSelected = false;
	bool onIsSelected = false;
	//1. length가 current보다 작은동안 반복한다.
	while (this->length > current) {
		//1.1. current번째 글자를 가져온다.
		letter = this->glyphs.GetAt(current);
		//1.2. 새로운 줄에 글자를 추가해준다.
		row->Add(letter->Clone());
		//1.3. current번째 글자를 지운다.
		delete letter;
		this->glyphs.Delete(current);
		this->capacity--;
		this->length--;
	}
	//2. 새로운 줄에서 첫번째로 이동한다.
	row->First();
	this->current = current;
	row->Select(this->onIsSelected);

	// 3. 현재 줄의 마지막 글자를 읽는다.
	if (this->length > 0) {
		lastLetter = this->glyphs.GetAt(this->length - 1);
		onIsLastLetterSelected = lastLetter->IsSelected();
	}

	// 4. 다음 줄의 첫 번째 글자를 읽는다.
	if (row->GetLength() > 0) {
		firstLetter = row->GetAt(0);
		onIsFirstLetterSelected = firstLetter->IsSelected();
	}

	// 5. 첫 번째 글자와 마지막 글자가 선택되어 있으면 줄을 선택한다.
	this->onIsSelected = false;
	if (onIsFirstLetterSelected == true && onIsLastLetterSelected == true) {
		this->onIsSelected = true;
	}
}
/*
rowLength = row->GetLength();
			firstGlyph = row->GetAt(current);
			lastGlyph = row->GetAt(current + 1);
			if (current + 1 < rowLength - 1 && firstGlyph->IsSelected() == true && lastGlyph->IsSelected() == true) {
				row->Select(true);
			}
*/

void Row::Combine(Glyph* row) { // 입력된 줄을 합친다. (2021.01.13)
	Glyph* letter;
	Glyph* letterCopy;

	// 1. 입력된 줄에서 글자가 하나도 없을 동안 반복한다.
	Long lastCurrent = this->length;
	Long length = row->GetLength();
	while (length > 0) {
		letter = row->GetAt(0); // 1.1. 글자를 읽는다.
		letterCopy = letter->Clone();
		this->glyphs.AppendFromRear(letterCopy); // 1.2. 이전 줄의 마지막에 추가한다.
		this->capacity++;
		this->length++;
		row->Remove(0); // 1.3. 입력된 줄의 글자를 지운다. (1.1, 1.2, 1.3 을 합쳐서 글자를 옮긴다.)
		length = row->GetLength();
	}
	this->current = lastCurrent;
	this->onIsSelected = row->IsSelected(); // 2. 다음줄의 줄 선택 여부를 이어 받는다.
}

void Row::Combine(Long index, Glyph* row) {
	Glyph* letter;
	Glyph* letterCopy;

	// 1. 입력된 줄에서 글자가 하나도 없을 동안 반복한다.
	Long length = row->GetLength();
	while (length > 0) {
		letter = row->GetAt(0); // 1.1. 글자를 읽는다.
		letterCopy = letter->Clone();
		this->glyphs.Insert(index, letterCopy); // 1.2. 이전 줄의 위치에 추가한다.
		index++;
		this->capacity++;
		this->length++;
		row->Remove(0); // 1.3. 입력된 줄의 글자를 지운다. (1.1, 1.2, 1.3 을 합쳐서 글자를 옮긴다.)
		length = row->GetLength();
	}
	this->current = index;
	this->onIsSelected = row->IsSelected(); // 2. 다음줄의 줄 선택 여부를 이어 받는다.
}

void Row::Select(bool onIsSelected) {
	this->onIsSelected = onIsSelected;
}

void Row::Select(Long start, Long end, bool onIsSelected) {
	Long i = start;
	while (i < end) {
		this->glyphs.GetAt(i)->Select(onIsSelected);
		i++;
	}
}

void Row::SelectAll(bool onIsSelected) {
	Long i = 0;
	while (i < this->length) {
		this->glyphs.GetAt(i)->Select(onIsSelected);
		i++;
	}
	this->onIsSelected = onIsSelected; // (2021.01.25) 수정
}

Long Row::Previous() {
	this->current--;
	if (this->current < 0) {
		this->current = 0;
	}

	return this->current;
}

Long Row::PreviousWord() {
	Glyph* glyph;
	string letter;
	bool onIsPrevious = false;
	bool onIsSpaceOrTab = false;

	if (this->current > 0) {
		glyph = this->glyphs.GetAt(this->current - 1);
		letter = glyph->GetString();
		if (letter == " " || letter == "\t") {
			onIsSpaceOrTab = true;
		}
	}

	while (onIsSpaceOrTab == true && this->current > 1) {
		this->current--;
		glyph = this->glyphs.GetAt(this->current - 1);
		letter = glyph->GetString();
		onIsSpaceOrTab = false;
		if (letter == " " || letter == "\t") {
			onIsSpaceOrTab = true;
		}
	}

	while (onIsSpaceOrTab != true && this->current > 1) {
		onIsPrevious = true;
		this->current--;
		glyph = this->glyphs.GetAt(this->current - 1);
		letter = glyph->GetString();
		onIsSpaceOrTab = true;
		if (letter != " " && letter != "\t") {
			onIsSpaceOrTab = false;
		}
	}

	onIsSpaceOrTab = true;
	if (letter != " " && letter != "\t") {
		onIsSpaceOrTab = false;
	}

	if (this->current == 1 && (onIsPrevious != true || onIsSpaceOrTab != true)) {
		this->current = 0;
	}
	else if (onIsSpaceOrTab != true && this->current > 0) {
		this->current++;
	}

	return this->current;
}

Long Row::Next() {
	this->current++;
	if (this->current >= this->length) {
		this->current = this->length;
	}

	return this->current;
}

Long Row::NextWord() {
	Glyph* glyph;
	string letter;
	bool onIsSpaceOrTab;
	Long i;
	bool onIsMoveToNextWord = false;




	i = this->current + 1;

	glyph = this->glyphs.GetAt(i - 1);
	letter = glyph->GetString();
	if (letter != " " && letter != "\t") {
		onIsSpaceOrTab = false;
	}
	else {
		onIsSpaceOrTab = true;
	}

	while (onIsSpaceOrTab != true && i < this->length) {
		i++;
		glyph = this->glyphs.GetAt(i - 1);
		letter = glyph->GetString();

		if (letter != " " && letter != "\t") {
			onIsSpaceOrTab = false;
		}
		else {
			onIsSpaceOrTab = true;
		}
	}

	while (onIsSpaceOrTab == true && i < this->length) {
		i++;
		glyph = this->glyphs.GetAt(i - 1);
		letter = glyph->GetString();

		if (letter == " " || letter == "\t") {
			onIsSpaceOrTab = true;
		}
		else {
			onIsSpaceOrTab = false;
		}
		onIsMoveToNextWord = true;
	}

	if (onIsSpaceOrTab != true && i <= this->length && onIsMoveToNextWord == true) {
		i--;
	}
	else if (i > this->length) {
		i = this->length;
	}

	this->current = i;

	return this->current;
}

Long Row::Last() {
	this->current = this->length;

	return this->current;
}

Glyph* Row::Clone() {
	return new Row(*this);
}

void Row::Accept(GlyphVisitor* glyphVisitor) {
	glyphVisitor->Visit(this);
}

string Row::GetString(Long startXPos, Long endXPos) {
	string str;
	string character;
	Long i = startXPos;
	while (i < endXPos && i < this->length) {
		character = this->glyphs.GetAt(i)->GetString();
		if (character.compare("\t") == 0) {
			character = "        ";
		}
		str += character;
		i++;
	}

	return str;
}

Long Row::MoveToFirstSelectedCurrent() {
	Glyph* letter;

	bool onIsSelected;
	bool onIsStop = false;

	Long ret = -1;
	Long i = 0;
	while (i < this->length && onIsStop != true) {
		letter = this->glyphs.GetAt(i);
		onIsSelected = letter->IsSelected();
		if (onIsSelected == true) {
			onIsStop = true;
		}
		else {
			onIsStop = false;
		}
		i++;
	}
	if (onIsStop == true) {
		this->current = i - 1;
		ret = this->current;
	}
	else if (this->onIsSelected == true && onIsStop != true) {
		this->current = this->length;
		ret = this->current;
	}

	return ret;
}

Long Row::MoveToLastSelectedCurrent() {
	Glyph* letter;

	bool onIsSelected;
	bool onIsStop = false;
	
	Long ret = -1;
	Long i;
	i = this->length;
	while (i > 0 && onIsStop != true) {
		letter = this->glyphs.GetAt(i - 1);
		onIsSelected = letter->IsSelected();
		if (onIsSelected == true) {
			onIsStop = true;
		}
		else {
			onIsStop = false;
		}
		i--;
	}
	if (onIsStop == true) {
		this->current = i + 1;
		ret = this->current;
	}
	else if (this->onIsSelected == true && onIsStop != true) {
		this->current = this->length;
		ret = this->current;
	}

	return ret;
}

string Row::GetString() {
	string str;
	string character;
	Long i = 0;
	while (i < this->length) {
		character = this->glyphs.GetAt(i)->GetString();
		if (character.compare("\t") == 0) {
			character = "        ";
		}
		str += character;
		i++;
	}

	return str;
}

string Row::GetSelectString() {
	string str;
	string character;

	Glyph* glyph;

	bool onIsSelected = false;

	Long i = 0;
	while (i < this->length) {
		glyph = this->glyphs.GetAt(i);
		character = glyph->GetString();
		onIsSelected = glyph->IsSelected();
		if (onIsSelected == true) {
			str += character;
		}
		i++;
	}

	return str;
}