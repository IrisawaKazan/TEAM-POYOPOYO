//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "input.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNavi* CNavi::Create(void)
{
	CNavi* pNavi = new CNavi;
	if (pNavi == NULL)
	{
		return NULL;
	}
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return NULL;
	}
	return pNavi;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNavi::Init(void)
{
	CObjectBillBoard::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNavi::Uninit(void)
{
	CObjectBillBoard::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNavi::Update(void)
{
	CreateRay(CManager::GetInputMouse()->GetPos());
	CObjectBillBoard::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNavi::Draw(void)
{
	CObjectBillBoard::Draw();
}

//--------------------------------
// ���C���쐬
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{

}