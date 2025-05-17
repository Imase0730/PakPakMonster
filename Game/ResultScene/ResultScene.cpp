//--------------------------------------------------------------------------------------
// File: ResultScene.cpp
//
//--------------------------------------------------------------------------------------
#include "ResultScene.h"
#include "Game/Game.h"
#include "Game/Common.h"

// �R���X�g���N�^
ResultScene::ResultScene(Game* pGame)
	: m_pGame{ pGame }
	, m_timer{}
{
}

// �f�X�g���N�^
ResultScene::~ResultScene()
{
}

// ����������
void ResultScene::Initialize()
{
	// ���̉�ʂɐ؂�ւ��܂ł̎��ԁi�R�b�j
	m_timer = RESULT_DISPLAY_TIME;
}

// �X�V����
void ResultScene::Update(int keyCondition, int keyTrigger)
{
	if (m_timer > 0) m_timer--;

	// ���̃X�e�[�W��
	if (m_timer == 0)
	{
		// �Q�[���v���C�V�[����
		m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
	}
}

// �`�揈��
void ResultScene::Render()
{
	// ���ʉ�ʂ̔w�i�̕`��
	m_pGame->DrawResult();

	// �Q�[�����[�h�̕\��
	m_pGame->DrawScore(18 * 16 + 8, 5 * 16 + 8, static_cast<int>(m_pGame->GetLevel()) + 1, 1, Common::Color::Cyan, 8);

	// �c�@��
	m_pGame->DrawScore(16 * 16 + 8, 20 * 16 + 8, m_pGame->GetRest(), 1, Common::Color::Cyan, 8);

	// ���_
	m_pGame->DrawScore(8 * 16, 15 * 16 + 4, m_pGame->GetScore(), 5, Common::Color::BlueCyan, 8);
}

// �I������
void ResultScene::Finalize()
{
}

