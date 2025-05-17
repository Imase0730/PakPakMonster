//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Stage.h"
#include "Game/Game.h"
#include "Game/Common.h"

Stage::Stage()
	: m_ghBg01{}
	, m_ghBg02{}
	, m_ghBg03{}
	, m_ghFood{}
	, m_bonusInformation{}
	, m_foodInformation{}
	, m_foodPosition{}
{
	// �G�̃��[�h
	m_ghBg01 = LoadGraph(L"Resources/Textures/bg_01.png");		// �Q�[�����̖��H�i�����X�^�[�̑��j
	m_ghBg02 = LoadGraph(L"Resources/Textures/bg_02.png");		// �Q�[�����̖��H�i����̒��j
	m_ghBg03 = LoadGraph(L"Resources/Textures/bg_03.png");		// �f�o�b�O�p�i�ʘH�̊G�j
	m_ghFood = LoadGraph(L"Resources/Textures/food.png");		// ������p���[����
}

Stage::~Stage()
{
	// �G�̃f�[�^������������폜
	DeleteGraph(m_ghBg01);
	DeleteGraph(m_ghBg02);
	DeleteGraph(m_ghFood);
}

void Stage::Initialize()
{
	// �{�[�i�X�̏�����
	m_bonusInformation.state = BonusState::Wait;
	m_bonusInformation.score = 0;
	m_bonusInformation.x = m_bonusInformation.y = -1;
	m_bonusInformation.timer = (BONUS_APPEAR_TIME_FIRST + GetRand(3)) * 60;
}

void Stage::Update()
{
	if (m_bonusInformation.timer > 0) m_bonusInformation.timer--;
	if (m_bonusInformation.timer == 0)
	{
		switch (m_bonusInformation.state)
		{
		case BonusState::Wait:	// �{�[�i�X�������o��
			SetBonus();
			m_bonusInformation.state = BonusState::Display;
			m_bonusInformation.timer = BONUS_DISPLAY_TIME;		// �{�[�i�X�̕\�����Ԃ�ݒ�
			break;
		case BonusState::Display:	// �{�[�i�X����������
		case BonusState::Score:	// �X�R�A������
			m_bonusInformation.state = BonusState::Wait;
			m_bonusInformation.timer = (BONUS_APPEAR_TIME_NEXT + GetRand(5)) * 60;	// ���̃{�[�i�X���ł�܂ł̎��Ԃ�ݒ�
			break;
		}
	}
}

void Stage::Render(Game* pGame, int stage) const
{
	// ��Փx�ɂ���ăX�e�[�W�̐F�̕ς�����ς���
	int id = stage % 3 + static_cast<int>(pGame->GetLevel());

	// �w�i�̕`��
	DrawGraph(0, 0, m_ghBg01, true);
	int color = static_cast<int>(STAGE_COLOR[id]);
	SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	DrawGraph(0, 0, m_ghBg02, true);
	SetDrawBright(255, 255, 255);

#if defined(_DEBUG)
	// �f�o�b�O�p�i�ʘH�̊G�j
	DrawGraph(0, 0, m_ghBg03, true);
#endif

	// �����̕`��
	for (int i = 0; i < FOOD_MAX; i++)
	{
		FoodState state = m_foodInformation[i].state;

		// �\���ʒu��ݒ�
		int x = m_foodInformation[i].x * Common::CHIP_SIZE;
		int y = m_foodInformation[i].y * Common::CHIP_SIZE;

		// �����̐F��ݒ�
		int color = static_cast<int>(FOOD_COLOR[static_cast<int>(state)]);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		switch (state)
		{
			// �ʏ�̂���
		case FoodState::Food_3:
		case FoodState::Food_2:
		case FoodState::Food_1:
			DrawRectGraph(x + 8, y, 0, 0, 56, 56, m_ghFood, true, false);
			break;
			// �p���[����
		case FoodState::Power_3:
		case FoodState::Power_2:
		case FoodState::Power_1:
			DrawRectGraph(x + 8, y, 56, 0, 56, 56, m_ghFood, true, false);
			break;
		}
	}
	SetDrawBright(255, 255, 255);

	// �{�[�i�X�����̕\��
	if (m_bonusInformation.state == BonusState::Display)
	{
		// �F�̓`�J�`�J������
		static int color = 0;
		color = (color + 1) % (static_cast<int>(Common::Color::ColorMax) - 1);
		SetDrawBright(Common::COLOR_TABLE[color + 1][0], Common::COLOR_TABLE[color + 1][1], Common::COLOR_TABLE[color + 1][2]);
		DrawRectGraph(m_bonusInformation.x + 8, m_bonusInformation.y, 56 * 2, 0, 56, 56, m_ghFood, true, false);
		SetDrawBright(255, 255, 255);
	}
	// �{�[�i�X�̓��_�̕\��
	else if (m_bonusInformation.state == BonusState::Score)
	{
		pGame->DrawScore(9 * 16 + 8, 12 * 16 + 8, m_bonusInformation.score, 3, Common::Color::Black, 16);
	}
}

void Stage::FoodInitiaize()
{
	int cnt = 0;

	// �����̔z�u��񂩂炦����ݒu����
	for (int j = 0; j < STAGE_SIZE; j++)
	{
		for (int i = 0; i < STAGE_SIZE; i++)
		{
			// �����Ȃ�
			if (m_foodPosition[j][i] != 0)
			{
				m_foodInformation[cnt].x = i;
				m_foodInformation[cnt].y = j;
				if (m_foodPosition[j][i] == 1)
				{
					// ����
					m_foodInformation[cnt].state = FoodState::Food_3;
				}
				else
				{
					// �p���[����
					m_foodInformation[cnt].state = FoodState::Power_3;
				}
				if (cnt < FOOD_MAX - 1) cnt++;
			}
		}
	}
}

// �w��ʒu�ɂ��������邩���ׂ�֐�
Stage::FoodInformation* Stage::GetFoodInformation(int x, int y)
{
	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInformation[i].x == x && m_foodInformation[i].y == y)
		{
			return &m_foodInformation[i];
		}
	}
	return nullptr;
}

// �{�[�i�X�ƐڐG���������ׂ�֐�
bool Stage::IsHitBonus(int mapX, int mapY) const
{
	int bonusX = m_bonusInformation.x / Common::CHIP_SIZE;
	int bonusY = m_bonusInformation.y / Common::CHIP_SIZE;
	if ((m_bonusInformation.state == Stage::BonusState::Display)	// �{�[�i�X���o����
		&& (mapX == bonusX && mapY == bonusY)				// �{�[�i�X�̈ʒu�ɂ���
		)
	{
		return true;
	}
	return false;
}

// �{�[�i�X���擾�������ĂԊ֐�
void Stage::GetBonus(Game* pGame)
{
	m_bonusInformation.state = BonusState::Score;
	m_bonusInformation.timer = BONUS_SCORE_DISPLAY_TIME;
	// ���_�����Z
	pGame->AddScore(m_bonusInformation.score);
}

// �X�e�[�W��̂����̐����擾����֐�
int Stage::GetFoodCnt() const
{
	int cnt = 0;

	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInformation[i].state != FoodState::None) cnt++;
	}
	return cnt;
}

// �X�e�[�W��ݒ肷��֐��i�p���[�A�b�v�����̏ꏊ���Ⴄ�j
void Stage::SetStage()
{
	if (GetRand(1))
	{
		m_foodPosition = FOOD_POSITION_1;
	}
	else
	{
		m_foodPosition = FOOD_POSITION_2;
	}
}

// �{�[�i�X�ݒ�֐�
void Stage::SetBonus()
{
	// �ʒu�ݒ�
	if (GetRand(1))
	{
		m_bonusInformation.x = 1 * Common::CHIP_SIZE;
	}
	else
	{
		m_bonusInformation.x = 5 * Common::CHIP_SIZE;
	}
	m_bonusInformation.y = 3 * Common::CHIP_SIZE;

	// ���_�ݒ�
	m_bonusInformation.score = BONUS_SCORE[GetRand(sizeof(BONUS_SCORE) / sizeof(int) - 1)];

	// �\�����ԃ^�C�}�[�ݒ�
	m_bonusInformation.timer = BONUS_DISPLAY_TIME;
}

// �f�o�b�O����\������֐�
void Stage::DisplayDebugInformation(int offsetX, int offsetY) const
{
	static const wchar_t* STATE[] =
	{
		L"WAIT",
		L"DISPLAY",
		L"SCORE",
	};

	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"BONUS");
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 1, GetColor(255, 255, 255)
		, L"DISPLAY TIMER %d", m_bonusInformation.timer);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"STATE %s", STATE[static_cast<int>(m_bonusInformation.state)]);
}


