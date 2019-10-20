#pragma once

#include "Util.h"

class Camera;

class Sky
{
public:
	Sky(ID3D11Device* device, const std::string& cubemapFilename, float skySphereRadius);
	~Sky();

	ID3D11ShaderResourceView* CubeMapSRV();

	void Render(ID3D11DeviceContext* deviceContext, const Camera* camera);

private:
	Sky(const Sky& rhs);
	Sky& operator=(const Sky& rhs);

private:
	// Sky mesh vertex and index buffers
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mCubeMapSRV;

	UINT mIndexCount;
};