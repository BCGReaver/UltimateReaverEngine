#include "BaseApp.h"
#include <string>
#include <Windows.h>

BaseApp::BaseApp(HINSTANCE, int) {}

int BaseApp::run(HINSTANCE hInst, int nCmdShow) {
  if (FAILED(m_window.init(hInst, nCmdShow, wndProc))) return 0;
  if (FAILED(init())) return 0;

  MSG msg = {};
  LARGE_INTEGER freq, prev;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&prev);

  while (WM_QUIT != msg.message) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      LARGE_INTEGER curr;
      QueryPerformanceCounter(&curr);
      float dt = static_cast<float>(curr.QuadPart - prev.QuadPart) / static_cast<float>(freq.QuadPart);
      prev = curr;
      update(dt);
      render();
    }
  }
  return (int)msg.wParam;
}

HRESULT BaseApp::init() {
  HRESULT hr = S_OK;

  // SwapChain / RTV / Depth / DSV / Viewport
  if (FAILED(hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window))) return hr;
  if (FAILED(hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM))) return hr;
  if (FAILED(hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height,
    DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0))) return hr;
  if (FAILED(hr = m_depthStencilView.init(m_device, m_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT))) return hr;
  if (FAILED(hr = m_viewport.init(m_window))) return hr;

  // Input Layout: POSITION(float3) + TEXCOORD(float2)
  std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
  {
    D3D11_INPUT_ELEMENT_DESC e = {};
    e.SemanticName = "POSITION";
    e.SemanticIndex = 0;
    e.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    e.InputSlot = 0;
    e.AlignedByteOffset = 0;
    e.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    e.InstanceDataStepRate = 0;
    layout.push_back(e);
  }
  {
    D3D11_INPUT_ELEMENT_DESC e = {};
    e.SemanticName = "TEXCOORD";
    e.SemanticIndex = 0;
    e.Format = DXGI_FORMAT_R32G32_FLOAT; // <-- float2 (OBJ vt)
    e.InputSlot = 0;
    e.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    e.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    e.InstanceDataStepRate = 0;
    layout.push_back(e);
  }

  if (FAILED(hr = m_shaderProgram.init(m_device, "UltimateReaverEngine.fx", layout))) return hr;

  // Log del Working Directory + existencia del OBJ
  wchar_t cwd[MAX_PATH]; GetCurrentDirectoryW(MAX_PATH, cwd);
  OutputDebugStringW((std::wstring(L"[CWD] ") + cwd + L"\n").c_str());

  // Ajusta el nombre si usas otro .obj
  const wchar_t* OBJ_NAME_W = L"thor hammer .obj";
  DWORD attr = GetFileAttributesW(OBJ_NAME_W);
  if (attr == INVALID_FILE_ATTRIBUTES) {
    MessageBoxW(nullptr, L"No existe thor hammer .obj en el Working Directory", L"RUTA", MB_OK | MB_ICONERROR);
    return E_FAIL;
  }

  // Cargar OBJ (sin fallback al cubo)
  if (!m_modelLoader.loadModel("thor hammer .obj", m_mesh)) {
    MessageBoxW(nullptr, L"Falló la carga de thor hammer .obj (parser).", L"ModelLoader", MB_OK | MB_ICONERROR);
    return E_FAIL;
  }

  // Buffers GPU a partir de m_mesh
  if (FAILED(hr = m_vertexBuffer.init(m_device, m_mesh, D3D11_BIND_VERTEX_BUFFER))) return hr;
  if (FAILED(hr = m_indexBuffer.init(m_device, m_mesh, D3D11_BIND_INDEX_BUFFER)))   return hr;

  m_deviceContext.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Const buffers
  if (FAILED(hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges))))      return hr;
  if (FAILED(hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize))))  return hr;
  if (FAILED(hr = m_cbChangesEveryFrame.init(m_device, sizeof(CBChangesEveryFrame)))) return hr;

  // Textura y sampler (temporal)
  if (FAILED(hr = m_textureCube.init(m_device, "thor color", ExtensionType::JPG))) return hr;
  if (FAILED(hr = m_samplerState.init(m_device))) return hr;

  // Matrices base
  m_World = XMMatrixIdentity();
  // Alejar más la cámara para ver el martillo completo
  XMVECTOR Eye = XMVectorSet(0.0f, 8.0f, -18.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_View = XMMatrixLookAtLH(Eye, At, Up);


  cbNeverChanges.mView = XMMatrixTranspose(m_View);
  m_Projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f),
    m_window.m_width / (FLOAT)m_window.m_height,
    0.01f,
    500.0f);

  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

  // Log de conteos del OBJ
  char tmp[128];
  sprintf_s(tmp, "[BaseApp] OBJ -> Verts:%d Indices:%d\n", m_mesh.m_numVertex, m_mesh.m_numIndex);
  OutputDebugStringA(tmp);

  return S_OK;
}

void BaseApp::update(float /*dt*/) {
  // View / Projection
  cbNeverChanges.mView = XMMatrixTranspose(m_View);
  m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);

  m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
    static_cast<float>(m_window.m_width) / static_cast<float>(m_window.m_height),
    0.01f, 100.0f);
  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
  m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

  // Sin rotación ni tinte (puro OBJ)
  m_vMeshColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
  m_World = XMMatrixIdentity();
  cb.mWorld = XMMatrixTranspose(m_World);
  cb.vMeshColor = m_vMeshColor;
  m_cbChangesEveryFrame.update(m_deviceContext, nullptr, 0, nullptr, &cb, 0, 0);
}

void BaseApp::render() {
  float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
  m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

  m_viewport.render(m_deviceContext);
  m_depthStencilView.render(m_deviceContext);
  m_shaderProgram.render(m_deviceContext);

  // VB/IB del OBJ
  m_vertexBuffer.render(m_deviceContext, 0, 1);
  m_indexBuffer.render(m_deviceContext, 0, 1, false, DXGI_FORMAT_R32_UINT);

  // Const buffers & textura
  m_cbNeverChanges.render(m_deviceContext, 0, 1);
  m_cbChangeOnResize.render(m_deviceContext, 1, 1);
  m_cbChangesEveryFrame.render(m_deviceContext, 2, 1);
  m_cbChangesEveryFrame.render(m_deviceContext, 2, 1, true);

  m_textureCube.render(m_deviceContext, 0, 1);
  m_samplerState.render(m_deviceContext, 0, 1);

  // Dibuja el OBJ (no cubo)
  m_deviceContext.DrawIndexed(static_cast<UINT>(m_mesh.m_numIndex), 0, 0);

  m_swapChain.present();
}

void BaseApp::destroy() {
  if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();
  m_samplerState.destroy();
  m_textureCube.destroy();
  m_cbNeverChanges.destroy();
  m_cbChangeOnResize.destroy();
  m_cbChangesEveryFrame.destroy();
  m_vertexBuffer.destroy();
  m_indexBuffer.destroy();
  m_shaderProgram.destroy();
  m_depthStencil.destroy();
  m_depthStencilView.destroy();
  m_renderTargetView.destroy();
  m_swapChain.destroy();
  m_backBuffer.destroy();
  m_deviceContext.destroy();
  m_device.destroy();
}

LRESULT BaseApp::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_CREATE: {
    CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);
  } return 0;
  case WM_PAINT: {
    PAINTSTRUCT ps; BeginPaint(hWnd, &ps); EndPaint(hWnd, &ps);
  } return 0;
  case WM_DESTROY: PostQuitMessage(0); return 0;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}
