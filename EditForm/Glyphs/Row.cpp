// Row.cpp
/*
���ϸ�Ī : Row.cpp
��� : �� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.06
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
	// 1. ������ ĭ���� �ݺ��Ѵ�.
	while (i < this->length) {
		// 1.1. ���ڸ� �д´�.
		character = this->glyphs.GetAt(i);
		// 1.2. ������ ĭ���� �۰�, ���ڰ� ���õǾ� �ִ� ���� �ݺ��Ѵ�.
		while (i < this->length && character->IsSelected() == true) {
			ret = true;
			// 1.2.1. ����� ������ ��ġ�� �����Ѵ�.
			startIndex = i;
			// 1.2.2. ���ڸ� �����.
			delete this->glyphs.GetAt(i);
			this->glyphs.Delete(i);
			// 1.2.3. ���ڸ� �д´�.
			character = this->glyphs.GetAt(i);
			this->capacity--;
			this->length--;
		}
		
		i++;
	}
	// 2. ����� ������ ��ġ�� �̵��Ѵ�.(2021.01.25)
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
	//1. length�� current���� �������� �ݺ��Ѵ�.
	while (this->length > current) {
		//1.1. current��° ���ڸ� �����´�.
		letter = this->glyphs.GetAt(current);
		//1.2. ���ο� �ٿ� ���ڸ� �߰����ش�.
		row->Add(letter->Clone());
		//1.3. current��° ���ڸ� �����.
		delete letter;
		this->glyphs.Delete(current);
		this->capacity--;
		this->length--;
	}
	//2. ���ο� �ٿ��� ù��°�� �̵��Ѵ�.
	row->First();
	this->current = current;
	row->Select(this->onIsSelected);

	// 3. ���� ���� ������ ���ڸ� �д´�.
	if (this->length > 0) {
		lastLetter = this->glyphs.GetAt(this->length - 1);
		onIsLastLetterSelected = lastLetter->IsSelected();
	}

	// 4. ���� ���� ù ��° ���ڸ� �д´�.
	if (row->GetLength() > 0) {
		firstLetter = row->GetAt(0);
		onIsFirstLetterSelected = firstLetter->IsSelected();
	}

	// 5. ù ��° ���ڿ� ������ ���ڰ� ���õǾ� ������ ���� �����Ѵ�.
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

void Row::Combine(Glyph* row) { // �Էµ� ���� ��ģ��. (2021.01.13)
	Glyph* letter;
	Glyph* letterCopy;

	// 1. �Էµ� �ٿ��� ���ڰ� �ϳ��� ���� ���� �ݺ��Ѵ�.
	Long lastCurrent = this->length;
	Long length = row->GetLength();
	while (length > 0) {
		letter = row->GetAt(0); // 1.1. ���ڸ� �д´�.
		letterCopy = letter->Clone();
		this->glyphs.AppendFromRear(letterCopy); // 1.2. ���� ���� �������� �߰��Ѵ�.
		this->capacity++;
		this->length++;
		row->Remove(0); // 1.3. �Էµ� ���� ���ڸ� �����. (1.1, 1.2, 1.3 �� ���ļ� ���ڸ� �ű��.)
		length = row->GetLength();
	}
	this->current = lastCurrent;
	this->onIsSelected = row->IsSelected(); // 2. �������� �� ���� ���θ� �̾� �޴´�.
}

void Row::Combine(Long index, Glyph* row) {
	Glyph* letter;
	Glyph* letterCopy;

	// 1. �Էµ� �ٿ��� ���ڰ� �ϳ��� ���� ���� �ݺ��Ѵ�.
	Long length = row->GetLength();
	while (length > 0) {
		letter = row->GetAt(0); // 1.1. ���ڸ� �д´�.
		letterCopy = letter->Clone();
		this->glyphs.Insert(index, letterCopy); // 1.2. ���� ���� ��ġ�� �߰��Ѵ�.
		index++;
		this->capacity++;
		this->length++;
		row->Remove(0); // 1.3. �Էµ� ���� ���ڸ� �����. (1.1, 1.2, 1.3 �� ���ļ� ���ڸ� �ű��.)
		length = row->GetLength();
	}
	this->current = index;
	this->onIsSelected = row->IsSelected(); // 2. �������� �� ���� ���θ� �̾� �޴´�.
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
	this->onIsSelected = onIsSelected; // (2021.01.25) ����
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