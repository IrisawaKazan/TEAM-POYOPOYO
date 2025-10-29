//************************************************************
//
//�@�v���C���[�̏���[player.cpp]
//�@Author:chikada shouya
//
//************************************************************
#include "player.h"
#include "math_T.h"
#include "game.h"
#include "navi.h"
#include "arrow.h"

// �ÓI�����o�ϐ��̒�`

// �R���X�g���N�^
CPlayer::CPlayer()
{

}

// �f�X�g���N�^
CPlayer::~CPlayer()
{

}

// ������
HRESULT CPlayer::Init(void)
{
	CModelCharacter::Init("data\\Model\\MOTION\\player003.txt",5.0f);

	m_CollisionShape = std::make_unique<btCapsuleShape>(btScalar(7.0f), btScalar(20.0f));

	btScalar mass = 1.0f; // ���ʂ�1�ȏ�ɂ��邱�Ƃœ��I���̂ɂȂ�
	btVector3 inertia(0, 0, 0);
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPos().x, GetPos().y + 20.0f, GetPos().z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	m_RigitBody = std::make_unique<btRigidBody>(info);
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	m_RigitBody->setUserPointer(this);
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	m_RigitBody->setFriction(btScalar(0.02f));

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	// �i�r�Q�[�V�����I�u�W�F�N�g�̐ڐG�񐔊Ǘ��p�}�b�v�̏����� sato Add
	m_naviObjectCountMap = std::unordered_map<size_t, unsigned short>();
	m_naviObjectIdxListOld = std::vector<size_t>();

	return S_OK;
}

// �j��
void CPlayer::Uninit(void)
{
	// ���̂̍폜
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
	}

	// �Փˌ`��̍폜
	m_CollisionShape.reset();

	CModelCharacter::Uninit();
}

// �X�V
void CPlayer::Update(void)
{
	btVector3 moveDir(0, 0, 0);
	btVector3 newPos;

	btTransform trans;

	if (m_RigitBody == nullptr) return;

	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// �i�r�Q�[�V�����I�u�W�F�N�g�ɐG��鏈�� sato Add
	std::vector<size_t> naviObjectIdxListNew{}; // ���t���[���ŐG�ꂽ�i�r�Q�[�V�����I�u�W�F�N�g�̃C���f�b�N�X���X�g
	std::vector<CNaviObject*> pObjects = CNavi::GetInstance()->GetObjects();
	for (const CNaviObject* pObject : pObjects)
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();

		size_t idx = 0;
		CNavi::TYPE naviType = pObject->ActivateTrigger(pos, &rot.y, &idx);

		// �G�ꂽ�i�r�Q�[�V�����^�C�v�ɂ���ď����𕪊�
		switch (naviType)
		{
		case CNavi::TYPE::Arrow:
			SetRotDest(rot);
			SetRot(rot);
			break;
		//case CNavi::TYPE::Climb:
		//	break;
		//case CNavi::TYPE::Attack:
			break;
		}

		if (naviType != CNavi::TYPE::None && naviType != CNavi::TYPE::Max)
		{
			// ���߂Ă̐ڐG�̏ꍇ�̓}�b�v�ɓo�^
			m_naviObjectCountMap.try_emplace(idx, short(0));

			// �O�̃t���[���ŐG��Ă��Ȃ������ꍇ�͐ڐG�񐔂��J�E���g�A�b�v
			if (std::find(m_naviObjectIdxListOld.begin(), m_naviObjectIdxListOld.end(), idx) == m_naviObjectIdxListOld.end())
			{
				m_naviObjectCountMap[idx]++;
			}

			// 5��ȏ�ɂȂ����玀��ł��܂�
			if (m_naviObjectCountMap[idx] >= 5)
			{
				CGame::GetPlayerManager()->DethMessage(this);
				Uninit();
				Release();
				return;
			}
			naviObjectIdxListNew.push_back(idx); // ���X�g�ɒǉ�
		}
	}
	m_naviObjectIdxListOld = naviObjectIdxListNew; // ���t���[���ŐG�ꂽ�i�r�Q�[�V�����I�u�W�F�N�g���X�g��ۑ�

	// �ړ����� sato Add
	D3DXVECTOR3 rot = GetRot();
	moveDir.setX(-sinf(rot.y) * MOVE_SPEED);
	moveDir.setZ(-cosf(rot.y) * MOVE_SPEED);

	moveDir.setY(m_RigitBody->getLinearVelocity().y());

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	{
		moveDir.setY(moveDir.y() + 50.0f);
	}

	m_RigitBody->setLinearVelocity(moveDir);

	newPos = trans.getOrigin();
	//newPos.setY(0.0f);
	SetPos(D3DXVECTOR3(newPos.x(), newPos.y(), newPos.z()));

	CModelCharacter::Update();
}

// �`��
void CPlayer::Draw(void)
{
	CModelCharacter::Draw();
}

// �ʒu��ݒ�
void CPlayer::SetPos(D3DXVECTOR3 Pos)
{
	btVector3 newPos;

	btTransform trans;
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3(Pos.x, Pos.y, Pos.z));
	m_RigitBody->setWorldTransform(trans);
	m_RigitBody->getMotionState()->setWorldTransform(trans);
	CModelCharacter::SetPos(D3DXVECTOR3(Pos.x, Pos.y - 20.0f, Pos.z));
}

// ����
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->SetRot(rot);
		pPlayer->SetRotDest(rot);
		pPlayer->Init();
		pPlayer->SetPos(pos);
		return pPlayer;
	}
	else
	{
		return nullptr;
	}
}