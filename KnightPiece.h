#pragma once
#include "ChessPiece.h"
class KnightPiece :
	public ChessPiece
{
	KnightPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~KnightPiece();
};

