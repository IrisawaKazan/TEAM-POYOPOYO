//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

// �ÓI�����o�ϐ��̒�`
const float CNavi::ENABLE_ANGLE = cosf(D3DXToRadian(60.0f));                // �����ĉ��x�܂�?
const D3DXVECTOR3 CNavi::MARKER_OFFSET = D3DXVECTOR3(0.0f, -1000.0f, 0.0f); // �i�r�}�[�J�[�̃I�t�Z�b�g�ʒu
const D3DXVECTOR2 CNavi::MARKER_SIZE = D3DXVECTOR2(60.0f, 60.0f);           // �i�r�}�[�J�[�̃T�C�Y

//--------------------------------
// ����������
//--------------------------------
HRESULT CNavi::Init(void)
{
	// �E�C���h�E�̃N���C�A���g���ł̃}�E�X���W���擾
	D3DXVECTOR2 mousePos = CManager::GetInputMouse()->GetPos();

	// �}�E�X���W��DirectX�̉�ʍ��W�ɕϊ�
	mousePos = ConvertMouseToScreen(mousePos);

	// ���C���쐬
	CreateRay(mousePos);

	// �����ʒu��ݒ�
	m_pos = MARKER_OFFSET;

	// �I�΂�Ă���I�u�W�F�N�g
	m_list = LIST::RightArrow;

	// �i�r�}�[�J�[��������
	m_pMarker = nullptr;
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNavi::Uninit(void)
{

}

//--------------------------------
// �X�V����
//--------------------------------
void CNavi::Update(void)
{
	// �E�C���h�E�̃N���C�A���g���ł̃}�E�X���W���擾
	D3DXVECTOR2 mousePos = CManager::GetInputMouse()->GetPos();

	// �}�E�X���W��DirectX�̉�ʍ��W�ɕϊ�
	mousePos = ConvertMouseToScreen(mousePos);

	// ���C���쐬
	CreateRay(mousePos);

	// �ʒu���X�V
	if (m_pMarker != nullptr)
	{
		m_pMarker->SetPos(m_pos);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) || CManager::GetInputMouse()->GetMouseState().lZ > 0)
	{// Q�L�[�ŃI�u�W�F�N�g��ύX
		m_list = static_cast<LIST>((static_cast<unsigned char>(m_list) + static_cast<unsigned char>(LIST::Max) - 1) % static_cast<unsigned char>(LIST::Max));
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) || CManager::GetInputMouse()->GetMouseState().lZ < 0)
	{// E�L�[�ŃI�u�W�F�N�g��ύX
		m_list = static_cast<LIST>((static_cast<unsigned char>(m_list) + 1) % static_cast<unsigned char>(LIST::Max));
	}

	if (m_pos.y > (MARKER_OFFSET.y + 1.0f) && CManager::GetInputMouse()->OnDown(0))
	{// ���N���b�N�����Ƃ�
		m_clickPos = m_pos; // �N���b�N�����ʒu��ۑ�

		// �I�u�W�F�N�g���Ƃ̕���
		switch (m_list)
		{
		case CNavi::LIST::RightArrow:
			// �����쐬
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(90.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::FrontArrow:
			// �����쐬
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(180.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::LeftArrow:
			// �����쐬
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(-90.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::BackArrow:
			// �����쐬
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(0.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		//case CNavi::LIST::Climb:
		//	break;
		//case CNavi::LIST::Attack:
		//	break;
		}
		if (m_apObject.empty()) return;

		auto pNewObject = m_apObject.back(); // �V�����쐬�����I�u�W�F�N�g�̃|�C���^

		bool isRepeat = false; // �I�u�W�F�N�g���d�Ȃ��Ă��邩����p�t���O
		do
		{// �I�u�W�F�N�g���d�Ȃ��Ă��邩����
			isRepeat = false;
			for (auto pObject : m_apObject)
			{// ���ɂ���I�u�W�F�N�g�ƐV�����쐬�����I�u�W�F�N�g���d�Ȃ��Ă��邩����
				if (pObject == pNewObject) continue; // �������g�̓X�L�b�v

				if (pNewObject->ReleaseTrigger(pObject->GetreleaseCollObject()))
				{// �d�Ȃ��Ă���ꍇ
					// �Â��I�u�W�F�N�g���폜
					pObject->RequestRelease();
					SwapRemove(m_apObject, pObject);

					isRepeat = true;
					break; // �����̃��[�v�𔲂��čēx������s��
				}
			}
		} while (isRepeat);

		for (size_t cntArrow = 0; cntArrow < m_apObject.size(); cntArrow++)
		{// �o�C�A�X�C���f�b�N�X�Đݒ�
			m_apObject[cntArrow]->SetBiasIdx(cntArrow);
		}

		m_apObject.shrink_to_fit(); // �������̖��ʂ��팸

		if (m_pMarker != nullptr)
		{
			m_pMarker->SetBiasID(m_apObject.size()); // �}�[�J�[�̃o�C�A�XID���X�V
		}
	}

	m_aRayCastTarget.clear(); // ���C�L���X�g�ΏۃI�u�W�F�N�g�z����N���A
}

//--------------------------------
// �}�E�X���W��DirectX�̉�ʃT�C�Y�ɕϊ�
//--------------------------------
D3DXVECTOR2 CNavi::ConvertMouseToScreen(D3DXVECTOR2 mousePos)
{
	// �E�C���h�E�̃N���C�A���g���ł̃}�E�X���W
	D3DXVECTOR2 windowClientPos = mousePos;

	// DirectX�̉�ʃT�C�Y���擾
	D3DXVECTOR2 directXScreenSize = { SCREEN_WIDTH,SCREEN_HEIGHT };

	// DirectX�f�o�C�X�ɓo�^����Ă���E�C���h�E�̃T�C�Y���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DDEVICE_CREATION_PARAMETERS creationParams{};
	D3DXVECTOR2 clientSize{ SCREEN_WIDTH,SCREEN_HEIGHT };
	if (SUCCEEDED(pDevice->GetCreationParameters(&creationParams)))
	{
		RECT rect{};
		if (creationParams.hFocusWindow != nullptr)
		{
			GetClientRect(creationParams.hFocusWindow, &rect);
			clientSize = { static_cast<float>(rect.right - rect.left),static_cast<float>(rect.bottom - rect.top) };
		}
	}

	// �}�E�X���W��DirectX�̉�ʃT�C�Y�ɕϊ�
	D3DXVECTOR2 resultMousePos{};
	resultMousePos.x = (windowClientPos.x / clientSize.x) * directXScreenSize.x;
	resultMousePos.y = (windowClientPos.y / clientSize.y) * directXScreenSize.y;

	return resultMousePos;
}

//--------------------------------
// ���C���쐬
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	// �ˉe�s��̎擾
	D3DXMATRIX mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�}�g���b�N�X�̎擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �n�_�ƏI�_
	D3DXVECTOR3 vNear, vFar;

	// �j�A�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenNear(mousePos.x, mousePos.y, 0.0f);
	D3DXVec3Unproject(
		&vNear,
		&screenNear,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// �t�@�[�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// ���C�̎n�_�ƕ���������
	m_RayPos = vNear;                        // ���C�̎n�_�̓j�A�N���b�v�ʏ�̓_
	m_RayDir = vFar - vNear;                 // �j�A����t�@�[�֌������x�N�g��
	D3DXVec3Normalize(&m_RayDir, &m_RayDir); // �����x�N�g���Ȃ̂Ő��K������
}

//--------------------------------
// ���C�L���X�g�̑ΏۃI�u�W�F�N�g��o�^
//--------------------------------
void CNavi::RegisterRayCastObject(LPD3DXMESH pMesh, const D3DXMATRIX& mtxWorld)
{
	// ���b�V����NULL�Ȃ�o�^���Ȃ�
	if (pMesh == nullptr) return;

	// �I�u�W�F�N�g��o�^
	RayCastTarget target;
	target.pMesh = pMesh;
	target.mtxWorld = mtxWorld;
	m_aRayCastTarget.push_back(target);
}

//--------------------------------
// ��_���v�Z
//--------------------------------
void CNavi::CalculateIntersection(void)
{
	D3DXVECTOR3 closestHitPos = MARKER_OFFSET; // �����ȃq�b�g�ʒu�ŏ�����
	float closestDistSq = -1.0f;               // �ł��߂������́u2��v
	D3DXMATRIX mtxRot{};                       // ��]�s��

	// �o�^���ꂽ�I�u�W�F�N�g�����[�v
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		// �I�u�W�F�N�g�̃��b�V���ƌ�������
		D3DXVECTOR3 normal{ 0.0f,1.0f,0.0f };
		D3DXVECTOR3 hitPos = MeshIntersect(target.pMesh, target.mtxWorld, ENABLE_ANGLE, &normal);

		if (hitPos.y > (MARKER_OFFSET.y + 1.0f))
		{// �q�b�g�������H (MeshIntersect �� MARKER_OFFSET �ȊO��Ԃ�����)
			// ���C�̎n�_�����_�܂ł̋�����2����v�Z
			D3DXVECTOR3 vecToHit = hitPos - m_RayPos;
			float distSq = D3DXVec3LengthSq(&vecToHit);

			// �u���߂Ẵq�b�g�v�܂��́u���Ɍ������_���߂��v�ꍇ
			if (closestDistSq < 0.0f || distSq < closestDistSq)
			{
				closestDistSq = distSq;                  // �������X�V
				closestHitPos = hitPos;                  // ��_���X�V
				mtxRot = CreateMatrixFromNormal(normal); // ��]�s����X�V
			}
		}
	}

	// �o�^���ꂽ�I�u�W�F�N�g�����[�v
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		if (CheckLatent(target.pMesh, target.mtxWorld, closestDistSq))
		{// �B��Ă����疳��
			m_pos = MARKER_OFFSET;
			return;
		}
	}

	// �ŏI�I�ɍł��߂��������W��o�^
	closestHitPos.y += HEIGHT;    // ������ɃI�t�Z�b�g
	m_pos = closestHitPos;        // �ʒu
	if (m_pMarker != nullptr)
	{
		m_pMarker->SetRotMtx(mtxRot); // �p�x
	}
}

//--------------------------------
// ���ʂƂ̌�_�����߂�
//--------------------------------
D3DXVECTOR3 CNavi::PlaneIntersect(float fHeight)
{
	// y=fHeight �̕���
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, -fHeight);

	// ��_
	D3DXVECTOR3 intersectionPoint;

	// ray�̏I�_
	D3DXVECTOR3 rayEnd = m_RayPos + m_RayDir * 10000.0f;

	// ���C�ƕ��ʂ̌�_���v�Z
	D3DXPlaneIntersectLine(
		&intersectionPoint, // ���ʂ̌�_
		&plane,             // ����
		&m_RayPos,          // ���C�̎n�_
		&rayEnd             // ���C�̏I�_
	);

	if (m_RayDir.y < 0)
	{// ���C���������̂Ƃ��̂݌�_��L���Ƃ���
		return intersectionPoint;
	}

	// ��_�������ȂƂ��͔��ɒႢ�ʒu��Ԃ�
	return D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
}

//----------------------------------------------------------------
// ���b�V���Ƃ̌�_�����߂� (D3DXIntersect ������)
//----------------------------------------------------------------
D3DXVECTOR3 CNavi::MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, const float enableAngle, D3DXVECTOR3* pNor)
{
	// Ray�̃��[�J�����W�n�ւ̕ϊ�
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	BOOL hit;
	DWORD faceIndex;                 // �ʂ̃C���f�b�N�X
	float dist;                      // ���� (���[�J��)
	float u, v;                      // �d�S���W
	LPD3DXBUFFER pHitInfo = nullptr; // �q�b�g���

	// Mesh��Ray�̂����蔻��
	if (FAILED(D3DXIntersect(
		pMesh,          // ���b�V��
		&localRayPos,   // ���C�̎n�_ (���[�J��)
		&localRayDir,   // ���C�̕��� (���[�J��)
		&hit,           // �q�b�g�������H
		&faceIndex,     // �ǂ̖ʂɃq�b�g�������H
		&u, &v,         // �d�S���W
		&dist,          // ����
		&pHitInfo,      // �ڍ׏��
		nullptr         // �S�q�b�g���
	)))
	{
		return MARKER_OFFSET;
	}

	if (pHitInfo != nullptr) pHitInfo->Release();

	// �q�b�g���Ȃ�����
	if (hit == FALSE)
	{
		return MARKER_OFFSET;
	}

	// �@���Ɗp�x���`�F�b�N
	if (pNor != nullptr || enableAngle > 0.0f)
	{
		DWORD fvf = pMesh->GetFVF();
		DWORD vertexSize = pMesh->GetNumBytesPerVertex();
		DWORD norOffset = (DWORD)-1;

		// �@���̃I�t�Z�b�g
		if (fvf & D3DFVF_XYZ) norOffset = sizeof(D3DXVECTOR3);
		if (!(fvf & D3DFVF_NORMAL)) norOffset = (DWORD)-1;

		if (norOffset != (DWORD)-1)
		{// �@�����Ȃ�
			void* pVertices = nullptr;
			void* pIndices = nullptr;
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVertices);
			pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIndices);

			// �q�b�g�����ʂ�3���_�̃C���f�b�N�X���擾
			WORD* pIndex = (WORD*)pIndices + faceIndex * 3;
			WORD i0 = pIndex[0];
			WORD i1 = pIndex[1];
			WORD i2 = pIndex[2];

			// ���_�f�[�^(�o�C�g�z��)�̐擪�A�h���X���擾
			BYTE* pVertData0 = (BYTE*)pVertices + i0 * vertexSize;
			BYTE* pVertData1 = (BYTE*)pVertices + i1 * vertexSize;
			BYTE* pVertData2 = (BYTE*)pVertices + i2 * vertexSize;

			// �I�t�Z�b�g���g���Ė@���̃|�C���^���擾
			D3DXVECTOR3* n0 = (D3DXVECTOR3*)(pVertData0 + norOffset);
			D3DXVECTOR3* n1 = (D3DXVECTOR3*)(pVertData1 + norOffset);
			D3DXVECTOR3* n2 = (D3DXVECTOR3*)(pVertData2 + norOffset);

			// ���[�J���@������
			D3DXVECTOR3 localNormal = (1.0f - u - v) * (*n0) + u * (*n1) + v * (*n2);

			pMesh->UnlockVertexBuffer();
			pMesh->UnlockIndexBuffer();

			// ���[���h�@���ɕϊ�
			D3DXMATRIX matInvTranspose;
			D3DXMatrixTranspose(&matInvTranspose, &mtxInvWorld);
			D3DXVECTOR3 worldNormal;
			D3DXVec3TransformNormal(&worldNormal, &localNormal, &matInvTranspose);
			D3DXVec3Normalize(&worldNormal, &worldNormal);

			// �p�x�`�F�b�N
			const D3DXVECTOR3 up = VEC_UP;
			if (D3DXVec3Dot(&worldNormal, &up) < enableAngle)
			{
				return MARKER_OFFSET; // �p�x���_���Ȃ�q�b�g����
			}

			if (pNor != nullptr) *pNor = worldNormal; // �@����Ԃ�
		}
		else
		{
			// �@�����Ȃ����b�V��
			if (pNor != nullptr) *pNor = VEC_UP;
		}
	}

	// ��_���v�Z
	D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * dist;
	D3DXVECTOR3 intersectionPoint;
	D3DXVec3TransformCoord(&intersectionPoint, &localIntersectionPoint, &mtxWorld);

	return intersectionPoint;
}

//----------------------------------------------------------------
// Ray�̂��̋����̒n�_�͉B��Ă��邩?
//----------------------------------------------------------------
bool CNavi::CheckLatent(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, float lengthSq)
{
	// ���[���h�s��̋t�s��
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);

	// ���C�̎n�_�ƕ��������[���h���W�n���烍�[�J�����W�n�ɕϊ�
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	BOOL hit;
	DWORD faceIndex;                 // �ʂ̃C���f�b�N�X
	float dist;                      // ���� (���[�J��)
	float u, v;                      // �d�S���W
	LPD3DXBUFFER pHitInfo = nullptr; // �q�b�g���

	// Mesh��Ray�̂����蔻��
	if (FAILED(D3DXIntersect(
		pMesh,          // ���b�V��
		&localRayPos,   // ���C�̎n�_ (���[�J��)
		&localRayDir,   // ���C�̕��� (���[�J��)
		&hit,           // �q�b�g�������H
		&faceIndex,     // �ǂ̖ʂɃq�b�g�������H
		&u, &v,         // �d�S���W
		&dist,          // ����
		&pHitInfo,      // �ڍ׏��
		nullptr         // �S�q�b�g���
	)))
	{
		return false;
	}

	if (pHitInfo != nullptr) pHitInfo->Release();

	// �B���Ă��Ȃ�
	if (hit == FALSE)
	{
		return false;
	}

	// ���[�J�����W�n�ł̌�_
	D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * dist;

	// ���[�J�����W�����[���h���W�ɕϊ�
	D3DXVECTOR3 worldIntersectionPoint;
	D3DXVec3TransformCoord(&worldIntersectionPoint, &localIntersectionPoint, &mtxWorld);

	// ���[���h��Ԃ̃��C�n�_����̋�����2��
	D3DXVECTOR3 vecToHit = worldIntersectionPoint - m_RayPos;
	float worldDistSq = D3DXVec3LengthSq(&vecToHit);

	// �����̔�r
	if (worldDistSq < lengthSq)
	{// �B���Ă���
		return true;
	}

	// �B���Ă��Ȃ�
	return false;
}

//----------------------------------------------------------------
// �@���x�N�g�������]�s����쐬����
//----------------------------------------------------------------
D3DXMATRIX CNavi::CreateMatrixFromNormal(D3DXVECTOR3 nor)
{
	D3DXVec3Normalize(&nor, &nor);

	D3DXVECTOR3 newY_Axis = nor; // Y���͖@��
	D3DXVECTOR3 newZ_Axis;
	D3DXVECTOR3 newX_Axis;

	D3DXVECTOR3 referenceForward(0.0f, 0.0f, 1.0f); // ��ƂȂ�O��

	if (fabsf(D3DXVec3Dot(&newY_Axis, &referenceForward)) > 0.999f)
	{// �����@�����قڐ^��܂��͐^���������Ă���
		// ��ƂȂ�O�����A���[���h�̏����(0,1,0)�ɕύX����
		referenceForward = VEC_UP;
	}

	// ��ƂȂ�O������A�@���ɕ��s�Ȑ������������ƂŁA���ʏ�ɓ��e����
	D3DXVECTOR3 parallelPart = D3DXVec3Dot(&referenceForward, &newY_Axis) * newY_Axis;
	newZ_Axis = referenceForward - parallelPart;
	D3DXVec3Normalize(&newZ_Axis, &newZ_Axis);

	// Y x Z = X
	D3DXVec3Cross(&newX_Axis, &newY_Axis, &newZ_Axis);
	D3DXVec3Normalize(&newX_Axis, &newX_Axis);

	// �s���g�ݗ��Ă�
	D3DXMATRIX mtxRot;
	D3DXMatrixIdentity(&mtxRot);

	mtxRot._11 = newX_Axis.x; mtxRot._12 = newX_Axis.y; mtxRot._13 = newX_Axis.z;
	mtxRot._21 = newY_Axis.x; mtxRot._22 = newY_Axis.y; mtxRot._23 = newY_Axis.z;
	mtxRot._31 = newZ_Axis.x; mtxRot._32 = newZ_Axis.y; mtxRot._33 = newZ_Axis.z;

	return mtxRot;
}

//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̍쐬
//--------------------------------
void CNavi::SetMarker(void)
{
	m_pMarker = CNaviMarker::Create(MARKER_TEXTURE_PATH, MARKER_SIZE);
}