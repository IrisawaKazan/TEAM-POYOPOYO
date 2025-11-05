//****************************************************************
//
// 計算の処理[math.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MATH_H_
#define _MATH_H_

// インクルード
#include "manager.h"

// クラスを定義
class CMath
{
public:
	// 静的メンバ関数
	static float randf(float Max, float Min);
	static D3DXVECTOR3 Get3Dto2DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 Get2Dto3DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 CalcMove(D3DXVECTOR3* Pos,float Speed);
	static D3DXVECTOR3 RandVector(D3DXVECTOR3 MaxDir, D3DXVECTOR3 MinDir);
	static float VectersFromAngle(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB, D3DXVECTOR3 Border);
	static float ConvertFloat(float Value, int Format = 2);
	static btQuaternion ConvertQuat(D3DXQUATERNION Set);
	static D3DXQUATERNION ConvertQuat(btQuaternion Set);

	static bool CalculateXZIntersection(D3DXVECTOR3& outIntersection, const D3DXVECTOR3& line1_pos, const D3DXVECTOR3& line1_dir, const D3DXVECTOR3& line2_pos, const D3DXVECTOR3& line2_dir); // sato Add
	static float GetDistanceToLineXZ(const D3DXVECTOR3& point, const D3DXVECTOR3& linePos, const D3DXVECTOR3& lineDir);                                                                        // sato Add
	static D3DXVECTOR3 GetNierToLineXZ(const D3DXVECTOR3& point, const D3DXVECTOR3& linePos, const D3DXVECTOR3& lineDir);                                                                      // sato Add
private:
	// コンストラクタ・デストラクタ
	CMath();
	~CMath();
};
#endif // !_MATH_H_