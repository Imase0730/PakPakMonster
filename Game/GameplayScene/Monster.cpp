//--------------------------------------------------------------------------------------
// File: Monster.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Monster.h"
#include "Game/Common.h"
#include "Game/Game.h"
#include "Stage.h"

// コンストラクタ
Monster::Monster()
	: m_ghMonster{}
	, m_type{}
	, m_state{}
	, m_x{}
	, m_y{}
	, m_vx{}
	, m_vy{}
	, m_dir{}
	, m_animeNo{}
	, m_animeTimer{}
	, m_weakTimer{}
{
}

// デストラクタ
Monster::~Monster()
{
}

// 初期化関数
void Monster::Initialize(int graphicHandle, Type type)
{
	m_ghMonster = graphicHandle;
	m_type = type;
	m_state = State::Normal;
	m_dir = Common::UP;
	m_x = START_POSITION[static_cast<int>(type)][0];
	m_y = START_POSITION[static_cast<int>(type)][1];
	m_vx = 0;
	m_vy = 0;
	m_weakTimer = 0;
	m_animeNo = AnimeNo::Normal_01;
	m_animeTimer = 0;
}

// 更新関数
void Monster::Update(Game* pGame, Stage* pStage, int stageNumber, int playerX, int playerY)
{
	// 復活
	if (m_state == State::Revive)
	{
		ChangeBody();
		return;
	}

	// モンスターが弱っている時間を減算する
	if ((m_state == State::Weak) && (m_weakTimer > 0))
	{
		m_weakTimer--;
		// モンスターは通常状態に戻る
		if (m_weakTimer == 0) m_state = State::Normal;
	}

	// モンスターの移動を移動する
	SetVelocity(pGame, stageNumber);
	m_x += m_vx;
	m_y += m_vy;

	// モンスターが巣に到着したかチェック
	if ((m_state == State::Return) && (CheckNestPosition() == true))
	{
		// 復活モード
		m_state = State::Revive;
		m_animeTimer = ANIME_INTERVEL_A[static_cast<int>(m_animeNo)];
	}

	// 方向転換できる場所かチェック！（幅６４ドットのグリッドにぴったりあった場所）
	if (((m_vx == 0) || (m_x % (Common::CHIP_SIZE << 4)) < abs(m_vx))
		&& ((m_vy == 0) || (m_y % (Common::CHIP_SIZE << 4)) < abs(m_vy)))
	{
		int mx = m_x >> 4;
		int my = m_y >> 4;
		int map_x = mx / Common::CHIP_SIZE;
		int map_y = my / Common::CHIP_SIZE;

		m_x = (map_x * Common::CHIP_SIZE) << 4;
		m_y = (map_y * Common::CHIP_SIZE) << 4;

		int moveFlag = Stage::MOVE_INFOMATION[map_y][map_x];
		int monsterFlag = Stage::RETURN_INFOMATION[map_y][map_x];

		// 現在のモンスターの進行方向を取得
		int dir = GetDirection();
		// 進行方向とは逆方向を取得する
		dir = GetReverseDirection(dir);
		// 進行方向とは逆方向に移動しないようする
		moveFlag &= ~dir;

		// 食べられて巣に戻るときなら
		if (m_state == State::Return && monsterFlag != 0)
		{
			moveFlag = monsterFlag;
		}
		else
		{
			// 追いかけモンスターで通常状態なら
			if ((m_type == Type::Type1) && (m_state == State::Normal))
			{
				// プレイヤーのいるエリア方向へ移動する
				moveFlag = ChasePlayer(moveFlag, playerX, playerY);
			}
			int dirCnt = GetDirectionCount(moveFlag);
			// 方向を変えるか判断
			if (monsterFlag & Common::UDLR)
			{
				int cnt = GetRand(dirCnt - 1);
				dir = 1;
				for (int j = 0; j < 4; j++)
				{
					if (moveFlag & dir) cnt--;
					if (cnt < 0)
					{
						moveFlag = dir;
						break;
					}
					dir <<= 1;
				}
			}
		}
		// 進行方向を設定
		m_dir = moveFlag;
	}
}

// 描画関数
void Monster::Render()
{
	AnimeNo animeNo[] =
	{
		AnimeNo::Normal_02, AnimeNo::Return_01, AnimeNo::Return_02, AnimeNo::Return_03
	};

	// 8ドット単位の移動にする
	int x = (m_x >> 7) << 3;
	int y = (m_y >> 7) << 3;

	// 通常または弱っている場合は移動アニメーション
	if ((m_state == State::Normal) || (m_state == State::Weak))
	{
		if ((x / 8 + y / 8) % 2)
		{
			m_animeNo = AnimeNo::Normal_01;
		}
		else
		{
			m_animeNo = AnimeNo::Normal_02;
		}
	}

	// 食べられた時のアニメーション
	else if (m_state == State::Eat)
	{
		int timer = m_animeTimer >> 1;
		if (timer < 6 * 9)
		{
			m_animeTimer++;
			if (timer > 6 * 3)
			{
				if ((timer >> 2) % 2)
				{
					m_animeNo = AnimeNo::Normal_01;
				}
				else
				{
					m_animeNo = AnimeNo::Normal_02;
				}
			}
		}
	}

	int anime_y;
	if (m_type == Type::Type1)
	{
		// 追いかけモンスターの色
		anime_y = 0;
		int color = static_cast<int>(Common::Color::Pink);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	}
	else
	{
		// 気まぐれモンスターの色
		anime_y = 1;
		int color = static_cast<int>(Common::Color::Purple);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	}
	// モンスターが弱っている
	if ( (m_state == State::Weak)
	  || (m_state == State::Dead)
	  || (m_state == State::Return)
	  || (m_state == State::Revive)
	   )
	{
		// 水色
		int color = static_cast<int>(Common::Color::Cyan);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		// 弱り状態が終わる２秒前から点滅させる
		if (m_weakTimer < 2 * 60 && (m_weakTimer >> 2) % 2)
		{
			// 灰色
			int color = static_cast<int>(Common::Color::Gray);
			SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		}
	}

	// 描画
	DrawRectGraph(x + 8, y,
		0 + static_cast<int>(m_animeNo) * 56, anime_y * 56,
		56, 56,
		m_ghMonster, true, false);

	// 色を元に戻す
	SetDrawBright(255, 255, 255);
}

// モンスターを弱くする関数
void Monster::Weaken(int powerupTime)
{
	if (m_state == State::Normal || m_state == State::Weak)
	{
		m_state = State::Weak;
		m_weakTimer = powerupTime;
	}
}

// プレイヤーと接触時に呼ばれる関数
void Monster::OnHit()
{
	// 通常時
	if (m_state == State::Normal)
	{
		// プレイヤーを食べた
		m_state = State::Eat;
	}
	else
	{
		// モンスターが弱っている
		if (m_state == State::Weak)
		{
			// モンスターは食べられた
			m_state = State::Dead;
			// モンスターの色を戻すためタイマーをリセットする
			m_weakTimer = 0;
			m_animeTimer = ANIME_INTERVEL_A[static_cast<int>(m_animeNo)];
		}
	}
}

// モンスターの移動速度を取得する関数
int Monster::GetSpeed(Game* pGame, int stageNumber) const
{
	if (m_state == State::Weak) return SPEED_TABLE[static_cast<int>(Speed::Weak)];
	if (m_state == State::Return) return SPEED_TABLE[static_cast<int>(Speed::Return)];
	if (m_state == State::Normal)
	{
		if (stageNumber > 3) stageNumber = 3;
		return SPEED_TABLE[stageNumber + static_cast<int>(pGame->GetLevel())];
	}
	return 0;
}

// モンスターの速度設定関数
void Monster::SetVelocity(Game* pGame, int stageNumber)
{
	switch (m_dir)
	{
	case Common::UP:
		m_vx = 0;
		m_vy = -GetSpeed(pGame, stageNumber);
		break;
	case Common::DOWN:
		m_vx = 0;
		m_vy = GetSpeed(pGame, stageNumber);
		break;
	case Common::LEFT:
		m_vx = -GetSpeed(pGame, stageNumber);
		m_vy = 0;
		break;
	case Common::RIGHT:
		m_vx = GetSpeed(pGame, stageNumber);
		m_vy = 0;
		break;
	}
}

// モンスターが食べられたり、復活するときの絵のパターンチェンジする関数
bool Monster::ChangeBody()
{
	// アニメーションタイマーが０でない場合はモンスターは変化中
	if (m_animeTimer == 0) return true;

	if (m_animeTimer > 0) m_animeTimer--;

	// アニメーションタイマーが０になったら
	if (m_animeTimer == 0)
	{
		// モンスターが死んだ場合
		if (m_state == State::Dead)
		{
			switch (m_animeNo)
			{
			case AnimeNo::Normal_01:
			case AnimeNo::Normal_02:
				m_animeNo = AnimeNo::Return_01;
				break;
			case AnimeNo::Return_01:	// 巣に戻るパターン１
				m_animeNo = AnimeNo::Return_02;
				break;
			case AnimeNo::Return_02:	// 巣に戻るパターン２
				m_animeNo = AnimeNo::Return_03;
				break;
			case AnimeNo::Return_03:	// 巣に戻るパターン３（目だけ）
				m_state = State::Return;
				m_animeTimer = 0;
				return true;
			}
			// 表示時間設定
			m_animeTimer = ANIME_INTERVEL_A[static_cast<int>(m_animeNo)];
		}
		else
		{
			switch (m_animeNo)
			{
			case AnimeNo::Normal_01:
			case AnimeNo::Normal_02:
				m_state = State::Normal;
				m_dir = Common::UP;
				m_animeTimer = 0;
				return true;
			case AnimeNo::Return_01:	// 巣に戻るパターン１
				m_animeNo = AnimeNo::Normal_02;
				break;
			case AnimeNo::Return_02:	// 巣に戻るパターン２
				m_animeNo = AnimeNo::Return_01;
				break;
			case AnimeNo::Return_03:	// 巣に戻るパターン３（目だけ）
				m_animeNo = AnimeNo::Return_02;
				break;
			}
			// 表示時間設定
			m_animeTimer = ANIME_INTERVEL_B[static_cast<int>(m_animeNo)];
		}
	}
	return false;
}

// モンスターの復活地点にいるかチェックする関数
bool Monster::CheckNestPosition() const
{
	if ( (m_x == START_POSITION[static_cast<int>(m_type)][0])
	  && (m_y == START_POSITION[static_cast<int>(m_type)][1])
	   )
	{
		return true;
	}
	return false;
}

// モンスターの移動方向の取得
int Monster::GetDirection() const
{
	if (m_vx != 0)
	{
		if (m_vx > 0) return Common::RIGHT;
		return Common::LEFT;
	}
	if (m_vy != 0)
	{
		if (m_vy > 0) return Common::DOWN;
		return Common::UP;
	}
	return 0;
}

// 進行方向とは逆方向を取得
int Monster::GetReverseDirection(int dir)
{
	switch (dir)
	{
	case Common::UP:
		return Common::DOWN;
	case Common::DOWN:
		return Common::UP;
	case Common::RIGHT:
		return Common::LEFT;
	case Common::LEFT:
		return Common::RIGHT;
	}
	return 0;
}

// プレイヤーを追いかける
int Monster::ChasePlayer(int moveFlag, int playerX, int playerY) const
{
	// プレイヤーが４隅のどのエリアにいるか調べ、違うエリアにいる場合はそちらのエリアに移動する
	int px = (playerX >> 4) / (Common::CHIP_SIZE * 4);
	int py = (playerY >> 4) / (Common::CHIP_SIZE * 4);
	int mx = (m_x >> 4) / (Common::CHIP_SIZE * 4);
	int my = (m_y >> 4) / (Common::CHIP_SIZE * 4);

	int move = 0;

	if (px < mx) move |= Common::LEFT;
	if (px > mx) move |= Common::RIGHT;
	if (py < my) move |= Common::UP;
	if (py > my) move |= Common::DOWN;

	int dir = moveFlag & move;

	// 同じエリアなら何もしない
	if (dir == 0) dir = moveFlag;

	return dir;
}

// 移動できる方向の数を取得する関数
int Monster::GetDirectionCount(int moveFlag)
{
	int count = 0;

	for (int i = 0; i < 4; i++)
	{
		count += moveFlag & 1;
		moveFlag >>= 1;
	}
	return count;
}

