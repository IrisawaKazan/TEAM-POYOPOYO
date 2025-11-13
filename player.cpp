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
#include "block.h"
#include "mapmanager.h"

// 静的メンバ変数の定義

// コンストラクタ
CPlayer::CPlayer() : m_activePos{ 0,0,0 }, m_CollisionShape{}, m_isGrounded{}, m_IsSlopeTrigger{}, m_naviObjectCountMap{}, m_naviObjectIdxListOld{}, m_pClimbBlock{}, m_RigitBody{}, m_state{}, m_turnAngle{}
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
	transform.setOrigin(btVector3(GetPos().x, GetPos().y + CAPSULE_HALF_HEIGHT, GetPos().z));

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

	m_pClimbBlock = nullptr;

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
	if (m_RigitBody == nullptr) return;

	// ナビゲーションチェック
	CheckNavigation();

	if (m_RigitBody == nullptr) return;

	// 着地チェック
	UpdateGroundedState();

	btVector3 moveDir(0, 0, 0); // 最終的に setLinearVelocity に渡す速度
	D3DXVECTOR3 rot = GetRot();
	btVector3 currentVel = m_RigitBody->getLinearVelocity(); // 現在の物理速度

	// 移動
	Move(moveDir, rot, currentVel);

	// 状態管理
	UpdateState(moveDir);

	if (m_state == STATE::Sliding)
	{
		if (m_IsSlopeTrigger == false)
		{
			// 物理に移動値を渡す
			m_RigitBody->applyCentralImpulse(moveDir);
			m_IsSlopeTrigger = true;
		}
	}
	else
	{
		// 物理に移動値を渡す
		m_RigitBody->setLinearVelocity(moveDir);
		m_IsSlopeTrigger = false;
	}

	// 物理の位置をモデルの位置に渡す
	btVector3 newPos;           // 位置
	btTransform trans;          // トランスフォーム
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

// ナビゲーション処理
void CPlayer::CheckNavigation()
{
	// ナビゲーションオブジェクトに触れる処理 sato Add
	std::vector<size_t> naviObjectIdxListNew{}; // 今フレームで触れたナビゲーションオブジェクトのインデックスリスト
	if (m_isGrounded && m_state == STATE::Normal)
	{// 地面にいるかつ通常状態
		std::vector<CNaviObject*> pObjects = CNavi::GetInstance()->GetObjects(); // ナビゲーションオブジェクトを取得
		for (const CNaviObject* pObject : pObjects)
		{
			// オブジェクトに触れているか
			D3DXVECTOR3 pos{};
			float angle{};
			size_t idx = 0;
			CNavi::TYPE naviType = pObject->ActivateTrigger(m_RigitBody.get(), &pos, &angle, &idx);

			// 前のフレームで触れていなかった場合は接触回数をカウントアップ
			if (std::find(m_naviObjectIdxListOld.begin(), m_naviObjectIdxListOld.end(), idx) == m_naviObjectIdxListOld.end())
			{
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
				{
					D3DXVECTOR3 myPos = GetPos(); // 自分の位置
					std::vector<CBlock*> pBlocks = CMapManager::Instance()->GetBlocks(); // ブロック配列

					float minLengthSq{ FLT_MAX };
					for (const auto& pBlock : pBlocks)
					{// ブロックを走査
						if (IsClimbingTarget(pBlock))
						{// 登れる
							D3DXVECTOR3 climbPos = pBlock->GetClosestPointOnSurface(myPos);
							D3DXVECTOR3 space = climbPos - myPos;
							float lengthSq = D3DXVec3LengthSq(&space);
							if (lengthSq < minLengthSq)
							{// より近いブロック
								m_pClimbBlock = pBlock;
								minLengthSq = lengthSq;
							}
						}
					}

					// ブロックに向かう
					FaceBlock();
					m_state = STATE::Climb; // クライム開始
					break;
				}
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
	}
	m_naviObjectIdxListOld = naviObjectIdxListNew; // 今フレームで触れたナビゲーションオブジェクトリストを保存
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
	const float rayLength = CAPSULE_HALF_HEIGHT + GROUND_SPACE;
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

			// 衝突したオブジェクトが坂道かチェック
			const CBlock* pSlope = CMapManager::Instance()->GetSlope();
			if (pSlope != nullptr && rayCallback.m_collisionObject == pSlope->GetRB())
			{// 坂道に着地した
				m_state = STATE::Sliding;
			}
			else
			{// 通常の地面
				if (m_state == STATE::Sliding)
				{
					m_state = STATE::Normal;
				}
			}
		}
	}
	else
	{// レイが当たっていない
		m_isGrounded = false;
	}
}

// 移動
void CPlayer::Move(btVector3& moveDir, D3DXVECTOR3& rot, btVector3& currentVel)
{
	// 移動処理
	if (m_state == STATE::Jumping || (!m_isGrounded && m_state != STATE::Climbing))
	{// 空中
		// 空中制御のための入力（目標）速度
		btVector3 inputVel(0, 0, 0);
		inputVel.setX(-sinf(rot.y) * MOVE_SPEED);
		inputVel.setZ(-cosf(rot.y) * MOVE_SPEED);

		// XZ速度を現在の速度から入力(inputVel)へ近づける
		// 慣性を維持と空中制御
		btVector3 blendedVel = currentVel.lerp(inputVel, AIR_CONTROL_FACTOR);

		moveDir.setX(blendedVel.x());
		moveDir.setZ(blendedVel.z());
	}
	else
	{
		moveDir.setX(-sinf(rot.y) * MOVE_SPEED);
		moveDir.setZ(-cosf(rot.y) * MOVE_SPEED);
	}

	if (m_isGrounded && m_state != STATE::Jumping && m_state != STATE::Climbing)
	{// 地面にいる
		// Y速度は0
		moveDir.setY(0.0f);
	}
	else
	{// 空中にいる
		// Y速度を維持
		moveDir.setY(m_RigitBody->getLinearVelocity().y());
	}
}

// 状態管理
void CPlayer::UpdateState(btVector3& moveDir)
{
	if (!m_isGrounded && m_state != STATE::Jumping && m_state != STATE::Climbing)
	{
		m_state = STATE::Jumping;
	}

	if (m_state != STATE::Sliding && (m_isGrounded && GetMotionInfo()->GetMotion() == 4 || m_isGrounded && GetMotionInfo()->GetMotion() == 2) && GetMotionInfo()->GetBlendMotion() != 1)
	{// 着地やスライディング終わったら歩きに戻す
		GetMotionInfo()->SetMotion(1, false);
	}

	switch (m_state)
	{
		// 通常
	case STATE::Normal:
		break;
		// ターン
	case STATE::Turn:
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
	case STATE::Climb:
		if (IsClimbingContact())
		{// 壁についた
			FaceBlock();               // 改めて壁を向く
			moveDir.setY(CLIMB_SPEED); // 登る
			m_state = STATE::Climbing; // 登っている状態
			GetMotionInfo()->SetMotion(5, false);
		}
		break;
		// 登っている
	case STATE::Climbing:
	{
		if (IsClimbingContact())
		{// 登り続けている
			FaceBlock();               // 改めて壁を向く
			moveDir.setY(CLIMB_SPEED); // 登る

			if (IsClimbingEnd())
			{// ブロックの上
				m_state = STATE::Jumping;
				m_pClimbBlock = nullptr;
			}
		}
		else
		{// 登ったまま少し浮いたか落ちた
			m_state = STATE::Jumping;
			m_pClimbBlock = nullptr;
		}
		break;
	}
	// 跳ぶ
	case STATE::Jump:
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
	// 飛んでいる
	case STATE::Jumping:
		if (m_isGrounded)
		{// 着地したら
			Landing();
		}
		break;
		// 滑っている
	case STATE::Sliding:
		if (GetMotionInfo()->GetBlendMotion() != 2)
		{// スライディングモーション
			GetMotionInfo()->SetMotion(2, false);
		}
		break;
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

// クライムターゲットチェック
bool CPlayer::IsClimbingTarget(const CBlock* pBlock)
{
	if (pBlock == nullptr) return false;

	// 自分の足元
	float myY = GetPos().y;

	// ブロックのあたり判定
	btCollisionObject* pBlockObject = pBlock->GetRB();

	// ブロックの半分のサイズ
	btVector3 baseHalfExtents = static_cast<btBoxShape*>(pBlockObject->getCollisionShape())->getHalfExtentsWithMargin();

	// ブロックの実際のサイズ
	btVector3 actuaExtents = baseHalfExtents * 2.0f;

	// ブロックの中心Y座標 + ブロックの高さの半分 = ブロックの天面のY座標
	float blockTopY = pBlock->GetPosition().y + actuaExtents.y();

	// プレイヤーの足元よりブロックの天面が一定値以上高ければ登る
	return (blockTopY - myY >= CLIMB_HEIGHT_MIN);
}

// クライムチェック
bool CPlayer::IsClimbingContact()
{
	if (m_pClimbBlock == nullptr) return false;
	btCollisionObject* pBlockObject = m_pClimbBlock->GetRB();
	// 何組が衝突しているか
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// 衝突回数分繰り返し
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// 衝突しているペアを取得
		btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

		// 衝突していたら
		if (manifold->getNumContacts() <= 0) continue;

		// 衝突オブジェクト１、２を取得
		const btCollisionObject* objA = manifold->getBody0();
		const btCollisionObject* objB = manifold->getBody1();

		// プレイヤーとゴールが当たっていたら
		const bool Condition = (objA == m_RigitBody.get() && objB == pBlockObject) || (objA == pBlockObject && objB == m_RigitBody.get());

		// 切り上げ
		if (Condition == true) return true;
	}
	return false;
}

// 登り切った?
bool CPlayer::IsClimbingEnd()
{
	if (m_pClimbBlock == nullptr) return false;

	// 自分の足元
	float myY = GetPos().y;

	// ブロックのあたり判定
	btCollisionObject* pBlockObject = m_pClimbBlock->GetRB();

	// ブロックの半分のサイズ
	btVector3 baseHalfExtents = static_cast<btBoxShape*>(pBlockObject->getCollisionShape())->getHalfExtentsWithMargin();

	// ブロックのスケール
	btVector3 currentScale = pBlockObject->getCollisionShape()->getLocalScaling();

	// ブロックの実際の半分のサイズ
	btVector3 actualHalfExtents = baseHalfExtents * currentScale;

	// ブロックの実際のサイズ
	btVector3 actuaExtents = actualHalfExtents * 2.0f;

	// ブロックの中心Y座標 + ブロックの高さの半分 = ブロックの天面のY座標
	float blockTopY = m_pClimbBlock->GetPosition().y + actuaExtents.y();

	// プレイヤーの足元がブロックの天面より上に来たら着地
	return (myY >= blockTopY);
}

// ジャンプ
void CPlayer::Jump(btVector3& moveDir)
{
	moveDir.setX(moveDir.x() * JUMP_SPEED_INA);
	moveDir.setZ(moveDir.z() * JUMP_SPEED_INA);
	moveDir.setY(JUMP_POWER);
	m_state = STATE::Jumping;
	GetMotionInfo()->SetMotion(3, true);
}

// 着地
void CPlayer::Landing()
{
	m_state = STATE::Normal;
	GetMotionInfo()->SetMotion(4, false);
}

// ブロックに向かう
void CPlayer::FaceBlock()
{
	if (m_pClimbBlock != nullptr)
	{
		D3DXVECTOR3 myPos = GetPos();
		D3DXVECTOR3 climbPos = m_pClimbBlock->GetClosestPointOnSurface(myPos);
		D3DXVECTOR3 space = climbPos - myPos;

		// XZ平面での距離の2乗
		float lengthXZ = hypotf(space.x, space.z);
		if (lengthXZ > CLIMB_LENGTH_MIN)
		{// 登れるブロックが遠い
			m_pClimbBlock = nullptr;
			return;
		}

		if (lengthXZ < 0.0001f)
		{// めり込んでいる
			// ブロックの底面の中心
			D3DXVECTOR3 blockBottomPos = m_pClimbBlock->GetPosition();

			// プレイヤーからブロックの中心へのベクトル
			space.x = blockBottomPos.x - myPos.x;
			space.z = blockBottomPos.z - myPos.z;
			space.y = 0.0f;

			lengthXZ = (space.x * space.x) + (space.z * space.z);
			if (lengthXZ < 0.0001f)
			{// ブロックの中心とも重なっている
				m_pClimbBlock = nullptr;
				return;
			}
		}

		D3DXVec3Normalize(&space, &space);
		SetRotDest(D3DXVECTOR3(0.0f, atan2f(-space.x, -space.z), 0.0f));
		SetRot(D3DXVECTOR3(0.0f, atan2f(-space.x, -space.z), 0.0f));
	}
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
	CModelCharacter::SetPos(D3DXVECTOR3(Pos.x, Pos.y - CAPSULE_HALF_HEIGHT, Pos.z));
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