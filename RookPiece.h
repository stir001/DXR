#pragma once
#include "ChessPiece.h"
class RookPiece :
	public ChessPiece
{
public:
	RookPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~RookPiece();
};

