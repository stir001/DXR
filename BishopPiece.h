#pragma once
#include "ChessPiece.h"
class BishopPiece :
	public ChessPiece
{
public:
	BishopPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~BishopPiece();
};

