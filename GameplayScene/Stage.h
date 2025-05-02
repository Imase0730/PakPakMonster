//--------------------------------------------------------------------------------------
// File: Stage.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// �N���X�̑O���錾
class Game;

class Stage
{
	// �񋓌^�̐錾 -------------------------------------------------
public:

	// �����̏��
	enum class FoodState
	{
		None,
		Food_3,
		Food_2,
		Food_1,
		Power_3,
		Power_2,
		Power_1,

		FoodStateMax,
	};

	// �{�[�i�X�̏��
	enum class BonusState
	{
		Wait,		// �\���҂�
		Display,	// �\����
		Score,		// �_���\����
	};

	// �\���̂̐錾 -------------------------------------------------
public:

	// �����̍\����
	struct FoodInformation
	{
		FoodState state;	// �����̏�� 
		int x, y;			// �����̈ʒu
	};

	// �{�[�i�X�����̍\����
	struct BonusInformation
	{
		BonusState state;	// ���
		int x, y;			// �ʒu
		int score;			// ���_
		int timer;			// �^�C�}�[
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �}�b�v�T�C�Y
	static constexpr int STAGE_SIZE = 7;

	// �ړ��\�}�b�v
	static constexpr int MOVE_INFOMATION[STAGE_SIZE][STAGE_SIZE] =
	{
		{ Common::DR,  Common::LR,  Common::DL,           0,  Common::DR,  Common::LR, Common::DL },
		{ Common::UD,           0, Common::UDR,  Common::LR, Common::UDL,           0, Common::UD },
		{ Common::UR, Common::DLR,  Common::UL,  Common::UD,  Common::UR, Common::DLR, Common::UL },
		{          0,  Common::UD,           0,  Common::UD,           0,  Common::UD,          0 },
		{ Common::DR, Common::ULR,  Common::DL,  Common::UD,  Common::DR, Common::ULR, Common::DL },
		{ Common::UD,           0, Common::UDR,  Common::LR, Common::UDL,           0, Common::UD },
		{ Common::UR,  Common::LR,  Common::UL,           0,  Common::UR,  Common::LR, Common::UL },
	};

	// �����X�^�[�p�ړ����߂�p�}�b�v
	static constexpr int RETURN_INFOMATION[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 0,         0,         0,         0,         0,         0, 0 },
		{ 0,         0, Common::R, Common::D, Common::L,         0, 0 },
		{ 0, Common::R,         0,         0,         0, Common::L, 0 },
		{ 0,         0,         0,         0,         0,         0, 0 },
		{ 0, Common::U,         0,         0,         0, Common::U, 0 },
		{ 0,         0, Common::U,         0, Common::U,         0, 0 },
		{ 0,         0,         0,         0,         0,         0, 0 },
	};

private:

	// �����̔z�u�P
	static constexpr int FOOD_POSITION_1[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 2, 0, 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 1 },
	};

	// �����̔z�u�Q
	static constexpr int FOOD_POSITION_2[STAGE_SIZE][STAGE_SIZE] =
	{
		{ 1, 0, 0, 0, 0, 0, 2 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 1 }
	};

	// �����̐�
	static constexpr int FOOD_MAX = 8;

	// �n�߂̃{�[�i�X�o������
	static constexpr int BONUS_APPEAR_TIME_FIRST = 9;

	// �Q��ڈȍ~�̃{�[�i�X�o������
	static constexpr int BONUS_APPEAR_TIME_NEXT = 15;

	// �{�[�i�X�����\������
	static constexpr int BONUS_DISPLAY_TIME = 5 * 60;

	// �{�[�i�X�̓��_�\������
	static constexpr int BONUS_SCORE_DISPLAY_TIME = 4 * 60;

	// �{�[�i�X�����̓��_
	static constexpr int BONUS_SCORE[] = { 100, 200, 300, 400, 500, 600, 700, 800 };

	// �����̐F
	static constexpr Common::Color FOOD_COLOR[static_cast<int>(FoodState::FoodStateMax)] =
	{
		Common::Color::Black,
		Common::Color::Green,
		Common::Color::Pink,
		Common::Color::Orange,
		Common::Color::Green,
		Common::Color::Pink,
		Common::Color::Orange,
	};

	// �X�e�[�W�̐F�i�ȒP 0�`2 ��� 1�`3 ���g�p�j
	static constexpr Common::Color STAGE_COLOR[] =
	{
		Common::Color::BlueCyan, Common::Color::Green, Common::Color::Cyan, Common::Color::Black
	};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �O���t�B�b�N�n���h��
	int m_ghBg01;
	int m_ghBg02;
	int m_ghBg03;
	int m_ghFood;

	// �{�[�i�X�̕\�����
	BonusInformation m_bonusInformation;

	// �X�e�[�W��ɔz�u����Ă��邦���̏��
	FoodInformation m_foodInformation[FOOD_MAX];

	// �Q�[�����Ɏg����}�b�v�f�[�^�ւ̃|�C���^
	const int (*m_foodPosition)[STAGE_SIZE];

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Stage();

	// �f�X�g���N�^
	~Stage();

	// ������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(Game* pGame, int stage) const;

	// �����̏�����
	void FoodInitiaize();
	
	// �w��ʒu�ɂ��������邩���ׂ�֐�
	FoodInformation* GetFoodInformation(int x, int y);
	
	// �{�[�i�X�̏����擾����֐�
	BonusInformation* GetBonusInformation() { return &m_bonusInformation; }

	// �{�[�i�X�ƐڐG���������ׂ�֐�
	bool IsHitBonus(int mapX, int mapY) const;

	// �{�[�i�X���擾�������ĂԊ֐�
	void GetBonus(Game* pGame);
	
	// �X�e�[�W��̂����̐����擾����֐�
	int GetFoodCnt() const;

	// �X�e�[�W��ݒ肷��֐��i�p���[�A�b�v�����̏ꏊ���Ⴄ�j
	void SetStage();

private:

	// �{�[�i�X�ݒ�֐�
	void SetBonus();

public:

	// �f�o�b�O����\������֐�
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};

