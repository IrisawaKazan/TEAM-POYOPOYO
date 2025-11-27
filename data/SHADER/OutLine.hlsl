//****************************************************************
//
// アウトラインシェーダー[OutLine.hlsl]
// Author Kensaku Hatori
//
//****************************************************************

// 定数バッファ
float4x4 g_mtxWorld;
float4x4 g_mtxView;
float4x4 g_mtxProj;
float4 g_Color;

// 頂点シェーダー
struct VS_INPUT
{
    // 位置
    float4 Pos : POSITION;
    // 法線
    float3 Normal : NORMAL;
};

// ピクセルシェーダーへの情報
struct VS_OUTPUT
{
    // 位置
    float4 Pos : POSITION;
};

//***************************************
// 頂点シェーダー(エントリーポイント)
//***************************************
VS_OUTPUT VS_main(VS_INPUT input)
{
    // 法線方向に拡張
    float3 expanded = input.Pos.xyz + input.Normal * 5.0;
	
	// 出力用変数
    VS_OUTPUT Out;
	
	// ワールド座標に変換
    Out.Pos = mul(float4(expanded, 1.0), g_mtxWorld);
	// カメラ空間に変換
    Out.Pos = mul(Out.Pos, g_mtxView);
    Out.Pos = mul(Out.Pos, g_mtxProj);
	
	// 出力
    return Out;
}

//***************************************
// ピクセルシェーダー
//***************************************
float4 PS_main() : COLOR
{
    // 指定された色で塗りつぶし
    return g_Color;
}

//***************************************
// テクニックハンドル
//***************************************
technique StandardDraw
{
    // パス
    pass P0
    {
        // 裏面を描画
        CullMode = CW;
        // 頂点シェーダーとピクセルシェーダーを設定
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_main();
    }
}