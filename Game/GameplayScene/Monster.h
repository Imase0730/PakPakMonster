//--------------------------------------------------------------------------------------
// File: Monster.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// �N���X�̑O���錾
class Game;
class Stage;

// �����X�^�[
class Monster
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	enum class Type
	{
		Type1,		// �ǂ����������X�^�[
		Type2,		// �C�܂��ꃂ���X�^�[

		TypeMax
	};

	enum class State
	{
		Normal,		// �ʏ�
		Weak,		// �����Ă�
		Dead,		// �H�ׂ�ꂽ
		Eat,		// �v���C���[��H�ׂ�
		Return,		// ���ɖ߂��Ă���
		Revive,		// ����
	};

	enum class AnimeNo
	{
		Normal_01,	// �ʏ�p�^�[���P
		Normal_02,	// �ʏ�p�^�[���Q
		Return_01,	// ���ɖ߂�p�^�[���P
		Return_02,	// ���ɖ߂�p�^�[���Q
		Return_03,	// ���ɖ߂�p�^�[���R�i�ڂ����j

		AnimeMax
	};

private:

	// �����X�^�[�̈ړ����x�̎��
	enum class Speed
	{
		Level1,		// Level1�i�x���j
		Level2,		// Level2   ��
		Level3,		// Level3   ��
		Level4,		// Level4   ��
		Level5,		// Level5�i�����j
		Weak,		// �����X�^�[������Ă��鎞
		Return,		// �����X�^�[�����ɖ߂鎞

		SpeedMax
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �A�j���[�V�����\�����ԁi�H�ׂ��ĖڂɂȂ�j
	static constexpr int ANIME_INTERVEL_A[] = { 16, 16, 16, 16, 40 };

	// �A�j���[�V�����\�����ԁi��������j
	static constexpr int ANIME_INTERVEL_B[] = { 32, 32, 32, 32, 40 };

	// �����X�^�[�̃X�^�[�g�n�_
	static constexpr int START_POSITION[static_cast<int>(Type::TypeMax)][2] =
	{
		{ (3 * Common::CHIP_SIZE) << 4, (2 * Common::CHIP_SIZE + 8) << 4 },	// �ǂ����������X�^�[
		{ (3 * Common::CHIP_SIZE) << 4, (4 * Common::CHIP_SIZE - 8) << 4 },	// �C�܂��ꃂ���X�^�[
	};

	// �����X�^�[�̈ړ����x
	static constexpr int SPEED_TABLE[static_cast<int>(Speed::SpeedMax)] = {12, 18, 24, 30, 36, 12, 32};

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �O���t�B�b�N�n���h��
	int m_ghMonster;

	// �����X�^�[�̃^�C�v
	Type m_type;

	// ���
	State m_state;

	// �ʒu
	int m_x, m_y;

	// ���x
	int m_vx, m_vy;

	// �i�s����
	int m_dir;

	// �A�j���[�V����No
	AnimeNo m_animeNo;

	// �A�j���[�V�����p�^�C�}�[
	int m_animeTimer;

	// ���^�C�}�[
	int m_weakTimer;

	// �����o�֐��̐錾 -------------------------------------------------
public:
	
	// �R���X�g���N�^
	Monster();

	// �f�X�g���N�^
	~Monster();

	// �������֐�
	void Initialize(int graphicHandle, Type type);

	// �X�V�֐�
	void Update(Game* pGame, Stage* pStage, int stageNumber, int playerX, int playerY);

	// �`��֐�
	void Render();

	// �ʒu���擾����֐�
	int GetPositionX() const { return m_x; }
	int GetPositionY() const { return m_y; }

	// �����X�^�[���キ����֐�
	void Weaken(int powerupTime);

	// �����X�^�[���H�ׂ�ꂽ��A��������Ƃ��̊G�̃p�^�[���`�F���W����֐�
	bool ChangeBody();

	// �v���C���[�ƐڐG���ɌĂ΂��֐�
	void OnHit();

	// ��Ԃ��擾����֐�
	State GetState() const { return m_state; }

private:

	// �����X�^�[�̈ړ����x���擾����֐�
	int GetSpeed(Game* pGame, int stageNumber) const;

	// �����X�^�[�̑��x�ݒ�֐�
	void SetVelocity(Game* pGame, int stageNumber);

	// �����X�^�[�̕����n�_�ɂ��邩�`�F�b�N����֐�
	bool CheckNestPosition() const;

	// �����X�^�[�̈ړ������̎擾
	int GetDirection() const;

	// �i�s�����Ƃ͋t�������擾
	int GetReverseDirection(int dir);

	// �v���C���[��ǂ�������
	int ChasePlayer(int moveFlag, int playerX, int playerY) const;

	// �ړ��ł�������̐����擾����֐�
	int GetDirectionCount(int moveFlag);

};

