#pragma once

class PickingTexture
{
public:
	PickingTexture();
	~PickingTexture();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void EnableWriting();

	void DisableWriting();

	struct PixelInfo {
		float ObjectID;
		float DrawID;
		float PrimID;

		PixelInfo() {
			ObjectID = 0.0f;
			DrawID = 0.0f;
			PrimID = 0.0f;
		}
	};

	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	unsigned int m_fbo;
	unsigned int m_pickingTexture;
	unsigned int m_depthTexture;
};

