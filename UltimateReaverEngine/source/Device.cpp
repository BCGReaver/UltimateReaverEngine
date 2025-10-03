/**
 * @file Device.cpp
 * @brief Wrap del ID3D11Device para crear recursos de D3D11 pero con validaciones y logs decentes.
 *
 * @details
 * Este archivo implementa los métodos helpers del device de UltimateReaverEngine:
 * crear RTVs, DSVs, texturas, buffers, shaders, etc. La idea es que:
 * - Checamos argumentos nulos para evitar crasheos raros.
 * - Regresamos HRESULTs claros.
 * - Imprimimos mensajitos de éxito/error para depurar sin dolor.
 *
 * Formato rápido:
 * - camelCase para métodos.
 * - miembros de clase con prefijo m_.
 * - indentación de 2 espacios para que se vea limpio.
 */

#include "Device.h"

 // ============================================================================
 // destroy
 // ============================================================================
 /**
  * @brief Libera el ID3D11Device (si existe) y lo deja en nullptr.
  */
void
Device::destroy() {
  SAFE_RELEASE(m_device);
}

// ============================================================================
// CreateRenderTargetView
// ============================================================================
/**
 * @brief Crea un Render Target View a partir de un recurso (normalmente una textura).
 * @param pResource     Recurso base (ej. backBuffer o textura 2D).
 * @param pDesc         Descripción del RTV (puede ser nullptr para default).
 * @param ppRTView      Doble puntero donde se guarda el RTV resultante.
 * @return HRESULT      S_OK si todo bien, error de D3D si falla.
 */
HRESULT
Device::CreateRenderTargetView(ID3D11Resource* pResource,
  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
  ID3D11RenderTargetView** ppRTView) {
  if (!pResource) {
    ERROR("Device", "CreateRenderTargetView", "pResource is nullptr");
    return E_INVALIDARG;
  }
  if (!ppRTView) {
    ERROR("Device", "CreateRenderTargetView", "ppRTView is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateRenderTargetView(pResource, pDesc, ppRTView);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateRenderTargetView", "Render Target View created successfully!");
  }
  else {
    ERROR("Device", "CreateRenderTargetView",
      ("Failed to create Render Target View. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateTexture2D
// ============================================================================
/**
 * @brief Crea una textura 2D de D3D11.
 * @param pDesc        Describe la textura (formato, tamaño, bind flags, etc.).
 * @param pInitialData Datos iniciales opcionales (puede ser nullptr).
 * @param ppTexture2D  Donde se guarda la textura creada.
 * @return HRESULT     S_OK si salió, error si no.
 */
HRESULT
Device::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Texture2D** ppTexture2D) {
  if (!pDesc) {
    ERROR("Device", "CreateTexture2D", "pDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppTexture2D) {
    ERROR("Device", "CreateTexture2D", "ppTexture2D is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateTexture2D", "Texture2D created successfully!");
  }
  else {
    ERROR("Device", "CreateTexture2D",
      ("Failed to create Texture2D. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateDepthStencilView
// ============================================================================
/**
 * @brief Crea un Depth Stencil View (para el z-buffer y stencil).
 * @param pResource          Recurso base (textura de depth).
 * @param pDesc              Descripción del DSV (puede ser nullptr para default).
 * @param ppDepthStencilView Donde se guarda el DSV.
 * @return HRESULT           S_OK si todo ok.
 */
HRESULT
Device::CreateDepthStencilView(ID3D11Resource* pResource,
  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
  ID3D11DepthStencilView** ppDepthStencilView) {
  if (!pResource) {
    ERROR("Device", "CreateDepthStencilView", "pResource is nullptr");
    return E_INVALIDARG;
  }
  if (!ppDepthStencilView) {
    ERROR("Device", "CreateDepthStencilView", "ppDepthStencilView is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateDepthStencilView", "Depth Stencil View created successfully!");
  }
  else {
    ERROR("Device", "CreateDepthStencilView",
      ("Failed to create Depth Stencil View. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateVertexShader
// ============================================================================
/**
 * @brief Crea un Vertex Shader a partir del bytecode compilado.
 * @param pShaderBytecode  Blob de bytecode del VS.
 * @param BytecodeLength   Tamaño del blob.
 * @param pClassLinkage    (Opcional) Class linkage para dynamic shader linkage.
 * @param ppVertexShader   Donde se guarda el VS creado.
 * @return HRESULT         S_OK si todo bien.
 */
HRESULT
Device::CreateVertexShader(const void* pShaderBytecode,
  unsigned int BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11VertexShader** ppVertexShader) {
  if (!pShaderBytecode) {
    ERROR("Device", "CreateVertexShader", "pShaderBytecode is nullptr");
    return E_INVALIDARG;
  }
  if (!ppVertexShader) {
    ERROR("Device", "CreateVertexShader", "ppVertexShader is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateVertexShader(pShaderBytecode,
    BytecodeLength,
    pClassLinkage,
    ppVertexShader);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateVertexShader", "Vertex Shader created successfully!");
  }
  else {
    ERROR("Device", "CreateVertexShader",
      ("Failed to create Vertex Shader. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateInputLayout
// ============================================================================
/**
 * @brief Crea el Input Layout que matchea el VS con el formato de tus vértices.
 * @param pInputElementDescs   Array con los elementos (POSITION, TEXCOORD, etc.).
 * @param NumElements          Cuántos elementos trae ese array.
 * @param pShaderBytecodeWithInputSignature Bytecode del VS con firma de entrada.
 * @param BytecodeLength       Tamaño del bytecode.
 * @param ppInputLayout        Donde cae el InputLayout creado.
 * @return HRESULT             S_OK si salió.
 */
HRESULT
Device::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
  unsigned int NumElements,
  const void* pShaderBytecodeWithInputSignature,
  unsigned int BytecodeLength,
  ID3D11InputLayout** ppInputLayout) {
  if (!pInputElementDescs) {
    ERROR("Device", "CreateInputLayout", "pInputElementDescs is nullptr");
    return E_INVALIDARG;
  }
  if (!ppInputLayout) {
    ERROR("Device", "CreateInputLayout", "ppInputLayout is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateInputLayout(pInputElementDescs,
    NumElements,
    pShaderBytecodeWithInputSignature,
    BytecodeLength,
    ppInputLayout);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateInputLayout", "Input Layout created successfully!");
  }
  else {
    ERROR("Device", "CreateInputLayout",
      ("Failed to create Input Layout. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreatePixelShader
// ============================================================================
/**
 * @brief Crea un Pixel Shader a partir de su bytecode.
 * @param pShaderBytecode  Blob del PS.
 * @param BytecodeLength   Tamaño del blob.
 * @param pClassLinkage    (Opcional) class linkage.
 * @param ppPixelShader    Donde se guarda el PS.
 * @return HRESULT         S_OK si bien.
 */
HRESULT
Device::CreatePixelShader(const void* pShaderBytecode,
  unsigned int BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11PixelShader** ppPixelShader) {
  if (!pShaderBytecode) {
    ERROR("Device", "CreatePixelShader", "pShaderBytecode is nullptr");
    return E_INVALIDARG;
  }
  if (!ppPixelShader) {
    ERROR("Device", "CreatePixelShader", "ppPixelShader is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreatePixelShader(pShaderBytecode,
    BytecodeLength,
    pClassLinkage,
    ppPixelShader);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreatePixelShader", "Pixel Shader created successfully!");
  }
  else {
    ERROR("Device", "CreatePixelShader",
      ("Failed to create Pixel Shader. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateSamplerState
// ============================================================================
/**
 * @brief Crea un Sampler State (cómo sampleas texturas).
 * @param pSamplerDesc   Descripción (filtros, wrap, etc.).
 * @param ppSamplerState Donde se guarda el sampler.
 * @return HRESULT       S_OK si ok.
 */
HRESULT
Device::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
  ID3D11SamplerState** ppSamplerState) {
  if (!pSamplerDesc) {
    ERROR("Device", "CreateSamplerState", "pSamplerDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppSamplerState) {
    ERROR("Device", "CreateSamplerState", "ppSamplerState is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateSamplerState(pSamplerDesc, ppSamplerState);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateSamplerState", "Sampler State created successfully!");
  }
  else {
    ERROR("Device", "CreateSamplerState",
      ("Failed to create Sampler State. HRESULT: " + std::to_string(hr)).c_str());
  }

  return hr;
}

// ============================================================================
// CreateBuffer
// ============================================================================
/**
 * @brief Crea un buffer de D3D11 (vertex, index, constant, etc.).
 * @param pDesc        Describe el buffer (uso, bind flags, tamaño…).
 * @param pInitialData Datos iniciales (opcional).
 * @param ppBuffer     Donde se guarda el buffer creado.
 * @return HRESULT     S_OK si salió.
 */
HRESULT
Device::CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Buffer** ppBuffer) {
  if (!pDesc) {
    ERROR("Device", "CreateBuffer", "pDesc is nullptr");
    return E_INVALIDARG;
  }
  if (!ppBuffer) {
    ERROR("Device", "CreateBuffer", "ppBuffer is nullptr");
    return E_POINTER;
  }

  HRESULT hr = m_device->CreateBuffer(pDesc, pInitialData, ppBuffer);

  if (SUCCEEDED(hr)) {
    MESSAGE("Device", "CreateBuffer", "Buffer created successfully!");
  }
  else {
    ERROR("Device", "CreateBuffer",
      ("Failed to create Buffer. HRESULT: " + std::to_string(hr)).c_str());
  }
  return hr;
}
