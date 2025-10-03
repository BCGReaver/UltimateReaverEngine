/**
 * @file DepthStencilView.cpp
 * @brief Wrapper del DSV (Depth Stencil View) para Direct3D 11.
 *
 * @details
 * Aquí armamos el Depth Stencil View que usa el z-buffer (y stencil) del engine.
 * La idea es tener funciones con validaciones, HRESULTs claros y que el uso sea directo.
 * Proyecto: UltimateReaverEngine.
 */

#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Texture.h"

 // ============================================================================
 // init
 // ============================================================================
 /**
  * @brief Crea el Depth Stencil View a partir de una textura de depth.
  *
  * @param device        Device de D3D11 válido (donde se crea el recurso).
  * @param depthStencil  Textura 2D ya creada con bind de depth (D3D11_BIND_DEPTH_STENCIL).
  * @param format        Formato del DSV (ej: DXGI_FORMAT_D24_UNORM_S8_UINT).
  * @return HRESULT      S_OK si todo ok; E_POINTER/E_FAIL u otro HRESULT si algo falla.
  *
  * @note Se configura como TEXTURE2DMS porque solemos usar MSAA en el swap chain.
  */
HRESULT
DepthStencilView::init(Device& device, Texture& depthStencil, DXGI_FORMAT format) {
  if (!device.m_device) {
    ERROR("DepthStencilView", "init", "Device is null.");
    return E_POINTER;
  }
  if (!depthStencil.m_texture) {
    ERROR("DepthStencilView", "init", "Texture is null.");
    return E_FAIL;
  }

  // Descripción del DSV
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  memset(&descDSV, 0, sizeof(descDSV));
  descDSV.Format = format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
  descDSV.Texture2D.MipSlice = 0;

  // Crear el DSV
  HRESULT hr = device.m_device->CreateDepthStencilView(depthStencil.m_texture,
    &descDSV,
    &m_depthStencilView);
  if (FAILED(hr)) {
    ERROR("DepthStencilView", "init",
      ("Failed to create depth stencil view. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

// ============================================================================
// render
// ============================================================================
/**
 * @brief Limpia el depth-stencil (profundidad y stencil) con valores por defecto.
 *
 * @param deviceContext  Contexto de dispositivo para emitir los comandos.
 *
 * @details
 * Deja depth = 1.0f y stencil = 0. Útil al inicio de cada frame antes de dibujar.
 */
void
DepthStencilView::render(DeviceContext& deviceContext) {
  if (!deviceContext.m_deviceContext) {
    ERROR("DepthStencilView", "render", "Device context is null.");
    return;
  }

  deviceContext.m_deviceContext->ClearDepthStencilView(m_depthStencilView,
    D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
    1.0f,
    0);
}

// ============================================================================
// destroy
// ============================================================================
/**
 * @brief Libera el DSV y lo deja en nullptr (seguro llamar más de una vez).
 */
void
DepthStencilView::destroy() {
  SAFE_RELEASE(m_depthStencilView);
}
