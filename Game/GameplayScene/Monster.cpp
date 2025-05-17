//--------------------------------------------------------------------------------------
// File: Monster.cpp
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Monster.h"
#include "Game/Common.h"
#include "Game/Game.h"
#include "Stage.h"

// �R���X�g���N�^
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

// �f�X�g���N�^
Monster::~Monster()
{
}

// �������֐�
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

// �X�V�֐�
void Monster::Update(Game* pGame, Stage* pStage, int stageNumber, int playerX, int playerY)
{
	// ����
	if (m_state == State::Revive)
	{
		ChangeBody();
		return;
	}

	// �����X�^�[������Ă��鎞�Ԃ����Z����
	if ((m_state == State::Weak) && (m_weakTimer > 0))
	{
		m_weakTimer--;
		// �����X�^�[�͒ʏ��Ԃɖ߂�
		if (m_weakTimer == 0) m_state = State::Normal;
	}

	// �����X�^�[�̈ړ����ړ�����
	SetVelocity(pGame, stageNumber);
	m_x += m_vx;
	m_y += m_vy;

	// �����X�^�[�����ɓ����������`�F�b�N
	if ((m_state == State::Return) && (CheckNestPosition() == true))
	{
		// �������[�h
		m_state = State::Revive;
		m_animeTimer = ANIME_INTERVEL_A[static_cast<int>(m_animeNo)];
	}

	// �����]���ł���ꏊ���`�F�b�N�I�i���U�S�h�b�g�̃O���b�h�ɂ҂����肠�����ꏊ�j
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

		// ���݂̃����X�^�[�̐i�s�������擾
		int dir = GetDirection();
		// �i�s�����Ƃ͋t�������擾����
		dir = GetReverseDirection(dir);
		// �i�s�����Ƃ͋t�����Ɉړ����Ȃ��悤����
		moveFlag &= ~dir;

		// �H�ׂ��đ��ɖ߂�Ƃ��Ȃ�
		if (m_state == State::Return && monsterFlag != 0)
		{
			moveFlag = monsterFlag;
		}
		else
		{
			// �ǂ����������X�^�[�Œʏ��ԂȂ�
			if ((m_type == Type::Type1) && (m_state == State::Normal))
			{
				// �v���C���[�̂���G���A�����ֈړ�����
				moveFlag = ChasePlayer(moveFlag, playerX, playerY);
			}
			int dirCnt = GetDirectionCount(moveFlag);
			// ������ς��邩���f
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
		// �i�s������ݒ�
		m_dir = moveFlag;
	}
}

// �`��֐�
void Monster::Render()
{
	AnimeNo animeNo[] =
	{
		AnimeNo::Normal_02, AnimeNo::Return_01, AnimeNo::Return_02, AnimeNo::Return_03
	};

	// 8�h�b�g�P�ʂ̈ړ��ɂ���
	int x = (m_x >> 7) << 3;
	int y = (m_y >> 7) << 3;

	// �ʏ�܂��͎���Ă���ꍇ�͈ړ��A�j���[�V����
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

	// �H�ׂ�ꂽ���̃A�j���[�V����
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
		// �ǂ����������X�^�[�̐F
		anime_y = 0;
		int color = static_cast<int>(Common::Color::Pink);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	}
	else
	{
		// �C�܂��ꃂ���X�^�[�̐F
		anime_y = 1;
		int color = static_cast<int>(Common::Color::Purple);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
	}
	// �����X�^�[������Ă���
	if ( (m_state == State::Weak)
	  || (m_state == State::Dead)
	  || (m_state == State::Return)
	  || (m_state == State::Revive)
	   )
	{
		// ���F
		int color = static_cast<int>(Common::Color::Cyan);
		SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		// ����Ԃ��I���Q�b�O����_�ł�����
		if (m_weakTimer < 2 * 60 && (m_weakTimer >> 2) % 2)
		{
			// �D�F
			int color = static_cast<int>(Common::Color::Gray);
			SetDrawBright(Common::COLOR_TABLE[color][0], Common::COLOR_TABLE[color][1], Common::COLOR_TABLE[color][2]);
		}
	}

	// �`��
	DrawRectGraph(x + 8, y,
		0 + static_cast<int>(m_animeNo) * 56, anime_y * 56,
		56, 56,
		m_ghMonster, true, false);

	// �F�����ɖ߂�
	SetDrawBright(255, 255, 255);
}

// �����X�^�[���キ����֐�
void Monster::Weaken(int powerupTime)
{
	if (m_state == State::Normal || m_state == State::Weak)
	{
		m_state = State::Weak;
		m_weakTimer = powerupTime;
	}
}

// �v���C���[�ƐڐG���ɌĂ΂��֐�
void Monster::OnHit()
{
	// �ʏ펞
	if (m_state == State::Normal)
	{
		// �v���C���[��H�ׂ�
		m_state = State::Eat;
	}
	else
	{
		// �����X�^�[������Ă���
		if (m_state == State::Weak)
		{
			// �����X�^�[�͐H�ׂ�ꂽ
			m_state = State::Dead;
			// �����X�^�[�̐F��߂����߃^�C�}�[�����Z�b�g����
			m_weakTimer = 0;
			m_animeTimer = ANIME_INTERVEL_A[static_cast<int>(m_animeNo)];
		}
	}
}

// �����X�^�[�̈ړ����x���擾����֐�
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

// �����X�^�[�̑��x�ݒ�֐�
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

// �����X�^�[���H�ׂ�ꂽ��A��������Ƃ��̊G�̃p�^�[���`�F���W����֐�
bool Monster::ChangeBody()
{
	// �A�j���[�V�����^�C�}�[���O�łȂ��ꍇ�̓����X�^�[�͕ω���
	if (m_animeTimer == 0) return true;

	if (m_animeTimer > 0) m_animeTimer--;

	// �A�j���[�V�����^�C�}�[���O�ɂȂ�����
	if (m_animeTimer == 0)
	{
		// �����X�^�[�����񂾏ꍇ
		if (m_state == State::Dead)
		{
			switch (m_animeNo)
			{
			case AnimeNo::Normal_01:
			case AnimeNo::Normal_02:
				m_animeNo = AnimeNo::Return_01;
				break;
			case AnimeNo::Return_01:	// ���ɖ߂�p�^�[���P
				m_animeNo = AnimeNo::Return_02;
				break;
			case AnimeNo::Return_02:	// ���ɖ߂�p�^�[���Q
				m_animeNo = AnimeNo::Return_03;
				break;
			case AnimeNo::Return_03:	// ���ɖ߂�p�^�[���R�i�ڂ����j
				m_state = State::Return;
				m_animeTimer = 0;
				return true;
			}
			// �\�����Ԑݒ�
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
			case AnimeNo::Return_01:	// ���ɖ߂�p�^�[���P
				m_animeNo = AnimeNo::Normal_02;
				break;
			case AnimeNo::Return_02:	// ���ɖ߂�p�^�[���Q
				m_animeNo = AnimeNo::Return_01;
				break;
			case AnimeNo::Return_03:	// ���ɖ߂�p�^�[���R�i�ڂ����j
				m_animeNo = AnimeNo::Return_02;
				break;
			}
			// �\�����Ԑݒ�
			m_animeTimer = ANIME_INTERVEL_B[static_cast<int>(m_animeNo)];
		}
	}
	return false;
}

// �����X�^�[�̕����n�_�ɂ��邩�`�F�b�N����֐�
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

// �����X�^�[�̈ړ������̎擾
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

// �i�s�����Ƃ͋t�������擾
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

// �v���C���[��ǂ�������
int Monster::ChasePlayer(int moveFlag, int playerX, int playerY) const
{
	// �v���C���[���S���̂ǂ̃G���A�ɂ��邩���ׁA�Ⴄ�G���A�ɂ���ꍇ�͂�����̃G���A�Ɉړ�����
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

	// �����G���A�Ȃ牽�����Ȃ�
	if (dir == 0) dir = moveFlag;

	return dir;
}

// �ړ��ł�������̐����擾����֐�
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

