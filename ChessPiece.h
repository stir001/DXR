#pragma once
#include "Mathatic.h"
#include <memory>
#include <vector>

enum class CHESS_PIECE : int;
class GameObject;

class ChessPiece
{
public:
	ChessPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	virtual ~ChessPiece();
	int BoardOffset(const Int2& index) const;
	virtual bool MoveTo(const Int2& index);//indexの場所に移動する　移動できた場合はtrue,移動できない場合はfalseを返す
	virtual bool IsMovable(const Int2& index);//現在の位置から移動可能かを返す true:移動可能 false:移動負荷
	bool IsSameColor(const Int2& index);
	bool Move();//目標の位置へ移動する。移動している状態はtrue、移動していない場合はfalseを返す(移動が終わった瞬間からfalse)
protected:
	std::shared_ptr<GameObject> mPiece;
	CHESS_PIECE* mBoard;
	Int2 mCurrentIndex;
	Int2 mGoalIndex;
	std::vector<Int2> mMovableOffsets;
	unsigned int mCurrentFrame;
private:
	bool IsBlackColor(const Int2& index);
	bool IsWhiteColor(const Int2& index);

	bool (ChessPiece::*mIsColor)(const Int2& index);
};

