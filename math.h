//****************************************************************
//
// �v�Z�̏���[math.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _MATH_H_
#define _MATH_H_

// �C���N���[�h
#include "manager.h"

// �N���X���`
class CMath
{
public:
	// �ÓI�����o�֐�
	static float randf(float Max, float Min);
	static D3DXVECTOR3 Get3Dto2DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 Get2Dto3DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 CalcMove(D3DXVECTOR3* Pos,float Speed);
	static D3DXVECTOR3 RandVector(D3DXVECTOR3 MaxDir, D3DXVECTOR3 MinDir);
	static float VectersFromAngle(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB, D3DXVECTOR3 Border);
	static float ConvertFloat(float Value, int Format = 2);
	static btQuaternion ConvertQuat(D3DXQUATERNION Set);
	static D3DXQUATERNION ConvertQuat(btQuaternion Set);
private:
	// �R���X�g���N�^�E�f�X�g���N�^
	CMath();
	~CMath();
};
#endif // !_MATH_H_