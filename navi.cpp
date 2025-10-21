//--------------------------------
//
// ナビゲーションマーカー [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNavi* CNavi::Create(const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNavi* pNavi = new CNavi;
	if (pNavi == nullptr)
	{
		return nullptr;
	}

	pNavi->SetFilePath(filePath);
	pNavi->SetSize(size);

	// 初期化
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return nullptr;
	}
	return pNavi;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNavi::Init(void)
{
	// 親クラスの初期化
	CObject3D::Init();

	// ウインドウのクライアント内でのマウス座標を取得
	D3DXVECTOR2 mousePos = CManager::GetInputMouse()->GetPos();

	// マウス座標をDirectXの画面座標に変換
	mousePos = ConvertMouseToScreen(mousePos);

	// レイを作成
	CreateRay(mousePos);

	// 初期位置を設定
	SetPosition(PlaneIntersect(HEIGHT));

	m_direction = ARROW_DIRECTION::Left;
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNavi::Uninit(void)
{
	CObject3D::Uninit();
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
	SetPosition(PlaneIntersect(HEIGHT));

	// 親クラスの更新
	CObject3D::Update();

	if (CManager::GetInputMouse()->GetMouseState().lZ > 0.0f)
	{// ホイールアップで矢印の向きを変更
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + static_cast<unsigned char>(ARROW_DIRECTION::Max) - 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}
	else if (CManager::GetInputMouse()->GetMouseState().lZ < 0.0f)
	{// ホイールダウンで矢印の向きを変更
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}

	if (CManager::GetInputMouse()->OnDown(0))
	{// 左クリックしたとき
		m_clickPos = GetPos(); // クリックした位置を保存

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
		m_apArrow.push_back(CArrow::Create(m_clickPos, D3DXVECTOR3(0.0f, arrowAngle, 0.0f), "data/TEXTURE/UI/ArrowMark001.png", { GetWidth(),GetVetical() }, m_apArrow.size()));

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
	}
}

//--------------------------------
// 描画処理
//--------------------------------
void CNavi::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_apArrow.size();                                 // オブジェクトID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	CObject3D::Draw(); // 親クラスの描画

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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