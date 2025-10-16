//****************************************************************
//
// ���t�g�̏���[uplift.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _UPLIFT_H_
#define _UPLIFT_H_

// �C���N���[�h
#include "objectX.h"

// �㏸���t�g
class CUpLift : public CObjectX
{
public:
    // �R���X�g���N�^�E�f�X�g���N�^
    CUpLift();
    ~CUpLift();

    // �����o�֐�
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    // �ÓI�����o�֐�
    static CUpLift* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Velocity,float Limit);
private:
    // �����o�ϐ�
    float m_Limit;                                      // �ǂ̂��炢�ŉ������邩
    bool m_Switch;                                      // �����p�̃t���O
    D3DXVECTOR3 m_Velocity;                             // �ړ�����
    D3DXVECTOR3 m_ApperPos;                             // �����ʒu
    std::unique_ptr<btCollisionShape> m_CollisionShape; // �����蔻��̌`��
    std::unique_ptr<btRigidBody> m_RigitBody;		    // ���W�b�g�{�f�B�[
};
#endif // !_UPLIFT_H_