#include "SpliteCommon.h"
#include "DirectXCommon.h"
#pragma once
class Splite
{
public:
	void Initialize(SpliteCommon* spliteCommon);

private:
	// SpliteCommonのインスタンス
	SpliteCommon* spliteCommon = nullptr;
};

