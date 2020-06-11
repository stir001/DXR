#pragma once
#include <vector>
#include <memory>

class ChessPiece;
class GameObject;
struct Vector3;
struct Int2;

enum class CHESS_PIECE
{
	W_ROOK1,
	W_KNIGHT1,
	W_BISHOP1,
	W_QUEEN,
	W_KING,
	W_BISHOP2,
	W_KNIGHT2,
	W_ROOK2,
	W_POWN0,
	W_POWN1,
	W_POWN2,
	W_POWN3,
	W_POWN4,
	W_POWN5,
	W_POWN6,
	W_POWN7,

	B_ROOK1,
	B_KNIGHT1,
	B_BISHOP1,
	B_QUEEN,
	B_KING,
	B_BISHOP2,
	B_KNIGHT2,
	B_ROOK2,
	B_POWN0,
	B_POWN1,
	B_POWN2,
	B_POWN3,
	B_POWN4,
	B_POWN5,
	B_POWN6,
	B_POWN7,

	NONE,
};

class ChessGame
{
public:
	ChessGame(std::vector<std::shared_ptr<GameObject>>& objects);
	~ChessGame();
	void InitBoard();
	void MovePiece(const Int2& before, const Int2& after);
private:
	void InitPieces(std::vector<std::shared_ptr<GameObject>>& objects);
	void InitPiecesPos();
	void SetPosLine(std::vector<std::shared_ptr<GameObject>>& objects, const Vector3& basePos, const Vector3& offset, const unsigned int baseIndex);
	bool MovePiece(std::shared_ptr<GameObject> object, unsigned int crrentFrame, unsigned int maxFrame, const Int2& startIndex, const Int2& goalIndex);

	CHESS_PIECE mBoard[64];//ƒ}ƒX•ªŠm•Û
	std::vector<std::shared_ptr<ChessPiece>> mPieces;
	std::vector<std::shared_ptr<GameObject>>& mObjects;
};

