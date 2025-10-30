//--------------------------------
//
// �i�r�Q�[�V�����I�u�W�F�N�g [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // ���̃C���f�b�N�X (Trigger�n���p)

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviObject::Init(void)
{
	// �f�o�C�X���擾
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// ���_�o�b�t�@�����b�N
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();

	// �����蔻��
	SetTriggerObject();
	SetReleaseObject();

	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviObject::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// �������̊J��
	Release();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviObject::Update(void)
{
	// ���[���h�ւ̃|�C���^���擾
	btDynamicsWorld* world = CManager::GetDynamicsWorld();

	// �I�u�W�F�N�g�����[���h�ɓo�^����Ă���ꍇ�A�폜
	if (m_triggerObject)
	{
		world->removeCollisionObject(m_triggerObject.get());
	}
	if (m_releaseObject)
	{
		world->removeCollisionObject(m_releaseObject.get());
	}
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_biasIdx;                                        // �o�C�A�XID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �A���O���̓K��
	const D3DXVECTOR3 up = VEC_UP;
	D3DXMatrixRotationAxis(&mtxWorld, &up, m_angle);

	// �����𔽓]
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�o�C�X����f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_texIdx));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// �G�ꂽ��N������
//--------------------------------
CNavi::TYPE CNaviObject::ActivateTrigger(const btCollisionObject* const& collisionObject, float* pAngle, size_t* pIdx) const
{
	// ���g���Փ˂��Ă��邩
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// �Փˉ񐔕��J��Ԃ�
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// �Փ˂��Ă���y�A���擾
		btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

		// �Փ˂��Ă�����
		if (manifold->getNumContacts() <= 0) continue;

		// �Փ˃I�u�W�F�N�g�P�A�Q���擾
		const btCollisionObject* objA = manifold->getBody0();
		const btCollisionObject* objB = manifold->getBody1();

		// �v���C���[�ƃX�C�b�`���������Ă�����
		if ((objA == m_triggerObject.get() && objB == collisionObject) || (objA == collisionObject && objB == m_triggerObject.get()))
		{
			Activate(pAngle);
			if (pIdx != nullptr)
			{
				*pIdx = m_idx;
			}
			return m_type;
		}
	}

	return CNavi::TYPE::None;
}

//--------------------------------
// �G�ꂽ�����
//--------------------------------
bool CNaviObject::ReleaseTrigger(const btCollisionObject* const& collisionObject) const
{
	// ���g���Փ˂��Ă��邩
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// �Փˉ񐔕��J��Ԃ�
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// �Փ˂��Ă���y�A���擾
		btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

		// �Փ˂��Ă�����
		if (manifold->getNumContacts() <= 0) continue;

		// �Փ˃I�u�W�F�N�g�P�A�Q���擾
		const btCollisionObject* objA = manifold->getBody0();
		const btCollisionObject* objB = manifold->getBody1();

		// �v���C���[�ƃX�C�b�`���������Ă�����
		if ((objA == m_releaseObject.get() && objB == collisionObject) || (objA == collisionObject && objB == m_releaseObject.get()))
		{
			return true;
		}
	}

	return false;
}

//--------------------------------
// ActivateTrigger�p�̓����蔻��
//--------------------------------
void CNaviObject::SetTriggerObject()
{
	// �~���̐ݒ�
	float radius = m_chengeLength;
	float halfHeight = CHENGE_HEIGHT;
	btVector3 cylinderHalfExtents(radius, halfHeight, radius);
	m_triggerShape = std::make_unique<btCylinderShape>(cylinderHalfExtents);

	m_triggerObject = std::make_unique<btCollisionObject>();
	m_triggerObject->setCollisionShape(m_triggerShape.get());

	// ���W�Ɖ�]
	btVector3 bulletPos(m_pos.x, m_pos.y, m_pos.z);

	// Bullet�p��3x3�s��iBasis�j���쐬
	btMatrix3x3 bulletBasis;
	bulletBasis.setValue(
		m_mtxRot._11, m_mtxRot._12, m_mtxRot._13,
		m_mtxRot._21, m_mtxRot._22, m_mtxRot._23,
		m_mtxRot._31, m_mtxRot._32, m_mtxRot._33
	);

	// �g�����X�t�H�[�����쐬
	btTransform triggerTransform;
	triggerTransform.setIdentity();
	triggerTransform.setOrigin(bulletPos);
	triggerTransform.setBasis(bulletBasis);

	// �~���Ƀg�����X�t�H�[����ݒ�
	m_triggerObject->setWorldTransform(triggerTransform);

	// isTrigger(�����Ԃ�����)
	m_triggerObject->setCollisionFlags(m_triggerObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// �ǉ�
	CManager::GetDynamicsWorld()->addCollisionObject(m_triggerObject.get());
}

//--------------------------------
// ReleaseTrigger�p�̓����蔻��
//--------------------------------
void CNaviObject::SetReleaseObject()
{
	// �~���̐ݒ�
	float radius = m_length;
	float halfHeight = CHENGE_HEIGHT;
	btVector3 cylinderHalfExtents(radius, halfHeight, radius);
	m_releaseShape = std::make_unique<btCylinderShape>(cylinderHalfExtents);

	m_releaseObject = std::make_unique<btCollisionObject>();
	m_releaseObject->setCollisionShape(m_releaseShape.get());

	// ���W�Ɖ�]
	btVector3 bulletPos(m_pos.x, m_pos.y, m_pos.z);

	// Bullet�p��3x3�s��iBasis�j���쐬
	btMatrix3x3 bulletBasis;
	bulletBasis.setValue(
		m_mtxRot._11, m_mtxRot._12, m_mtxRot._13,
		m_mtxRot._21, m_mtxRot._22, m_mtxRot._23,
		m_mtxRot._31, m_mtxRot._32, m_mtxRot._33
	);

	// �g�����X�t�H�[�����쐬
	btTransform triggerTransform;
	triggerTransform.setIdentity();
	triggerTransform.setOrigin(bulletPos);
	triggerTransform.setBasis(bulletBasis);

	// �~���Ƀg�����X�t�H�[����ݒ�
	m_releaseObject->setWorldTransform(triggerTransform);

	// �ǉ�
	CManager::GetDynamicsWorld()->addCollisionObject(m_releaseObject.get());
}