//--------------------------------------------------------------------------------------
// File: GameplayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GameplayScene.h"
#include "Game/Game.h"
#include "Stage.h"

// �R���X�g���N�^
GameplayScene::GameplayScene(Game* pGame)
	: m_pGame{ pGame }
	, m_stage{}
	, m_pause{}
	, m_pauseReason{}
	, m_pauseTimer{}
	, m_stageNumber{}
	, m_ghPlayer{}
	, m_ghMonster{}
{
	// �G�̃��[�h
	m_ghPlayer = LoadGraph(L"Resources/Textures/player.png");	// �v���C���[
	m_ghMonster = LoadGraph(L"Resources/Textures/monster.png");	// �����X�^�[

	// �Q�[���X�^�[�g���̏�����
	StartInitialize();
}

// �f�X�g���N�^
GameplayScene::~GameplayScene()
{
	// �G�̃f�[�^������������폜
	DeleteGraph(m_ghPlayer);
	DeleteGraph(m_ghMonster);
}

// ����������
void GameplayScene::Initialize()
{
	// �|�[�Y���t���O
	m_pause = true;

	// �|�[�Y�̗��R
	m_pauseReason = PauseReason::Start;

	// �|�[�Y�^�C�}�[
	m_pauseTimer = PAUSE_TIME_START;

	// �}�b�v�̏�����
	m_stage.Initialize();

	// �v���C���[�̏�����
	m_player.Initialize(m_ghPlayer);

	// �����X�^�[�̏�����
	m_monster[static_cast<int>(Monster::Type::Type1)].Initialize(m_ghMonster, Monster::Type::Type1);
	m_monster[static_cast<int>(Monster::Type::Type2)].Initialize(m_ghMonster, Monster::Type::Type2);
}

// �X�V����
void GameplayScene::Update(int keyCondition, int keyTrigger)
{
	// ��ʂ��~�܂������̏���
	if (Pause()) return;

	// �X�e�[�W�̍X�V
	m_stage.Update();
	
	// �v���C���[�̍X�V
	m_player.Update(m_pGame, &m_stage, keyCondition, m_monster);

	// �����X�^�[�̍X�V
	m_monster[static_cast<int>(Monster::Type::Type1)].Update(m_pGame, &m_stage, m_stageNumber, m_player.GetPositionX(), m_player.GetPositionY());
	m_monster[static_cast<int>(Monster::Type::Type2)].Update(m_pGame, &m_stage, m_stageNumber, m_player.GetPositionX(), m_player.GetPositionY());

	// �v���C���[�ƃ����X�^�[�̐ڐG����
	for (int i = 0; i < static_cast<int>(Monster::Type::TypeMax); i++)
	{
		if (IsHit(i))
		{
			Monster::State state = m_monster[i].GetState();

			// �����X�^�[���������͎���
			if (state == Monster::State::Normal)
			{
				// �v���C���[�͎���
				m_player.Dead();

				// �|�[�Y����
				m_pauseReason = PauseReason::Dead;
				m_pauseTimer = PAUSE_TIME_DEAD;
			}
			else
			{
				// �����X�^�[���ア���̓����X�^�[�����ނ���
				if (state == Monster::State::Weak)
				{
					// ���_�����Z
					m_player.AddRepelScore(m_pGame);

					// �|�[�Y����
					m_pauseReason = PauseReason::Repel;
				}
			}

			// �ڐG���̃����X�^�[�̏���
			m_monster[i].OnHit();
		}
	}

	// �X�e�[�W��̂�����S���H�ׂăX�e�[�W�N���A�����H
	if (m_stage.GetFoodCnt() == 0)
	{
		// �v���C���[���~
		m_player.SetState(Player::State::Stop);

		// �|�[�Y����
		m_pauseReason = PauseReason::Clear;
		m_pauseTimer = PAUSE_TIME_CLEAR;
	}
}

// �`�揈��
void GameplayScene::Render()
{
	// �X�e�[�W�̕`��
	m_stage.Render(m_pGame, m_stageNumber);

	// �v���C���[�̕`��
	m_player.Render();

	// �����X�^�[�̕`��
	m_monster[static_cast<int>(Monster::Type::Type1)].Render();
	m_monster[static_cast<int>(Monster::Type::Type2)].Render();
}

// �I������
void GameplayScene::Finalize()
{
}

// �Q�[�����~�܂������̏���
bool GameplayScene::Pause()
{
	// �|�[�Y���łȂ��Ȃ牽�����Ȃ�
	if (m_pauseReason == PauseReason::None) return false;

	// �|�[�Y���Ȃ�
	if (m_pauseTimer > 0)
	{
		// �^�C�}�[���Z
		m_pauseTimer--;
		// �Q�[���I�[�o�[���͉�ʂ�_�ł���
		if (m_pauseReason == PauseReason::GameOver)
		{
			if ((m_pauseTimer >> 3) % 2)
			{
				m_pGame->SetBgColor(Common::Color::Pink);
			}
			else
			{
				m_pGame->SetBgColor(Common::Color::Black);
			}
		}
	}
	else
	{
		// �^�C�}�[���O�ɂȂ����玟�̏������s��
		switch (m_pauseReason)
		{

		case PauseReason::Start:	// �Q�[���X�^�[�g��
			// �|�[�Y���������ăQ�[���X�^�[�g�I
			m_pauseReason = PauseReason::None;
			// �v���C���[�������o��
			m_player.SetState(Player::State::Normal);
			break;

		case PauseReason::Repel:	// �����X�^�[�����ނ�����
			// �����X�^�[���ڂɂȂ�܂ł̓|�[�Y����
			if ( (m_monster[static_cast<int>(Monster::Type::Type1)].ChangeBody() == true)
			  && (m_monster[static_cast<int>(Monster::Type::Type2)].ChangeBody() == true)
			   )
			{
				m_pauseReason = PauseReason::None;
			}
			break;

		case PauseReason::Clear:	// �Q�[���N���A��
			// ���ʉ�ʂ�
			m_pGame->RequestSceneChange(Game::SceneID::Result);
			// �Q�[���N���A���̏�����
			ClearInitialize();
			break;

		case PauseReason::Dead:		// �v���C���[�����񂾎�
			// �c�@�������炷
			m_pGame->SubtractRest();
			if (m_pGame->GetRest() == 0)
			{
				// �Q�[���I�[�o�[��
				m_pauseReason = PauseReason::GameOver;
				m_pauseTimer = PAUSE_TIME_GAMEOVER;
			}
			else
			{
				// ���ʉ�ʂ�
				m_pGame->RequestSceneChange(Game::SceneID::Result);
			}
			break;

		case PauseReason::GameOver:	// �Q�[���I�[�o�[��
			// �^�C�g����
			m_pGame->RequestSceneChange(Game::SceneID::Title);
			// �n�C�X�R�A�̍X�V
			m_pGame->UpdateHighScore();
			// �Q�[���X�^�[�g���̏�����
			StartInitialize();
			break;
		}
	}

	return true;
}

// �v���C���[�ƃ����X�^�[�̐ڐG����֐�
bool GameplayScene::IsHit(int monster)
{
	int px = (m_player.GetPositionX() >> 7) << 3;
	int py = (m_player.GetPositionY() >> 7) << 3;
	int mx = (m_monster[monster].GetPositionX() >> 7) << 3;
	int my = (m_monster[monster].GetPositionY() >> 7) << 3;
	if ( ((px + 8 < mx + CHARACTER_SIZE - 8) && (mx + 8 < px + CHARACTER_SIZE - 8))
	  && ((py + 8 < my + CHARACTER_SIZE - 8) && (my + 8 < py + CHARACTER_SIZE - 8))
	   )
	{
		return true;
	}
	return false;
}

// �Q�[���X�^�[�g���̏������֐�
void GameplayScene::StartInitialize()
{
	// �X�R�A�̏�����
	m_pGame->SetScore(0);

	// �X�e�[�W������
	m_stageNumber = 0;

	// �����_���ŃX�e�[�W��ݒ�
	m_stage.SetStage();

	// ������������
	m_stage.FoodInitiaize();
}

// �X�e�[�W�N���A���̏������֐�
void GameplayScene::ClearInitialize()
{
	// �X�e�[�W�����Z
	m_stageNumber++;

	// �����_���ŃX�e�[�W��ݒ�
	m_stage.SetStage();

	// ������������
	m_stage.FoodInitiaize();
}

// �f�o�b�O����\������֐�
void GameplayScene::DisplayDebugInformation(int offsetX, int offsetY) const
{
	static const wchar_t* PAUSE_REASON[] =
	{
		L"NONE",
		L"START",
		L"CLEAR",
		L"REPEL",
		L"DEAD",
		L"GAMEOVER",
	};

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"GAMEPLAY");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 1, GetColor(255, 255, 255)
		, L"PAUSE TIMER %d", m_pauseTimer);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"PAUSE REASON %s", PAUSE_REASON[static_cast<int>(m_pauseReason)]);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"STAGE %d", m_stageNumber);

	m_player.DisplayDebugInformation(offsetX, offsetY + Game::FONT_SIZE * 5);
	m_stage.DisplayDebugInformation(offsetX, offsetY + Game::FONT_SIZE * 10);
}

