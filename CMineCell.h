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
	// �ֻ��� ��Ʈ�� ���ڿ���, ���� 3��Ʈ�� ĭ�� ����,
	// ���� 4��Ʈ�� 8������ ���� ���� ������ ����
	BYTE mState;
};