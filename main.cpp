#define DIRECTINPUT_VERSION    0x0800 //DirectInputのバージョン指定

#include "GameScene.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	Vector4 color;//色(RGBA)
};

//定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	Matrix4 mat; //3D変換行列
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SIFrameWork* game = new GameScene();
	
	game->Run();

	delete game;

	return 0;
}
