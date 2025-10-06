//****************************************************************
//
// �|���S������[Object.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "Object.h"
#include "manager.h"

// �ÓI�����o�ϐ��錾
int CObject::m_nNumAll = 0;
CObject* CObject::m_pTop[Config::Priority] = {};
CObject* CObject::m_pCur[Config::Priority] = {};

//*********************************************
// �R���X�g���N�^
//*********************************************
CObject::CObject(int Priority)
{
	// ID�ƃI�u�W�F�N�g�̎�ނƕ`��̗D��x��ݒ�
	m_nID = 0;
	m_nPriority = Priority;
	m_bDeath = false;
	m_nNumAll++;

	CObject* pObjectCur = m_pCur[Priority];
	m_pCur[Priority] = this;
	m_pPrev = pObjectCur;
	if (m_pPrev != nullptr)
	{
		m_pPrev->m_pNext = this;
	}
	if (m_pTop[Priority] == nullptr)
	{
		m_pTop[Priority] = this;
	}
}

//*********************************************
// �f�X�g���N�^
//*********************************************
CObject::~CObject()
{

}

//*********************************************
// ���S�t���O�������Ă���z���D��x�P�ʂō폜
//*********************************************
void CObject::Delete(const int PriorityCount)
{
	// �����Ă���I�u�W�F�N�g�����̗D��x�̒��̈�Ԑ擪�ɐݒ肷��
	CObject* pObject = m_pTop[PriorityCount];

	// �Ō���܂ő�����
	while (pObject != nullptr)
	{
		// ������������Ȃ����玟�̂��ۑ�
		CObject* pObjectNext = pObject->GetNext();
		// ���S�t���O�����Ă�����
		if (pObject->m_bDeath == true)
		{
			// �f���[�g����O��ID�ƗD��x�����[�J���ϐ��ɕۑ�
			int Priority = pObject->m_nPriority;

			// �擪����폜
			if (pObject->m_pNext != nullptr && pObject->m_pPrev == nullptr)
			{
				// ��Ԑ擪�������̎��̂�ɂ���
				m_pTop[Priority] = pObject->m_pNext;
				// �����̎��̑O�̂�����ꂢ�ɂ���
				pObject->m_pNext->m_pPrev = nullptr;
			}
			// �Ō������폜
			else if (pObject->m_pNext == nullptr && pObject->m_pPrev != nullptr)
			{
				// ��Ԍ��������̑O�̂�ɂ���
				m_pCur[Priority] = pObject->m_pPrev;
				// �����̑O�̎��̂�����ꂢ�ɂ���
				pObject->m_pPrev->m_pNext = nullptr;
			}
			// �^�񒆂������ꍇ
			else if (pObject->m_pNext != nullptr && pObject->m_pPrev != nullptr)
			{
				// �������猩�đO�̂�̎����������猩�����̂�ɐݒ�
				pObject->m_pPrev->m_pNext = pObject->m_pNext;
				// �������猩�Ď��̂�̑O���������猩���O�̂�ɐݒ�
				pObject->m_pNext->m_pPrev = pObject->m_pPrev;
			}
			else
			{
				// ��Ԑ擪�����ꂢ�ɂ���
				m_pTop[Priority] = nullptr;
				// ��Ԍ������ꂢ�ɂ���
				m_pCur[Priority] = nullptr;
			}

			// nullptr����Ȃ������烊�X�g��������
			if (pObject != nullptr)
			{
				pObject->m_pNext = nullptr;
				pObject->m_pPrev = nullptr;
			}
			m_nNumAll--;
			// ����������̉��
			delete pObject;
			pObject = nullptr;
		}
		// �����Ă���I�u�W�F�N�g�����̃I�u�W�F�N�g�ɐݒ肷��
		pObject = pObjectNext;
	}
}

//*********************************************
// ���ׂĂ̔j��
//*********************************************
void CObject::ReleaseAll(void)
{
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Uninit();
			pObject = pObjectNext;
		}
	}
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		Delete(nCount);
	}
}

//*********************************************
// ���ׂĂ̍X�V
//*********************************************
void CObject::UpdateAll(void)
{
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Update();
			pObject = pObjectNext;
		}
	}
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		Delete(nCount);
	}
}

//*********************************************
// ���ׂĂ̕`��
//*********************************************
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();
	pCamera->SetCamera();

	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Draw();
			pObject = pObjectNext;
		}
	}
}

//*********************************************
// ���S�t���O�𗧂Ă�
//*********************************************
void CObject::Release(void)
{
	m_bDeath = true;
}