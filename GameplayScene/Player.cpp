//--------------------------------------------------------------------------------------
// File: Player.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Player.h"
#include "Game/Common.h"
#include "Game/Game.h"
#include "Stage.h"
#include "Monster.h"

// �R���X�g���N�^
Player::Player()
	: m_ghPlayer{}
	, m_state{}
	, m_x{}
	, m_y{}
	, m_vx{}
	, m_vy{}
	, m_dir{}
	, m_leftFlag{}
	, m_foodX{}
	, m_foodY{}
	, m_powerTimer{}
	, m_animeNo{}
	, m_animeTimer{}
	, m_repelScore{}
{
}

// �f�X�g���N�^
Player::~Player()
{
}

// �������֐�
void Player::Initialize(int graphicHandle)
{
	m_ghPlayer = graphicHandle;
	m_state = State::Stop;
	m_x = START_POSITION[0];
	m_y = START_POSITION[1];
	m_dir = Common::RIGHT;
	m_leftFlag = false;			// �E����
	m_foodX = m_foodY = 0;
	m_powerTimer = 0;
	m_animeTimer = 0;
	m_animeNo = AnimeNo::Normal_01;
}

// �X�V�֐�
void Player::Update(Game* pGame, Stage* pStage, int key, Monster* monster)
{
	// �p���[������A���ŐH�ׂ��Ȃ��悤�ɂ���^�C�}�[�����Z����
	if (m_powerTimer > 0)
	{
		m_powerTimer--;
	}
	else
	{
		m_repelScore = Common::SCORE_MONSTER;
	}

	// �v���C���[���ړ�����
	SetVelocity();
	m_x += m_vx;
	m_y += m_vy;

	// �����]���ł���ꏊ���`�F�b�N�I�i���U�S�h�b�g�̃O���b�h�ɂ҂����肠�����ꏊ�j
	if ( ((m_vx == 0) || (m_x % (Common::CHIP_SIZE << 4)) < abs(m_vx))
	  && ((m_vy == 0) || (m_y % (Common::CHIP_SIZE << 4)) < abs(m_vy))
	   )
	{
		int px = m_x >> 4;
		int py = m_y >> 4;
		int mapX = px / Common::CHIP_SIZE;
		int mapY = py / Common::CHIP_SIZE;
		int moveFlag = Stage::MOVE_INFOMATION[mapY][mapX];

		// �i�s�������ړ��s�Ȃ�~�߂�
		if (!(moveFlag & Common::UP) && m_vy < 0) m_dir = 0;
		if (!(moveFlag & Common::DOWN) && m_vy > 0) m_dir = 0;
		if (!(moveFlag & Common::LEFT) && m_vx < 0) m_dir = 0;
		if (!(moveFlag & Common::RIGHT) && m_vx > 0) m_dir = 0;

		// �����]��
		if ((moveFlag & Common::UP) && (key & PAD_INPUT_UP)) m_dir = Common::UP;
		if ((moveFlag & Common::DOWN) && (key & PAD_INPUT_DOWN)) m_dir = Common::DOWN;
		if ((moveFlag & Common::LEFT) && (key & PAD_INPUT_LEFT)) m_dir = Common::LEFT;
		if ((moveFlag & Common::RIGHT) && (key & PAD_INPUT_RIGHT)) m_dir = Common::RIGHT;

		// ����������ΐH�ׂ�
		Stage::FoodInformation* food = pStage->GetFoodInformation(mapX, mapY);
		// ����������H
		if (food != nullptr && food->state != Stage::FoodState::None)
		{
			// �p���[�����̏�H
			if ( (food->state == Stage::FoodState::Power_3)
			  || (food->state == Stage::FoodState::Power_2)
			  || (food->state == Stage::FoodState::Power_1)
				)
			{
				// �p���[������H�ׂ����ԁH
				if (m_powerTimer == 0)
				{
					// �H�ׂ������̏ꏊ���L�^���Ă���
					m_foodX = mapX;
					m_foodY = mapY;

					m_powerTimer = POWERUP_TIME;
					// �����̏�Ԃ�ς���
					switch (food->state)
					{
					case Stage::FoodState::Power_3:
						food->state = Stage::FoodState::Power_2;
						break;
					case Stage::FoodState::Power_2:
						food->state = Stage::FoodState::Power_1;
						break;
					case Stage::FoodState::Power_1:
						food->state = Stage::FoodState::None;
						break;
					}
					// ���_���Z
					pGame->AddScore(Common::SCORE_POWER);
					// �����X�^�[���キ�Ȃ�
					monster[static_cast<int>(Monster::Type::Type1)].Weaken(POWERUP_TIME);
					monster[static_cast<int>(Monster::Type::Type2)].Weaken(POWERUP_TIME);
				}
			}
			else
			{
				// �O��H�ׂ������H
				if (m_foodX != mapX || m_foodY != mapY)
				{
					// �H�ׂ������̏ꏊ���L�^���Ă���
					m_foodX = mapX;
					m_foodY = mapY;

					switch (food->state)
					{
					case Stage::FoodState::Food_3:
						food->state = Stage::FoodState::Food_2;
						break;
					case Stage::FoodState::Food_2:
						food->state = Stage::FoodState::Food_1;
						break;
					case Stage::FoodState::Food_1:
						food->state = Stage::FoodState::None;
						break;
					}
					// ���_���Z
					pGame->AddScore(Common::SCORE_FOOD);
				}
			}
		}
		else
		{
			// �����͂Ȃ����Ōウ���Ȃ̂ňʒu���X�V����
			if (pStage->GetFoodCnt() == 1)
			{
				m_foodX = mapX;
				m_foodY = mapY;
			}
		}
		// �{�[�i�X�ƐڐG������
		if (pStage->IsHitBonus(mapX, mapY))
		{
			// �{�[�i�X��������̂Ń{�[�i�X�\����_�ł�����
			pStage->GetBonus(pGame);
		}
	}
	else
	{
		// �㉺�Ɉړ����͏㉺�݈̂ړ��\
		if (m_vy != 0)
		{
			if (key & PAD_INPUT_UP) m_dir = Common::UP;
			if (key & PAD_INPUT_DOWN) m_dir = Common::DOWN;
		}

		// ���E�Ɉړ����͍��E�݈̂ړ��\�i�������ς��Ă���j
		if (m_vx != 0)
		{
			if (key & PAD_INPUT_LEFT) m_dir = Common::LEFT;
			if (key & PAD_INPUT_RIGHT) m_dir = Common::RIGHT;
		}
	}
}

// �`��֐�
void Player::Render()
{
	// ���񂾎��̃A�j���[�V�����e�[�u��
	AnimeNo animeNo[6] =
	{
		AnimeNo::Normal_01, AnimeNo::Dead_01, AnimeNo::Dead_02, AnimeNo::None,
		AnimeNo::Dead_02, AnimeNo::Dead_01
	};

	// �v���C���[�����񂾎��Ȃ�
	if (m_state == State::Dead)
	{
		// ���񂾎��̃A�j���[�V����
		int timer = m_animeTimer >> 1;
		if (timer < 6 * 9)
		{
			m_animeTimer++;
			if (timer > 6 * 3) m_animeNo = animeNo[timer % 6];
		}
		else {
			m_animeNo = AnimeNo::None;
		}
	}
	// �ʏ펞�̓p�N�p�N
	else if (m_state == State::Normal)
	{
		m_animeTimer++;
		if (m_animeTimer == 8)
		{
			m_animeTimer = 0;
			if (m_animeNo == AnimeNo::Normal_01)
			{
				m_animeNo = AnimeNo::Normal_02;
			}
			else
			{
				m_animeNo = AnimeNo::Normal_01;
			}
		}
	}

	// �\���Ȃ�
	if (m_animeNo == AnimeNo::None) return;

	// 8�h�b�g�P�ʂ̈ړ��ɂ���
	int x = (m_x >> 7) << 3;
	int y = (m_y >> 7) << 3;
	DrawRectGraph(x + 8, y, 0 + static_cast<int>(m_animeNo) * 56, 0, 56, 56, m_ghPlayer, true, m_leftFlag);
}

// �v���C���[���H�ׂ�ꂽ���Ă΂��֐�
void Player::Dead()
{
	m_state = State::Dead;
	m_animeTimer = 0;
}

// �����X�^�[�����ނ������̓��_�����Z����֐�
void Player::AddRepelScore(Game* pGame)
{
	// ���_�����Z
	pGame->AddScore(m_repelScore);
	// ���̌��ގ��̓��_�͂Q�{�ɂȂ�
	m_repelScore = Common::SCORE_MONSTER * 2;
}

// ���x��ݒ肷��֐�
void Player::SetVelocity()
{
	m_vx = m_vy = 0;

	switch (m_dir)
	{
	case Common::UP:
		m_vx = 0;
		m_vy = -PLAYER_SPEED;
		break;
	case Common::DOWN:
		m_vx = 0;
		m_vy = PLAYER_SPEED;
		break;
	case Common::LEFT:
		m_leftFlag = true;
		m_vx = -PLAYER_SPEED;
		m_vy = 0;
		break;
	case Common::RIGHT:
		m_leftFlag = false;
		m_vx = PLAYER_SPEED;
		m_vy = 0;
		break;
	}
}

// �f�o�b�O����\������֐�
void Player::DisplayDebugInformation(int offsetX, int offsetY) const
{
	static const wchar_t* STATE[] =
	{
		L"NORMAL",
		L"DEAD",
		L"STOP",
	};

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"PLAYER");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 1, GetColor(255, 255, 255)
		, L"POWERUP TIMER %d", m_powerTimer);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"GET FOOD (%d, %d)", m_foodX, m_foodY);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"STATE %s", STATE[static_cast<int>(m_state)]);
}

