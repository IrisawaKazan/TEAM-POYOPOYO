// 定数バッファ
float4x4 g_mtxWorld;
float4x4 g_mtxView;
float4x4 g_mtxProj;
float4 g_Color;

// 頂点シェーダー（アウトライン用）
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION;
};

VS_OUTPUT VS_main(VS_INPUT input)
{
    // 法線方向に拡張
    float3 expanded = input.Pos.xyz + input.Normal * 5.0;
	
	// 出力用の位置
    VS_OUTPUT Out;
	
	// ワールド座標に変換
    Out.Pos = mul(float4(expanded, 1.0), g_mtxWorld);
	
	// カメラ空間に変換
    Out.Pos = mul(Out.Pos, g_mtxView);
    Out.Pos = mul(Out.Pos, g_mtxProj);
	
	// 出力
    return Out;
}

// ピクセルシェーダー（アウトライン用）
float4 PS_main() : COLOR
{
    return g_Color; // 黒色で塗りつぶし
}

// テクニック
technique StandardDraw
{
    // --- Pass 1: アウトライン描画 ---
    pass P0
    {
        CullMode = CW; // 裏面を描画
        VertexShader = compile vs_3_0 VS_main();
        PixelShader = compile ps_3_0 PS_main();
    }
}