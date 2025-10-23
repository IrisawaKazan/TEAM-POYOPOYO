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

//***************************************
// �R���X�g���N�^
//***************************************
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_NONE;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_VtxMax = { -10000.0f,-10000.0f,-10000.0f };
	m_VtxMin = { 10000.0f,10000.0f,10000.0f };
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	sNamePath = {};
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
		pBlock->SetFilePath(sName);
		pBlock->sNamePath = sName;
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
//�@�u���b�N����������
//***************************************
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));

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

	// ���_������
	for (int nCntBlock = 0; nCntBlock < nNumVtx; nCntBlock++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���W�̔�r
		if (Vtx.x > m_VtxMax.x)
		{// x���ő�l���傫��������
			// ���
			m_VtxMax.x = Vtx.x;
		}
		if (Vtx.y > m_VtxMax.y)
		{// y���ő�l���傫��������
			// ���
			m_VtxMax.y = Vtx.y;
		}
		if (Vtx.z > m_VtxMax.z)
		{// z���ő�l���傫��������
			// ���
			m_VtxMax.z = Vtx.z;
		}

		if (Vtx.x < m_VtxMin.x)
		{// x���ŏ��l��菬����������
			// ���
			m_VtxMin.x = Vtx.x;
		}
		if (Vtx.y < m_VtxMin.y)
		{// y���ŏ��l��菬����������
			// ���
			m_VtxMin.y = Vtx.y;
		}
		if (Vtx.z < m_VtxMin.z)
		{// z���ŏ��l��菬����������
			// ���
			m_VtxMin.z = Vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���i�߂�
		pVtxBuff += sizeFVF;
	}

	// �T�C�Y��������
	m_size.x = m_VtxMax.x;			// size��x
	m_size.y = m_VtxMax.y * 0.5f;	// size��y
	m_size.z = m_VtxMax.z;			// size��z

	// ���W�b�g�{�f�B�[�̃I�t�Z�b�g��ݒ�
	m_RBOffset.y = m_size.y * GetScale().y;

	// �A�����b�N
	pMesh->UnlockVertexBuffer();

	InitRB();

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
	rotation = CObjectX::ConvertQuad(GetQuad());

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
	// �X�VW
	CObjectX::Update();

	// �i�r�Ƀ��C�L���X�g�I�u�W�F�N�g��o�^ sato ��
	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));
	CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
//�@�u���b�N�`�揈��
//***************************************
void CBlock::Draw(void)
{
	// �`��
	CObjectX::Draw();
}