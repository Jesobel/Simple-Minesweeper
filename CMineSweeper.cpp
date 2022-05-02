#include "stdafx.h"

int CMineSweeper::newGame(int r, int c, int m)
{
	mRow = r;
	mCol = c;
	mMine = m;
	mGrid = std::vector<std::vector<CMineCell>>(mRow, std::vector<CMineCell>(mCol));
	init();
	return 0;
}

void CMineSweeper::init()
{
	mGameResult = 0;
	mFlaggedCnt = 0;
	mClickedCnt = 0;
	mFirstClick = true;
	for (int r = 0; r < mRow; ++r) {
		for (int c = 0; c < mCol; ++c) {
			at(r, c).init();
		}
	}
}

CMineCell& CMineSweeper::at(int r, int c)
{
	assert(r >= 0 && r < mRow&& c >= 0 && c < mCol);
	return mGrid[r][c];
}

void CMineSweeper::flag(int r, int c)
{
	if (GameResult())
		return;

	at(r, c).flag();
	if (at(r, c).isFlagged())
		mFlaggedCnt++;
	else
		mFlaggedCnt--;
}

int CMineSweeper::click(int r, int c)
{
	if (GameResult())
		return 0;

	if (at(r, c).isFlagged())
		return 0;

	// ù �õ� ��, ������ ����
	// ù �õ��� �ݵ�� ���ڸ� ����
	if (mFirstClick) {
		mFirstClick = false;

		std::mt19937 engine((unsigned int)time(NULL));
		std::uniform_int_distribution<int> distribution(0, mRow * mCol - 1);
		auto generator = std::bind(distribution, engine);

		for (int i = 0; i < mMine; ++i) {
			while (1) {
				int num = generator();
				if (r == num / mCol && c == num % mCol) continue;
				CMineCell& rfCell = at(num / mCol, num % mCol);
				if (!rfCell.isMine()) {
					rfCell.setMine();
					break;
				}
			}
		}
		for (int r = 0; r < mRow; ++r) {
			for (int c = 0; c < mCol; ++c) {
				if (!at(r, c).isMine()) {
					int MineCnt = 0;
					for (int dr = -1; dr <= 1; ++dr) {
						for (int dc = -1; dc <= 1; ++dc) {
							if (dr || dc) {
								int nr = r + dr, nc = c + dc;
								if (isValidPos(nr, nc) && at(nr, nc).isMine())
									MineCnt++;
							}
						}
					}
					at(r, c).setNumber(MineCnt);
				}
			}
		}
	}

	// 8���� ���� �ۿ���, Ŭ���� ���� Ŭ������ ���� ���� Ŭ������ ���� ���� Ŭ���Ͽ� 0�� ������ ���� �����
	if (at(r, c).isCovered()) {
		at(r, c).click();
		mClickedCnt++;

		if (mClickedCnt == mRow * mCol - mMine) {
			return mGameResult = 2;
		}
		if (at(r, c).isMine()) {
			for (int r = 0; r < mRow; ++r) {
				for (int c = 0; c < mCol; ++c) {
					if (at(r, c).isMine() && at(r, c).isCovered() && !at(r, c).isFlagged())
						at(r, c).click();
				}
			}
			return mGameResult = 1;
		}
		if (at(r, c).getNumber() > 0) {
			return 0;
		}
	}

	// 8���� �÷��� ���� �˻�
	int FlagCnt = 0;
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr || dc) {
				int nr = r + dr;
				int nc = c + dc;
				if (isValidPos(nr, nc) && at(nr, nc).isFlagged())
					FlagCnt++;
			}
		}
	}
	// �÷��� ������ ��ġ�ϸ�, ���� ������ ���� ���� ĭ�� �ѹ��� ��������
	if (FlagCnt == (int)at(r, c).getNumber()) {
		for (int dr = -1; dr <= 1; ++dr) {
			for (int dc = -1; dc <= 1; ++dc) {
				if (dr || dc) {
					int nr = r + dr;
					int nc = c + dc;
					if (isValidPos(nr, nc) && at(nr, nc).isCovered() && click(nr, nc))
						return 1;
				}
			}
		}
	}

	return 0;
}

int CMineSweeper::GameResult()
{
	return mGameResult;
}

int CMineSweeper::getWidth()
{
	return mCol;
}

int CMineSweeper::getHeight()
{
	return mRow;
}

bool CMineSweeper::isValidPos(int r, int c)
{
	return r >= 0 && r < mRow && c >= 0 && c < mCol;
}