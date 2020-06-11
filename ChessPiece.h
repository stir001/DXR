#pragma once
#include "Mathatic.h"
#include <memory>
#include <vector>

enum class CHESS_PIECE : int;
class GameObject;

class ChessPiece
{
public:
	ChessPiece(CHESS_PIECE* board, const Int2& index, std::shared_ptr<GameObject>& object, bool isWhite);
	virtual ~ChessPiece();
	int BoardOffset(const Int2& index) const;
	virtual bool MoveTo(const Int2& index);//index�̏ꏊ�Ɉړ�����@�ړ��ł����ꍇ��true,�ړ��ł��Ȃ��ꍇ��false��Ԃ�
	virtual bool IsMovable(const Int2& index);//���݂̈ʒu����ړ��\����Ԃ� true:�ړ��\ false:�ړ�����
	bool IsSameColor(const Int2& index);
	bool Move();//�ڕW�̈ʒu�ֈړ�����B�ړ����Ă����Ԃ�true�A�ړ����Ă��Ȃ��ꍇ��false��Ԃ�(�ړ����I������u�Ԃ���false)
protected:
	std::shared_ptr<GameObject> mPiece;
	CHESS_PIECE* mBoard;
	Int2 mCurrentIndex;
	Int2 mGoalIndex;
	std::vector<Int2> mMovableOffsets;
	unsigned int mCurrentFrame;
private:
	bool IsBlackColor(const Int2& index);
	bool IsWhiteColor(const Int2& index);

	bool (ChessPiece::*mIsColor)(const Int2& index);
};

