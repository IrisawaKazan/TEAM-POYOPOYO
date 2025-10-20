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
	CModelCharacter::Init("data\\Model\\MOTION\\player000.txt",5.0f);

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

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

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
	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// ���ɐG�ꂽ�������ς��� sato Add
	std::vector<CArrow*> apArrow = CGame::GetNavi().GetArrow();
	for (const CArrow* pArrow : apArrow)
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		pArrow->ChengeAngle(pos, &rot);

		SetRotDest(rot);
		SetRot(rot);
	}

	// �ړ����� sato Add
	D3DXVECTOR3 rot = GetRot();
	moveDir.setX(-sinf(rot.y) * MOVE_SPEED);
	moveDir.setZ(-cosf(rot.y) * MOVE_SPEED);

	moveDir.setY(m_RigitBody->getLinearVelocity().y());
	m_RigitBody->setLinearVelocity(moveDir);

	newPos = trans.getOrigin();
	//newPos.setY(0.0f);
	SetPos(D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));

	CModelCharacter::Update();
}

// �`��
void CPlayer::Draw(void)
{
	CModelCharacter::Draw();
}

// ����
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->SetPos(pos);
		pPlayer->SetRot(rot);
		pPlayer->Init();
		return pPlayer;
	}
	else
	{
		return nullptr;
	}
}