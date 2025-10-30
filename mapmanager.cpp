//****************************************************************
//
// �}�b�v�}�l�[�W���[�̏���[mapmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "mapmanager.h"
#include "block.h"
#include "modelmanager.h"
#include "switch.h"
#include "scene.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "playermanager.h"

// �l�[���X�y�[�X
using namespace nlohmann;
using namespace std;

//***************************************
// �f�X�g���N�^
//***************************************
CMapManager::~CMapManager()
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
	// �N���A
	m_vMapObject.clear();
	m_vDoor.clear();
	m_vMapSwitch.clear();
}

//***************************************
// ����������
//***************************************
HRESULT CMapManager::Init(void)
{
	// �x�N�^�[���N���A
	m_vMapObject.clear();
	m_vDoor.clear();
	m_vMapSwitch.clear();

	// ����I��
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CMapManager::Uninit(void)
{
}

//***************************************
// �X�V����
//***************************************
void CMapManager::Update(void)
{
	// ���̃X�C�b�`��������Ă��邩
	int nFinished = 0;

	// �X�C�b�`�ɃA�N�Z�X
	for (auto Switchs = m_vMapSwitch.begin(); Switchs != m_vMapSwitch.end(); Switchs++)
	{
		// ������Ă�����
		if ((*Switchs)->IsPress() == true)
		{
			// �C���N�������g
			nFinished++;
		}
	}

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
			const bool Condition = (objA == (*Players)->GetRB() && objB == m_RigitBody.get()) || (objA == m_RigitBody.get() && objB == (*Players)->GetRB());

			// �؂�グ
			if (Condition == false) continue;

			if (CFade::GetFadeSingle()->GetFade() != CFade::FADE_OUT)
			{
				CFade::SetFade(new CResult);
			}

			// ������؂�グ��
			break;
		}
	}

	// ���ׂĉ�����Ă�����
	if (nFinished >= (int)m_vMapSwitch.size())
	{
		// �N��
		m_vDoor[0]->Begin();
	}
	else
	{
		// �V���b�g�_�E��
		m_vDoor[0]->End();
	}
}

//***************************************
// �`�揈��
//***************************************
void CMapManager::Draw(void)
{
}

//***************************************
// �I�u�W�F�N�g�𐶐�
//***************************************
void CMapManager::CreateObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	// �����A�v�f�ɒǉ�
	CBlock* LocalObject = NULL;
	LocalObject = CBlock::Create(Path, Pos, Rot);
	m_vMapObject.push_back(LocalObject);
}

//***************************************
// �ǂݍ���
//***************************************
void CMapManager::Load(std::string Path)
{
	// json�t�@�C�����J��
	ifstream ifs(Path);

	if (!ifs)
	{
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł���", "�I�����b�Z�[�W", MB_OK);
		return;
	}
	if (ifs.is_open() == false)
	{
		MessageBox(NULL, "�t�@�C�����ǂݍ��߂܂���ł���", "�I�����b�Z�[�W", MB_OK);
		return;
	}

	// �N���A
	m_vMapObject.clear();
	m_vDoor.clear();
	m_vMapSwitch.clear();

	// json�f�[�^��錾
	ordered_json jsonData;
	// json��ǂݍ���
	ifs >> jsonData;

	// SetStage�z��̒��ɂ���f�[�^�������݂��Ȃ�������I���
	for (const auto& obj : jsonData)
	{
		bool Static;
		float Mass;
		D3DXVECTOR3 Pos, Scale;
		D3DXQUATERNION Quad;
		std::string LocalPath;

		// IsStatic�^�O�̃f�[�^�����o��
		Static = obj["RigitBody"]["IsStatic"];
		// Mass�^�O�̃f�[�^�����o��
		Mass = obj["RigitBody"]["Mass"];

		// Path�^�O�̃f�[�^�����o��
		LocalPath = obj["Assets"]["Path"];

		// Pos�^�O�̃f�[�^�����o��
		Pos.x = obj["Transform"]["Pos"]["x"];
		Pos.y = obj["Transform"]["Pos"]["y"];
		Pos.z = obj["Transform"]["Pos"]["z"];

		// Rot�^�O�̃f�[�^�����o��
		Quad.w = obj["Transform"]["Rot"]["w"];
		Quad.x = obj["Transform"]["Rot"]["x"];
		Quad.y = obj["Transform"]["Rot"]["y"];
		Quad.z = obj["Transform"]["Rot"]["z"];

		// Scale�^�O�̃f�[�^�����o��
		Scale.x = obj["Transform"]["Scale"]["x"];
		Scale.y = obj["Transform"]["Scale"]["y"];
		Scale.z = obj["Transform"]["Scale"]["z"];

		if (LocalPath.find("Switch-Button") != string::npos)
		{
			// �����A�v�f�ɒǉ�
			CSwitch* LocalObject = NULL;
			LocalObject = CSwitch::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// �A��
			m_vMapSwitch.push_back(LocalObject);
		}
		else if (LocalPath.find("Door") != string::npos)
		{
			// �����A�v�f�ɒǉ�
			CDoor* LocalObject = NULL;
			LocalObject = CDoor::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// �A��
			m_vDoor.push_back(LocalObject);

			// �������m��OBB�̑傫����ݒ�
			m_CollisionShape = std::make_unique<btBoxShape>(btVector3(LocalObject->GetSize().x, LocalObject->GetSize().y, LocalObject->GetSize().z));

			// ����
			btScalar mass = 0.0f;

			// �������[�����g�̌v�Z�p�ϐ�
			btVector3 inertia(0, 0, 0);

			// �v�Z
			m_CollisionShape->calculateLocalInertia(mass, inertia);

			// �ŏI�I�Ȍv�Z���ʁA�ʒu�A�ʒu����̃I�t�Z�b�g
			btTransform transform;

			// ������
			transform.setIdentity();

			// ����
			transform.setOrigin(btVector3(Pos.x, Pos.y + LocalObject->GetSize().y * 0.5f, Pos.z));

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
		else
		{
			// �����A�v�f�ɒǉ�
			CBlock* LocalObject = NULL;
			LocalObject = CBlock::Create(LocalPath, Pos, VEC3_NULL);
			LocalObject->SetScale(Scale);
			LocalObject->SetQuat(CMath::ConvertQuat(Quad));
			LocalObject->SetIdx(LocalPath);
			// �A��
			m_vMapObject.push_back(LocalObject);
		}
	}
}