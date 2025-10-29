//=================================================
//
//	block.cpp
// 
// Author:�ߓc ����
//
//=================================================

//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "block.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"
#include "switch.h"

//***************************************
// �R���X�g���N�^
//***************************************
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_NONE;
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	m_sNamePath = {};
}

//***************************************
// �f�X�g���N�^
//***************************************
CBlock::~CBlock()
{

}

//***************************************
// �u���b�N�̐���
//***************************************
CBlock* CBlock::Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale)
{
	CBlock* pBlock = nullptr;
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pBlock->m_sNamePath = sName;
		pBlock->SetPosition(pos);
		pBlock->SetRotasion(rot);
		pBlock->SetScale(Scale);
		pBlock->Init();
		return pBlock;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
// �N�H�[�^�j�I����ݒ�
//***************************************
void CBlock::SetQuat(btQuaternion Quad)
{
	if (m_RigitBody == nullptr) return;
	D3DXQUATERNION myQuat = CMath::ConvertQuat(Quad);
	SetQuad(myQuat);
}

//***************************************
//�@�u���b�N����������
//***************************************
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(m_sNamePath));

	LPD3DXMESH pMesh = modelinfo.pMesh;

	// ���[�J���ϐ��錾-----------------------------
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	//----------------------------------------------

	// ���_���̎擾
	nNumVtx = pMesh->GetNumVertices();

	// ���_�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 Max, Min;
	Max = VEC3_NULL;
	Min = VEC3_NULL;

	// ���_������
	for (int nCntBlock = 0; nCntBlock < nNumVtx; nCntBlock++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���W�̔�r
		if (Vtx.x > Max.x)
		{// x���ő�l���傫��������
			// ���
			Max.x = Vtx.x;
		}
		if (Vtx.y > Max.y)
		{// y���ő�l���傫��������
			// ���
			Max.y = Vtx.y;
		}
		if (Vtx.z > Max.z)
		{// z���ő�l���傫��������
			// ���
			Max.z = Vtx.z;
		}

		if (Vtx.x < Min.x)
		{// x���ŏ��l��菬����������
			// ���
			Min.x = Vtx.x;
		}
		if (Vtx.y < Min.y)
		{// y���ŏ��l��菬����������
			// ���
			Min.y = Vtx.y;
		}
		if (Vtx.z < Min.z)
		{// z���ŏ��l��菬����������
			// ���
			Min.z = Vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���i�߂�
		pVtxBuff += sizeFVF;
	}

	// �T�C�Y��������
	m_size.x = Max.x;			// size��x
	m_size.y = Max.y * 0.5f;	// size��y
	m_size.z = Max.z;			// size��z

	// ���W�b�g�{�f�B�[�̃I�t�Z�b�g��ݒ�
	m_RBOffset.y = m_size.y * GetScale().y;

	// �A�����b�N
	pMesh->UnlockVertexBuffer();

	InitRB();

	SetIdx(m_sNamePath);

	return S_OK;
}

//***************************************
//�@���W�b�g�{�f�B�[�̏�����
//***************************************
void CBlock::InitRB(void)
{
	// �������m��OBB�̑傫����ݒ�
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x * GetScale().x, m_size.y * GetScale().y, m_size.z * GetScale().z));

	// ����
	btScalar mass = 0.0f;

	// �������[�����g�̌v�Z�p�ϐ�
	btVector3 inertia(0, 0, 0);

	// �v�Z
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	// �ŏI�I�Ȍv�Z���ʁA�ʒu�A�ʒu����̃I�t�Z�b�g
	btTransform transform,Origin,Offset;

	// ������
	transform.setIdentity();
	Origin.setIdentity();
	Offset.setIdentity();

	// ����
	btQuaternion rotation;

	// ���
	rotation = CMath::ConvertQuat(GetQuad());

	// �ʒu�ƌ�����ݒ�
	Origin.setRotation(rotation);
	Origin.setOrigin(btVector3(GetPosition().x, GetPosition().y, GetPosition().z));

	// �I�t�Z�b�g��ݒ�
	Offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));

	// ����
	transform.mult(Origin, Offset);

	// �ʒu�ƌ������Ǘ����郂�[�V�����𐶐�
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	// ����
	m_RigitBody = std::make_unique<btRigidBody>(info);

	// �ړ�������ݒ�
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// �����{�f�B�[�Ɏ������g��R�t��
	m_RigitBody->setUserPointer(this);

	// ���I�I�u�W�F�N�g���ÓI�I�u�W�F�N�g��
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// �������E�ɕ����{�f�B�[��ǉ�
	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
}

//***************************************
//�@�u���b�N�I������
//***************************************
void CBlock::Uninit(void)
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

	// �j��
	CObjectX::Uninit();
}

//***************************************
//�@�u���b�N�X�V����
//***************************************
void CBlock::Update(void)
{
	if (m_RigitBody == nullptr) return;

	// ���W�b�g�{�f�B�[�̍X�V
	UpdateRB();

	// �g�����X*�t�H�[��
	btTransform trans;

	// �Q�b�g
	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// ��]�s����擾�I�t�Z�b�g���|����
	// �������E�ł̈ʒu����I�t�Z�b�g�����炵���������E�ł̈ʒu���v�Z����p�̕ϐ�
	btVector3 worldoffet = trans.getBasis() * btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z);

	// �������E�̈ʒu�����]�s����������킹���I�t�Z�b�g������
	btVector3 pos = trans.getOrigin() - worldoffet;

	// �ʒu��ݒ�
	SetPosition(D3DXVECTOR3(pos.x(), pos.y(), pos.z()));

	// �i�r�Ƀ��C�L���X�g�I�u�W�F�N�g��o�^ sato ��
	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(GetIndx());
	CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
// ���W�b�g�{�f�B�[�̍X�V����
//***************************************
void CBlock::UpdateRB(void)
{
	// ���̂̍폜
	if (m_RigitBody)
	{
		// �������E����폜
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());

		// ���[�V�����X�e�[�g���擾null�`�F�b�N
		if (m_RigitBody->getMotionState())
		{
			// ���[�V�����X�e�[�g���폜
			delete m_RigitBody->getMotionState();
		}
		// ���W�b�g�{�f�B���N���A
		m_RigitBody.reset();
	}

	D3DXVECTOR3 Scale = GetScale();

	// �����蔻����Đ���
	m_CollisionShape.reset(new btBoxShape(btVector3(m_size.x * Scale.x, m_size.y * Scale.y, m_size.z * Scale.z)));
	m_RBOffset.y = m_size.y * Scale.y;

	// ���ʂ�錾
	btScalar Mass = 0;

	// �R�͂���
	btVector3 Inertia = { 0.0f,0.0f,0.0f };

	// �R�͂�ݒ�
	m_CollisionShape->calculateLocalInertia(Mass, Inertia);

	// �������E�̈ʒu�Ȃǂ��擾
	btTransform transform, origin, offset;

	// ������
	transform.setIdentity();
	origin.setIdentity();
	offset.setIdentity();

	// OBB�̉�]�i��FY���܂���45�x��]�j
	btQuaternion rotation;
	rotation = CMath::ConvertQuat(GetQuad());
	origin.setRotation(rotation);
	origin.setOrigin(btVector3(GetPosition().x, GetPosition().y, GetPosition().z));
	offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));
	transform.mult(origin, offset);

	// �C���^�[�t�F�C�X��ݒ�
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(Mass, motionState, m_CollisionShape.get());

	// ���W�b�g�{�f�B�[���Đ���
	m_RigitBody.reset(new btRigidBody(info));

	// �ړ������𐧌�
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ���[�U�[�|�C���^��ݒ�
	m_RigitBody->setUserPointer(this);

	// �X���[�v��Ԃ�ݒ�
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// �������E�Ƀ��W�b�g�{�f�B�[��ǉ�
	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
}

//***************************************
//�@�u���b�N�`�揈��
//***************************************
void CBlock::Draw(void)
{
	// �`��
	CObjectX::Draw();
}