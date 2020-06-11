#pragma once
#include "ChessPiece.h"
class PownPiece :
	public ChessPiece
{
public:
	//@isWhite 白か黒かで方向を判定する true:白-方向 false:黒+方向
	PownPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~PownPiece();
	bool MoveTo(const Int2& index);
private:
	int mSign;
	bool FirstMove(const Int2& index);
	bool CommonMove(const Int2& index);

	bool (PownPiece::*mMove)(const Int2& index);
};
