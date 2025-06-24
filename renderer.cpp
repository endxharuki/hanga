/*==============================================================================

   �����_�����O�Ǘ� [renderer.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "renderer.h"
#include "directxmath.h"
#include "OnGameData.h"

//*********************************************************
// �\����
//*********************************************************
// �萔�o�b�t�@�p�̍\����
struct MaskOffsetBuffer
{
	D3DXVECTOR2 MaskPos;
	D3DXVECTOR2 MaskSize;
};

// �萔�o�b�t�@�쐬
ID3D11Buffer* g_pMaskOffsetBuffer = nullptr;

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
D3D_FEATURE_LEVEL       g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           g_D3DDevice = NULL;
ID3D11DeviceContext*    g_ImmediateContext = NULL;
static IDXGISwapChain*         g_SwapChain = NULL;
ID3D11RenderTargetView* g_RenderTargetView = NULL;
ID3D11DepthStencilView* g_DepthStencilView = NULL;



ID3D11VertexShader*     g_VertexShader = NULL;
ID3D11VertexShader* g_VertexMaskShader = NULL;
ID3D11PixelShader*      g_PixelShader = NULL;
ID3D11PixelShader* g_PixelShaderPermation = NULL;
ID3D11PixelShader* g_PixelShaderRed = NULL;
ID3D11InputLayout*      g_VertexLayout = NULL;
ID3D11Buffer*			g_ConstantBuffer = NULL;
ID3D11Buffer*			g_MaterialBuffer = NULL;

D3DXMATRIX				g_WorldMatrix;
D3DXMATRIX				g_ViewMatrix;
D3DXMATRIX				g_ProjectionMatrix;

static ID3D11DepthStencilState*		g_DepthStateEnable;
static ID3D11DepthStencilState*		g_DepthStateDisable;

static ID3D11BlendState*		g_BlendStateNone;
static ID3D11BlendState*		g_BlendStateAlphaBlend;
static ID3D11BlendState*		g_BlendStateAdd;
static ID3D11BlendState*		g_BlendStateSubtract;
static BLEND_MODE				g_BlendStateParam;


static ID3D11RasterizerState*	g_RasterStateCullOff;
static ID3D11RasterizerState*	g_RasterStateCullCW;
static ID3D11RasterizerState*	g_RasterStateCullCCW;

static D3DXCOLOR				g_BorderColor;

static OnGameData* g_GameData = OnGameData::GetInstance();


ID3D11Device* GetDevice( void )
{
	return g_D3DDevice;
}


ID3D11DeviceContext* GetDeviceContext( void )
{
	return g_ImmediateContext;
}


IDXGISwapChain* GetSwapChain()
{
	return g_SwapChain;
}


void SetDepthEnable( bool Enable )
{
	if( Enable )
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );
	else
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateDisable, NULL );

}

void SetBlendState(BLEND_MODE bm)
{
	g_BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (g_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		g_ImmediateContext->OMSetBlendState(g_BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		g_ImmediateContext->OMSetBlendState(g_BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		g_ImmediateContext->RSSetState(g_RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		g_ImmediateContext->RSSetState(g_RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		g_ImmediateContext->RSSetState(g_RasterStateCullCCW);
		break;
	}
}

void SetSamplerState(FILTER_MODE fm, ADDRESS_MODE am)
{
	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	switch (fm)
	{
	case FILTER_MODE_POINT:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case FILTER_MODE_LINEAR:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case FILTER_MODE_ANISOTROPIC:
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	//�e�N�X�`���̃A�h���b�V���O���[�h�̐ݒ�		
	switch (am)
	{
	case ADDRESS_MODE_WRAP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case ADDRESS_MODE_MIRROR:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case ADDRESS_MODE_CLAMP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case ADDRESS_MODE_BORDER:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	}
													   													   
	samplerDesc.BorderColor[0] = g_BorderColor.r;
	samplerDesc.BorderColor[1] = g_BorderColor.g;
	samplerDesc.BorderColor[2] = g_BorderColor.b;
	samplerDesc.BorderColor[3] = g_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	g_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	g_ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	samplerState->Release();
	samplerState = NULL;
}

void SetSamplerBorderColor(D3DXCOLOR col)
{
	g_BorderColor = col;
}

void SetVertexShader(int s)
{
	if (s == 0)
	{
		g_ImmediateContext->VSSetShader(g_VertexShader, NULL, 0);
	}
	else
	{
		g_ImmediateContext->VSSetShader(g_VertexMaskShader, NULL, 0);
	}
}

void SetPixelShader(int s)
{
	if (s == 0)
	{
		g_ImmediateContext->PSSetShader(g_PixelShader, NULL, 0);
	}
	else
	{
		g_ImmediateContext->PSSetShader(g_PixelShaderRed, NULL, 0);
	}
}

void SetWorldViewProjection2D( void )
{
	D3DXMATRIX worldViewProjection;

	D3DXMATRIX mWorld, mView, mProj, m;

	D3DXMatrixIdentity(&m);
	m(0, 0) = -1.0f;	// y���ɋ��f������
	mView = m * mView;


	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource( g_ConstantBuffer, 0, NULL, &worldViewProjection, 0, 0 );
}


void SetWorldMatrix()
{
	//XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f, 1.0f);

	// �v���W�F�N�V�����s��̎擾
	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);

	if (g_GameData->HasSwap())
	{
		D3DXMATRIX m;

		D3DXMatrixIdentity(&m);
		m(0, 0) = -1.0f;	// y���ɋ��f������

		projection = projection * m;
	}


	D3DXMatrixTranspose(&projection, &projection);

	GetDeviceContext()->UpdateSubresource( g_ConstantBuffer, 0, NULL, &projection, 0, 0 );
}

void SetViewMatrix( D3DXMATRIX *ViewMatrix )
{
	g_ViewMatrix = *ViewMatrix;

	D3DXMATRIX worldViewProjection = g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix;
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &worldViewProjection, 0, 0);
}

void SetProjectionMatrix( D3DXMATRIX *ProjectionMatrix )
{
	D3DXMATRIX projection;
	projection = *ProjectionMatrix;
	D3DXMatrixTranspose(&projection, &projection);

	GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, NULL, &projection, 0, 0);
}

void SetMask(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	MaskOffsetBuffer newData;
	newData.MaskPos = pos;
	newData.MaskSize = size;

	// �o�b�t�@�ւ̃f�[�^�̏�������
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	g_ImmediateContext->Map(g_pMaskOffsetBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &newData, sizeof(MaskOffsetBuffer));
	g_ImmediateContext->Unmap(g_pMaskOffsetBuffer, 0);

	g_ImmediateContext->VSSetConstantBuffers(2, 1, &g_pMaskOffsetBuffer);
	g_ImmediateContext->PSSetConstantBuffers(2, 1, &g_pMaskOffsetBuffer);
}

ID3D11ShaderResourceView* GetScreenShot()
{
	// �o�b�N�o�b�t�@����̉摜�擾
	ID3D11Texture2D* pBackBuffer;
	g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	// �X�N���[���V���b�g�p�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC desc;
	pBackBuffer->GetDesc(&desc);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11Texture2D* pScreenshotTexture;
	GetDevice()->CreateTexture2D(&desc, NULL, &pScreenshotTexture);

	// �o�b�N�o�b�t�@�̓��e���R�s�[
	GetDeviceContext()->CopyResource(pScreenshotTexture, pBackBuffer);

	// �V�F�[�_�[���\�[�X�r���[�̍쐬
	ID3D11ShaderResourceView* pShaderResourceView;
	GetDevice()->CreateShaderResourceView(pScreenshotTexture, NULL, &pShaderResourceView);

	pBackBuffer->Release();

	return pShaderResourceView;
}




void SetMaterial( MATERIAL Material )
{

	GetDeviceContext()->UpdateSubresource( g_MaterialBuffer, 0, NULL, &Material, 0, 0 );

}



//=============================================================================
// ����������
//=============================================================================
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										D3D11_CREATE_DEVICE_BGRA_SUPPORT,	// directx2D�p�ɕύX
										NULL,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&g_SwapChain,
										&g_D3DDevice,
										&g_FeatureLevel,
										&g_ImmediateContext );
	if( FAILED( hr ) )
		return hr;


	// �����_�[�^�[�Q�b�g�r���[�����A�ݒ�
	ID3D11Texture2D* pBackBuffer = NULL;
	g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	g_D3DDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_RenderTargetView );
	pBackBuffer->Release();



	//�X�e���V���p�e�N�X�`���[�쐬
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	g_D3DDevice->CreateTexture2D( &td, NULL, &depthTexture );

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	g_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &g_DepthStencilView );

	g_ImmediateContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_ImmediateContext->RSSetViewports( 1, &vp );



	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;//(CW)
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;//(CCW)
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCCW);

	// �J�����O���[�h�ݒ�iCCW�j
	SetCullingMode(CULL_MODE_BACK);



	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAlphaBlend);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateSubtract);

	// �A���t�@�u�����h�ݒ�
	SetBlendState(BLEND_MODE_ALPHABLEND);



	// �[�x�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateEnable );//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateDisable );//�[�x�����X�e�[�g

	g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );



	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );	 //POINT:��ԋ߂��e�N�Z���̐F���T���v�����O����
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//LINEAR:�ߖT���ς�������e�N�Z���̐F���T���v�����O����
	//�e�N�X�`���̃A�h���b�V���O���[�h�̐ݒ�			 //ANISOTROPIC:�ٕ����t�B���^�@3D�ɑΉ������T���v�����O
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//WRAP�F�J��Ԃ�
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;//MIRROR:���ʌJ��Ԃ�
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;//CLAMP:�Ō�̐F���J��Ԃ�
													   //BORDER:�w��F���J��Ԃ�
													   //MIRROR_ONCE:���ʈ��
	g_BorderColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	samplerDesc.BorderColor[0] = g_BorderColor.r;//BORDER�̐F���w��
	samplerDesc.BorderColor[1] = g_BorderColor.g;
	samplerDesc.BorderColor[2] = g_BorderColor.b;
	samplerDesc.BorderColor[3] = g_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	g_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState );

	g_ImmediateContext->PSSetSamplers( 0, 1, &samplerState );
	samplerState->Release();


	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;
	hr = D3DX11CompileFromFile( "shader.hlsl", NULL, NULL, "VertexShaderPolygon", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pVSBlob, &pErrorBlob, NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR );
	}
	g_D3DDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexShader );

	hr = D3DX11CompileFromFile("shader.hlsl", NULL, NULL, "VertexShaderMask", "vs_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}
	g_D3DDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_VertexMaskShader);



	// ���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	g_D3DDevice->CreateInputLayout( layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&g_VertexLayout );

	pVSBlob->Release();


	// �s�N�Z���V�F�[�_�R���p�C���E����
	ID3DBlob* pPSBlob = NULL;
	hr = D3DX11CompileFromFile( "shader.hlsl", NULL, NULL, "PixelShaderPolygon", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pPSBlob, &pErrorBlob, NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShader );
	pPSBlob->Release();

	// mask�p����
	hr = D3DX11CompileFromFile("shader.hlsl", NULL, NULL, "PixelShaderRedPolygon", "ps_4_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	g_D3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_PixelShaderRed);
	pPSBlob->Release();

	// �萔�o�b�t�@�̐ݒ�
	MaskOffsetBuffer initData;
	initData.MaskPos = {0.0f, 0.0f};
	initData.MaskSize = { 0.0f, 0.0f };

	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_D3DDevice->CreateBuffer( &hBufferDesc, NULL, &g_ConstantBuffer );
	g_ImmediateContext->VSSetConstantBuffers( 0, 1, &g_ConstantBuffer );


	hBufferDesc.ByteWidth = sizeof(MATERIAL);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_D3DDevice->CreateBuffer( &hBufferDesc, NULL, &g_MaterialBuffer );
	g_ImmediateContext->VSSetConstantBuffers( 1, 1, &g_MaterialBuffer );

	// mask�p�o�b�t�@
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(MaskOffsetBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initDataDesc = {};
	initDataDesc.pSysMem = &initData;

	g_D3DDevice->CreateBuffer(&bufferDesc, &initDataDesc, &g_pMaskOffsetBuffer);
	g_ImmediateContext->VSSetConstantBuffers(2, 1, &g_pMaskOffsetBuffer);
	g_ImmediateContext->PSSetConstantBuffers(2, 1, &g_pMaskOffsetBuffer);




	// ���̓��C�A�E�g�ݒ�
	g_ImmediateContext->IASetInputLayout( g_VertexLayout );

	// �V�F�[�_�ݒ�
	g_ImmediateContext->VSSetShader( g_VertexShader, NULL, 0 );
	g_ImmediateContext->PSSetShader( g_PixelShader, NULL, 0 );


	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitRenderer(void)
{
	// �I�u�W�F�N�g���
	if (g_DepthStateEnable)		g_DepthStateEnable->Release();
	if (g_DepthStateDisable)	g_DepthStateDisable->Release();

	if (g_BlendStateNone)		g_BlendStateNone->Release();
	if (g_BlendStateAlphaBlend) g_BlendStateAlphaBlend->Release();
	if (g_BlendStateAdd)		g_BlendStateAdd->Release();
	if (g_BlendStateSubtract)	g_BlendStateSubtract->Release();

	if (g_RasterStateCullOff)	g_RasterStateCullOff->Release();
	if (g_RasterStateCullCW)	g_RasterStateCullCW->Release();
	if (g_RasterStateCullCCW)	g_RasterStateCullCCW->Release();

	if( g_ConstantBuffer )		g_ConstantBuffer->Release();
	if( g_MaterialBuffer )		g_MaterialBuffer->Release();
	if( g_VertexLayout )		g_VertexLayout->Release();
	if( g_VertexShader )		g_VertexShader->Release();
	if( g_PixelShader )			g_PixelShader->Release();

	if( g_ImmediateContext )	g_ImmediateContext->ClearState();
	if( g_RenderTargetView )	g_RenderTargetView->Release();
	if( g_SwapChain )			g_SwapChain->Release();
	if( g_ImmediateContext )	g_ImmediateContext->Release();
	if( g_D3DDevice )			g_D3DDevice->Release();
}


//=============================================================================
// �o�b�N�o�b�t�@�N���A
//=============================================================================
void Clear(void)
{
	// �o�b�N�o�b�t�@�N���A
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	g_ImmediateContext->ClearRenderTargetView( g_RenderTargetView, ClearColor );
	g_ImmediateContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


//=============================================================================
// �v���[���g
//=============================================================================
void Present(void)
{
	g_SwapChain->Present( 0, 0 );
	int a = 0;
}


void SplitTexture(ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView** R_outputSRV, ID3D11ShaderResourceView** L_outputSRV) 
{
	// ���̃e�N�X�`���̏����擾
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	inputSRV->GetDesc(&srvDesc);

	// �e�N�X�`���̕��ƍ������擾
	ID3D11Resource* resource;
	inputSRV->GetResource(&resource);
	ID3D11Texture2D* texture;
	resource->QueryInterface(&texture);
	D3D11_TEXTURE2D_DESC texDesc;
	texture->GetDesc(&texDesc);
	UINT originalWidth = texDesc.Width;
	UINT originalHeight = texDesc.Height;

	// �V�����e�N�X�`���̕��ƍ������v�Z�i�c�ɔ����ɕ����j
	UINT newWidth = originalWidth * 0.5f;
	UINT newHeight = originalHeight;

	// �V�����e�N�X�`���̃f�[�^���쐬
	D3D11_TEXTURE2D_DESC newTexDesc;
	newTexDesc.Width = newWidth;
	newTexDesc.Height = newHeight;
	newTexDesc.MipLevels = texDesc.MipLevels;
	newTexDesc.ArraySize = texDesc.ArraySize;
	newTexDesc.Format = texDesc.Format;
	newTexDesc.SampleDesc = texDesc.SampleDesc;
	newTexDesc.Usage = D3D11_USAGE_DEFAULT;
	newTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	newTexDesc.CPUAccessFlags = 0;
	newTexDesc.MiscFlags = 0;

	D3D11_BOX sourceRegionLeft;
	sourceRegionLeft.left = 0;
	sourceRegionLeft.right = newTexDesc.Width;
	sourceRegionLeft.top = 0; 
	sourceRegionLeft.bottom = newTexDesc.Height;
	sourceRegionLeft.front = 0;
	sourceRegionLeft.back = 1;

	D3D11_BOX sourceRegionRight;
	sourceRegionRight.left = newTexDesc.Width;
	sourceRegionRight.right = originalWidth;
	sourceRegionRight.top = 0;
	sourceRegionRight.bottom = newTexDesc.Height;
	sourceRegionRight.front = 0;
	sourceRegionRight.back = 1;


	// �V�����e�N�X�`�����쐬
	ID3D11Texture2D* newTextureLeft = nullptr;
	ID3D11Texture2D* newTextureRight = nullptr;
	GetDevice()->CreateTexture2D(&newTexDesc, nullptr, &newTextureLeft);
	GetDevice()->CreateTexture2D(&newTexDesc, nullptr, &newTextureRight);

	// ���̃e�N�X�`������V�����e�N�X�`���փf�[�^���R�s�[�i�������j
	GetDeviceContext()->CopySubresourceRegion(newTextureLeft, 0, 0, 0, 0, texture, 0, &sourceRegionLeft);
	// ���̃e�N�X�`������V�����e�N�X�`���փf�[�^���R�s�[�i�E�����j
	GetDeviceContext()->CopySubresourceRegion(newTextureRight, 0, 0, 0, 0, texture, 0, &sourceRegionRight);

	GetDevice()->CreateShaderResourceView(newTextureLeft, &srvDesc, L_outputSRV);
	GetDevice()->CreateShaderResourceView(newTextureRight, &srvDesc, R_outputSRV);


	// ���\�[�X�̉��
	texture->Release();
	resource->Release();
}


