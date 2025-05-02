//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
//--------------------------------------------------------------------------------------
#include "TitleScene.h"
#include "Game/Game.h"
#include "Game/Common.h"

// �R���X�g���N�^
TitleScene::TitleScene(Game* pGame)
	: m_pGame{ pGame }
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
}

// ����������
void TitleScene::Initialize()
{
	// �c�@���̏�����
	if (m_pGame->GetLevel() == Game::Level::Easy)
	{
		m_pGame->SetRest(Common::LEVEL1_PLAYER_CNT);
	}
	else
	{
		m_pGame->SetRest(Common::LEVEL2_PLAYER_CNT);
	}
}

// �X�V����
void TitleScene::Update(int keyCondition, int keyTrigger)
{
	// Z�L�[�ŃX�^�[�g
	if (keyCondition & PAD_INPUT_1)
	{
		// �Q�[���v���C�V�[����
		m_pGame->RequestSceneChange(Game::SceneID::GamePlay);
	}
	// X�L�[�œ�Փx�ύX 
	if (keyTrigger & PAD_INPUT_2)
	{
		if (m_pGame->GetLevel() == Game::Level::Easy)
		{
			m_pGame->SetLevel(Game::Level::Difficult);		// ��Փx�i����j
			m_pGame->SetRest(Common::LEVEL2_PLAYER_CNT);	// �c�@��
		}
		else
		{
			m_pGame->SetLevel(Game::Level::Easy);			// ��Փx�i�ȒP�j
			m_pGame->SetRest(Common::LEVEL1_PLAYER_CNT);	// �c�@��
		}
	}
}

// �`�揈��
void TitleScene::Render()
{
	Game::Level level = m_pGame->GetLevel();

	// ���ʉ�ʂ̔w�i�̕`��
	m_pGame->DrawResult();

	// �Q�[�����[�h�̕\��
	m_pGame->DrawScore(18 * 16 + 8, 5 * 16 + 8, static_cast<int>(level) + 1, 1, Common::Color::Cyan, 8);

	// �c�@��
	m_pGame->DrawScore(16 * 16 + 8, 20 * 16 + 8, m_pGame->GetRest(), 1, Common::Color::Cyan, 8);

	// �n�C�X�R�A
	m_pGame->DrawScore(8 * 16, 15 * 16 + 4, m_pGame->GetHighScore(level), 5, Common::Color::BlueCyan, 8);
}

// �I������
void TitleScene::Finalize()
{
}

