//--------------------------------------------------------------------------------------
// File: GameplayScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

#include "Game/Screen.h"
#include "Stage.h"
#include "Player.h"
#include "Monster.h"

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class GameplayScene
{
	// �񋓌^�̒�` -----------------------------------------------------
private:

	// �|�[�Y�������R
	enum class PauseReason
	{
		None,		// �|�[�Y���Ă��Ȃ�
		Start,		// �Q�[���X�^�[�g��
		Clear,		// �Q�[���N���A��
		Repel,		// �����X�^�[�����ނ�����
		Dead,		// ���񂾎�
		GameOver,	// �Q�[���I����
	};

// �N���X�萔�̐錾 -------------------------------------------------
private:
	
	// �L�����N�^�[�T�C�Y�i�T�U�h�b�g�j
	static constexpr int CHARACTER_SIZE = 56;

	// �Q�[�����n�܂�܂ł̑҂����ԁi�Q�b�j
	static constexpr int PAUSE_TIME_START = 2 * 60;

	// �X�e�[�W�N���A���̑҂����ԁi�R�b�j
	static constexpr int PAUSE_TIME_CLEAR = 3 * 60;
	
	// �v���C���[�����S�������̑҂����ԁi�T�b�j
	static constexpr int PAUSE_TIME_DEAD = 5 * 60;
	
	// �Q�[���I�[�o�[���̑҂����ԁi�T�b�j
	static constexpr int PAUSE_TIME_GAMEOVER = 5 * 60;

// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �X�e�[�W
	Stage m_stage;

	// �|�[�Y���t���O
	bool m_pause;

	// �|�[�Y�������R
	PauseReason m_pauseReason;

	// �|�[�Y�^�C�}�[
	int m_pauseTimer;

	// �V��ł���X�e�[�W��
	int m_stageNumber;

	// �O���t�B�b�N�n���h��
	int m_ghPlayer;		// �v���C���[
	int m_ghMonster;	// �����X�^�[

	// �v���C���[
	Player m_player;

	// �����X�^�[
	Monster m_monster[static_cast<int>(Monster::Type::TypeMax)];

// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	GameplayScene(Game* pGame);

	// �f�X�g���N�^
	~GameplayScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

private:

	// �Q�[�����~�܂������̏���
	bool Pause();

	// �v���C���[�ƃ����X�^�[�̐ڐG����֐�
	bool IsHit(int monster);

	// �Q�[���X�^�[�g���̏������֐�
	void StartInitialize();

	// �X�e�[�W�N���A���̏������֐�
	void ClearInitialize();

public:

	// �f�o�b�O����\������֐�
	void DisplayDebugInformation(int offsetX, int offsetY) const;

};
