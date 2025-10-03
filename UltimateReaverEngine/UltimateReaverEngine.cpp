/**
 * @file UltimateReaverEngine.cpp
 * @brief App base de DirectX11 que levanta ventana, device, shaders y renderiza un cubo texturizado que gira.
 *
 * @details
 * Aquí montamos todo el “pipeline” típico:
 * - Ventana (Win32) + loop de mensajes.
 * - Device/Context/SwapChain (DX11) con sus vistas (RTV/DSV) y viewport.
 * - Shaders (VS/PS), input layout, buffers (vértices, índices, constantes).
 * - Carga de textura y sampler.
 * - Render por frame del cubito con color animado y `Present`.
 *
 * Estilo que seguimos:
 * - **CamelCase** para variables y métodos (primera minúscula, luego Mayúscula).
 * - **Globales** con prefijo `g_` para que se note que viven en todos lados.
 */

 //--------------------------------------------------------------------------------------
 // Includes del engine y DX
 //--------------------------------------------------------------------------------------
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"

//--------------------------------------------------------------------------------------
// Variables globales (prefijo g_ para ubicarlas rápido)
//--------------------------------------------------------------------------------------

/**
 * @var g_window
 * @brief Ventana Win32 + helpers; la usamos para crear el swap chain y leer tamaño.
 */
Window                              g_window;

/**
 * @var g_device
 * @brief Dispositivo de D3D11. Básicamente el “don” que crea recursos (buffers, shaders, etc.).
 */
Device                              g_device;

/**
 * @var g_deviceContext
 * @brief Contexto de D3D11. Aquí seteamos estados y mandamos draw calls.
 */
DeviceContext                       g_deviceContext;

/**
 * @var g_swapChain
 * @brief Intercambiador de buffers (back/front). Con `present()` mostramos frame.
 */
SwapChain                           g_swapChain;

/**
 * @var g_backBuffer
 * @brief Textura del back buffer; de aquí sale el RTV.
 */
Texture                             g_backBuffer;

/**
 * @var g_renderTargetView
 * @brief Render Target View para dibujar en el back buffer.
 */
RenderTargetView                    g_renderTargetView;

/**
 * @var g_depthStencil
 * @brief Textura para profundidad + stencil.
 */
Texture                             g_depthStencil;

/**
 * @var g_depthStencilView
 * @brief Vista de la textura de profundidad para activarla durante el render.
 */
DepthStencilView                    g_depthStencilView;

// Shaders y objetos DX “crudos” (este sample mezcla wrappers + punteros DX nativos)
ID3D11VertexShader* g_pVertexShader = NULL; ///< Vertex Shader cargado desde .fx
ID3D11PixelShader* g_pPixelShader = NULL; ///< Pixel Shader cargado desde .fx
ID3D11InputLayout* g_pVertexLayout = NULL; ///< Layout que describe el input del VS
ID3D11Buffer* g_pVertexBuffer = NULL; ///< Buffer de vértices del cubo
ID3D11Buffer* g_pIndexBuffer = NULL; ///< Buffer de índices del cubo
ID3D11Buffer* g_pCBNeverChanges = NULL; ///< Constantes “fijas” (view)
ID3D11Buffer* g_pCBChangeOnResize = NULL; ///< Constantes que dependen de la proyección/viewport
ID3D11Buffer* g_pCBChangesEveryFrame = NULL; ///< Constantes que cambian cada frame (world + color)
ID3D11ShaderResourceView* g_pTextureRV = NULL; ///< SRV para la textura (seafloor.dds)
ID3D11SamplerState* g_pSamplerLinear = NULL; ///< Sampler linear para la textura

// Matrices de cámara y proyección + color animado del mesh
XMMATRIX g_World;       ///< Transform del objeto
XMMATRIX g_View;        ///< Cámara (LookAt)
XMMATRIX g_Projection;  ///< Proyección (FOV/Aspect/Near/Far)

/**
 * @var g_vMeshColor
 * @brief Color que vamos animando para que el cubo no sea tan soso.
 */
XMFLOAT4 g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------

/**
 * @brief Arma todo el contexto de DX: swapchain, RTV/DSV, viewport, shaders y buffers.
 * @return HRESULT S_OK si armó todo; error de DX si algo falló.
 */
HRESULT InitDevice();

/**
 * @brief Suelta todos los recursos que creamos (release/destroy).
 */
void CleanupDevice();

/**
 * @brief WinProc de la ventana. Maneja mensajes básicos (paint/destroy).
 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * @brief Dibuja un frame: limpia RTV/DSV, actualiza constantes y hace DrawIndexed.
 */
void Render();

/**
 * @brief Compila un shader HLSL desde archivo usando D3DX11.
 * @param szFileName Ruta del .fx
 * @param szEntryPoint Función principal del shader (VS/PS)
 * @param szShaderModel Perfil a compilar (vs_4_0, ps_4_0, etc.)
 * @param ppBlobOut Blob de salida con el bytecode.
 * @return HRESULT del compilado.
 */
HRESULT CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

//--------------------------------------------------------------------------------------
// Entry point — crea ventana, device y entra al loop de mensajes/render.
//--------------------------------------------------------------------------------------

/**
 * @brief Punto de entrada WinMain. Arranca la ventana, DX y el loop principal.
 * @return Código de salida del proceso.
 *
 * @details
 * Mientras no llegue `WM_QUIT`, procesamos mensajes y, si no hay, renderizamos.
 */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  if (FAILED(g_window.init(hInstance, nCmdShow, WndProc)))
    return 0;

  if (FAILED(InitDevice()))
  {
    CleanupDevice();
    return 0;
  }

  // Loop de mensajes
  MSG msg = { 0 };
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      Render();
    }
  }

  CleanupDevice();
  return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Compilación de shaders (helper chiquito)
//--------------------------------------------------------------------------------------

HRESULT CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // En debug metemos info extra para que el shader sea más fácil de debugear.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;
  hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
    dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
  if (FAILED(hr))
  {
    if (pErrorBlob != NULL)
      OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
    if (pErrorBlob) pErrorBlob->Release();
    return hr;
  }
  if (pErrorBlob) pErrorBlob->Release();

  return S_OK;
}

//--------------------------------------------------------------------------------------
// InitDevice — crea swapchain, RTV/DSV, shaders, buffers y estado base
//--------------------------------------------------------------------------------------

HRESULT InitDevice()
{
  HRESULT hr = S_OK;

  // SwapChain + backbuffer
  hr = g_swapChain.init(g_device, g_deviceContext, g_backBuffer, g_window);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice", ("Failed to initialize SwapChain. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  // RTV (para dibujar al backbuffer)
  hr = g_renderTargetView.init(g_device, g_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice", ("Failed to initialize RenderTargetView. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  // Depth Stencil (textura + view)
  hr = g_depthStencil.init(g_device, g_window.m_width, g_window.m_height,
    DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice", ("Failed to initialize DepthStencil. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  hr = g_depthStencilView.init(g_device, g_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT);
  if (FAILED(hr)) {
    ERROR("Main", "InitDevice", ("Failed to initialize DepthStencilView. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  // Viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)g_window.m_width;
  vp.Height = (FLOAT)g_window.m_height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  g_deviceContext.RSSetViewports(1, &vp);

  // --- Shaders (VS/PS) + InputLayout
  ID3DBlob* pVSBlob = NULL;
  hr = CompileShaderFromFile((char*)"UltimateReaverEngine.fx", "VS", "vs_4_0", &pVSBlob);
  if (FAILED(hr)) {
    MessageBox(NULL,
      "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
      "Error", MB_OK);
    return hr;
  }

  hr = g_device.m_device->CreateVertexShader(pVSBlob->GetBufferPointer(),
    pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
  if (FAILED(hr)) { pVSBlob->Release(); return hr; }

  // Layout de entrada (posición + UV)
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT numElements = ARRAYSIZE(layout);

  hr = g_device.m_device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
    pVSBlob->GetBufferSize(), &g_pVertexLayout);
  pVSBlob->Release();
  if (FAILED(hr)) return hr;

  g_deviceContext.IASetInputLayout(g_pVertexLayout);

  // Pixel Shader
  ID3DBlob* pPSBlob = NULL;
  hr = CompileShaderFromFile((char*)"UltimateReaverEngine.fx", "PS", "ps_4_0", &pPSBlob);
  if (FAILED(hr)) {
    MessageBox(NULL,
      "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.",
      "Error", MB_OK);
    return hr;
  }

  hr = g_device.m_device->CreatePixelShader(pPSBlob->GetBufferPointer(),
    pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
  pPSBlob->Release();
  if (FAILED(hr)) return hr;

  // --- Geometría del cubo (vértices + índices)
  SimpleVertex vertices[] =
  {
    { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2(0.0f, 1.0f) },
  };

  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(SimpleVertex) * 24;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  ZeroMemory(&initData, sizeof(initData));
  initData.pSysMem = vertices;

  hr = g_device.m_device->CreateBuffer(&bd, &initData, &g_pVertexBuffer);
  if (FAILED(hr)) return hr;

  // Bind de VB
  UINT stride = sizeof(SimpleVertex);
  UINT offset = 0;
  g_deviceContext.IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

  // Índices
  WORD indices[] =
  {
    3,1,0,  2,1,3,
    6,4,5,  7,4,6,
    11,9,8, 10,9,11,
    14,12,13, 15,12,14,
    19,17,16, 18,17,19,
    22,20,21, 23,20,22
  };

  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(WORD) * 36;
  bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
  bd.CPUAccessFlags = 0;

  initData.pSysMem = indices;
  hr = g_device.m_device->CreateBuffer(&bd, &initData, &g_pIndexBuffer);
  if (FAILED(hr)) return hr;

  g_deviceContext.IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  g_deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Buffers de constantes
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(CBNeverChanges);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  hr = g_device.m_device->CreateBuffer(&bd, NULL, &g_pCBNeverChanges);
  if (FAILED(hr)) return hr;

  bd.ByteWidth = sizeof(CBChangeOnResize);
  hr = g_device.m_device->CreateBuffer(&bd, NULL, &g_pCBChangeOnResize);
  if (FAILED(hr)) return hr;

  bd.ByteWidth = sizeof(CBChangesEveryFrame);
  hr = g_device.m_device->CreateBuffer(&bd, NULL, &g_pCBChangesEveryFrame);
  if (FAILED(hr)) return hr;

  // Textura y sampler
  hr = D3DX11CreateShaderResourceViewFromFile(g_device.m_device, "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
  if (FAILED(hr)) return hr;

  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = g_device.m_device->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  if (FAILED(hr)) return hr;

  // Matrices base
  g_World = XMMatrixIdentity();

  // Cámara (look-at)
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  g_View = XMMatrixLookAtLH(Eye, At, Up);

  CBNeverChanges cbNeverChanges;
  cbNeverChanges.mView = XMMatrixTranspose(g_View);
  g_deviceContext.UpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

  // Proyección
  g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, g_window.m_width / (FLOAT)g_window.m_height, 0.01f, 100.0f);

  CBChangeOnResize cbChangesOnResize;
  cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
  g_deviceContext.UpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

  return S_OK;
}

//--------------------------------------------------------------------------------------
// Cleanup — liberar todo (orden inverso a la creación generalmente)
//--------------------------------------------------------------------------------------

void CleanupDevice()
{
  if (g_deviceContext.m_deviceContext) g_deviceContext.m_deviceContext->ClearState();

  if (g_pSamplerLinear)        g_pSamplerLinear->Release();
  if (g_pTextureRV)            g_pTextureRV->Release();
  if (g_pCBNeverChanges)       g_pCBNeverChanges->Release();
  if (g_pCBChangeOnResize)     g_pCBChangeOnResize->Release();
  if (g_pCBChangesEveryFrame)  g_pCBChangesEveryFrame->Release();
  if (g_pVertexBuffer)         g_pVertexBuffer->Release();
  if (g_pIndexBuffer)          g_pIndexBuffer->Release();
  if (g_pVertexLayout)         g_pVertexLayout->Release();
  if (g_pVertexShader)         g_pVertexShader->Release();
  if (g_pPixelShader)          g_pPixelShader->Release();

  g_depthStencil.destroy();
  g_depthStencilView.destroy();
  g_renderTargetView.destroy();
  g_swapChain.destroy();
  g_backBuffer.destroy();
  g_deviceContext.destroy();
  g_device.destroy();
}

//--------------------------------------------------------------------------------------
// WinProc — manejamos mensajes básicos (paint/destroy)
//--------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

//--------------------------------------------------------------------------------------
// Render — limpia, actualiza constantes, dibuja y presenta
//--------------------------------------------------------------------------------------

void Render()
{
  // Tiempo (para animar rotación y color)
  static float t = 0.0f;
  if (g_swapChain.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += (float)XM_PI * 0.0125f;
  }
  else
  {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0) dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  // Rotación del cubo
  g_World = XMMatrixRotationY(t);

  // Color animado medio psicodélico para que se note que vive
  g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
  g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
  g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

  // Limpiar + bind de RTV/DSV
  float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
  g_renderTargetView.render(g_deviceContext, g_depthStencilView, 1, clearColor);
  g_depthStencilView.render(g_deviceContext);

  // Actualizamos constantes por frame
  CBChangesEveryFrame cb;
  cb.mWorld = XMMatrixTranspose(g_World);
  cb.vMeshColor = g_vMeshColor;
  g_deviceContext.UpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);

  // Bind de shaders, constantes, textura y sampler
  g_deviceContext.VSSetShader(g_pVertexShader, NULL, 0);
  g_deviceContext.VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
  g_deviceContext.VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
  g_deviceContext.VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);

  g_deviceContext.PSSetShader(g_pPixelShader, NULL, 0);
  g_deviceContext.PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
  g_deviceContext.PSSetShaderResources(0, 1, &g_pTextureRV);
  g_deviceContext.PSSetSamplers(0, 1, &g_pSamplerLinear);

  // Dibujamos el cubo (36 índices = 12 triángulos)
  g_deviceContext.DrawIndexed(36, 0, 0);

  // Present (swap buffers)
  g_swapChain.present();
}
