#pragma once

class CMineSweeper
{
public:
	int newGame(int, int, int);
	void init();
	CMineCell& at(int, int);
	void flag(int, int);
	int click(int, int);
	int GameResult();
	int getWidth();
	int getHeight();

protected:
	bool isValidPos(int, int);

private:
	CMineSweeper() = default;
	virtual ~CMineSweeper() = default;

	std::vector<std::vector<CMineCell>> mGrid;
	int mRow, mCol, mMine;
	int mFlaggedCnt;
	int mClickedCnt;
	int mGameResult;
	bool mFirstClick;

public:
	// Singleton Design
	static CMineSweeper* getInst()
	{
		static CMineSweeper Instance;
		return &Instance;
	}
};
