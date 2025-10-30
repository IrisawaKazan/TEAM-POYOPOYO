//--------------------------------
//
// ナビゲーションマーカー [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

// 静的メンバ変数の定義
const float CNavi::ENABLE_ANGLE = cosf(D3DXToRadian(60.0f));                // 床って何度まで?
const D3DXVECTOR3 CNavi::MARKER_OFFSET = D3DXVECTOR3(0.0f, -1000.0f, 0.0f); // ナビマーカーのオフセット位置
const D3DXVECTOR2 CNavi::MARKER_SIZE = D3DXVECTOR2(60.0f, 60.0f);           // ナビマーカーのサイズ

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNavi::Init(void)
{
	// ウインドウのクライアント内でのマウス座標を取得
	D3DXVECTOR2 mousePos = CManager::GetInputMouse()->GetPos();

	// マウス座標をDirectXの画面座標に変換
	mousePos = ConvertMouseToScreen(mousePos);

	// レイを作成
	CreateRay(mousePos);

	// 初期位置を設定
	m_pos = MARKER_OFFSET;

	// 選ばれているオブジェクト
	m_list = LIST::RightArrow;

	// ナビマーカーを初期化
	m_pMarker = nullptr;
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNavi::Uninit(void)
{

}

//--------------------------------
// 更新処理
//--------------------------------
void CNavi::Update(void)
{
	// ウインドウのクライアント内でのマウス座標を取得
	D3DXVECTOR2 mousePos = CManager::GetInputMouse()->GetPos();

	// マウス座標をDirectXの画面座標に変換
	mousePos = ConvertMouseToScreen(mousePos);

	// レイを作成
	CreateRay(mousePos);

	// 位置を更新
	if (m_pMarker != nullptr)
	{
		m_pMarker->SetPos(m_pos);
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) || CManager::GetInputMouse()->GetMouseState().lZ > 0)
	{// Qキーでオブジェクトを変更
		m_list = static_cast<LIST>((static_cast<unsigned char>(m_list) + static_cast<unsigned char>(LIST::Max) - 1) % static_cast<unsigned char>(LIST::Max));
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) || CManager::GetInputMouse()->GetMouseState().lZ < 0)
	{// Eキーでオブジェクトを変更
		m_list = static_cast<LIST>((static_cast<unsigned char>(m_list) + 1) % static_cast<unsigned char>(LIST::Max));
	}

	if (m_pos.y > (MARKER_OFFSET.y + 1.0f) && CManager::GetInputMouse()->OnDown(0))
	{// 左クリックしたとき
		m_clickPos = m_pos; // クリックした位置を保存

		// オブジェクトごとの分岐
		switch (m_list)
		{
		case CNavi::LIST::RightArrow:
			// 矢印を作成
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(90.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::FrontArrow:
			// 矢印を作成
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(180.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::LeftArrow:
			// 矢印を作成
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(-90.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		case CNavi::LIST::BackArrow:
			// 矢印を作成
			m_apObject.push_back(CArrow::Create(m_clickPos, m_pMarker->GetRotMtx(), D3DXToRadian(0.0f), "data/TEXTURE/UI/ArrowMark001.png", m_pMarker->GetSize(), m_apObject.size()));
			break;
		//case CNavi::LIST::Climb:
		//	break;
		//case CNavi::LIST::Attack:
		//	break;
		}
		if (m_apObject.empty()) return;

		auto pNewObject = m_apObject.back(); // 新しく作成したオブジェクトのポインタ

		bool isRepeat = false; // オブジェクトが重なっているか判定用フラグ
		do
		{// オブジェクトが重なっているか判定
			isRepeat = false;
			for (auto pObject : m_apObject)
			{// 既にあるオブジェクトと新しく作成したオブジェクトが重なっているか判定
				if (pObject == pNewObject) continue; // 自分自身はスキップ

				if (pNewObject->ReleaseTrigger(pObject->GetreleaseCollObject()))
				{// 重なっている場合
					// 古いオブジェクトを削除
					pObject->RequestRelease();
					SwapRemove(m_apObject, pObject);

					isRepeat = true;
					break; // 内側のループを抜けて再度判定を行う
				}
			}
		} while (isRepeat);

		for (size_t cntArrow = 0; cntArrow < m_apObject.size(); cntArrow++)
		{// バイアスインデックス再設定
			m_apObject[cntArrow]->SetBiasIdx(cntArrow);
		}

		m_apObject.shrink_to_fit(); // メモリの無駄を削減

		if (m_pMarker != nullptr)
		{
			m_pMarker->SetBiasID(m_apObject.size()); // マーカーのバイアスIDを更新
		}
	}

	m_aRayCastTarget.clear(); // レイキャスト対象オブジェクト配列をクリア
}

//--------------------------------
// マウス座標をDirectXの画面サイズに変換
//--------------------------------
D3DXVECTOR2 CNavi::ConvertMouseToScreen(D3DXVECTOR2 mousePos)
{
	// ウインドウのクライアント内でのマウス座標
	D3DXVECTOR2 windowClientPos = mousePos;

	// DirectXの画面サイズを取得
	D3DXVECTOR2 directXScreenSize = { SCREEN_WIDTH,SCREEN_HEIGHT };

	// DirectXデバイスに登録されているウインドウのサイズを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DDEVICE_CREATION_PARAMETERS creationParams{};
	D3DXVECTOR2 clientSize{ SCREEN_WIDTH,SCREEN_HEIGHT };
	if (SUCCEEDED(pDevice->GetCreationParameters(&creationParams)))
	{
		RECT rect{};
		if (creationParams.hFocusWindow != nullptr)
		{
			GetClientRect(creationParams.hFocusWindow, &rect);
			clientSize = { static_cast<float>(rect.right - rect.left),static_cast<float>(rect.bottom - rect.top) };
		}
	}

	// マウス座標をDirectXの画面サイズに変換
	D3DXVECTOR2 resultMousePos{};
	resultMousePos.x = (windowClientPos.x / clientSize.x) * directXScreenSize.x;
	resultMousePos.y = (windowClientPos.y / clientSize.y) * directXScreenSize.y;

	return resultMousePos;
}

//--------------------------------
// レイを作成
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの取得
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	// 射影行列の取得
	D3DXMATRIX mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューマトリックスの取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 始点と終点
	D3DXVECTOR3 vNear, vFar;

	// ニアクリップ面上の3D座標を計算
	D3DXVECTOR3 screenNear(mousePos.x, mousePos.y, 0.0f);
	D3DXVec3Unproject(
		&vNear,
		&screenNear,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// ファークリップ面上の3D座標を計算
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// レイの始点と方向を決定
	m_RayPos = vNear;                        // レイの始点はニアクリップ面上の点
	m_RayDir = vFar - vNear;                 // ニアからファーへ向かうベクトル
	D3DXVec3Normalize(&m_RayDir, &m_RayDir); // 方向ベクトルなので正規化する
}

//--------------------------------
// レイキャストの対象オブジェクトを登録
//--------------------------------
void CNavi::RegisterRayCastObject(LPD3DXMESH pMesh, const D3DXMATRIX& mtxWorld)
{
	// メッシュがNULLなら登録しない
	if (pMesh == nullptr) return;

	// オブジェクトを登録
	RayCastTarget target;
	target.pMesh = pMesh;
	target.mtxWorld = mtxWorld;
	m_aRayCastTarget.push_back(target);
}

//--------------------------------
// 交点を計算
//--------------------------------
void CNavi::CalculateIntersection(void)
{
	D3DXVECTOR3 closestHitPos = MARKER_OFFSET; // 無効なヒット位置で初期化
	float closestDistSq = -1.0f;               // 最も近い距離の「2乗」
	D3DXMATRIX mtxRot{};                       // 回転行列

	// 登録されたオブジェクトをループ
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		// オブジェクトのメッシュと交差判定
		D3DXVECTOR3 normal{ 0.0f,1.0f,0.0f };
		D3DXVECTOR3 hitPos = MeshIntersect(target.pMesh, target.mtxWorld, ENABLE_ANGLE, &normal);

		if (hitPos.y > (MARKER_OFFSET.y + 1.0f))
		{// ヒットしたか？ (MeshIntersect が MARKER_OFFSET 以外を返したか)
			// レイの始点から交点までの距離の2乗を計算
			D3DXVECTOR3 vecToHit = hitPos - m_RayPos;
			float distSq = D3DXVec3LengthSq(&vecToHit);

			// 「初めてのヒット」または「既に見つけた点より近い」場合
			if (closestDistSq < 0.0f || distSq < closestDistSq)
			{
				closestDistSq = distSq;                  // 距離を更新
				closestHitPos = hitPos;                  // 交点を更新
				mtxRot = CreateMatrixFromNormal(normal); // 回転行列を更新
			}
		}
	}

	// 登録されたオブジェクトをループ
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		if (CheckLatent(target.pMesh, target.mtxWorld, closestDistSq))
		{// 隠れていたら無効
			m_pos = MARKER_OFFSET;
			return;
		}
	}

	// 最終的に最も近かった座標を登録
	closestHitPos.y += HEIGHT;    // 少し上にオフセット
	m_pos = closestHitPos;        // 位置
	if (m_pMarker != nullptr)
	{
		m_pMarker->SetRotMtx(mtxRot); // 角度
	}
}

//--------------------------------
// 平面との交点を求める
//--------------------------------
D3DXVECTOR3 CNavi::PlaneIntersect(float fHeight)
{
	// y=fHeight の平面
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, -fHeight);

	// 交点
	D3DXVECTOR3 intersectionPoint;

	// rayの終点
	D3DXVECTOR3 rayEnd = m_RayPos + m_RayDir * 10000.0f;

	// レイと平面の交点を計算
	D3DXPlaneIntersectLine(
		&intersectionPoint, // 結果の交点
		&plane,             // 平面
		&m_RayPos,          // レイの始点
		&rayEnd             // レイの終点
	);

	if (m_RayDir.y < 0)
	{// レイが下向きのときのみ交点を有効とする
		return intersectionPoint;
	}

	// 交点が無効なときは非常に低い位置を返す
	return D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
}

//----------------------------------------------------------------
// メッシュとの交点を求める (D3DXIntersect 高速版)
//----------------------------------------------------------------
D3DXVECTOR3 CNavi::MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, const float enableAngle, D3DXVECTOR3* pNor)
{
	// Rayのローカル座標系への変換
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	BOOL hit;
	DWORD faceIndex;                 // 面のインデックス
	float dist;                      // 距離 (ローカル)
	float u, v;                      // 重心座標
	LPD3DXBUFFER pHitInfo = nullptr; // ヒット情報

	// MeshとRayのあたり判定
	if (FAILED(D3DXIntersect(
		pMesh,          // メッシュ
		&localRayPos,   // レイの始点 (ローカル)
		&localRayDir,   // レイの方向 (ローカル)
		&hit,           // ヒットしたか？
		&faceIndex,     // どの面にヒットしたか？
		&u, &v,         // 重心座標
		&dist,          // 距離
		&pHitInfo,      // 詳細情報
		nullptr         // 全ヒット情報
	)))
	{
		return MARKER_OFFSET;
	}

	if (pHitInfo != nullptr) pHitInfo->Release();

	// ヒットしなかった
	if (hit == FALSE)
	{
		return MARKER_OFFSET;
	}

	// 法線と角度をチェック
	if (pNor != nullptr || enableAngle > 0.0f)
	{
		DWORD fvf = pMesh->GetFVF();
		DWORD vertexSize = pMesh->GetNumBytesPerVertex();
		DWORD norOffset = (DWORD)-1;

		// 法線のオフセット
		if (fvf & D3DFVF_XYZ) norOffset = sizeof(D3DXVECTOR3);
		if (!(fvf & D3DFVF_NORMAL)) norOffset = (DWORD)-1;

		if (norOffset != (DWORD)-1)
		{// 法線がない
			void* pVertices = nullptr;
			void* pIndices = nullptr;
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVertices);
			pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIndices);

			// ヒットした面の3頂点のインデックスを取得
			WORD* pIndex = (WORD*)pIndices + faceIndex * 3;
			WORD i0 = pIndex[0];
			WORD i1 = pIndex[1];
			WORD i2 = pIndex[2];

			// 頂点データ(バイト配列)の先頭アドレスを取得
			BYTE* pVertData0 = (BYTE*)pVertices + i0 * vertexSize;
			BYTE* pVertData1 = (BYTE*)pVertices + i1 * vertexSize;
			BYTE* pVertData2 = (BYTE*)pVertices + i2 * vertexSize;

			// オフセットを使って法線のポインタを取得
			D3DXVECTOR3* n0 = (D3DXVECTOR3*)(pVertData0 + norOffset);
			D3DXVECTOR3* n1 = (D3DXVECTOR3*)(pVertData1 + norOffset);
			D3DXVECTOR3* n2 = (D3DXVECTOR3*)(pVertData2 + norOffset);

			// ローカル法線を補間
			D3DXVECTOR3 localNormal = (1.0f - u - v) * (*n0) + u * (*n1) + v * (*n2);

			pMesh->UnlockVertexBuffer();
			pMesh->UnlockIndexBuffer();

			// ワールド法線に変換
			D3DXMATRIX matInvTranspose;
			D3DXMatrixTranspose(&matInvTranspose, &mtxInvWorld);
			D3DXVECTOR3 worldNormal;
			D3DXVec3TransformNormal(&worldNormal, &localNormal, &matInvTranspose);
			D3DXVec3Normalize(&worldNormal, &worldNormal);

			// 角度チェック
			const D3DXVECTOR3 up = VEC_UP;
			if (D3DXVec3Dot(&worldNormal, &up) < enableAngle)
			{
				return MARKER_OFFSET; // 角度がダメならヒット無効
			}

			if (pNor != nullptr) *pNor = worldNormal; // 法線を返す
		}
		else
		{
			// 法線がないメッシュ
			if (pNor != nullptr) *pNor = VEC_UP;
		}
	}

	// 交点を計算
	D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * dist;
	D3DXVECTOR3 intersectionPoint;
	D3DXVec3TransformCoord(&intersectionPoint, &localIntersectionPoint, &mtxWorld);

	return intersectionPoint;
}

//----------------------------------------------------------------
// Rayのその距離の地点は隠れているか?
//----------------------------------------------------------------
bool CNavi::CheckLatent(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld, float lengthSq)
{
	// ワールド行列の逆行列
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);

	// レイの始点と方向をワールド座標系からローカル座標系に変換
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	BOOL hit;
	DWORD faceIndex;                 // 面のインデックス
	float dist;                      // 距離 (ローカル)
	float u, v;                      // 重心座標
	LPD3DXBUFFER pHitInfo = nullptr; // ヒット情報

	// MeshとRayのあたり判定
	if (FAILED(D3DXIntersect(
		pMesh,          // メッシュ
		&localRayPos,   // レイの始点 (ローカル)
		&localRayDir,   // レイの方向 (ローカル)
		&hit,           // ヒットしたか？
		&faceIndex,     // どの面にヒットしたか？
		&u, &v,         // 重心座標
		&dist,          // 距離
		&pHitInfo,      // 詳細情報
		nullptr         // 全ヒット情報
	)))
	{
		return false;
	}

	if (pHitInfo != nullptr) pHitInfo->Release();

	// 隠していない
	if (hit == FALSE)
	{
		return false;
	}

	// ローカル座標系での交点
	D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * dist;

	// ローカル座標をワールド座標に変換
	D3DXVECTOR3 worldIntersectionPoint;
	D3DXVec3TransformCoord(&worldIntersectionPoint, &localIntersectionPoint, &mtxWorld);

	// ワールド空間のレイ始点からの距離の2乗
	D3DXVECTOR3 vecToHit = worldIntersectionPoint - m_RayPos;
	float worldDistSq = D3DXVec3LengthSq(&vecToHit);

	// 距離の比較
	if (worldDistSq < lengthSq)
	{// 隠している
		return true;
	}

	// 隠していない
	return false;
}

//----------------------------------------------------------------
// 法線ベクトルから回転行列を作成する
//----------------------------------------------------------------
D3DXMATRIX CNavi::CreateMatrixFromNormal(D3DXVECTOR3 nor)
{
	D3DXVec3Normalize(&nor, &nor);

	D3DXVECTOR3 newY_Axis = nor; // Y軸は法線
	D3DXVECTOR3 newZ_Axis;
	D3DXVECTOR3 newX_Axis;

	D3DXVECTOR3 referenceForward(0.0f, 0.0f, 1.0f); // 基準となる前方

	if (fabsf(D3DXVec3Dot(&newY_Axis, &referenceForward)) > 0.999f)
	{// もし法線がほぼ真上または真下を向いている
		// 基準となる前方を、ワールドの上方向(0,1,0)に変更する
		referenceForward = VEC_UP;
	}

	// 基準となる前方から、法線に平行な成分を引くことで、平面上に投影する
	D3DXVECTOR3 parallelPart = D3DXVec3Dot(&referenceForward, &newY_Axis) * newY_Axis;
	newZ_Axis = referenceForward - parallelPart;
	D3DXVec3Normalize(&newZ_Axis, &newZ_Axis);

	// Y x Z = X
	D3DXVec3Cross(&newX_Axis, &newY_Axis, &newZ_Axis);
	D3DXVec3Normalize(&newX_Axis, &newX_Axis);

	// 行列を組み立てる
	D3DXMATRIX mtxRot;
	D3DXMatrixIdentity(&mtxRot);

	mtxRot._11 = newX_Axis.x; mtxRot._12 = newX_Axis.y; mtxRot._13 = newX_Axis.z;
	mtxRot._21 = newY_Axis.x; mtxRot._22 = newY_Axis.y; mtxRot._23 = newY_Axis.z;
	mtxRot._31 = newZ_Axis.x; mtxRot._32 = newZ_Axis.y; mtxRot._33 = newZ_Axis.z;

	return mtxRot;
}

//--------------------------------
// ナビゲーションマーカーの作成
//--------------------------------
void CNavi::SetMarker(void)
{
	m_pMarker = CNaviMarker::Create(MARKER_TEXTURE_PATH, MARKER_SIZE);
}