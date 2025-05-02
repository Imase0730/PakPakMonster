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
	// 絵のロード
	m_ghBg01 = LoadGraph(L"Resources/Textures/bg_01.png");		// ゲーム中の迷路（モンスターの巣）
	m_ghBg02 = LoadGraph(L"Resources/Textures/bg_02.png");		// ゲーム中の迷路（周りの柱）
	m_ghBg03 = LoadGraph(L"Resources/Textures/bg_03.png");		// デバッグ用（通路の絵）
	m_ghFood = LoadGraph(L"Resources/Textures/food.png");		// えさやパワーえさ
}

Stage::~Stage()
{
	// 絵のデータをメモリから削除
	DeleteGraph(m_ghBg01);
	DeleteGraph(m_ghBg02);
	DeleteGraph(m_ghFood);
}

void Stage::Initialize()
{
	// ボーナスの初期化
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
		case BonusState::Wait:	// ボーナスえさを出す
			SetBonus();
			m_bonusInformation.state = BonusState::Display;
			m_bonusInformation.timer = BONUS_DISPLAY_TIME;		// ボーナスの表示時間を設定
			break;
		case BonusState::Display:	// ボーナスえさを消す
		case BonusState::Score:	// スコアを消す
			m_bonusInformation.state = BonusState::Wait;
			m_bonusInformation.timer = (BONUS_APPEAR_TIME_NEXT + GetRand(5)) * 60;	// 次のボーナスがでるまでの時間を設定
			break;
		}
	}
}

void Stage::Render(Game* pGame, int stage) const
{
	// 難易度によってステージの色の変え方を変える
	int id = stage % 3 + static_cast<int>(pGame->GetLevel());

	// 背景の描画
	DrawGraph(0, 0, m_ghBg01, true);
	int color = static_cast<int>(STAGE_COLOR[id]);
	SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	DrawGraph(0, 0, m_ghBg02, true);
	SetDrawBright(255, 255, 255);

#if defined(_DEBUG)
	// デバッグ用（通路の絵）
	DrawGraph(0, 0, m_ghBg03, true);
#endif

	// えさの描画
	for (int i = 0; i < FOOD_MAX; i++)
	{
		FoodState state = m_foodInformation[i].state;

		// 表示位置を設定
		int x = m_foodInformation[i].x * Common::CHIP_SIZE;
		int y = m_foodInformation[i].y * Common::CHIP_SIZE;

		// えさの色を設定
		int color = static_cast<int>(FOOD_COLOR[static_cast<int>(state)]);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		switch (state)
		{
			// 通常のえさ
		case FoodState::Food_3:
		case FoodState::Food_2:
		case FoodState::Food_1:
			DrawRectGraph(x + 8, y, 0, 0, 56, 56, m_ghFood, true, false);
			break;
			// パワーえさ
		case FoodState::Power_3:
		case FoodState::Power_2:
		case FoodState::Power_1:
			DrawRectGraph(x + 8, y, 56, 0, 56, 56, m_ghFood, true, false);
			break;
		}
	}
	SetDrawBright(255, 255, 255);

	// ボーナスえさの表示
	if (m_bonusInformation.state == BonusState::Display)
	{
		// 色はチカチカさせる
		static int color = 0;
		color = (color + 1) % (static_cast<int>(Common::Color::ColorMax) - 1);
		SetDrawBright(Common::COLOR_TABLE[color + 1][0], Common::COLOR_TABLE[color + 1][1], Common::COLOR_TABLE[color + 1][2]);
		DrawRectGraph(m_bonusInformation.x + 8, m_bonusInformation.y, 56 * 2, 0, 56, 56, m_ghFood, true, false);
		SetDrawBright(255, 255, 255);
	}
	// ボーナスの得点の表示
	else if (m_bonusInformation.state == BonusState::Score)
	{
		pGame->DrawScore(9 * 16 + 8, 12 * 16 + 8, m_bonusInformation.score, 3, Common::Color::Black, 16);
	}
}

void Stage::FoodInitiaize()
{
	int cnt = 0;

	// えさの配置情報からえさを設置する
	for (int j = 0; j < STAGE_SIZE; j++)
	{
		for (int i = 0; i < STAGE_SIZE; i++)
		{
			// えさなら
			if (m_foodPosition[j][i] != 0)
			{
				m_foodInformation[cnt].x = i;
				m_foodInformation[cnt].y = j;
				if (m_foodPosition[j][i] == 1)
				{
					// えさ
					m_foodInformation[cnt].state = FoodState::Food_3;
				}
				else
				{
					// パワーえさ
					m_foodInformation[cnt].state = FoodState::Power_3;
				}
				if (cnt < FOOD_MAX - 1) cnt++;
			}
		}
	}
}

// 指定位置にえさがあるか調べる関数
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

// ボーナスと接触したか調べる関数
bool Stage::IsHitBonus(int mapX, int mapY) const
{
	int bonusX = m_bonusInformation.x / Common::CHIP_SIZE;
	int bonusY = m_bonusInformation.y / Common::CHIP_SIZE;
	if ((m_bonusInformation.state == Stage::BonusState::Display)	// ボーナスが出現中
		&& (mapX == bonusX && mapY == bonusY)				// ボーナスの位置にいる
		)
	{
		return true;
	}
	return false;
}

// ボーナスを取得した時呼ぶ関数
void Stage::GetBonus(Game* pGame)
{
	m_bonusInformation.state = BonusState::Score;
	m_bonusInformation.timer = BONUS_SCORE_DISPLAY_TIME;
	// 得点を加算
	pGame->AddScore(m_bonusInformation.score);
}

// ステージ上のえさの数を取得する関数
int Stage::GetFoodCnt() const
{
	int cnt = 0;

	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInformation[i].state != FoodState::None) cnt++;
	}
	return cnt;
}

// ステージを設定する関数（パワーアップえさの場所が違う）
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

// ボーナス設定関数
void Stage::SetBonus()
{
	// 位置設定
	if (GetRand(1))
	{
		m_bonusInformation.x = 1 * Common::CHIP_SIZE;
	}
	else
	{
		m_bonusInformation.x = 5 * Common::CHIP_SIZE;
	}
	m_bonusInformation.y = 3 * Common::CHIP_SIZE;

	// 得点設定
	m_bonusInformation.score = BONUS_SCORE[GetRand(sizeof(BONUS_SCORE) / sizeof(int) - 1)];

	// 表示時間タイマー設定
	m_bonusInformation.timer = BONUS_DISPLAY_TIME;
}

// デバッグ情報を表示する関数
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


