#pragma once
#include "ChessPiece.h"
class KingPiece :
	public ChessPiece
{
public:
	KingPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	~KingPiece();
};

