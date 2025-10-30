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

// �l�[���X�y�[�X
using namespace nlohmann;
using namespace std;

//***************************************
// ����������
//***************************************
HRESULT CMapManager::Init(void)
{
	// �x�N�^�[���N���A
	m_vMapObject.clear();

	// ����I��
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CMapManager::Uninit(void)
{
	// ���u���Ă���I�u�W�F�N�g�̔j��
	for (auto MapObjects = m_vMapObject.begin(); MapObjects != m_vMapObject.end(); MapObjects++)
	{
		(*MapObjects)->Uninit();
		MapObjects = m_vMapObject.erase(MapObjects);
		if (MapObjects == m_vMapObject.end()) break;
	}

	// �N���A
	m_vMapObject.clear();
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

	// ���u���Ă���I�u�W�F�N�g�̔j��
	for (auto MapObjects = m_vMapObject.begin(); MapObjects != m_vMapObject.end(); MapObjects++)
	{
		(*MapObjects)->Uninit();
		MapObjects = m_vMapObject.erase(MapObjects);
		if (MapObjects == m_vMapObject.end()) break;
	}

	// �N���A
	m_vMapObject.clear();

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