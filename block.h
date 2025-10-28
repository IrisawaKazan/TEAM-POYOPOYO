//=================================================
//
//	block.h
// 
// Author:chikada shouya
//
//=================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectx.h"

//�}�N��
#define HALF (0.5f)					// �����̒l

class CBlock :public CObjectX
{
public:
	//***************************
	// �u���b�N�̎�ނ̗񋓌^�錾
	//***************************
	typedef enum
	{
		TYPE_NONE = 0,	//
		TYPE_0,		//
		TYPE_1,		//
		TYPE_2,		//
		TYPE_3,		//
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 3);
	~CBlock();
	HRESULT Init(void);
	void InitRB(void);
	void Uninit(void);
	void Update(void);
	void UpdateRB(void);
	void Draw(void);

	static CBlock* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });

	// �Z�b�^�[
	void SetFilepath(std::string sName) { m_sNamePath = sName; };
	void SetQuat(btQuaternion Quad);
private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	TYPE m_type;			// �u���b�N�̃^�C�v
	D3DXVECTOR3 m_size;		// �T�C�Y
	D3DXVECTOR3 m_Scale = { 1.0f,1.0f,1.0f };			// �g�嗦
	D3DXVECTOR3 m_VtxMax;	// ���_�ő�l
	D3DXVECTOR3 m_VtxMin;	// ���_�ŏ��l
	D3DXVECTOR3 m_RBOffset;	// �I�t�Z�b�g
	D3DXQUATERNION m_Quad;	// �N�H�[�^�j�I��
	std::string m_sNamePath;	// �t�@�C���p�X
	int m_nIdx;				// �C���f�b�N�X
	std::unique_ptr<btCollisionShape> m_CollisionShape;	// �����蔻��̌`��
	std::unique_ptr<btRigidBody> m_RigitBody;			// ���W�b�g�{�f�B�[
};
#endif