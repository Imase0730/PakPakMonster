//--------------------------------------------------------------------------------------
// File: Game.cpp
//
//--------------------------------------------------------------------------------------
#include "Game.h"
#include "Game/Screen.h"
#include "Game/Common.h"

// コンストラクタ
Game::Game()
	: m_frameTimer{}
	, m_currentSceneID{ SceneID::None }
	, m_requestedSceneID{ SceneID::None }
	, m_key{}
	, m_oldKey{}
	, m_titleScene{ this }
	, m_gameplayScene{ this }
	, m_resultScene{ this }
	, m_level{}
	, m_highScore{}
	, m_score{}
	, m_rest{}
	, m_ghNumber{}
	, m_ghScreen{}
	, m_bgColor{ Common::Color::Black }
{
	// 描画先のグラフィックを作成する
	m_ghScreen = MakeScreen(Screen::GAME_WIDTH, Screen::GAME_HEIGHT, false);

	// 乱数の初期値を設定
	SRand(static_cast<int>(time(NULL)));

	// 絵のロード
	m_ghNumber = LoadGraph(L"Resources/Textures/number.png");	// 数字
	m_ghResult = LoadGraph(L"Resources/Textures/result.png");	// 結果画面
}

// デストラクタ
Game::~Game()
{
	// 絵のデータをメモリから削除
	DeleteGraph(m_ghNumber);
	DeleteGraph(m_ghResult);
}

// 初期化処理
void Game::Initialize()
{
	// 各変数の初期化
	SetScore(0);						// 得点
	SetHighScore(Level::Easy, 0);		// ハイスコア（簡単）
	SetHighScore(Level::Difficult, 0);	// ハイスコア（難しい）
	SetLevel(Level::Easy);				// ゲームモード（簡単）

	// スタートシーンの設定
	SetStartScene(SceneID::Title);
}

// 更新処理
void Game::Update(float elapsedTime)
{
	// フレームタイマーの更新
	m_frameTimer.Update();

	// キー入力の取得
	m_oldKey = m_key;
	m_key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// シーンの切り替えリクエストがある？
	if (m_requestedSceneID != SceneID::None)
	{
		// シーンを切り替える
		ChangeScene();
	}

	// 現在のシーンを更新
	UpdateCurrentScene(m_key, ~m_oldKey & m_key);
}

// 描画処理
void Game::Render()
{
	// 作成した画像を描画対象にする
	SetDrawScreen(m_ghScreen);

	// 背景色を設定
	int color = static_cast<int>(m_bgColor);
	SetBackgroundColor(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);

	// 描画対象をクリア
	ClearDrawScreen();

	// 現在のシーンを描画
	RenderCurrentScene();

	// 描画対象を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// 描画対象画像を拡大して描画する
	int width = static_cast<int>(Screen::GAME_WIDTH * Screen::ZOOM_RATIO);
	int height = static_cast<int>(Screen::GAME_HEIGHT * Screen::ZOOM_RATIO);
	int x = Screen::WIDTH / 2 - width / 2;
	int y = Screen::HEIGHT / 2 - height / 2;
	DrawExtendGraph(x, y, x + width, y + height, m_ghScreen, false);

#if defined(_DEBUG)
	// デバッグ情報の表示
	DisplayDebugInformation(20, 60);
#endif

}

// 終了処理
void Game::Finalize()
{
	// ゲームの終了処理

	// 現在のシーンの終了処理
	FinalizeCurrentScene();
}

// シーン変更の要求
void Game::RequestSceneChange(SceneID nextSceneID)
{
	m_requestedSceneID = nextSceneID;
}

// 結果画面の背景の描画
void Game::DrawResult() const
{
	DrawGraph(0, 0, m_ghResult, true);
}

// スコア表示関数
void Game::DrawScore(int x, int y, int score, int keta, Common::Color color, int dis) const
{
	int max = 1;

	SetDrawBright( Common::COLOR_TABLE[static_cast<int>(color)][0]
				 , Common::COLOR_TABLE[static_cast<int>(color)][1]
				 , Common::COLOR_TABLE[static_cast<int>(color)][2] );

	// スコアが桁数に入らない場合表示を修正する
	for (int i = 0; i < keta; i++)
	{
		max *= 10;
	}
	if (score >= max) score = max - 1;

	// 得点を桁ごとに描画していく
	for (int i = 0; i < keta; i++)
	{
		int val = score % 10;
		score /= 10;
		DrawRectGraph(x - (32 + dis) * i + (keta - 1) * (32 + dis), y, 40 * val, 0, 5 * 8, 5 * 8, m_ghNumber, true, false);
	}

	SetDrawBright(255, 255, 255);
}

// 開始シーンの設定
void Game::SetStartScene(SceneID startSceneID)
{
	m_currentSceneID = startSceneID;
	InitializeCurrentScene();
}

// シーンの変更
void Game::ChangeScene()
{
	// 現在のシーンを終了させる
	FinalizeCurrentScene();
	// シーンIDの変更
	m_currentSceneID = m_requestedSceneID;
	// 新しいシーンを初期化する
	InitializeCurrentScene();
	// シーンの変更が終わったので変更要求をリセット
	m_requestedSceneID = SceneID::None;
}

// 現在のシーンの初期化処理
void Game::InitializeCurrentScene()
{
	// 背景色（黒）
	SetBgColor(Common::Color::Black);

	switch (m_currentSceneID)
	{
	case SceneID::Title:	// タイトル
		m_titleScene.Initialize();
		break;
	case SceneID::GamePlay:	// ゲーム中
		m_gameplayScene.Initialize();
		break;
	case SceneID::Result:	// 結果画面
		m_resultScene.Initialize();
		break;
	default:
		break;
	}
}

// 現在のシーンの更新処理
void Game::UpdateCurrentScene(int keyCondition, int keyTrigger)
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:	// タイトル
		m_titleScene.Update(keyCondition, keyTrigger);
		break;
	case SceneID::GamePlay:	// ゲーム中
		m_gameplayScene.Update(keyCondition, keyTrigger);
		break;
	case SceneID::Result:	// 結果画面
		m_resultScene.Update(keyCondition, keyTrigger);
		break;
	default:
		break;
	}
}

// 現在のシーンの描画処理
void Game::RenderCurrentScene()
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:	// タイトル
		m_titleScene.Render();
		break;
	case SceneID::GamePlay:	// ゲーム中
		m_gameplayScene.Render();
		break;
	case SceneID::Result:	// 結果画面
		m_resultScene.Render();
		break;
	default:
		break;
	}

}

// 現在のシーンの終了処理
void Game::FinalizeCurrentScene()
{
	switch (m_currentSceneID)
	{
	case SceneID::Title:	// タイトル
		m_titleScene.Finalize();
		break;
	case SceneID::GamePlay:	// ゲーム中
		m_gameplayScene.Finalize();
		break;
	case SceneID::Result:	// 結果画面
		m_resultScene.Finalize();
		break;
	default:
		break;
	}
}

// デバッグ情報を表示する関数
void Game::DisplayDebugInformation(int offsetX, int offsetY) const
{
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 0, GetColor(255, 255, 255)
		, L"LEVEL %d", static_cast<int>(m_level));
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 1, GetColor(255, 255, 255)
		, L"HIGH SCORE %5d, %5d", m_highScore[0], m_highScore[1]);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 2, GetColor(255, 255, 255)
		, L"SCORE %5d", m_score);
	DrawFormatString(offsetX, offsetY + Game::FONT_SIZE * 3, GetColor(255, 255, 255)
		, L"REST %d", m_rest);

	m_gameplayScene.DisplayDebugInformation(offsetX, offsetY + Game::FONT_SIZE * 5);
}

