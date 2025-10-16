//****************************************************************
//
// リフトの処理[uplift.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _UPLIFT_H_
#define _UPLIFT_H_

// インクルード
#include "objectX.h"

// 上昇リフト
class CUpLift : public CObjectX
{
public:
    // コンストラクタ・デストラクタ
    CUpLift();
    ~CUpLift();

    // メンバ関数
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    // 静的メンバ関数
    static CUpLift* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Velocity,float Limit);
private:
    // メンバ変数
    float m_Limit;                                      // どのくらいで往復するか
    bool m_Switch;                                      // 往復用のフラグ
    D3DXVECTOR3 m_Velocity;                             // 移動方向
    D3DXVECTOR3 m_ApperPos;                             // 初期位置
    std::unique_ptr<btCollisionShape> m_CollisionShape; // 当たり判定の形状
    std::unique_ptr<btRigidBody> m_RigitBody;		    // リジットボディー
};
#endif // !_UPLIFT_H_