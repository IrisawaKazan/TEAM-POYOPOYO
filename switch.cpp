//=================================================
//
//	switch.cpp
//
// Author:chikada shouya
//
//=================================================

//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "switch.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"
#include "playermanager.h"

//***************************************
// �R���X�g���N�^
//***************************************
CSwitch::CSwitch(int nPriority) :CBlock(nPriority)
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CSwitch::~CSwitch()
{
}

//***************************************
// �X�C�b�`����
//***************************************
CSwitch* CSwitch::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	// �C���X�^���X��錾
	CSwitch* pSwitch = nullptr;

	// ���������m��
	pSwitch = new CSwitch;

	// �m�ۂł�����
	if (pSwitch != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pSwitch->SetFilepath(sName);
		pSwitch->SetPosition(pos);
		pSwitch->SetRotasion(rot);
		pSwitch->SetScale(Scale);
		pSwitch->Init();

		// �Ԃ�
		return pSwitch;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//�@�X�C�b�`����������
//***************************************
HRESULT CSwitch::Init(void)
{
	// ������
	CBlock::Init();

	// �����ʒu���L��
	m_ApperPos = GetPosition();

	return S_OK;
}

//***************************************
//�@�X�C�b�`�I������
//***************************************
void CSwitch::Uninit(void)
{
	// �j��
	CBlock::Uninit();
}

//***************************************
//�@�X�C�b�`�X�V����
//***************************************
void CSwitch::Update(void)
{
	// �����Ă��Ȃ�����ɂ���
	m_IsPressed = false;

	// ���̈ʒu���擾
	D3DXVECTOR3 CurrentPos = GetPosition();

	// �����蔻��
	CollisionPlayer();

	// ������Ă�����
	if (m_IsPressed == true)
	{
		// ���ݐ؂��Ă��Ȃ�������
		if (m_ApperPos.y + Config::Depth < CurrentPos.y - Config::PressSpeed)
		{
			// ���߂�
			CurrentPos.y -= Config::PressSpeed;
		}
	}
	else
	{
		// �オ�肫���Ă��Ȃ�������
		if (m_ApperPos.y > CurrentPos.y + Config::ReturnSpeed)
		{
			// ������
			CurrentPos.y += Config::ReturnSpeed;
		}
	}
	// �ʒu��ݒ�
	SetPosition(CurrentPos);

	// �X�V
	CBlock::Update();
}

//***************************************
//�@�X�C�b�`�`�揈��
//***************************************
void CSwitch::Draw(void)
{
	// �`��
	CBlock::Draw();
}

//***************************************
// �v���C���[�Ƃ̓����蔻��
//***************************************
void CSwitch::CollisionPlayer(void)
{
	// ���g���Փ˂��Ă��邩
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// �Փˉ񐔕��J��Ԃ�
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// �v���C���[�̔z��ɃA�N�Z�X
		for (auto Players = CGame::GetPlayerManager()->GetVPlayer().begin(); Players != CGame::GetPlayerManager()->GetVPlayer().end(); Players++)
		{
			// �Փ˂��Ă���y�A���擾
			btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

			// �Փ˂��Ă�����
			if (manifold->getNumContacts() <= 0) continue;

			// �Փ˃I�u�W�F�N�g�P�A�Q���擾
			const btCollisionObject* objA = manifold->getBody0();
			const btCollisionObject* objB = manifold->getBody1();

			// �v���C���[�ƃX�C�b�`���������Ă�����
			const bool Condition = (objA == (*Players)->GetRB() && objB == GetRB()) || (objA == GetRB() && objB == (*Players)->GetRB());

			// �؂�グ
			if (Condition == false) continue;

			// �����ꂽ��Ԃɂ���
			m_IsPressed = true;

			// ������؂�グ��
			break;
		}
		// ������Ă����珟����؂�グ��
		if (m_IsPressed == true) break;
	}
}


//***************************************
// �h�A
//***************************************
//***************************************
// �R���X�g���N�^
//***************************************
CDoor::CDoor(int nPriority) : CBlock(nPriority)
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CDoor::~CDoor()
{
}

//***************************************
// ����������
//***************************************
HRESULT CDoor::Init(void)
{
	// ������
	CBlock::Init();

	// �����ʒu��ۑ�
	m_ApperPos = GetPosition();

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CDoor::Uninit(void)
{
	// �j��
	CBlock::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CDoor::Update(void)
{
	// �ʒu���擾
	D3DXVECTOR3 Pos = GetPosition();

	// �N�����Ă�����
	if (m_IsFunc)
	{
		// �h�A���オ�肫���Ă��Ȃ�������
		if (m_ApperPos.y + Config::Limit > Pos.y + Config::UpSpeed)
		{
			// ������
			Pos.y += Config::UpSpeed;
		}
	}
	// �N�����Ă��Ȃ�������
	else
	{
		// �h�A�������肫���Ă��Ȃ�������
		if (m_ApperPos.y < Pos.y - Config::DownSpeed)
		{
			// ������
			Pos.y -= Config::DownSpeed;
		}
	}

	// �ʒu��ݒ�
	SetPosition(Pos);

	// �X�V
	CBlock::Update();
}

//***************************************
// �`�揈��
//***************************************
void CDoor::Draw(void)
{
	// �`��
	CBlock::Draw();
}

//***************************************
// ����
//***************************************
CDoor* CDoor::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	// �C���X�^���X��錾
	CDoor* pDoor = nullptr;

	// �������m��
	pDoor = new CDoor;

	// �m�ۂł�����
	if (pDoor != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pDoor->SetFilepath(sName);
		pDoor->SetPosition(pos);
		pDoor->SetRotasion(rot);
		pDoor->SetScale(Scale);
		pDoor->Init();

		// �������m��
		return pDoor;
	}
	else
	{
		return nullptr;
	}
}