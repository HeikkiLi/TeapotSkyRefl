#include "Sky.h"
#include "Mesh.h"
#include "GeometryGenerator.h"
#include "TextureManager.h"

Sky::Sky()
{
	mCubeMapSRV = NULL;
	mIndexCount = 0;
	mVB = NULL;
	mIB = NULL;
}

bool Sky::Init(ID3D11Device* device, const std::string& cubemapFilename, float skySphereRadius)
{
	// load cubemap from file
	mCubeMapSRV = TextureManager::Instance()->CreateTexture(cubemapFilename);

	if (mCubeMapSRV == NULL)
	{
		return false;
	}

	MeshData sphere;
	GeometryGenerator::Instance()->CreateSphere(skySphereRadius, 32, 32, sphere);

	// Sphere Vertex Buffer
	std::vector<XMFLOAT3> vertices(sphere.Vertices.size());
	for (size_t i = 0; i < sphere.Vertices.size(); ++i)
	{
		vertices[i] = sphere.Vertices[i].Position;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];

	HR(device->CreateBuffer(&vbd, &vinitData, &mVB));

	// Index Buffer
	mIndexCount = sphere.Indices.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	std::vector<USHORT> indices16;
	indices16.assign(sphere.Indices.begin(), sphere.Indices.end());

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices16[0];

	HR(device->CreateBuffer(&ibd, &iinitData, &mIB));


	// TODO create Sky HLSL

	return true;
}

Sky::~Sky()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mCubeMapSRV);
}

ID3D11ShaderResourceView* Sky::CubeMapSRV()
{
	return mCubeMapSRV;
}

void Sky::Render(ID3D11DeviceContext* deviceContext, const Camera* camera)
{
	// TODO sky::render
}

