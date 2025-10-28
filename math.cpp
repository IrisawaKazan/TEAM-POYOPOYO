//****************************************************************
//
// �v�Z�̏���[math.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "math.h"
#include "math_T.h"

//***************************************
// ���������_�̃����_��
//***************************************
float CMath::randf(float Max, float Min)
{
	if (Max <= 0 || Min <= 0) return 0.0f;
	// �\�{���ď\���̈�ɂ���
	float Out = (rand() % (int)(Max * 10.0f) + (int)(Min * 10.0f)) * 0.1f;
	return Out;
}

//***************************************
// ���W��3D����2D�ɕϊ�����
//***************************************
D3DXVECTOR3 CMath::Get3Dto2DPosition(const D3DXVECTOR3 Position)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 LocalViewPort;
	D3DXMATRIX LocalProjMtx, LocalViewMtx, mtxWorld;

	// ���̃J�����̏����擾
	pDevice->GetViewport(&LocalViewPort);
	pDevice->GetTransform(D3DTS_PROJECTION, &LocalProjMtx);
	pDevice->GetTransform(D3DTS_VIEW, &LocalViewMtx);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 OutPos;
	// �QD�ɂ���
	D3DXVec3Project(&OutPos, &Position, &LocalViewPort, &LocalProjMtx, &LocalViewMtx, &mtxWorld);
	return OutPos;
}

//***************************************
// ���W��2D����3D�ɕϊ�����
//***************************************
D3DXVECTOR3 CMath::Get2Dto3DPosition(const D3DXVECTOR3 Position)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DVIEWPORT9 LocalViewPort;
	D3DXMATRIX LocalProjMtx, LocalViewMtx, mtxWorld;

	// ���̃J�����̏����擾
	pDevice->GetViewport(&LocalViewPort);
	pDevice->GetTransform(D3DTS_PROJECTION, &LocalProjMtx);
	pDevice->GetTransform(D3DTS_VIEW, &LocalViewMtx);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 OutPos;
	// 3D�ɂ���
	D3DXVec3Unproject(&OutPos, &Position, &LocalViewPort, &LocalProjMtx, &LocalViewMtx, &mtxWorld);
	return OutPos;
}

//***************************************
// �ړ��ʂ��v�Z
//***************************************
D3DXVECTOR3 CMath::CalcMove(D3DXVECTOR3* Pos,float Speed)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	D3DXVECTOR3 Move = VEC3_NULL;

	if (pInput->GetPress(DIK_W) == true)
	{
		if (pInput->GetPress(DIK_A) == true)
		{
			Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.75f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.75f)) * Speed;
		}
		else if (pInput->GetPress(DIK_D) == true)
		{
			Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * Speed;
		}
		else
		{
			Move.x = sinf(pCamera->GetRot().y + D3DX_PI) * Speed;
			Move.z = cosf(pCamera->GetRot().y + D3DX_PI) * Speed;
		}
	}
	else if (pInput->GetPress(DIK_S) == true)
	{
		if (pInput->GetPress(DIK_A) == true)
		{
			Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.25f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.25f)) * Speed;
		}
		else if (pInput->GetPress(DIK_D) == true)
		{
			Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * Speed;
			Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * Speed;
		}
		else
		{
			Move.x = sinf(pCamera->GetRot().y) * Speed;
			Move.z = cosf(pCamera->GetRot().y) * Speed;
		}
	}
	else if (pInput->GetPress(DIK_A) == true)
	{
		Move.x = sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Speed;
		Move.z = cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Speed;
	}
	else if (pInput->GetPress(DIK_D) == true)
	{
		Move.x = sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * Speed;
		Move.z = cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * Speed;
	}
	return *Pos += Move;
}

//***************************************
// �����_���ȃx�N�g�����Z�o
//***************************************
D3DXVECTOR3 CMath::RandVector(D3DXVECTOR3 MaxDir, D3DXVECTOR3 MinDir)
{
	D3DXVECTOR3 randVec;

	// �n�_ + ���� * ����
	randVec.x = MinDir.x + (float)(rand()) / RAND_MAX * (MaxDir.x - MinDir.x);
	randVec.y = MinDir.y + (float)(rand()) / RAND_MAX * (MaxDir.y - MinDir.y);
	randVec.z = MinDir.z + (float)(rand()) / RAND_MAX * (MaxDir.z - MinDir.z);

	// ���K�����ĕ����x�N�g���ɂ���
	D3DXVec3Normalize(&randVec, &randVec);

	return randVec;
}

//***************************************
// �x�N�g���̂Ȃ��p�����߂�
//***************************************
float CMath::VectersFromAngle(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB,D3DXVECTOR3 Border)
{
	D3DXVec3Normalize(&VecA, &VecA);
	D3DXVec3Normalize(&VecB, &VecB);
	float Dot = D3DXVec3Dot(&VecA, &Border);
	Dot = Dot / fabsf(Dot);
	return (float)acos(D3DXVec3Dot(&VecA, &VecB)) * Dot;
}

//***************************************
// �����_����ʂ܂łɕϊ�
//***************************************
float CMath::ConvertFloat(float Value,int Format)
{
	int Convert;
	float Out;
	Convert = (int)(Value * powf(10.0f, (float)Format));
	Out = Convert / powf(10.0f, (float)Format);
	return Out;
}

//***************************************
// �o���b�g�p�̃N�H�[�^�j�I���ɕϊ�
//***************************************
btQuaternion CMath::ConvertQuat(D3DXQUATERNION Set)
{
	return btQuaternion(Set.x, Set.y, Set.z, Set.w);
}

//***************************************
// DirectX�p�̃N�H�[�^�j�I��
//***************************************
D3DXQUATERNION CMath::ConvertQuat(btQuaternion Set)
{
	return D3DXQUATERNION(Set.x(), Set.y(), Set.z(), Set.w());
}

////***************************************
//// �����Ɛ����̍Őړ_����
////***************************************
//float CMath::CalcSegSegDist(CCollision::Segment Seg1, CCollision::Segment Seg2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2)
//{
//	// �x�N�g��������
//	D3DXVECTOR3 Vec1 = Seg1.End - Seg1.Start;
//	D3DXVECTOR3 Vec2 = Seg2.End - Seg2.Start;
//	D3DXVECTOR3 toVec = Seg1.Start - Seg2.Start;
//	// ������}��
//	float Length1 = D3DXVec3Dot(&Vec1, &Vec1);
//	float Length2 = D3DXVec3Dot(&Vec2, &Vec2);
//	float toLength = D3DXVec3Dot(&Vec2, &toVec);
//	// �����_100�����̈ʒu�܂ōl������
//	float Epsilon = 1e-6f;
//
//	if (Length1 <= Epsilon && Length2 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = 0.0f;
//		*C1 = Seg1.Start;
//		*C2 = Seg2.Start;
//		D3DXVECTOR3 VecC = *C1 - *C2;
//		return D3DXVec3Dot(&VecC,&VecC);
//	}
//	if (Length1 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = toLength / Length2;
//		*T = Clamp(*T, 0.0f, 1.0f);
//	}
//	else
//	{
//		float C = D3DXVec3Dot(&Vec1, &toVec);
//		if (Length2 <= Epsilon)
//		{
//			*T = 0.0f;
//			*S = Clamp(-C / Length1, 0.0f, 1.0f);
//		}
//		else
//		{
//			float B = D3DXVec3Dot(&Vec1, &Vec2);
//			float Ttom = B * (*S) + toLength;
//			if (Ttom < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (Ttom > Length2)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//			else
//			{
//				*T = Ttom / Length2;
//			}
//			float Denom = (Length1 * Length2) - (B * B);
//			if (Denom != 0.0f)
//			{
//				*S = Clamp((B * toLength) / Denom, 0.0f, 1.0f);
//			}
//			else
//			{
//				*S = 0.0f;
//			}
//			*T = (B * (*S) + toLength) / Length2;
//			if (*T < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (*T > 1.0f)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//		}
//	}
//	*C1 = Seg1.Start + Vec1 * (*S);
//	*C2 = Seg2.Start + Vec2 * (*T);
//	D3DXVECTOR3 VecC = *C1 - *C2;
//	return D3DXVec3Dot(&VecC,&VecC);
//}
//
/////
///// <<�J�v�Z�����m�̍Őړ_�������v�Z>>
///// <�����>
///// �H������
///// <param name="Cap1"><�J�v�Z���P>
///// <param name="Cap2"><�J�v�Z���Q>
///// <param name="S"><�Őړ_�P�̃X�J���[�l(�o�͗p)>
///// <param name="T"><�Őړ_�Q�̃X�J���[�l(�o�͗p)>
///// <param name="C1"><�Őړ_�P(�o�͗p)>
///// <param name="C2"><�Őړ_�Q(�o�͗p)>
///// <returns><��̃J�v�Z���̍Őړ_���������^�[��>
///// 
//float CMath::CalcCapCapDist(CCollision::Capsule Cap1, CCollision::Capsule Cap2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2)
//{
//	// �J�v�Z���̏I�_����n�_�܂ł̃x�N�g��
//	D3DXVECTOR3 Vec1 = Cap1.End - Cap1.Start;
//	D3DXVECTOR3 Vec2 = Cap2.End - Cap2.Start;
//	// �J�v�Z�������������̃J�v�Z���܂ł̃x�N�g��
//	D3DXVECTOR3 toVec = Cap1.Start - Cap2.Start;
//	// ���ꂼ��̃x�N�g���̂Ȃ���
//	float Length1 = D3DXVec3Dot(&Vec1, &Vec1);
//	float Length2 = D3DXVec3Dot(&Vec2, &Vec2);
//	float toLength = D3DXVec3Dot(&Vec2, &toVec);
//	// �덷���Ȃ������߂̈ꎞ�ϐ�(100������1�܂ōl��)
//	float Epsilon = 1e-6f;
//	// �����̃J�v�Z�����_��������
//	if (Length1 <= Epsilon && Length2 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = 0.0f;
//		*C1 = Cap1.Start;
//		*C2 = Cap2.Start;
//		D3DXVECTOR3 VecC = *C1 - *C2;
//		return D3DXVec3Dot(&VecC, &VecC);
//	}
//	// �Е��̃J�v�Z���P���_��������
//	if (Length1 <= Epsilon)
//	{
//		*S = 0.0f;
//		*T = toLength / Length2;
//		*T = Clamp(*T, 0.0f, 1.0f);
//	}
//	// ����ȊO
//	else
//	{
//		float C = D3DXVec3Dot(&Vec1, &toVec);
//		// �J�v�Z���Q���_��������
//		if (Length2 <= Epsilon)
//		{
//			*T = 0.0f;
//			*S = Clamp(-C / Length1, 0.0f, 1.0f);
//		}
//		// ����ȊO
//		else
//		{
//			float B = D3DXVec3Dot(&Vec1, &Vec2);
//			float Ttom = B * (*S) + toLength;
//			if (Ttom < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (Ttom > Length2)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//			else
//			{
//				*T = Ttom / Length2;
//			}
//			float Denom = (Length1 * Length2) - (B * B);
//			if (Denom != 0.0f)
//			{
//				*S = Clamp((B * toLength) / Denom, 0.0f, 1.0f);
//			}
//			else
//			{
//				*S = 0.0f;
//			}
//			*T = (B * (*S) + toLength) / Length2;
//			if (*T < 0.0f)
//			{
//				*T = 0.0f;
//				*S = Clamp(-C / Length1, 0.0f, 1.0f);
//			}
//			else if (*T > 1.0f)
//			{
//				*T = 1.0f;
//				*S = Clamp((B - C) / Length1, 0.0f, 1.0f);
//			}
//		}
//	}
//	*C1 = Cap1.Start + Vec1 * (*S);
//	*C2 = Cap2.Start + Vec2 * (*T);
//	D3DXVECTOR3 VecC = *C1 - *C2;
//	return D3DXVec3Dot(&VecC, &VecC);
//}

//***************************************
// �f�X�g���N�^
//***************************************
CMath::~CMath()
{

}