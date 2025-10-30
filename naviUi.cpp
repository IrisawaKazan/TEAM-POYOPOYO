//--------------------------------
//
// �i�r�Q�[�V����UI [naviUI.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviUi.h"
#include "object2D.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "navi.h"
#include "math_T.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNaviUI* CNaviUI::Create(const char* frameTexturePath, std::vector<const char*> objectTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �C���X�^���X�̐���
	CNaviUI* pNaviUI = new CNaviUI;
	if (pNaviUI == nullptr)
	{
		return nullptr;
	}

	pNaviUI->SetFrameTexturePath(frameTexturePath);
	pNaviUI->SetObjectTexturePaths(objectTexturePaths);
	pNaviUI->SetPos(pos);
	pNaviUI->SetSize(size);

	// ������
	if (FAILED(pNaviUI->Init()))
	{
		delete pNaviUI;
		return nullptr;
	}
	return pNaviUI;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviUI::Init(void)
{
	// �t���[���̃Z�b�g
	CTextureManager* textureManager = CTextureManager::Instance();
	m_pFrame = CObject2D::Create(m_pos, VEC3_NULL, m_size);
	m_pFrame->SetTexIndx(textureManager->Register(m_frameTexturePath));
	m_pFrame->SetAlphaTest(true);

	m_pObjects[0] = CObject2D::Create(m_pos, VEC3_NULL, m_size * 0.7f);
	m_pObjects[1] = CObject2D::Create(m_pos + D3DXVECTOR3(-m_size.x * 0.7f, m_size.y * 0.7f, 0.0f), VEC3_NULL, m_size * 0.2f);
	m_pObjects[2] = CObject2D::Create(m_pos + D3DXVECTOR3(m_size.x * 0.7f, m_size.y * 0.7f, 0.0f), VEC3_NULL, m_size * 0.2f);
	for (auto pObject : m_pObjects)
	{
		pObject->SetAlphaTest(true);
	}

	SetObjectUI(); // �I�u�W�F�N�gUI�̃Z�b�g

	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviUI::Uninit(void)
{

}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviUI::Update(void)
{
	SetObjectUI(); // �I�u�W�F�N�gUI�̃Z�b�g
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviUI::Draw(void)
{

}

//--------------------------------
// �I�u�W�F�N�gUI�̃Z�b�g
//--------------------------------
void CNaviUI::SetObjectUI(void)
{
	CTextureManager* textureManager = CTextureManager::Instance();

	// ���̃I�u�W�F�N�g
	unsigned int listID = static_cast<int>(CNavi::GetInstance()->GetList());
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > listID)
	{// �e�N�X�`��������΃Z�b�g
		if (m_pObjects[0] != nullptr)
		{
			m_pObjects[0]->SetTexIndx(textureManager->Register(m_objectTexturePaths[listID]));
		}
	}

	// �O�̃I�u�W�F�N�g
	unsigned int lastListID = Wrap(int(listID - 1), int(0), int(CNavi::LIST::Max) - 1);
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > lastListID)
	{// �e�N�X�`��������΃Z�b�g
		if (m_pObjects[1] != nullptr)
		{
			m_pObjects[1]->SetTexIndx(textureManager->Register(m_objectTexturePaths[lastListID]));
		}
	}

	// ���̃I�u�W�F�N�g
	unsigned int nextListID = Wrap(int(listID + 1), int(0), int(CNavi::LIST::Max) - 1);
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > nextListID)
	{// �e�N�X�`��������΃Z�b�g
		if (m_pObjects[2] != nullptr)
		{
			m_pObjects[2]->SetTexIndx(textureManager->Register(m_objectTexturePaths[nextListID]));
		}
	}
}