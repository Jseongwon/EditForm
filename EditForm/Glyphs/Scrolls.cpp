// Scrolls.cpp
/*
파일명칭 : Scrolls.cpp
기능 : 스크롤과 관련된 클래스들를 만든다.
작성자 : 정성원
작성일자 : 2021.08.25
*/
#include "Scrolls.h"

Scroll::Scroll() {
	this->minimum = 0;
	this->maximum = 0;
	this->lineLength = 0;
	this->pageLength = 0;
	this->position = -1;
}

Scroll::Scroll(Long minimum, Long maximum, Long rowLength, Long pageLength, Long position) {
	this->minimum = minimum;
	this->maximum = maximum;
	this->lineLength = rowLength;
	this->pageLength = pageLength;
	this->position = position;
}

Scroll::Scroll(const Scroll& source) {
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->lineLength = source.lineLength;
	this->pageLength = source.pageLength;
	this->position = source.position;
}

Scroll::~Scroll() {
}

Scroll& Scroll::operator =(const Scroll& source) {
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->lineLength = source.lineLength;
	this->pageLength = source.pageLength;
	this->position = source.position;

	return* this;
}

void Scroll::First() {
	this->position = 0;
}

void Scroll::PreviousRow() {
	this->position -= this->lineLength;
	if (this->position < 0) {
		this->position = 0;
	}
}

void Scroll::NextRow() {
	this->position += this->lineLength;
	if (this->position >= this->maximum - this->pageLength) {
		this->position = this->maximum - this->pageLength + 2;
	}
}

void Scroll::PreviousPage() {
	this->position -= this->pageLength - this->lineLength;
	if (this->position < 0) {
		this->position = 0;
	}
}

void Scroll::NextPage() {
	this->position += this->pageLength - this->lineLength;
	if (this->position >= this->maximum - this->pageLength) {
		this->position = this->maximum - this->pageLength + 2;
	}
}

void Scroll::PreviousOneFifth() {
	this->position -= this->pageLength / 5;
	if (this->position < 0) {
		this->position = 0;
	}
}

void Scroll::NextOneFifth() {
	this->position += this->pageLength / 5;
	if (this->position >= this->maximum - this->pageLength) {
		this->position = this->maximum - this->pageLength + 2;
	}
}

void Scroll::Last() {
	this->position = this->maximum - this->pageLength + 2;
}

void Scroll::Move(Long amount) {
	this->position = amount;
	if (this->position < 0) {
		this->position = 0;
	}
	else if (this->position >= this->maximum - this->pageLength && this->maximum > this->pageLength) {
		this->position = this->maximum - this->pageLength + 2;
	}
}

// HorizontalScroll
HorizontalScroll::HorizontalScroll() {
}

HorizontalScroll::HorizontalScroll(Long minimum, Long maximum, Long rowLength, Long pageLength, Long position)
	: Scroll(minimum, maximum, rowLength, pageLength, position) {
}

HorizontalScroll::HorizontalScroll(const HorizontalScroll& source)
	: Scroll(source.minimum, source.maximum, source.lineLength, source.pageLength, source.position) {
}

HorizontalScroll::~HorizontalScroll() {
}

HorizontalScroll& HorizontalScroll::operator =(const HorizontalScroll& source) {
	Scroll::operator=(source);

	return* this;
}

Scroll* HorizontalScroll::Clone() {
	return new HorizontalScroll(this->minimum, this->maximum, this->lineLength, this->pageLength, this->position);
}

// VerticalScroll
VerticalScroll::VerticalScroll() {
}

VerticalScroll::VerticalScroll(Long minimum, Long maximum, Long rowLength, Long pageLength, Long position)
	: Scroll(minimum, maximum, rowLength, pageLength, position) {
}

VerticalScroll::VerticalScroll(const VerticalScroll& source)
	: Scroll(source.minimum, source.maximum, source.lineLength, source.pageLength, source.position) {
}

VerticalScroll::~VerticalScroll() {
}

VerticalScroll& VerticalScroll::operator =(const VerticalScroll& source) {
	Scroll::operator=(source);

	return* this;
}

Scroll* VerticalScroll::Clone() {
	return new VerticalScroll(this->minimum, this->maximum, this->lineLength, this->pageLength, this->position);
}