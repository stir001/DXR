#include "PownPiece.h"
#include "ChessGame.h"

PownPiece::PownPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite)
	:ChessPiece(board, index, object, isWhite), mSign(isWhite ? -1 : 1), mMove(&PownPiece::FirstMove)
{
	mMovableOffsets.resize(2);
	mMovableOffsets[0] = Int2(0, 1 * mSign);
	mMovableOffsets[0] = Int2(0, 2 * mSign);
}

PownPiece::~PownPiece()
{
}

bool PownPiece::MoveTo(const Int2& index)
{
	return (this->*mMove)(index);
}

bool PownPiece::FirstMove(const Int2& index)
{
	bool rtn = false;
	if (rtn = ChessPiece::MoveTo(index))
	{
		mMovableOffsets.resize(1);
		mMovableOffsets[0] = Int2(0, 1 * mSign);
		mMove = &PownPiece::CommonMove;
	}
	return rtn;
}

bool PownPiece::CommonMove(const Int2& index)
{
	return ChessPiece::MoveTo(index);
}
