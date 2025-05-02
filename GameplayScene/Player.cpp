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

// コンストラクタ
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

// デストラクタ
Player::~Player()
{
}

// 初期化関数
void Player::Initialize(int graphicHandle)
{
	m_ghPlayer = graphicHandle;
	m_state = State::Stop;
	m_x = START_POSITION[0];
	m_y = START_POSITION[1];
	m_dir = Common::RIGHT;
	m_leftFlag = false;			// 右向き
	m_foodX = m_foodY = 0;
	m_powerTimer = 0;
	m_animeTimer = 0;
	m_animeNo = AnimeNo::Normal_01;
}

// 更新関数
void Player::Update(Game* pGame, Stage* pStage, int key, Monster* monster)
{
	// パワーえさを連続で食べれれないようにするタイマーを減算する
	if (m_powerTimer > 0)
	{
		m_powerTimer--;
	}
	else
	{
		m_repelScore = Common::SCORE_MONSTER;
	}

	// プレイヤーを移動する
	SetVelocity();
	m_x += m_vx;
	m_y += m_vy;

	// 方向転換できる場所かチェック！（幅６４ドットのグリッドにぴったりあった場所）
	if ( ((m_vx == 0) || (m_x % (Common::CHIP_SIZE << 4)) < abs(m_vx))
	  && ((m_vy == 0) || (m_y % (Common::CHIP_SIZE << 4)) < abs(m_vy))
	   )
	{
		int px = m_x >> 4;
		int py = m_y >> 4;
		int mapX = px / Common::CHIP_SIZE;
		int mapY = py / Common::CHIP_SIZE;
		int moveFlag = Stage::MOVE_INFOMATION[mapY][mapX];

		// 進行方向が移動不可なら止める
		if (!(moveFlag & Common::UP) && m_vy < 0) m_dir = 0;
		if (!(moveFlag & Common::DOWN) && m_vy > 0) m_dir = 0;
		if (!(moveFlag & Common::LEFT) && m_vx < 0) m_dir = 0;
		if (!(moveFlag & Common::RIGHT) && m_vx > 0) m_dir = 0;

		// 方向転換
		if ((moveFlag & Common::UP) && (key & PAD_INPUT_UP)) m_dir = Common::UP;
		if ((moveFlag & Common::DOWN) && (key & PAD_INPUT_DOWN)) m_dir = Common::DOWN;
		if ((moveFlag & Common::LEFT) && (key & PAD_INPUT_LEFT)) m_dir = Common::LEFT;
		if ((moveFlag & Common::RIGHT) && (key & PAD_INPUT_RIGHT)) m_dir = Common::RIGHT;

		// えさがあれば食べる
		Stage::FoodInformation* food = pStage->GetFoodInformation(mapX, mapY);
		// えさがある？
		if (food != nullptr && food->state != Stage::FoodState::None)
		{
			// パワーえさの上？
			if ( (food->state == Stage::FoodState::Power_3)
			  || (food->state == Stage::FoodState::Power_2)
			  || (food->state == Stage::FoodState::Power_1)
				)
			{
				// パワーえさを食べられる状態？
				if (m_powerTimer == 0)
				{
					// 食べたえさの場所を記録しておく
					m_foodX = mapX;
					m_foodY = mapY;

					m_powerTimer = POWERUP_TIME;
					// えさの状態を変える
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
					// 得点加算
					pGame->AddScore(Common::SCORE_POWER);
					// モンスターが弱くなる
					monster[static_cast<int>(Monster::Type::Type1)].Weaken(POWERUP_TIME);
					monster[static_cast<int>(Monster::Type::Type2)].Weaken(POWERUP_TIME);
				}
			}
			else
			{
				// 前回食べたえさ？
				if (m_foodX != mapX || m_foodY != mapY)
				{
					// 食べたえさの場所を記録しておく
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
					// 得点加算
					pGame->AddScore(Common::SCORE_FOOD);
				}
			}
		}
		else
		{
			// えさはないが最後えさなので位置を更新する
			if (pStage->GetFoodCnt() == 1)
			{
				m_foodX = mapX;
				m_foodY = mapY;
			}
		}
		// ボーナスと接触したか
		if (pStage->IsHitBonus(mapX, mapY))
		{
			// ボーナスを取ったのでボーナス表示を点滅させる
			pStage->GetBonus(pGame);
		}
	}
	else
	{
		// 上下に移動中は上下のみ移動可能
		if (m_vy != 0)
		{
			if (key & PAD_INPUT_UP) m_dir = Common::UP;
			if (key & PAD_INPUT_DOWN) m_dir = Common::DOWN;
		}

		// 左右に移動中は左右のみ移動可能（向きも変えている）
		if (m_vx != 0)
		{
			if (key & PAD_INPUT_LEFT) m_dir = Common::LEFT;
			if (key & PAD_INPUT_RIGHT) m_dir = Common::RIGHT;
		}
	}
}

// 描画関数
void Player::Render()
{
	// 死んだ時のアニメーションテーブル
	AnimeNo animeNo[6] =
	{
		AnimeNo::Normal_01, AnimeNo::Dead_01, AnimeNo::Dead_02, AnimeNo::None,
		AnimeNo::Dead_02, AnimeNo::Dead_01
	};

	// プレイヤーが死んだ時なら
	if (m_state == State::Dead)
	{
		// 死んだ時のアニメーション
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
	// 通常時はパクパク
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

	// 表示なし
	if (m_animeNo == AnimeNo::None) return;

	// 8ドット単位の移動にする
	int x = (m_x >> 7) << 3;
	int y = (m_y >> 7) << 3;
	DrawRectGraph(x + 8, y, 0 + static_cast<int>(m_animeNo) * 56, 0, 56, 56, m_ghPlayer, true, m_leftFlag);
}

// プレイヤーが食べられた時呼ばれる関数
void Player::Dead()
{
	m_state = State::Dead;
	m_animeTimer = 0;
}

// モンスターを撃退した時の得点を加算する関数
void Player::AddRepelScore(Game* pGame)
{
	// 得点を加算
	pGame->AddScore(m_repelScore);
	// 次の撃退時の得点は２倍になる
	m_repelScore = Common::SCORE_MONSTER * 2;
}

// 速度を設定する関数
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

// デバッグ情報を表示する関数
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

