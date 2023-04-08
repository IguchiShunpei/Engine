#pragma once

#include "Vector3.h"
#include <Vector>

class Spline 
{
public:
	//初期化
	void Initialize();

	//更新
	Vector3 Update(std::vector<Vector3>& points,float t,float val);

	//描画
	void Draw();

	//制御点の集合(vectorコンテナ)、補間する区間の添字、時間経過率
	Vector3 SplinePosition(const std::vector<Vector3>& point, size_t startIndex, float t);

private:
	//時間計算に必要なデータ
	long long startCount;
	long long nowCount;
	double elapsedCount = 0;

	float maxTime = 5.0f;				//全体時間[s]

	//P1からスタートする
	size_t startIndex = 1;

	//球の位置
	Vector3 position;
};