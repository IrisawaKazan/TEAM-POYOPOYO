//****************************************************************
//
// �}�b�v�}�l�[�W���[�̏���[mapmanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

// �O���錾
class CBlock;
class CSwitch;

// �N���X���`
class CMapManager
{
public:
	// �f�X�g���N�^
	~CMapManager() { m_vMapObject.clear(); }

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ����
	void CreateObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path);

	// �ǂݍ���
	void Load(std::string Path);

	// �ÓI�����o�֐�
	// �V���O���g��
	static CMapManager* Instance(void) {
		static std::unique_ptr<CMapManager> Instance(new CMapManager);
		return Instance.get();
	};
private:
	// �R���X�g���N�^
	CMapManager() { Init(); }
	// �����o�ϐ�
	std::vector<CBlock*> m_vMapObject;			// �}�b�v�ɒu���I�u�W�F�N�g
	std::vector<CSwitch*> m_vMapSwitch;			// �}�b�v�ɒu���X�C�b�`
};

#endif // !_MAPMANAGER_H_