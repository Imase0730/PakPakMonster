//--------------------------------------------------------------------------------------
// File: ResultScene.h
//
//--------------------------------------------------------------------------------------
#pragma once

// �N���X�̑O���錾
class Game;

// �Q�[���v���C�V�[��
class ResultScene
{
	// �N���X�萔�̐錾 -------------------------------------------------
public:

	// ���ʉ�ʂ̕\�����ԁi�R�b�j
	static constexpr int RESULT_DISPLAY_TIME = 3 * 60;

	// �f�[�^�����o�̐錾 -----------------------------------------------
private:

	// ���̃V�[�����܂ރQ�[���I�u�W�F�N�g�ւ̃|�C���^
	Game* m_pGame;

	// �^�C�}�[
	int m_timer;

	// �����o�֐��̐錾 -------------------------------------------------
public:

	// �R���X�g���N�^
	ResultScene(Game* pGame);

	// �f�X�g���N�^
	~ResultScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update(int keyCondition, int keyTrigger);

	// �`�揈��
	void Render();

	// �I������
	void Finalize();

};
