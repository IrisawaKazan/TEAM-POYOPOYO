#include "manager.h"
#include "uplift.h"

CUpLift::CUpLift()
{
}

CUpLift::~CUpLift()
{
}

HRESULT CUpLift::Init(void)
{
	CObjectX::Init();

	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(10.0f,10.0f,10.0f));

	btScalar mass = 10.0f; // Ž¿—Ê‚ð1ˆÈã‚É‚·‚é‚±‚Æ‚Å“®“I„‘Ì‚É‚È‚é
	btVector3 inertia(0, 0, 0);
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPosition().x, GetPosition().y + 20.0f, GetPosition().z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	m_RigitBody = std::make_unique<btRigidBody>(info);
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	m_RigitBody->setUserPointer(this);
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	return S_OK;
}

void CUpLift::Uninit(void)
{
	// „‘Ì‚Ìíœ
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
	}

	// Õ“ËŒ`ó‚Ìíœ
	m_CollisionShape.reset();

	CObjectX::Uninit();
}

void CUpLift::Update(void)
{
	btVector3 Pos;
	btTransform trans;
	trans.setIdentity();
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	Pos = trans.getOrigin();
	m_RigitBody->setLinearVelocity(btVector3(m_Velocity.x, m_Velocity.y, m_Velocity.z));
	SetPosition({ Pos.x() ,Pos.y() ,Pos.z() });
	CObjectX::Update();
}

void CUpLift::Draw(void)
{
	CObjectX::Draw();
}

CUpLift* CUpLift::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Velocity, float Limit)
{
	CUpLift* pInstance = nullptr;
	pInstance = new CUpLift;

	if (pInstance != nullptr)
	{
		pInstance->SetPosition(Pos);
		pInstance->m_Velocity = Velocity;
		pInstance->m_Limit = Limit;
		pInstance->SetFilePath("data\\MODEL\\test.x");
		pInstance->Init();
		return pInstance;
	}
	else
	{
		return nullptr;
	}
}