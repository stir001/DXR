#include "ChessGame.h"
#include "Mathatic.h"
#include "GameObject.h"

CHESS_PIECE INIT_BOARD[64] =
{
	CHESS_PIECE::W_ROOK1, CHESS_PIECE::W_KNIGHT1, CHESS_PIECE::W_BISHOP1, CHESS_PIECE::W_QUEEN, CHESS_PIECE::W_KING,  CHESS_PIECE::W_BISHOP2, CHESS_PIECE::W_KNIGHT2, CHESS_PIECE::W_ROOK2,
	CHESS_PIECE::W_POWN0, CHESS_PIECE::W_POWN1,   CHESS_PIECE::W_POWN2,   CHESS_PIECE::W_POWN3, CHESS_PIECE::W_POWN4, CHESS_PIECE::W_POWN5,   CHESS_PIECE::W_POWN6,   CHESS_PIECE::W_POWN7,
	CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,    CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,
	CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,    CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,
	CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,    CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,
	CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,    CHESS_PIECE::NONE,    CHESS_PIECE::NONE,      CHESS_PIECE::NONE,      CHESS_PIECE::NONE,
	CHESS_PIECE::B_POWN0, CHESS_PIECE::B_POWN1,   CHESS_PIECE::B_POWN2,   CHESS_PIECE::B_POWN3, CHESS_PIECE::B_POWN4, CHESS_PIECE::B_POWN5,   CHESS_PIECE::B_POWN6,   CHESS_PIECE::B_POWN7,
	CHESS_PIECE::B_ROOK1, CHESS_PIECE::B_KNIGHT1, CHESS_PIECE::B_BISHOP1, CHESS_PIECE::B_QUEEN, CHESS_PIECE::B_KING,  CHESS_PIECE::B_BISHOP2, CHESS_PIECE::B_KNIGHT2, CHESS_PIECE::B_ROOK2,
};

ChessGame::ChessGame(std::vector<std::shared_ptr<GameObject>>& objects) : mObjects(objects)
{
	InitBoard();
	InitPieces(objects);
}

ChessGame::~ChessGame()
{
}

void ChessGame::InitBoard()
{
	memcpy(mBoard, INIT_BOARD, sizeof(CHESS_PIECE) * 64);
	InitPiecesPos();
}

void ChessGame::MovePiece(const Int2& start, const Int2& goal)
{
	
}

void ChessGame::InitPieces(std::vector<std::shared_ptr<GameObject>>& objects)
{
	//mPieces.resize(objects.size());
	//mPieces
}

void ChessGame::InitPiecesPos()
{
	float boardLength = 3.0f;
	Vector3 offset = { boardLength, 0.0f, 0.0f };
	const Vector3 origin = { boardLength * 0.5f, 0.011f, boardLength * 0.5f };
	unsigned int objectOffset = 0;
	unsigned int linePieceNum = 8;

	Vector3 leftFront = Vector3(boardLength * -4.0f, 0.0f, boardLength * 3.0f) + origin;
	SetPosLine(mObjects, leftFront, offset, objectOffset);
	objectOffset += linePieceNum;
	SetPosLine(mObjects, leftFront + Vector3(0.0f, 0.0f, -boardLength), offset, objectOffset);
	objectOffset += linePieceNum;

	Vector3 leftBack = Vector3(boardLength * -4.0f, 0.0f, boardLength * -4.0f) + origin;
	SetPosLine(mObjects, leftBack, offset, objectOffset);
	objectOffset += linePieceNum;
	SetPosLine(mObjects, leftBack + Vector3(0.0f, 0.0f, boardLength), offset, objectOffset);
}

void ChessGame::SetPosLine(std::vector<std::shared_ptr<GameObject>>& objects, const Vector3& basePos, const Vector3& offset, const unsigned int baseIndex)
{
	const unsigned int linePieceNum = 8;
	for (unsigned int i = 0; i < linePieceNum; ++i)
	{
		mObjects[baseIndex + i]->SetPos(basePos + offset * i);
	}
}

bool ChessGame::MovePiece(std::shared_ptr<GameObject> object, unsigned int currentFrame, unsigned int maxFrame, const Int2& startIndex, const Int2& goalIndex)
{
	unsigned int moveFrame = 60;

	if (moveFrame < currentFrame)
	{
		return false;
	}

	float maxY = 5.0f;
	float boardLength = 3.0f;
	float minY = 0.011f;
	const Vector3 toCenterOffset = { boardLength * 0.5f, 0.0f, boardLength * 0.5f };
	const Vector3 leftForntOffset = { boardLength * -4.0f, 0.0f, boardLength * 4.0f };
	Vector3 firstPos = Vector3(startIndex.x * boardLength, minY, startIndex.y * boardLength) + toCenterOffset + leftForntOffset;
	Vector3 goalPos = Vector3(goalIndex.x * boardLength, minY, goalIndex.y * boardLength) + toCenterOffset + leftForntOffset;

	float t = static_cast<float>(currentFrame) / static_cast<float>(moveFrame);
	float harfT = 0.0f;
	if (currentFrame > moveFrame / 2)
	{
		harfT = 1.0f - static_cast<float>(currentFrame) / static_cast<float>(moveFrame / 2);
	}
	else
	{
		harfT = static_cast<float>(currentFrame - moveFrame / 2) / static_cast<float>(moveFrame / 2);
	}

	auto pos = firstPos * (1.0f - t) + goalPos * t;
	pos.y = minY * harfT + maxY * (1.0f - harfT);

	object->SetPos(pos);
	return true;
}
