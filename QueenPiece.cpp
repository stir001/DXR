#include "QueenPiece.h"

QueenPiece::QueenPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite)
	: ChessPiece(board, index, object, isWhite)
{
	mMovableOffsets.resize(7 * 8);

	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 0] = Int2(i, 0);
	}
	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 1] = Int2(-i, 0);
	}
	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 2] = Int2(0, i);
	}
	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 3] = Int2(0, -i);
	}

	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 4] = Int2((i + 1), (i + 1));
	}

	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 5] = Int2(-(i + 1), -(i + 1));
	}

	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 6] = Int2(-(i + 1), (i + 1));
	}

	for (int i = 0; i < 7; ++i)
	{
		mMovableOffsets[i + 7 * 7] = Int2((i + 1), -(i + 1));
	}
}

QueenPiece::~QueenPiece()
{
}
