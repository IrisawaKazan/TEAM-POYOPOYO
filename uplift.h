#ifndef _UPLIFT_H_
#define _UPLIFT_H_

#include "objectX.h"

class CUpLift : public CObjectX
{
public:
    CUpLift();
    ~CUpLift();

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CUpLift* Create(D3DXVECTOR3 Pos,D3DXVECTOR3 Velocity,float Limit);
private:
    float m_Limit;
    D3DXVECTOR3 m_Velocity;
    std::unique_ptr<btCollisionShape> m_CollisionShape;
    std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー
};
#endif // !_UPLIFT_H_