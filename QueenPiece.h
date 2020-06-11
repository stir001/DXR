#pragma once
#include "ChessPiece.h"
class QueenPiece :
	public ChessPiece
{
public:
	QueenPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~QueenPiece();

};

