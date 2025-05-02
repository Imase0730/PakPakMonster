//--------------------------------------------------------------------------------------
// File: GameplayScene.cpp
//
//--------------------------------------------------------------------------------------
#include "GameplayScene.h"
#include "Game/Game.h"
#include "Stage.h"

// コンストラクタ
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
	// 絵のロード
	m_ghPlayer = LoadGraph(L"Resources/Textures/player.png");	// プレイヤー
	m_ghMonster = LoadGraph(L"Resources/Textures/monster.png");	// モンスター

	// ゲームスタート時の初期化
	StartInitialize();
}

// デストラクタ
GameplayScene::~GameplayScene()
{
	// 絵のデータをメモリから削除
	DeleteGraph(m_ghPlayer);
	DeleteGraph(m_ghMonster);
}

// 初期化処理
void GameplayScene::Initialize()
{
	// ポーズ中フラグ
	m_pause = true;

	// ポーズの理由
	m_pauseReason = PauseReason::Start;

	// ポーズタイマー
	m_pauseTimer = PAUSE_TIME_START;

	// マップの初期化
	m_stage.Initialize();

	// プレイヤーの初期化
	m_player.Initialize(m_ghPlayer);

	// モンスターの初期化
	m_monster[static_cast<int>(Monster::Type::Type1)].Initialize(m_ghMonster, Monster::Type::Type1);
	m_monster[static_cast<int>(Monster::Type::Type2)].Initialize(m_ghMonster, Monster::Type::Type2);
}

// 更新処理
void GameplayScene::Update(int keyCondition, int keyTrigger)
{
	// 画面が止まった時の処理
	if (Pause()) return;

	// ステージの更新
	m_stage.Update();
	
	// プレイヤーの更新
	m_player.Update(m_pGame, &m_stage, keyCondition, m_monster);

	// モンスターの更新
	m_monster[static_cast<int>(Monster::Type::Type1)].Update(m_pGame, &m_stage, m_stageNumber, m_player.GetPositionX(), m_player.GetPositionY());
	m_monster[static_cast<int>(Monster::Type::Type2)].Update(m_pGame, &m_stage, m_stageNumber, m_player.GetPositionX(), m_player.GetPositionY());

	// プレイヤーとモンスターの接触判定
	for (int i = 0; i < static_cast<int>(Monster::Type::TypeMax); i++)
	{
		if (IsHit(i))
		{
			Monster::State state = m_monster[i].GetState();

			// モンスターが強い時は死ぬ
			if (state == Monster::State::Normal)
			{
				// プレイヤーは死んだ
				m_player.Dead();

				// ポーズする
				m_pauseReason = PauseReason::Dead;
				m_pauseTimer = PAUSE_TIME_DEAD;
			}
			else
			{
				// モンスターが弱い時はモンスターを撃退する
				if (state == Monster::State::Weak)
				{
					// 得点を加算
					m_player.AddRepelScore(m_pGame);

					// ポーズする
					m_pauseReason = PauseReason::Repel;
				}
			}

			// 接触時のモンスターの処理
			m_monster[i].OnHit();
		}
	}

	// ステージ上のえさを全部食べてステージクリアした？
	if (m_stage.GetFoodCnt() == 0)
	{
		// プレイヤーを停止
		m_player.SetState(Player::State::Stop);

		// ポーズする
		m_pauseReason = PauseReason::Clear;
		m_pauseTimer = PAUSE_TIME_CLEAR;
	}
}

// 描画処理
void GameplayScene::Render()
{
	// ステージの描画
	m_stage.Render(m_pGame, m_stageNumber);

	// プレイヤーの描画
	m_player.Render();

	// モンスターの描画
	m_monster[static_cast<int>(Monster::Type::Type1)].Render();
	m_monster[static_cast<int>(Monster::Type::Type2)].Render();
}

// 終了処理
void GameplayScene::Finalize()
{
}

// ゲームが止まった時の処理
bool GameplayScene::Pause()
{
	// ポーズ中でないなら何もしない
	if (m_pauseReason == PauseReason::None) return false;

	// ポーズ中なら
	if (m_pauseTimer > 0)
	{
		// タイマー減算
		m_pauseTimer--;
		// ゲームオーバー時は画面を点滅する
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
		// タイマーが０になったら次の処理を行う
		switch (m_pauseReason)
		{

		case PauseReason::Start:	// ゲームスタート時
			// ポーズを解除してゲームスタート！
			m_pauseReason = PauseReason::None;
			// プレイヤーも動き出す
			m_player.SetState(Player::State::Normal);
			break;

		case PauseReason::Repel:	// モンスターを撃退した時
			// モンスターが目になるまではポーズする
			if ( (m_monster[static_cast<int>(Monster::Type::Type1)].ChangeBody() == true)
			  && (m_monster[static_cast<int>(Monster::Type::Type2)].ChangeBody() == true)
			   )
			{
				m_pauseReason = PauseReason::None;
			}
			break;

		case PauseReason::Clear:	// ゲームクリア時
			// 結果画面へ
			m_pGame->RequestSceneChange(Game::SceneID::Result);
			// ゲームクリア時の初期化
			ClearInitialize();
			break;

		case PauseReason::Dead:		// プレイヤーが死んだ時
			// 残機数を減らす
			m_pGame->SubtractRest();
			if (m_pGame->GetRest() == 0)
			{
				// ゲームオーバーへ
				m_pauseReason = PauseReason::GameOver;
				m_pauseTimer = PAUSE_TIME_GAMEOVER;
			}
			else
			{
				// 結果画面へ
				m_pGame->RequestSceneChange(Game::SceneID::Result);
			}
			break;

		case PauseReason::GameOver:	// ゲームオーバー時
			// タイトルへ
			m_pGame->RequestSceneChange(Game::SceneID::Title);
			// ハイスコアの更新
			m_pGame->UpdateHighScore();
			// ゲームスタート時の初期化
			StartInitialize();
			break;
		}
	}

	return true;
}

// プレイヤーとモンスターの接触判定関数
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

// ゲームスタート時の初期化関数
void GameplayScene::StartInitialize()
{
	// スコアの初期化
	m_pGame->SetScore(0);

	// ステージ初期化
	m_stageNumber = 0;

	// ランダムでステージを設定
	m_stage.SetStage();

	// えさを初期化
	m_stage.FoodInitiaize();
}

// ステージクリア時の初期化関数
void GameplayScene::ClearInitialize()
{
	// ステージ数加算
	m_stageNumber++;

	// ランダムでステージを設定
	m_stage.SetStage();

	// えさを初期化
	m_stage.FoodInitiaize();
}

// デバッグ情報を表示する関数
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

