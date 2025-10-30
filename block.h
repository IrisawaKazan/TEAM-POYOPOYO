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
	D3DXVECTOR3 GetSize(void) { return m_size; }
	btRigidBody* GetRB(void) { return m_RigitBody.get(); }

	static CBlock* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });

	// �Z�b�^�[
	void SetFilepath(std::string sName) { m_sNamePath = sName; };
	void SetQuat(btQuaternion Quad);
private:
	TYPE m_type;			// �u���b�N�̃^�C�v
	D3DXVECTOR3 m_size;		// �T�C�Y
	D3DXVECTOR3 m_RBOffset;	// �I�t�Z�b�g
	std::string m_sNamePath;// �t�@�C���p�X
	std::unique_ptr<btCollisionShape> m_CollisionShape;	// �����蔻��̌`��
	std::unique_ptr<btRigidBody> m_RigitBody;			// ���W�b�g�{�f�B�[
};
#endif