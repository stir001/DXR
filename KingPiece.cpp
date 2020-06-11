#include "KingPiece.h"

KingPiece::KingPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite)
	:ChessPiece(board, index, object, isWhite)
{
	mMovableOffsets.resize(8);
	mMovableOffsets[0] = Int2(-1,  0);
	mMovableOffsets[1] = Int2(-1,  1);
	mMovableOffsets[2] = Int2( 0,  1);
	mMovableOffsets[3] = Int2( 1,  1);
	mMovableOffsets[4] = Int2( 1,  0);
	mMovableOffsets[5] = Int2( 1, -1);
	mMovableOffsets[6] = Int2(-1, -1);
	mMovableOffsets[7] = Int2(-1, -1);
}

KingPiece::~KingPiece()
{
}
