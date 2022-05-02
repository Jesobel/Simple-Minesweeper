#pragma once

using BYTE = unsigned char;

class CMineCell
{

public:
	CMineCell() = default;
	virtual ~CMineCell() = default;

	void init();

	bool isMine();
	bool isCovered();
	bool isFlagged();
	bool isClicked();
	BYTE getNumber();

	void setMine();
	void setNumber(int);
	void flag();
	void click();

private:
	// 최상위 비트는 지뢰여부, 상위 3비트는 칸의 상태,
	// 하위 4비트는 8방향의 지뢰 개수 정보를 저장
	BYTE mState;
};