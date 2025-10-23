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

	// 矢印の向きを初期化
	m_direction = ARROW_DIRECTION::Left;

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
		m_pMarker->SetPosition(m_pos);
	}

	if (CManager::GetInputMouse()->GetMouseState().lZ > 0.0f)
	{// ホイールアップで矢印の向きを変更
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + static_cast<unsigned char>(ARROW_DIRECTION::Max) - 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}
	else if (CManager::GetInputMouse()->GetMouseState().lZ < 0.0f)
	{// ホイールダウンで矢印の向きを変更
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}

	if (m_pos.y > (MARKER_OFFSET.y + 1.0f) && CManager::GetInputMouse()->OnDown(0))
	{// 左クリックしたとき
		m_clickPos = m_pos; // クリックした位置を保存

		// 矢印の角度を決定
		float arrowAngle = 0.0f;
		switch (m_direction)
		{
		case CNavi::ARROW_DIRECTION::Left:
			arrowAngle = D3DXToRadian(-90.0f);
			break;
		case CNavi::ARROW_DIRECTION::Front:
			arrowAngle = D3DXToRadian(0.0f);
			break;
		case CNavi::ARROW_DIRECTION::Back:
			arrowAngle = D3DXToRadian(180.0f);
			break;
		}

		// 矢印を作成
		m_apArrow.push_back(CArrow::Create(m_clickPos, D3DXVECTOR3(0.0f, arrowAngle, 0.0f), "data/TEXTURE/UI/ArrowMark001.png", { m_pMarker->GetWidth(),m_pMarker->GetVetical() }, m_apArrow.size()));

		auto pNewArrow = m_apArrow.back(); // 新しく作成した矢印のポインタ

		bool isRepeat = false; // 矢印が重なっているか判定用フラグ
		do
		{// 矢印が重なっているか判定
			isRepeat = false;
			for (auto pArrow : m_apArrow)
			{// 既にある矢印と新しく作成した矢印が重なっているか判定
				if (pArrow == pNewArrow) continue; // 自分自身はスキップ

				if (pNewArrow->ReleaseHit(pArrow->GetPos(), pArrow->GetChengeLength()))
				{// 重なっている場合
					// 古い矢印を削除
					pArrow->RequestRelease();
					SwapRemove(m_apArrow, pArrow);

					isRepeat = true;
					break; // 内側のループを抜けて再度判定を行う
				}
			}
		} while (isRepeat);

		for (size_t cntArrow = 0; cntArrow < m_apArrow.size(); cntArrow++)
		{// インデックス再設定
			m_apArrow[cntArrow]->SetIdx(cntArrow);
		}

		m_apArrow.shrink_to_fit(); // メモリの無駄を削減

		if (m_pMarker != nullptr)
		{
			m_pMarker->SetBiasID(m_apArrow.size()); // ナビマーカーのバイアスIDを更新
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

	// 登録されたオブジェクトをループ
	for (const RayCastTarget& target : m_aRayCastTarget)
	{
		// オブジェクトのメッシュと交差判定
		D3DXVECTOR3 hitPos = MeshIntersect(target.pMesh, target.mtxWorld);

		if (hitPos.y > (MARKER_OFFSET.y + 1.0f))
		{// ヒットしたか？ (MeshIntersect が MARKER_OFFSET 以外を返したか)
			// レイの始点から交点までの距離の2乗を計算
			D3DXVECTOR3 vecToHit = hitPos - m_RayPos;
			float distSq = D3DXVec3LengthSq(&vecToHit);

			// 「初めてのヒット」または「既に見つけた点より近い」場合
			if (closestDistSq < 0.0f || distSq < closestDistSq)
			{
				closestDistSq = distSq; // 距離を更新
				closestHitPos = hitPos; // 交点を更新
			}
		}
	}

	// 最終的に最も近かった座標を登録
	closestHitPos.y += HEIGHT; // 少し上にオフセット
	m_pos = closestHitPos;
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

//--------------------------------
// メッシュとの交点を求める
//--------------------------------
D3DXVECTOR3 CNavi::MeshIntersect(const LPD3DXMESH& pMesh, const D3DXMATRIX& mtxWorld)
{
	// ワールド行列の逆行列を計算
	D3DXMATRIX mtxInvWorld;
	D3DXMatrixInverse(&mtxInvWorld, nullptr, &mtxWorld);

	// レイの始点と方向をワールド座標系からローカル座標系に変換
	D3DXVECTOR3 localRayPos, localRayDir;
	D3DXVec3TransformCoord(&localRayPos, &m_RayPos, &mtxInvWorld);
	D3DXVec3TransformNormal(&localRayDir, &m_RayDir, &mtxInvWorld);
	D3DXVec3Normalize(&localRayDir, &localRayDir);

	// メッシュの情報を取得
	DWORD numFaces = pMesh->GetNumFaces();
	DWORD vertexSize = pMesh->GetNumBytesPerVertex();

	// 頂点バッファとインデックスバッファのポインタ
	void* pVertices = nullptr;
	void* pIndices = nullptr;

	// バッファをロック
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVertices);
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIndices);

	float closestDist = -1.0f;                           // 最も近い交点までの距離

	// 全ての面（三角形）をループしてチェック
	for (DWORD i = 0; i < numFaces; ++i)
	{
		// インデックスバッファから3頂点のインデックスを取得
		WORD* pIndex = (WORD*)pIndices + i * 3;
		WORD i0 = pIndex[0];
		WORD i1 = pIndex[1];
		WORD i2 = pIndex[2];

		// 頂点バッファから3頂点の座標を取得
		D3DXVECTOR3* v0 = (D3DXVECTOR3*)((BYTE*)pVertices + i0 * vertexSize);
		D3DXVECTOR3* v1 = (D3DXVECTOR3*)((BYTE*)pVertices + i1 * vertexSize);
		D3DXVECTOR3* v2 = (D3DXVECTOR3*)((BYTE*)pVertices + i2 * vertexSize);

		float dist;
		BOOL hit = D3DXIntersectTri(
			v0, v1, v2,             // 三角形の3頂点
			&localRayPos,           // レイの始点 (ローカル)
			&localRayDir,           // レイの方向 (ローカル)
			nullptr, nullptr,       // U, V座標 (重心座標系)
			&dist                   // 交点までの距離
		);

		if (hit)
		{
			// 初めてのヒット、またはより近い点でヒットした場合
			if (closestDist < 0.0f || dist < closestDist)
			{
				closestDist = dist;
			}
		}
	}

	// バッファをアンロック
	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	// 交点座標を格納する変数 (無効値で初期化)
	D3DXVECTOR3 intersectionPoint = MARKER_OFFSET;

	// ヒットがあった場合、交点を計算
	if (closestDist >= 0.0f)
	{
		// ローカル座標系での交点をワールド座標系に変換
		D3DXVECTOR3 localIntersectionPoint = localRayPos + localRayDir * closestDist;
		D3DXVec3TransformCoord(&intersectionPoint, &localIntersectionPoint, &mtxWorld);
	}

	return intersectionPoint;
}

//--------------------------------
// ナビゲーションマーカーの作成
//--------------------------------
void CNavi::SetMarker(void)
{
	m_pMarker = CNaviMarker::Create(MARKER_TEXTURE_PATH, MARKER_SIZE);
}