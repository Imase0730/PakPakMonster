//--------------------------------------------------------------------------------------
// File: Player.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Common.h"

// �N���X�̑O���錾
class Game;
class Stage;
class Monster;

// �v���C���[
class Player
{
	// �񋓌^�̒�` -----------------------------------------------------
public:

	// �v���C���[�̏��
	enum class State
	{
		Normal,	// �ʏ�
		Dead,	// ����
		Stop,	// ��~
	};

private:

	// �A�j���[�V�����ԍ�
	enum class AnimeNo
	{
		Normal_01,	// �ʏ�p�^�[���P
		Normal_02,	// �ʏ�p�^�[���Q
		Dead_01,	// ���S�p�^�[���P
		Dead_02,	// ���S�p�^�[���Q
		None,		// �\���Ȃ�

		AnimeMax
	};

	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// �v���C���[�̃X�^�[�g�n�_
	static constexpr int START_POSITION[2] = { (3 * Common::CHIP_SIZE) << 4, (5 * Common::CHIP_SIZE) << 4 };

	// �v���C���[�̑���
	static constexpr int PLAYER_SPEED = (2 << 4);

	// �p���[�A�b�v�̎c�莞��
	static constexpr int POWERUP_TIME = 5 * 60;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// �O���t�B�b�N�n���h��
	int m_ghPlayer;

	// ���
	State m_state;

	// �ʒu
	int m_x, m_y;

	// ���x
	int m_vx, m_vy;

	// �i�s����
	int m_dir;

	// true�ꍇ�͍�����
	bool m_leftFlag;

	// �H�ׂ������̏ꏊ
	int m_foodX, m_foodY;

	// �p���[�A�b�v�̎c�莞��
	int m_powerTimer;

	// �A�j���[�V����No
	AnimeNo m_animeNo;

	// �A�j���[�V�����p�^�C�}�[
	int m_animeTimer;

	// �����X�^�[�����ނ������̃X�R�A
	int m_repelScore;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// �������֐�
	void Initialize(int graphicHandle);

	// �X�V�֐�
	void Update(Game* pGame, Stage* pStage, int key, Monster* monster);

	// �`��֐�
	void Render();

	// �v���C���[�̏�Ԃ�ݒ肷��֐�
	void SetState(Player::State state) { m_state = state; }

	// �v���C���[�̈ʒu���擾����֐�
	int GetPositionX() const { return m_x; }
	int GetPositionY() const { return m_y; }

	// �v���C���[���H�ׂ�ꂽ���Ă΂��֐�
	void Dead();

	// �����X�^�[�����ނ������̓��_�����Z����֐�
	void AddRepelScore(Game* pGame);

private:

	// ���x��ݒ肷��֐�
	void SetVelocity();

public:

	// �f�o�b�O����\������֐�
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};

