//************************************************************
//
//�@���U���g�̏���[rezult.cpp]
//�@Author:Hatori Kensaku
//
//************************************************************
//  �C���N���[�h
#include "result.h"
#include "object2D.h"

//  �R���X�g���N�^
CResult::CResult()  :CScene(MODE_RESULT)
{

}

//  �f�X�g���N�^
CResult::~CResult()
{

}

//  ������
HRESULT CResult::Init(void)
{
    CObject2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f), VEC3_NULL);

    return E_NOTIMPL;
}

//  �X�V
void CResult::Update(void)
{

}

//  �I��
void CResult::Uninit(void)
{
    delete this;
}

//  �`��
void CResult::Draw(void)
{

}
