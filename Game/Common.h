#pragma once

class Common
{
	// �N���X�萔�̐錾
public:

	// �}�b�v�`�b�v�̃T�C�Y�i�U�S�h�b�g�j
	static constexpr int CHIP_SIZE = 64;

	// �ړ�����
	static constexpr int UP    = (1 << 0);
	static constexpr int DOWN  = (1 << 1);
	static constexpr int LEFT  = (1 << 2);
	static constexpr int RIGHT = (1 << 3);

	static constexpr int U = UP;
	static constexpr int D = DOWN;
	static constexpr int L = LEFT;
	static constexpr int R = RIGHT;

	static constexpr int DR = (D | R);
	static constexpr int DL = (D | L);
	static constexpr int UR = (U | R);
	static constexpr int UL = (U | L);
	static constexpr int LR = (L | R);
	static constexpr int UD = (U | D);
	static constexpr int UDR = (U | D | R);
	static constexpr int UDL = (U | D | L);
	static constexpr int ULR = (U | L | R);
	static constexpr int DLR = (D | L | R);
	static constexpr int UDLR = (U | D | L | R);

	// �c�@��
	static constexpr int LEVEL1_PLAYER_CNT = 6;
	static constexpr int LEVEL2_PLAYER_CNT = 3;

	// ���_
	static constexpr int SCORE_FOOD = 20;
	static constexpr int SCORE_POWER = 100;
	static constexpr int SCORE_MONSTER = 200;
	
	// �F
	enum class Color
	{
		Black,
		Green,
		Pink,
		Orange,
		Cyan,
		Purple,
		Gray,
		Yellow,
		BlueCyan,

		ColorMax,
	};

	// �F�e�[�u��
	static constexpr int COLOR_TABLE[static_cast<int>(Color::ColorMax)][3] =
	{
		{ 0,   0,   0 },	// ��
		{ 193, 255,  73 },	// ��
		{ 250, 169, 240 },	// �s���N
		{ 255, 213, 157 },	// �I�����W
		{ 178, 233, 255 },	// ���F
		{ 215, 174, 255 },	// ��
		{ 224, 227, 196 },	// �O���[
		{ 255, 209,  75 },	// ���F
		{ 172, 255, 220 },	// �u���[�V�A��
	};

};
