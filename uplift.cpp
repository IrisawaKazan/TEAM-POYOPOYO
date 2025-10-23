//****************************************************************
//
// ���t�g�̏���[uplift.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "manager.h"
#include "uplift.h"

//***************************************
// �R���X�g���N�^
//***************************************
CUpLift::CUpLift()
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CUpLift::~CUpLift()
{
}

//***************************************
// ����������
//***************************************
HRESULT CUpLift::Init(void)
{
	CObjectX::Init();

	// �����蔻��𐶐�
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(5.0f,5.0f,5.0f));

	// ���ʂ�ݒ�
	btScalar mass = 100.0f;

	// �ړ�������ݒ�
	btVector3 inertia(0, 0, 0);

	// �v�Z
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	// �g�����X�t�H�[����錾�A������
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPosition().x, GetPosition().y + 20.0f, GetPosition().z));

	// ���[�V�����X�e�[�g��錾
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	// ���W�b�g�{�f�B�[�̏���錾
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	// ���W�b�g�{�f�B�𐶐�
	m_RigitBody = std::make_unique<btRigidBody>(info);

	// �ړ�����
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ���[�U�[�|�C���^��ݒ�
	m_RigitBody->setUserPointer(this);

	// �ÓI�����I���ݒ�
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CUpLift::Uninit(void)
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

	CObjectX::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CUpLift::Update(void)
{
	// ��������
	D3DXVECTOR3 Dest = m_ApperPos + (m_Velocity * m_Limit);
	float LmitDis = D3DXVec3Length(&Dest);

	// �ʒu
	btVector3 Pos;
	btTransform trans;
	trans.setIdentity();
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	Pos = trans.getOrigin();

	// ���̈ʒu���珉���ʒu�̃x�N�g���̒������v�Z
	Dest = D3DXVECTOR3(Pos.x(), Pos.y(), Pos.z()) - m_ApperPos;
	float NowDis = D3DXVec3Length(&Dest);

	// �X�C�b�`��������Ă��Ȃ��Đ����܂ňړ�������
	if (!m_Switch && NowDis > LmitDis)
	{
		// �ړ��ʂ𔽓]
		m_Velocity = -m_Velocity;

		// �t���O��L����
		m_Switch = true;
	}
	// �X�C�b�`��������Ă��ď����ʒu�ɋ߂�������
	else if (m_Switch && NowDis < 10.0f)
	{
		// �ړ��ʂ𔽓]
		m_Velocity = -m_Velocity;

		// �t���O�𖳌���
		m_Switch = false;
	}

	// �ړ��ʂ�ݒ�
	m_RigitBody->setLinearVelocity(btVector3(m_Velocity.x, m_Velocity.y, m_Velocity.z));

	// �����ڂ̈ʒu��ݒ�
	SetPosition({ Pos.x() ,Pos.y() ,Pos.z() });

	// �X�V
	CObjectX::Update();
}

//***************************************
// �`��
//***************************************
void CUpLift::Draw(void)
{
	CObjectX::Draw();
}

//***************************************
// ����
//***************************************
CUpLift* CUpLift::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Velocity, float Limit)
{
	// �C���X�^���X��錾
	CUpLift* pInstance = nullptr;

	// �������m��
	pInstance = new CUpLift;

	// �m�ۂł�����
	if (pInstance != nullptr)
	{
		// ����ݒ�
		pInstance->SetPosition(Pos);
		pInstance->m_Velocity = Velocity;
		pInstance->m_Limit = Limit;
		pInstance->m_ApperPos = Pos;
		pInstance->m_Switch = false;
		pInstance->SetIdx("data\\MODEL\\test.x");

		// ����������
		pInstance->Init();

		// �C���X�^���X��Ԃ�
		return pInstance;
	}
	else
	{
		return nullptr;
	}
}