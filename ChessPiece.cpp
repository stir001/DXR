#include "ChessPiece.h"
#include "ChessGame.h"
#include "GameObject.h"

ChessPiece::ChessPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite) 
	: mBoard(board), mCurrentIndex(index),mGoalIndex(index), mPiece(object), mCurrentFrame(0)
{
	mIsColor = isWhite ? &ChessPiece::IsWhiteColor : &ChessPiece::IsBlackColor;
}

ChessPiece::~ChessPiece()
{
}

int ChessPiece::BoardOffset(const Int2& index) const
{
	return index.x + index.y * 8;
}

bool ChessPiece::MoveTo(const Int2& index)
{
	bool rtn = false;
	if (rtn = IsMovable(index))
	{
		mBoard[BoardOffset(index)] = mBoard[BoardOffset(mCurrentIndex)];
		mBoard[BoardOffset(mCurrentIndex)] = CHESS_PIECE::NONE;
		mGoalIndex = index;
	}
	return rtn;
}

bool ChessPiece::IsMovable(const Int2& index)
{
	auto offset = mCurrentIndex - index;
	for (auto& m : mMovableOffsets)
	{
		if (m == offset && !IsSameColor(index))
		{
			return true;
		}
	}
	return false;
}

bool ChessPiece::IsSameColor(const Int2& index)
{
	return (this->*mIsColor)(index);
}

bool ChessPiece::Move()
{
	unsigned int moveFrame = 60;

	if (moveFrame < mCurrentFrame)
	{
		mCurrentFrame = 0;
		return false;
	}

	float maxY = 5.0f;
	float boardLength = 3.0f;
	float minY = 0.011f;
	const Vector3 toCenterOffset = { boardLength * 0.5f, 0.0f, boardLength * 0.5f };
	const Vector3 leftForntOffset = { boardLength * -4.0f, 0.0f, boardLength * 4.0f };
	Vector3 firstPos = Vector3(mCurrentIndex.x * boardLength, minY, mCurrentIndex.y * boardLength) + toCenterOffset + leftForntOffset;
	Vector3 goalPos = Vector3(mGoalIndex.x * boardLength, minY, mGoalIndex.y * boardLength) + toCenterOffset + leftForntOffset;

	float t = static_cast<float>(mCurrentFrame) / static_cast<float>(moveFrame);
	float harfT = 0.0f;
	if (mCurrentFrame > moveFrame / 2)
	{
		harfT = 1.0f - static_cast<float>(mCurrentFrame) / static_cast<float>(moveFrame / 2);
	}
	else
	{
		harfT = static_cast<float>(mCurrentFrame - moveFrame / 2) / static_cast<float>(moveFrame / 2);
	}

	auto pos = firstPos * (1.0f - t) + goalPos * t;
	pos.y = minY * harfT + maxY * (1.0f - harfT);

	mPiece->SetPos(pos);

	++mCurrentFrame;
	return true;
}

bool ChessPiece::IsBlackColor(const Int2& index)
{
	int offset = BoardOffset(index);
	return mBoard[offset] >= CHESS_PIECE::B_ROOK1 && mBoard[offset] <= CHESS_PIECE::B_POWN7;
}

bool ChessPiece::IsWhiteColor(const Int2& index)
{
	int offset = BoardOffset(index);
	return mBoard[offset] >= CHESS_PIECE::W_ROOK1 && mBoard[offset] <= CHESS_PIECE::W_POWN7;
}
