//************************************************************
//
//　プレイヤーの処理[player.cpp]
//　Author:chikada shouya
//
//************************************************************
#include "player.h"
#include "math_T.h"
#include "game.h"
#include "navi.h"
#include "arrow.h"

// 静的メンバ変数の定義

// コンストラクタ
CPlayer::CPlayer()
{

}

// デストラクタ
CPlayer::~CPlayer()
{

}

// 初期化
HRESULT CPlayer::Init(void)
{
	CModelCharacter::Init("data\\Model\\MOTION\\player003.txt",5.0f);

	m_CollisionShape = std::make_unique<btCapsuleShape>(btScalar(CAPSULE_RADIUS), btScalar(CAPSULE_HEIGHT));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPos().x, GetPos().y + CAPSULE_HEIGHT, GetPos().z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	m_RigitBody = std::make_unique<btRigidBody>(info);
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	m_RigitBody->setUserPointer(this);
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	m_RigitBody->setFriction(btScalar(0.02f));

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	// ナビゲーションオブジェクトの接触回数管理用マップの初期化 sato Add
	m_naviObjectCountMap = std::unordered_map<size_t, unsigned short>();
	m_naviObjectIdxListOld = std::vector<size_t>();

	m_state = STATE::Normal;              // 通常
	m_activePos = { 0.0f,-1000.0f,0.0f }; // 初期値
	m_turnAngle = 0.0f;                   // 初期値

	m_isGrounded = false;
	return S_OK;
}

// 破棄
void CPlayer::Uninit(void)
{
	// 剛体の削除
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
	}

	// 衝突形状の削除
	m_CollisionShape.reset();

	CModelCharacter::Uninit();
}

// 更新
void CPlayer::Update(void)
{
	UpdateGroundedState(); // 着地更新

	// ナビゲーションオブジェクトに触れる処理 sato Add
	std::vector<size_t> naviObjectIdxListNew{}; // 今フレームで触れたナビゲーションオブジェクトのインデックスリスト
	std::vector<CNaviObject*> pObjects = CNavi::GetInstance()->GetObjects(); // ナビゲーションオブジェクトを取得
	for (const CNaviObject* pObject : pObjects)
	{
		// オブジェクトに触れているか
		D3DXVECTOR3 pos{};
		float angle{};
		size_t idx = 0;
		CNavi::TYPE naviType = pObject->ActivateTrigger(m_RigitBody.get(), &pos, &angle, &idx);

		// 触れたナビゲーションタイプによって処理を分岐
		switch (naviType)
		{
		case CNavi::TYPE::Arrow:
		{
			// Playerの位置と方向
			D3DXVECTOR3 myPos = GetPos();

			// 矢印の位置と方向
			D3DXVECTOR3 objectPos = pos;                                         // 矢印の中心座標
			D3DXVECTOR3 objectDir = D3DXVECTOR3(sinf(angle), 0.0f, cosf(angle)); // 矢印の向き

			m_activePos = CMath::GetNierToLineXZ(myPos, objectPos, objectDir); // 矢印のベクトル上の近い地点
			m_turnAngle = angle;   // ターン方向
			m_state = STATE::Turn; // ターン開始
			break;
		}
		case CNavi::TYPE::Climb:
			m_state = STATE::Climb; // クライム開始
			break;
		case CNavi::TYPE::Jump:
			// Playerの位置と方向
			D3DXVECTOR3 myPos = GetPos();

			D3DXVECTOR3 objectPos = pos; // 矢印の中心座標

			// 進む方向に対して垂直なベクトル
			float myAngle = GetRot().y;
			D3DXVECTOR3 verticalDir = D3DXVECTOR3(sinf(myAngle - D3DXToRadian(90.0f)), 0.0f, cosf(myAngle - D3DXToRadian(90.0f))); // 矢印の向き

			m_activePos = CMath::GetNierToLineXZ(myPos, objectPos, verticalDir); // 矢印のベクトル上の近い地点

			m_state = STATE::Jump;  // ジャンプ開始
			break;
		}

		if (naviType != CNavi::TYPE::None && naviType != CNavi::TYPE::Max)
		{
			// 初めての接触の場合はマップに登録
			m_naviObjectCountMap.try_emplace(idx, short(0));

			// 前のフレームで触れていなかった場合は接触回数をカウントアップ
			if (std::find(m_naviObjectIdxListOld.begin(), m_naviObjectIdxListOld.end(), idx) == m_naviObjectIdxListOld.end())
			{
				m_naviObjectCountMap[idx]++;
			}

			// 5回以上になったら死んでしまう
			if (m_naviObjectCountMap[idx] >= 5)
			{
				CGame::GetPlayerManager()->DethMessage(this);
				Uninit();
				Release();
				return;
			}
			naviObjectIdxListNew.push_back(idx); // リストに追加
		}
	}
	m_naviObjectIdxListOld = naviObjectIdxListNew; // 今フレームで触れたナビゲーションオブジェクトリストを保存

	if (m_RigitBody == nullptr) return;

	btVector3 moveDir(0, 0, 0); // 移動

	// 移動処理
	D3DXVECTOR3 rot = GetRot();
	moveDir.setX(-sinf(rot.y) * MOVE_SPEED);
	moveDir.setZ(-cosf(rot.y) * MOVE_SPEED);
	moveDir.setY(m_RigitBody->getLinearVelocity().y());

	// 状態処理
	switch (m_state)
	{
		// 通常
	case CPlayer::STATE::Normal:
		break;
		// ターン
	case CPlayer::STATE::Turn:
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 space = m_activePos - pos;
		if (D3DXVec3Length(&space) <= TURN_RADIUS)
		{// ターン位置に来たら
			Turn();
		}
		else
		{
			D3DXVec3Normalize(&space, &space);
			D3DXVECTOR3 movevec = D3DXVECTOR3(moveDir.normalized());
			if (D3DXVec3Dot(&space, &movevec) <= 0.0f)
			{// もう後ろの場合ターン
				Turn();
			}
		}
		break;
	}
		// 登る
	case CPlayer::STATE::Climb:
		break;
		// 跳ぶ
	case CPlayer::STATE::Jump:
	{
		// 真ん中に行ってから
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 space = m_activePos - pos;
		D3DXVec3Normalize(&space, &space);
		D3DXVECTOR3 movevec = D3DXVECTOR3(moveDir.normalized());
		if (D3DXVec3Dot(&space, &movevec) <= 0.0f)
		{// もう後ろの場合ジャンプ
			Jump(moveDir);
		}
		break;
	}
	case CPlayer::STATE::Jumping:
		// 飛んでいる
		moveDir.setX(moveDir.x() * JUMP_SPEED_INA);
		moveDir.setZ(moveDir.z() * JUMP_SPEED_INA);
		if (m_isGrounded)
		{// 着地したら
			m_state = STATE::Normal; // 通常に戻す
		}
		break;
	}

	// 物理に移動値を渡す
	m_RigitBody->setLinearVelocity(moveDir);

	btVector3 newPos;           // 位置
	btTransform trans;          // トランスフォーム

	// 物理の位置をモデルの位置に渡す
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	newPos = trans.getOrigin();
	SetPos(D3DXVECTOR3(newPos.x(), newPos.y(), newPos.z()));

	// モデルの更新
	CModelCharacter::Update();
}

// 描画
void CPlayer::Draw(void)
{
	CModelCharacter::Draw();
}

// 位置を設定
void CPlayer::SetPos(D3DXVECTOR3 Pos)
{
	btVector3 newPos;

	btTransform trans;
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3(Pos.x, Pos.y, Pos.z));
	m_RigitBody->setWorldTransform(trans);
	m_RigitBody->getMotionState()->setWorldTransform(trans);
	CModelCharacter::SetPos(D3DXVECTOR3(Pos.x, Pos.y - 20.0f, Pos.z));
}

// 生成
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->SetRot(rot);
		pPlayer->SetRotDest(rot);
		pPlayer->Init();
		pPlayer->SetPos(pos);
		return pPlayer;
	}
	else
	{
		return nullptr;
	}
}

// 着地確認
void CPlayer::UpdateGroundedState()
{
	if (m_RigitBody == nullptr)
	{
		m_isGrounded = false;
		return;
	}

	// レイの開始点
	btVector3 rayFrom = m_RigitBody->getWorldTransform().getOrigin(); // プレイヤーの中心

	// カプセルの高さの半分 + 坂道や段差用のマージン
	const float rayLength = CAPSULE_HEIGHT + GROUND_SPACE;
	btVector3 rayTo = rayFrom + btVector3(0, -rayLength, 0);

	// レイキャストのコールバック
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	// レイキャスト
	CManager::GetDynamicsWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{// レイが何かに当たった
		if (rayCallback.m_collisionObject == m_RigitBody.get())
		{// 自分自身
			m_isGrounded = false;
		}
		else
		{// 自分以外
			m_isGrounded = true;
		}
	}
	else
	{// レイが当たっていない
		m_isGrounded = false;
	}
}

// ターン
void CPlayer::Turn()
{
	// ターン
	SetRotDest(D3DXVECTOR3(0.0f, m_turnAngle, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, m_turnAngle, 0.0f));

	// 元に戻す
	m_activePos = { 0.0f,-1000.0f,0.0f };
	m_turnAngle = 0.0f;
	m_state = STATE::Normal;
}

// クライム
void CPlayer::Climb()
{

}

// ジャンプ
void CPlayer::Jump(btVector3& moveDir)
{
	moveDir.setX(moveDir.x() * JUMP_SPEED_INA);
	moveDir.setZ(moveDir.z() * JUMP_SPEED_INA);
	moveDir.setY(moveDir.y() + JUMP_POWER);
	m_state = STATE::Jumping;
}