//--------------------------------
//
// �i�r�Q�[�V����UI [naviUI.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviUi.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "navi.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNaviUI* CNaviUI::Create(const char* filePath, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �C���X�^���X�̐���
	CNaviUI* pNaviUI = new CNaviUI;
	if (pNaviUI == nullptr)
	{
		return nullptr;
	}
	CTextureManager* pTexmanager = CTextureManager::Instance();
	pNaviUI->SetTexIndx(pTexmanager->Register(filePath));
	pNaviUI->SetSize(size);
	pNaviUI->SetPosition(pos);

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
	// �e�N���X�̏�����
	CObject2D::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviUI::Uninit(void)
{
	CObject2D::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviUI::Update(void)
{
	// �e�N���X�̍X�V
	CObject2D::Update();

	switch (CGame::GetNavi().GetDirection())
	{
	case CNavi::ARROW_DIRECTION::Left:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark002.png"));
		break;
	case CNavi::ARROW_DIRECTION::Front:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark001.png"));
		break;
	case CNavi::ARROW_DIRECTION::Back:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark000.png"));
		break;
	}
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviUI::Draw(void)
{
	CObject2D::Draw(); // �e�N���X�̕`��
}