#include "Wave.h"

DirectX::XMFLOAT3 Wave::m_WavePos[8][6];
DirectX::XMFLOAT3 Wave::m_WaveOldPos[8][6];

namespace {
	const float offset = 1.0f;
}

Wave::Wave()
{
	static bool InitFlag = false;

	if (!InitFlag) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 6; j++) {
				m_WavePos[i][j].x = j * offset - (offset * (float)(j) / 2.0f);
				m_WavePos[i][j].z = i * offset - (offset * (float)(i) / 2.0f);
				m_WavePos[i][j].y = -0.01f;

				m_WaveOldPos[i][j] = m_WavePos[i][j];
			}
		}
		m_WavePos[8 / 2][6 / 2].y = 0.2f;
		InitFlag = true;
	}
}

void Wave::Update()
{
	float buf[8][6] = {};

	static float vtime = 0.0f;
	vtime += 0.4f;
	float c = 0.2f;	// 波の伝わる速度
	float dt = vtime;		// 時間の刻み
	float h = 8;		// グリッドの幅

	float mul = c * c * dt * dt / (h * h);	// 定数
	if (mul > 0.5f) mul = 0.5f;

	for (int i = 1; i < 8 - 1; i++) {
		for (int j = 1; j < 6 - 1; j++) {
			float Left = m_WavePos[i][j - 1].y;
			float Right = m_WavePos[i][j + 1].y;
			float Top = m_WavePos[i - 1][j].y;
			float Botom = m_WavePos[i + 1][j].y;

			float Pre = m_WaveOldPos[i][j].y;
			float Cur = m_WavePos[i][j].y;

			// 式本体
			buf[i][j] = Cur + Cur - Pre + mul * (-4.0f * Cur + Left + Right + Top + Botom);
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			// 前データを保持しておく
			m_WaveOldPos[i][j].y = m_WavePos[i][j].y;
			m_WavePos[i][j].y = buf[i][j];
		}
	}
}

