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

	// ���̌�����������
	m_direction = ARROW_DIRECTION::Left;

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
		m_pMarker->SetPosition(m_pos);
	}

	if (CManager::GetInputMouse()->GetMouseState().lZ > 0.0f)
	{// �z�C�[���A�b�v�Ŗ��̌�����ύX
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + static_cast<unsigned char>(ARROW_DIRECTION::Max) - 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}
	else if (CManager::GetInputMouse()->GetMouseState().lZ < 0.0f)
	{// �z�C�[���_�E���Ŗ��̌�����ύX
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}

	if (m_pos.y > (MARKER_OFFSET.y + 1.0f) && CManager::GetInputMouse()->OnDown(0))
	{// ���N���b�N�����Ƃ�
		m_clickPos = m_pos; // �N���b�N�����ʒu��ۑ�

		// ���̊p�x������
		float arrowAngle = 0.0f;
		switch (m_direction)
		{
		case CNavi::ARROW_DIRECTION::Left:
			arrowAngle = D3DXToRadian(-90.0f);
			break;
		case CNavi::ARROW_DIRECTION::Front:
			arrowAngle = D3DXToRadian(0.0f);
			break;
		case CNavi::ARROW_DIRECTION::Back:
			arrowAngle = D3DXToRadian(180.0f);
			break;
		}

		// �����쐬
		m_apArrow.push_back(CArrow::Create(m_clickPos, D3DXVECTOR3(0.0f, arrowAngle, 0.0f), "data/TEXTURE/UI/ArrowMark001.png", { m_pMarker->GetWidth(),m_pMarker->GetVetical() }, m_apArrow.size()));

		auto pNewArrow = m_apArrow.back(); // �V�����쐬�������̃|�C���^

		bool isRepeat = false; // ��󂪏d�Ȃ��Ă��邩����p�t���O
		do
		{// ��󂪏d�Ȃ��Ă��邩����
			isRepeat = false;
			for (auto pArrow : m_apArrow)
			{// ���ɂ�����ƐV�����쐬������󂪏d�Ȃ��Ă��邩����
				if (pArrow == pNewArrow) continue; // �������g�̓X�L�b�v

				if (pNewArrow->ReleaseHit(pArrow->GetPos(), pArrow->GetChengeLength()))
				{// �d�Ȃ��Ă���ꍇ
					// �Â������폜
					pArrow->RequestRelease();
					SwapRemove(m_apArrow, pArrow);

					isRepeat = true;
					break; // �����̃��[�v�𔲂��čēx������s��
				}
			}
		} while (isRepeat);

		for (size_t cntArrow = 0; cntArrow < m_apArrow.size(); cntArrow++)
		{// �C���f�b�N�X�Đݒ�
			m_apArrow[cntArrow]->SetIdx(cntArrow);
		}

		m_apArrow.shrink_to_fit(); // �������̖��ʂ��팸

		if (m_pMarker != nullptr)
		{
			m_pMarker->SetBiasID(m_apArrow.size()); // �i�r�}�[�J�[�̃o�C�A�XID���X�V
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

	// �o�^���ꂽ�I�u�W�F�N�g�����[�v
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		// �I�u�W�F�N�g�̃��b�V���ƌ�������
		D3DXVECTOR3 hitPos = MeshIntersect(target.pMesh, target.mtxWorld);

		if (hitPos.y > (MARKER_OFFSET.y + 1.0f))
		{// �q�b�g�������H (MeshIntersect �� MARKER_OFFSET �ȊO��Ԃ�����)
			// ���C�̎n�_�����_�܂ł̋�����2����v�Z
			D3DXVECTOR3 vecToHit = hitPos - m_RayPos;
			float distSq = D3DXVec3LengthSq(&vecToHit);

			// �u���߂Ẵq�b�g�v�܂��́u���Ɍ������_���߂��v�ꍇ
			if (closestDistSq < 0.0f || distSq < closestDistSq)
			{
				closestDistSq = distSq; // �������X�V
				closestHitPos = hitPos; // ��_���X�V
			}
		}
	}

	// �ŏI�I�ɍł��߂��������W��o�^
	closestHitPos.y += HEIGHT; // ������ɃI�t�Z�b�g
	m_pos = closestHitPos;
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

//--------------------------------
// ���b�V���Ƃ̌�_�����߂�
//--------------------------------
D3DXVECTOR3 CNavi::MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld)
{
	// ���[���h�s��̋t�s����v�Z
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);

	// ���C�̎n�_�ƕ��������[���h���W�n���烍�[�J�����W�n�ɕϊ�
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	// ���b�V���̏����擾
	DWORD numFaces = pMesh->GetNumFaces();
	DWORD vertexSize = pMesh->GetNumBytesPerVertex();

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̃|�C���^
	void* pVertices = nullptr;
	void* pIndices = nullptr;

	// �o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVertices);
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIndices);

	float closestDist = -1.0f;                           // �ł��߂���_�܂ł̋���

	// �S�Ă̖ʁi�O�p�`�j�����[�v���ă`�F�b�N
	for (DWORD i = 0; i < numFaces; ++i)
	{
		// �C���f�b�N�X�o�b�t�@����3���_�̃C���f�b�N�X���擾
		WORD* pIndex = (WORD*)pIndices + i * 3;
		WORD i0 = pIndex[0];
		WORD i1 = pIndex[1];
		WORD i2 = pIndex[2];

		// ���_�o�b�t�@����3���_�̍��W���擾
		D3DXVECTOR3* v0 = (D3DXVECTOR3*)((BYTE*)pVertices + i0 * vertexSize);
		D3DXVECTOR3* v1 = (D3DXVECTOR3*)((BYTE*)pVertices + i1 * vertexSize);
		D3DXVECTOR3* v2 = (D3DXVECTOR3*)((BYTE*)pVertices + i2 * vertexSize);

		float dist;
		BOOL hit = D3DXIntersectTri(
			v0, v1, v2,             // �O�p�`��3���_
			&localRayPos,           // ���C�̎n�_ (���[�J��)
			&localRayDir,           // ���C�̕��� (���[�J��)
			nullptr, nullptr,       // U, V���W (�d�S���W�n)
			&dist                   // ��_�܂ł̋���
		);

		if (hit)
		{
			// ���߂Ẵq�b�g�A�܂��͂��߂��_�Ńq�b�g�����ꍇ
			if (closestDist < 0.0f || dist < closestDist)
			{
				closestDist = dist;
			}
		}
	}

	// �o�b�t�@���A�����b�N
	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	// ��_���W���i�[����ϐ� (�����l�ŏ�����)
	D3DXVECTOR3 intersectionPoint = MARKER_OFFSET;

	// �q�b�g���������ꍇ�A��_���v�Z
	if (closestDist >= 0.0f)
	{
		// ���[�J�����W�n�ł̌�_�����[���h���W�n�ɕϊ�
		D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * closestDist;
		D3DXVec3TransformCoord(&intersectionPoint, &localIntersectionPoint, &mtxWorld);
	}

	return intersectionPoint;
}

//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̍쐬
//--------------------------------
void CNavi::SetMarker(void)
{
	m_pMarker = CNaviMarker::Create(MARKER_TEXTURE_PATH, MARKER_SIZE);
}