#include "Splite.h"

//頂点データ
XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},//左下
	{-0.5f,+0.5f,0.0f},//左上
	{+0.5f,-0.5f,0.0f},//右下
};

//頂点データの全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

void Splite::Initialize(SpliteCommon* spliteCommon)
{
	//描画コマンド
	spliteCommon->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);  //全ての頂点を使って描画
}