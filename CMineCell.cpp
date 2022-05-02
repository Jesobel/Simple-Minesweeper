#include "stdafx.h"

void CMineCell::init()
{
	mState = 0x0;
}

bool CMineCell::isMine()
{
	return mState & 0x80;
}

bool CMineCell::isCovered()
{
	return !isClicked();
}

bool CMineCell::isFlagged()
{
	return isCovered() && (mState & 0x10);
}

bool CMineCell::isClicked()
{
	return mState & 0x20;
}

void CMineCell::setMine()
{
	mState |= 0x80;
}

void CMineCell::flag()
{
	mState ^= 0x10;
}

void CMineCell::click()
{
	mState |= 0x20;
}

BYTE CMineCell::getNumber()
{
	return mState & 0xF;
}

void CMineCell::setNumber(int num)
{
	assert(num >= 0 && num < 9);
	mState |= num;
}

