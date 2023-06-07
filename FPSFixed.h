#pragma once

#include <chrono>
#include <thread>

class FPSFixed
{
public: // メンバ関数
	// FPS固定初期化
	void InitializeFixFPS();
	// FPS固定更新
	void UpdateFixFPS();

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};