// Note.cpp
/*
���ϸ�Ī : Note.cpp
��� : �޸��� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.20
*/
#include "Note.h"
#include "Glyph.h"
#include "GlyphVisitors.h"
#include "GlyphFactory.h"

Note::Note(Long capacity)
	: Composite(capacity) {
}

Note::Note(const Note& source)
	: Composite(source) {
}

Note::~Note() {
}

Note& Note::operator =(const Note& source) {
	Composite::operator =(source);

	return* this;
}

Long Note::Add(Long current, Glyph* glyph) {
	Long index;
	index = this->glyphs.Insert(current, glyph);
	this->capacity++;
	this->length++;
	this->current = index;

	return this->current;
}

void Note::Paste(Glyph* dummyNote) {
	GlyphFactory glyphFactory;
	Glyph* dummyRow;
	Glyph* currentRow;
	Glyph* splitRow;
	Glyph* letter;
	Glyph* letterCopy;

	bool onIsSelected;
	
	Long i = 0;
	Long j;

	currentRow = this->glyphs.GetAt(this->current);
	splitRow = glyphFactory.Create((char*)"\r");
	currentRow->Split(splitRow, currentRow->GetCurrent());

	while (i < dummyNote->GetLength()) {
		dummyRow = dummyNote->GetAt(i);
		j = 0;
		while (j < dummyRow->GetLength()) {
			letter = dummyRow->GetAt(j);
			letterCopy = letter->Clone();
			currentRow->Add(currentRow->GetCurrent(), letterCopy);
			j++;
		}
		onIsSelected = dummyRow->IsSelected();
		currentRow->Select(onIsSelected);
		if (i + 1 < dummyNote->GetLength()) {
			currentRow = glyphFactory.Create((char*)"\r");
			this->glyphs.Insert(this->current + 1, currentRow);
			this->capacity++;
			this->length++;
			this->current++;
		}
		i++;
	}

	if (splitRow->GetLength() > 0) {
		currentRow->Combine(splitRow);
		currentRow->Select(false);
	}
	if (splitRow != 0) {
		delete splitRow;
	}
}

void Note::Paste(string text) {
	GlyphFactory glyphFactory;
	Glyph* currentRow = 0;
	Glyph* glyph;

	char character[2];

	Long rowCurrent;
	Long length = text.length();
	Long i = 0;

	currentRow = this->glyphs.GetAt(this->current);
	while (i < length) {
		character[0] = text[i];
		if (character[0] == '\r' || character[0] == '\n') {
			glyph = glyphFactory.Create(character);
			currentRow->Split(glyph, currentRow->GetCurrent());
			currentRow = glyph;
			this->current = this->glyphs.Insert(this->current + 1, glyph);
			this->capacity++;
			this->length++;
			if (i + 1 < length && text[i] == '\r' && text[i + 1] == '\n') {
				i++;
			}
		}
		else if (character[0] == 0x09 || character[0] >= 0x20 && character[0] < 0x7F) {
			glyph = glyphFactory.Create(character);
			rowCurrent = currentRow->GetCurrent();
			currentRow->Add(rowCurrent, glyph);
		}
		else if (character[0] & 0x80) {
			i++;
			character[1] = text[i];
			glyph = glyphFactory.Create(character);
			rowCurrent = currentRow->GetCurrent();
			currentRow->Add(rowCurrent, glyph);
		}
		i++;
	}
}

bool Note::RemoveSelect() {
	bool ret = false;
	bool onIsFound = false;
	bool onIsRemoved = false;
	bool onIsCurrentSelected = false;

	Long noteCurrent = 0;
	Long j;
	Long i = 0;

	Glyph* currentRow = 0;
	Glyph* row = 0;
	Glyph* character = 0;
	// 1. ������ ������ �ٺ��� �۰�, �޸��� ��ġ�� ã�� ���� �ݺ��Ѵ�.(�޸����� ���õ� ���� ��ġ�� ã�´�)
	while (i < this->length && onIsFound != true) {
		// 1.1. ���� �д´�.
		row = this->glyphs.GetAt(i);
		j = 0;
		// 1.2. ���� ������ ĭ���� �۰�, �޸��� ��ġ�� ã�� ���� �ݺ��Ѵ�.
		while (j < row->GetLength() && onIsFound != true) {
			// 1.2.1. ���ڸ� �д´�.
			character = row->GetAt(j);
			// 1.2.2. ���ڰ� ���õǾ� ������ �޸��� ��ġ�� ���Ѵ�.
			if (character->IsSelected() == true) {
				noteCurrent = i;
				onIsFound = true;
			}
			j++;
		}
		// 1.3. ���� ������ ĭ�̰� ���� ���� ���õǾ� �ְ� ã�� �������� �޸��� ��ġ�� ���Ѵ�.
		if (j >= row->GetLength() && row->IsSelected() == true && onIsFound != true) {
			noteCurrent = i;
			onIsFound = true;
		}
		i++;
	}
	// 2. ��ġ�� ã������ �޸������� ���� ��ġ�� �̵��ϰ�, ���� ���� ���Ѵ�.
	if (onIsFound == true) {
		ret = onIsFound;
		this->current = noteCurrent;
		currentRow = this->glyphs.GetAt(this->current);
		onIsCurrentSelected = currentRow->IsSelected();
	}
	// 3. ��ġ�� ã������ �ٿ��� ���õ� ���ڵ��� �����.
	i = noteCurrent;
	if (i < this->length && onIsFound == true) {
		row = this->glyphs.GetAt(i);
		row->RemoveSelect();
		i++;
	}
	// 4. ������ �ٺ��� �۰�, ������ ���๮�ڰ� ���õǾ� ������ �ݺ��Ѵ�.
	while (i < this->length && onIsCurrentSelected == true) {
		// 4.1. ���� �д´�.
		row = this->glyphs.GetAt(i);
		// 4.2. �ٿ��� �����.
		row->RemoveSelect();
		// 4.3. �������� ���๮�ڰ� ���õǾ� ���� ������ ���� ���θ� �������� �Ѵ�.
		if (row->IsSelected() != true) {
			onIsCurrentSelected = false;
		}
		// 4.4. ���� �ٿ��� ���� ��ģ��.
		currentRow->Combine(row);
		delete this->glyphs.GetAt(i);
		this->glyphs.Delete(i);
		this->capacity--;
		this->length--;
	}

	return ret;
}

void Note::Combine(Long current) { // ���� ���� �ڵ������ �ٵ��� ��ġ�� ����
	Glyph* row = this->glyphs.GetAt(current);
	Glyph* nextRow = 0;

	Long noteCurrent;
	Long rowCurrent = 0;
	Long i = 0;
	while (i <= current) {// 1. ���� ���� ����(onIsRow�� true��) ���� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrent = i;
		}
		i++;
	}

	i = noteCurrent;
	while (i < current) { // 2. ��ġ�� �� �� �̵��� ĭ�� �̸� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	i = noteCurrent; // 3. ���� ���� �ڵ������ �ٸ� �����ش�.
	if (this->length > i + 1) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
	}
	while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
		row->Combine(nextRow);
		row->Select(nextRow->IsSelected()); // ���� ���� ���ÿ��θ� Ȯ���Ͽ� �����Ѵ�.(2021.01.18)
		delete nextRow;
		this->glyphs.Delete(i + 1);
		nextRow = this->glyphs.GetAt(i + 1);
		this->capacity--;
		this->length--;
	}
	// 4. ��ġ�� �� �� ��ġ�� �̵��Ѵ�.
	this->current = noteCurrent;
	this->glyphs.GetAt(this->current)->Move(rowCurrent);
}

void Note::CombineSelectedRows() {
	Glyph* row;
	Glyph* nextRow;
	Glyph* glyph;

	bool onIsStop = false;
	bool onIsSelected = false;
	bool onIsCurrentRowSelected = false;

	Long startSelectedtNoteCurrent = -1;
	Long noteCurrentForRowCurrent;
	Long noteCurrent = -1;
	Long rowCurrent = 0;
	Long j;
	Long i = 0;
	// 1. ���õ� ���� ���� ��ġ�� ã�´�.
	while (i < this->length && onIsStop != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsStop != true) {
			glyph = row->GetAt(j);
			if (glyph->IsSelected() == true) {
				startSelectedtNoteCurrent = i;
				onIsStop = true;
			}
			j++;
		}
		if (row->IsSelected() == true) {
			startSelectedtNoteCurrent = i;
			onIsStop = true;
		}
		i++;
	}

	// 2. ��ġ�� �� ���� ĭ�� ���ϱ� ���� ���� ��ġ�� / ��ġ�� ������ �� ��ġ�� / �̸� ���Ѵ�.
	i = 0;
	while (i <= this->current) {
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			noteCurrent++;
		}
		i++;
	}

	// 3. ��ġ�� �� ���� ĭ�� ���Ѵ�.
	i = noteCurrentForRowCurrent;
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	// 4. ���õ� �ڵ������ �ٵ��� ��ģ��.
	// ===================================
	// 1. ���õ� ù ��° ���� ������
	i = startSelectedtNoteCurrent;
	if (startSelectedtNoteCurrent >= 0 && i < this->length) {
		// 1.1. ���õ� �ٰ� �������� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);

		// 1.2. �������� ��ģ��.
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow);
			onIsSelected = nextRow->IsSelected();
			row->Select(onIsSelected);
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		// 1.3. ������ ���� ���� ���θ� ���Ѵ�.
		onIsCurrentRowSelected = row->IsSelected();
	}

	// 2. ���õ� ���� �������� ���� ��ģ��.
	while (i < this->length - 1 && onIsCurrentRowSelected == true) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow);
			onIsSelected = nextRow->IsSelected();
			row->Select(onIsSelected);
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		onIsCurrentRowSelected = row->IsSelected();
		i++;
	}
	// ===================================
	// 5. ���� ��ġ�� �̵��Ѵ�.
	this->current = noteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(rowCurrent);
}

void Note::CombineAll() { // ��� ���� �ڵ������ �ٵ��� ��ģ��.
	Glyph* row;
	Glyph* nextRow;

	Long noteCurrentForRowCurrent;
	Long noteCurrent = -1;
	Long rowCurrent = 0;
	Long i = 0;
	while (i <= this->current) {// 1. ��ġ�� �� ���� ĭ�� ���ϱ� ���� ���� ��ġ�� / ��ġ�� ������ �� ��ġ�� / �̸� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			noteCurrent++;
		}
		i++;
	}

	i = noteCurrentForRowCurrent;// 2. ��ġ�� �� ���� ĭ�� ���Ѵ�.
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	i = 0;
	while (i < this->length - 1) {// 3. ��� �ڵ������ ���� ��ģ��.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow); // �˾Ƽ� �Ҵ����� ���ش�.
			row->Select(nextRow->IsSelected()); // ���� ���� ���ÿ��θ� Ȯ���Ͽ� �����Ѵ�.(2021.01.18)
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		i++;
	}
	// 4. ��ġ�� �� �� ��ġ�� �̵��Ѵ�.
	this->current = noteCurrent;
	this->glyphs.GetAt(this->current)->Move(rowCurrent);
}

void Note::Select(Long startRowCurrent, Long startNoteCurrent, Long endRowCurrent, Long endNoteCurrent, bool onIsSelect) {
	Long tempRowCurrent = startRowCurrent;
	Long tempNoteCurrent = startNoteCurrent;

	Glyph* row;

	if (startNoteCurrent > endNoteCurrent || (startNoteCurrent == endNoteCurrent && startRowCurrent > endRowCurrent)) {
		startRowCurrent = endRowCurrent; //lLeft = lRight;
		startNoteCurrent = endNoteCurrent; //lTop = lBottom;
		endRowCurrent = tempRowCurrent; //lRight = left;
		endNoteCurrent = tempNoteCurrent; //lBottom = top;
	}

	row = this->glyphs.GetAt(startNoteCurrent);
	if (startNoteCurrent != endNoteCurrent) {
		row->Select(startRowCurrent, row->GetLength(), onIsSelect);
		row->Select();
	}
	else {
		row->Select(startRowCurrent, endRowCurrent, onIsSelect);
	}

	Long i = startNoteCurrent + 1;
	while (i < endNoteCurrent) {
		row = this->glyphs.GetAt(i);
		row->Select(0, row->GetLength(), onIsSelect);
		row->Select();
		i++;
	}

	if (startNoteCurrent != endNoteCurrent && i <= endNoteCurrent) {
		row = this->glyphs.GetAt(i);
		row->Select(0, endRowCurrent, onIsSelect);
	}
}

void Note::SelectCount(Long startRowCurrent, Long startNoteCurrent, Long textCount) {
	Glyph* row;
	Glyph* nextRow;
	Glyph* glyph;

	bool onIsFirst;
	
	Long endRowCurrent = startRowCurrent;
	Long endNoteCurrent = startNoteCurrent;
	Long i;

	// 2.1. ������ ���õ� ���ڵ��� ��� ���� ���� �Ѵ�.
	i = 0;
	while (i < this->length) {
		row = this->glyphs.GetAt(i);
		row->SelectAll(false);
		i++;
	}

	// 2.2. ���� ��ġ���� ���� ������ŭ �����Ѵ�.
	onIsFirst = true;
	i = startNoteCurrent;
	while (i < this->length && textCount > 0) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		endRowCurrent = 0;
		if (onIsFirst == true) {
			endRowCurrent = startRowCurrent;
			onIsFirst = false;
		}
		while (endRowCurrent < row->GetLength() && textCount > 0) {
			glyph = row->GetAt(endRowCurrent);
			glyph->Select(true);
			textCount--;
			endRowCurrent++;
		}
		if (endRowCurrent >= row->GetLength() && i < this->length - 1 && textCount > 0) {
			row->Select(true);
		}
		if (i < this->length - 1 && nextRow->GetOnIsRow() == true) {
			textCount--;
		}
		endNoteCurrent = i;
		i++;
	}
	// 2.3. ������ ��ġ�� �̵��Ѵ�.
	this->current = endNoteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(endRowCurrent);
}

void Note::SelectAll(bool onIsSelect) {
	Glyph* row;

	Long i = 0;
	while (i < this->length - 1) {
		row = this->glyphs.GetAt(i);
		row->SelectAll(onIsSelect);
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		row->Select(0, row->GetLength(), onIsSelect);
	}
}

bool Note::FindPrevious(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase) {
	bool ret = false;
	bool onIsFirst = true;
	bool onIsPrevious = false;
	bool onIsMatching = false;

	Glyph* row = 0;
	Glyph* nextRow;
	Glyph* glyph;
	string character;
	char findText[3];

	Long findTextLength = (Long)strFindWhat.length();
	Long matchCount = 0;
	Long textCount = strFindWhatLen;
	Long startRowCurrent;
	Long startNoteCurrent;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long k = (Long)strFindWhat.length() - 1;
	Long j;
	Long i = this->current;

	// 1. ���� ��ġ�� �ٺ��� ������ �ٺ��� �۰�, ��Ī�Ǵ� ������ ã�� ������ ������ ���� ���� ���� �ݺ��Ѵ�.
	while (i >= 0 && matchCount != strFindWhatLen) {
		// 1.1. ���� �д´�.
		row = this->glyphs.GetAt(i);
		j = row->GetLength() - 1;
		// 1.2. ó���̸� ���� ��ġ�� ���� ���� ĭ���� �����Ѵ�.
		if (onIsFirst == true) {
			j = row->GetCurrent() - 1;
			onIsFirst = false;
		}
		// 1.3. ���� ������ ĭ���� �۰�, ��Ī�Ǵ� ������ ã�� ������ ������ ���� ���� ���� �ݺ��Ѵ�.
		while (j >= 0 && row->GetLength() > 0 && matchCount != strFindWhatLen) {
			// 1.3.1. ���ڸ� �д´�.
			character = row->GetAt(j)->GetString();
			onIsMatching = false;
			// 1.3.2. ã�� ������ �� ���ڸ� ���´�.
			if (strFindWhat[k] & 0x80) {
				findText[0] = strFindWhat[k - 1];
				findText[1] = strFindWhat[k];
				findText[2] = '\0';
				k -= 2;
			}
			else {
				findText[0] = strFindWhat[k];
				findText[1] = '\0';
				k--;
			}
			// 1.3.3. ã�� �� ���ڿ� ���� ���ڰ� ������ ��Ī ���θ� ������ �Ѵ�.
			if ((character[0] & 0x80 && character.compare(findText) == 0) ||
				(onIsMatchCase == true && character.compare(findText) == 0)) {
				onIsMatching = true;
			}
			// 1.3.4. ��/�ҹ��� ���п��ΰ� üũ�Ǿ� ���� �ʰ� ã�� �� ���ڿ� ���� ���ڸ� ��/�ҹ��� ���о��� ���ؼ� ������
			// ��Ī ���θ� ������ �Ѵ�.
			else if (onIsMatchCase != true && _stricmp(character.c_str(), findText) == 0) {
				onIsMatching = true;
			}
			// 1.3.5. ��Ī ���ΰ� ���̸� ��Ī ������ ������Ų��.
			if (onIsMatching == true) {
				matchCount++;
				onIsPrevious = true;
			}
			// 1.3.6. ��Ī ���ΰ� �����̸� ��Ī������ ����ġ ��Ų��.
			else {
				matchCount = 0;
				k = (Long)strFindWhat.length() - 1;
				onIsPrevious = false;
			}
			j--;
		}
		// 1.4. ã�� ���ڸ� �д´�.
		if (k > 0) {
			findText[0] = strFindWhat[k];
			findText[1] = '\0';
		}

		// 1.5. ���ڰ� ���๮���̸� ��Ī������ �����Ѵ�.
		if (findText[0] == '\n') {
			k--;
			matchCount++;
			onIsPrevious = true;
		}
		// 1.6. ���� ���� ���� ���̰�, �������� �̵��� ���� �ְ�, ��Ī ������ ã�� ���� ������ ���� ������ ��Ī������ ����ġ ��Ų��.
		// �ڵ������� Ǯ�� �ʰ� ã�� �� �ְ� �� ����.
		else if (row->GetOnIsRow() == true && onIsPrevious == true && matchCount != strFindWhatLen) {
			matchCount = 0;
			k = (Long)strFindWhat.length() - 1;
			onIsPrevious = false;
		}
		
		i--;
	}
	// 2. ��Ī������ ã�� ���� ������ ������(ã������)
	if (matchCount == strFindWhatLen && onIsFirst != true) {
		// 2.1. ���� ��ġ�� ���Ѵ�.
		startRowCurrent = j + 1;
		startNoteCurrent = i + 1;

		// 2.2. ������ ���õ� ���ڵ��� ��� ���� ���� �Ѵ�.
		i = 0;
		while (i < this->length) {
			row = this->glyphs.GetAt(i);
			row->SelectAll(false);
			i++;
		}

		// 2.3. ���� ��ġ���� ���� ������ŭ �����Ѵ�.
		onIsFirst = true;
		i = startNoteCurrent;
		while (i < this->length && textCount > 0) {
			row = this->glyphs.GetAt(i);
			nextRow = this->glyphs.GetAt(i + 1);
			endRowCurrent = 0;
			if (onIsFirst == true) {
				endRowCurrent = startRowCurrent;
				onIsFirst = false;
			}
			while (endRowCurrent < row->GetLength() && textCount > 0) {
				glyph = row->GetAt(endRowCurrent);
				glyph->Select(true);
				textCount--;
				endRowCurrent++;
			}
			if (endRowCurrent >= row->GetLength() && i < this->length - 1 && textCount > 0) {
				row->Select(true);
			}
			if (i < this->length - 1 && nextRow->GetOnIsRow() == true) {
				textCount--;
			}
			endNoteCurrent = i;
			i++;
		}
		// 2.4. ������ ��ġ�� �̵��Ѵ�.
		this->current = endNoteCurrent;
		row = this->glyphs.GetAt(this->current);
		row->Move(endRowCurrent);

		ret = true;
	}

	return ret;
}

bool Note::FindNext(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase) {
	bool ret = false;

	Glyph* previous = 0;
	Glyph* row = 0;
	Glyph* nextRow;
	Glyph* glyph;
	string character;
	char findText[3];

	bool onIsFirst = true;
	bool onIsNext = false;
	bool onIsMatching = false;

	Long findTextLength = (Long)strFindWhat.length();
	Long textCount = strFindWhatLen;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long matchCount = 0;
	Long k = 0;
	Long j;
	Long i = this->current;

	// 1. ���� ��ġ�� �ٺ��� ������ �ٺ��� �۰�, ��Ī�Ǵ� ������ ã�� ������ ������ ���� ���� ���� �ݺ��Ѵ�.
	while (i < this->length && matchCount != strFindWhatLen) {
		// 2. �ٰ� �������� �д´�.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);

		j = 0;
		// 1.2. ó���̸� ���� ��ġ�� ���� ���� ĭ���� �����Ѵ�.
		if (onIsFirst == true) {
			j = row->GetCurrent();
			onIsFirst = false;
		}
		// 1.3. ���� ������ ĭ���� �۰�, ��Ī�Ǵ� ������ ã�� ������ ������ ���� ���� ���� �ݺ��Ѵ�.
		while (j < row->GetLength() && matchCount != strFindWhatLen) {
			// 1.3.1. ���ڸ� �д´�.
			glyph = row->GetAt(j);
			character = glyph->GetString();
			onIsMatching = false;
			// 1.3.2. ã�� ������ �� ���ڸ� ���´�.
			if (strFindWhat[k] & 0x80) {
				findText[0] = strFindWhat[k];
				findText[1] = strFindWhat[k + 1];
				findText[2] = '\0';
				k += 2;
			}
			else {
				findText[0] = strFindWhat[k];
				findText[1] = '\0';
				k++;
			}
			// 1.3.3. ã�� �� ���ڿ� ���� ���ڰ� ������ ��Ī ���θ� ������ �Ѵ�.
			if ((character[0] & 0x80 && character.compare(findText) == 0) ||
				(onIsMatchCase == true && character.compare(findText) == 0)) {
				onIsMatching = true;
			}
			// 1.3.4. ��/�ҹ��� ���п��ΰ� üũ�Ǿ� ���� �ʰ� ã�� �� ���ڿ� ���� ���ڸ� ��/�ҹ��� ���о��� ���ؼ� ������
			// ��Ī ���θ� ������ �Ѵ�.
			else if (onIsMatchCase != true && _stricmp(character.c_str(), findText) == 0) {
				onIsMatching = true;
			}
			// 1.3.5. ��Ī ���ΰ� ���̸� ��Ī ������ ������Ų��.
			if (onIsMatching == true) {
				matchCount++;
				onIsNext = true;
			}
			// 1.3.6. ��Ī ���ΰ� �����̸� ��Ī������ ����ġ ��Ų��.
			else {
				matchCount = 0;
				k = 0;
				onIsNext = false;
			}
			j++;
		}
		// 1.4. ã�� ���ڸ� �д´�.
		if (k < findTextLength) {
			findText[0] = strFindWhat[k];
			findText[1] = '\0';
		}
		
		// 1.5. ���ڰ� ���๮���̸� ��Ī������ �����Ѵ�.
		if (findText[0] == '\n') {
			k++;
			matchCount++;
			onIsNext = true;
		}
		// 1.6. ã�� ���ڰ� ���๮�ڰ� �ƴϰ�, �������� �ְ�, �������� ���� ���̰�, �����̵� ���ΰ� ���̰�,  
		// ��Ī������ ã�� ���ڰ����� ���� ������
		// �ڵ������� Ǯ�� �ʰ� ã�� �� �ְ� �� ����.
		else if (i < this->length - 1 && nextRow->GetOnIsRow() == true && onIsNext == true && matchCount != strFindWhatLen) {
			matchCount = 0;
			k = 0;
			onIsNext = false;
		}
		i++;
	}

	// 2. ��Ī������ ã�� ���� ������ ������(ã������)
	if (matchCount == strFindWhatLen) {
		// 2.1. ������ ��ġ�� ���Ѵ�. �Ųٷ� ���ߵ�
		endRowCurrent = j;
		endNoteCurrent = i - 1;

		// 2.2. ������ ���õ� ���ڵ��� ��� ���� ���� �Ѵ�.
		i = 0;
		while (i < this->length) {
			row = this->glyphs.GetAt(i);
			row->SelectAll(false);
			i++;
		}

		// 2.3. ������ ��ġ���� ���� ������ŭ �����Ѵ�.
		onIsFirst = true;
		i = endNoteCurrent;
		while (i >= 0 && textCount > 0) {
			previous = this->glyphs.GetAt(i - 1);
			row = this->glyphs.GetAt(i);
			j = row->GetLength() - 1;
			if (onIsFirst == true) {
				j = endRowCurrent - 1;
				onIsFirst = false;
			}
			while (j >= 0 && textCount > 0) {
				glyph = row->GetAt(j);
				glyph->Select(true);
				textCount--;
				j--;
			}
			if (j < 0 && i > 0 && textCount > 0) {
				previous->Select(true);
			}
			if (row->GetOnIsRow() == true) {
				textCount--;
			}
			i--;
		}

		// 2.4. ������ ��ġ�� �̵��Ѵ�.
		this->current = endNoteCurrent;
		row = this->glyphs.GetAt(this->current);
		row->Move(endRowCurrent);

		ret = true;
	}

	return ret;
}

Long Note::Previous() {
	this->current--;
	if (this->current < 0) {
		this->current = 0;
	}

	return this->current;
}

Long Note::Next() {
	this->current++;
	if (this->current >= this->length) {
		this->current = this->length - 1;
	}

	return this->current;
}

Long Note::Last() {
	this->current = this->length - 1;

	return this->current;
}

void Note::MoveActualCurrent(Long rowCurrent, Long noteCurrent) {
	Glyph* row;

	Long rowCount = 0;
	Long findNoteCurrent = 0;
	Long i = 0;

	while (i < this->length && rowCount <= noteCurrent) {// 1. ���� ���� ����(onIsRow�� true��) ���� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			findNoteCurrent = i;
			rowCount++;
		}
		i++;
	}

	row = this->glyphs.GetAt(findNoteCurrent);
	i = findNoteCurrent;
	while (i < this->glyphs.GetLength() - 1 && rowCurrent > row->GetLength()) {
		findNoteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphs.GetAt(i + 1);
		i++;
	}

	this->current = findNoteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(rowCurrent);
}

Long Note::MoveToFirstSelectedCurrent() {
	Glyph* row;

	Long noteCurrent = -1;
	Long rowCurrent = -1;
	Long i = 0;
	while (i < this->length && rowCurrent == -1) {
		row = this->glyphs.GetAt(i);
		rowCurrent = row->MoveToFirstSelectedCurrent();
		i++;
	}
	if (rowCurrent != -1) {
		noteCurrent = i - 1;
		this->current = noteCurrent;
	}

	return noteCurrent;
}

Long Note::MoveToLastSelectedCurrent() {
	Glyph* row;

	bool onIsSelected;

	Long noteCurrent = 0;
	Long rowCurrent = -1;
	Long i = this->length;
	while (i > 0 && rowCurrent == -1) {
		row = this->glyphs.GetAt(i - 1);
		rowCurrent = row->MoveToLastSelectedCurrent();
		onIsSelected = row->IsSelected();
		i--;
	}
	if (rowCurrent != -1) {
		noteCurrent = i;
		if (onIsSelected == true) {
			noteCurrent++;
		}
		this->current = noteCurrent;
	}

	return noteCurrent;
}

void Note::GetCombineCurrent(Long* rowCurrent, Long* noteCurrent) {
	Glyph* row;

	Long noteCurrentForRowCurrent;
	Long findNoteCurrent = -1;
	Long findRowCurrent = 0;
	Long i = 0;
	while (i <= this->current) {// 1. ��ġ�� �� ���� ĭ�� ���ϱ� ���� ���� ��ġ�� / ��ġ�� ������ �� ��ġ�� / �̸� ���Ѵ�.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			findNoteCurrent++;
		}
		i++;
	}

	i = noteCurrentForRowCurrent;// 2. ��ġ�� �� ���� ĭ�� ���Ѵ�.
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		findRowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		findRowCurrent += row->GetCurrent();
	}

	// 3. �̸� ���� ��ġ�� ����Ѵ�.
	*noteCurrent = findNoteCurrent;
	*rowCurrent = findRowCurrent;
}

void Note::GetSelectRange(Long* startRowCurrent, Long* startNoteCurrent, Long* endRowCurrent, Long* endNoteCurrent) {
	Long j;
	Long i = 0;

	Glyph* currentRow = this->glyphs.GetAt(this->current);
	Glyph* row;
	Glyph* character = 0;

	bool onIsFound = false; // ���� ���õ� ��ġ�� ã�� ���� �÷���
	bool onIsFirst = true; // ������ ���õ� ��ġ�� ã�� ���� �÷���

	*startRowCurrent = 0;
	*startNoteCurrent = 0;
	*endRowCurrent = 0;
	*endNoteCurrent = 0;

	// 1. ���� ���õ� ��ġ�� ���Ѵ�.
	while (i < this->length && onIsFound != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsFound != true) {
			character = row->GetAt(j);
			if (character->IsSelected() == true) {
				*startRowCurrent = j;
				*startNoteCurrent = i;
				onIsFound = true;
			}
			j++;
		}
		if (j >= row->GetLength() && row->IsSelected() == true && onIsFound != true) {
			*startRowCurrent = j;
			*startNoteCurrent = i;
			onIsFound = true;
		}
		i++;
	}

	if (onIsFound != true) {
		*startRowCurrent = currentRow->GetCurrent();
		*startNoteCurrent = this->current;
	}
	
	bool onIsStop = false;
	// 2. ������ ���õ� ��ġ�� ���Ѵ�.
	i =* startNoteCurrent;
	while (i < this->length && onIsStop != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		if (onIsFirst == true) {
			j =* startRowCurrent;
			onIsFirst = false;
		}
		while (j < row->GetLength() && onIsStop != true) {
			character = row->GetAt(j);
			if (character->IsSelected() == true) {
				*endRowCurrent = j + 1;
				*endNoteCurrent = i;
			}
			else {
				onIsStop = true;
			}
			j++;
		}
		if (j >= row->GetLength() && row->IsSelected() == true) {
			*endRowCurrent = 0;
			*endNoteCurrent = i + 1;
		}
		i++;
	}

	if (onIsFound != true) {
		*endRowCurrent = currentRow->GetCurrent();
		*endNoteCurrent = this->current;
	}
}

Glyph* Note::Clone() {
	return new Note(*this);
}

void Note::Accept(GlyphVisitor* glyphVisitor) {
	glyphVisitor->Visit(this);
}

string Note::GetString() {
	string str;
	string text;

	Glyph* row;
	Glyph* nextRow;
	
	Long j;
	Long i = 0;
	while (i < this->length - 1) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		j = 0;
		while (j < row->GetLength()) {
			text = row->GetAt(j)->GetString();
			str += text;

			j++;
		}
		if (nextRow->GetOnIsRow() == true) {
			str += "\n";
		}

		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength()) {
			text = row->GetAt(j)->GetString();
			str += text;

			j++;
		}
	}

	return str;
}

string Note::GetSelectString() {
	string str;

	Glyph* row;
	Glyph* nextRow;

	Long i = 0;
	// 1. ������ �� ������ �ݺ��Ѵ�.
	while (i < this->length - 1) {
		// 1.1. ���� �д´�.
		row = this->glyphs.GetAt(i);
		// 1.2. ���� ���� �д´�.
		nextRow = this->glyphs.GetAt(i + 1);

		// 1.3. �ٿ��� ���õ� ���ڵ��� �����Ѵ�.
		str += row->GetSelectString();
		// 1.4. ���� ���� ���õǾ� �ְ�, ���� ���� ���� ���̸� ���๮�ڸ� �����Ѵ�.
		if (row->IsSelected() == true && nextRow->GetOnIsRow() == true) {
			str += "\n";
		}

		i++;
	}
	// 2. ������ ���� ���õ� ���ڵ��� �����Ѵ�.
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		str += row->GetSelectString();
	}
	// 3. ������ ���ڵ��� ����Ѵ�.
	return str;
}

bool Note::IsSelected() const {
	bool onIsSelect = false;

	Glyph* row;
	Glyph* character;

	Long j;
	Long i = 0;
	while (i < this->length && onIsSelect != true) {
		row = const_cast<Note*>(this)->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsSelect != true) {
			character = row->GetAt(j);
			onIsSelect = character->IsSelected();
			j++;
		}
		if (row->IsSelected() == true) {
			onIsSelect = row->IsSelected();
		}
		i++;
	}

	return onIsSelect;
}