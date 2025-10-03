/**
 * @file RenderTargetView.cpp
 * @brief Implementación del RenderTargetView en UltimateReaverEngine.
 *
 * @details
 * El RenderTargetView es como la "pantalla de dibujo" donde DirectX
 * pinta antes de mandar la imagen final al backbuffer. Aquí se configuran
 * y limpian las vistas de render y se asocian con el depth stencil.
 */

#include "RenderTargetView.h"
#include "Device.h"
#include "Texture.h"
#include "DeviceContext.h"
#include "DepthStencilView.h"

 // ============================================================================
 // init() - versión simple con backbuffer
 // ============================================================================
 /**
  * @brief Inicializa un RenderTargetView a partir de un backbuffer.
  * @param device Referencia al Device de DirectX (donde se crean recursos).
  * @param backBuffer Textura del backbuffer (frame que se va a mostrar).
  * @param Format Formato de la vista (ej: DXGI_FORMAT_R8G8B8A8_UNORM).
  * @return S_OK si se creó bien, HRESULT de error si algo falla.
  */
HRESULT
RenderTargetView::init(Device& device, Texture& backBuffer, DXGI_FORMAT Format) {
  if (!device.m_device) {
    ERROR("RenderTargetView", "init", "Device is nullptr.");
    return E_POINTER;
  }
  if (!backBuffer.m_texture) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }
  if (Format == DXGI_FORMAT_UNKNOWN) {
    ERROR("RenderTargetView", "init", "Format is DXGI_FORMAT_UNKNOWN.");
    return E_INVALIDARG;
  }

  // Config de la descripción de la RTV
  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = Format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

  // Crear RTV
  HRESULT hr = device.m_device->CreateRenderTargetView(backBuffer.m_texture,
    &desc,
    &m_renderTargetView);
  if (FAILED(hr)) {
    ERROR("RenderTargetView", "init",
      ("Failed to create render target view. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

// ============================================================================
// init() - versión con parámetros más detallados
// ============================================================================
/**
 * @brief Inicializa un RenderTargetView con parámetros más específicos.
 * @param device Device de DirectX.
 * @param inTex Textura base sobre la que se va a crear el RTV.
 * @param ViewDimension Tipo de vista (2D, multisample, etc.).
 * @param Format Formato de la textura.
 * @return S_OK si se creó correctamente, HRESULT de error si falla.
 */
HRESULT
RenderTargetView::init(Device& device,
  Texture& inTex,
  D3D11_RTV_DIMENSION ViewDimension,
  DXGI_FORMAT Format) {
  if (!device.m_device) {
    ERROR("RenderTargetView", "init", "Device is nullptr.");
    return E_POINTER;
  }
  if (!inTex.m_texture) {
    ERROR("RenderTargetView", "init", "Texture is nullptr.");
    return E_POINTER;
  }
  if (Format == DXGI_FORMAT_UNKNOWN) {
    ERROR("RenderTargetView", "init", "Format is DXGI_FORMAT_UNKNOWN.");
    return E_INVALIDARG;
  }

  // Config de la descripción de la RTV
  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Format = Format;
  desc.ViewDimension = ViewDimension;

  // Crear RTV
  HRESULT hr = device.m_device->CreateRenderTargetView(inTex.m_texture,
    &desc,
    &m_renderTargetView);
  if (FAILED(hr)) {
    ERROR("RenderTargetView", "init",
      ("Failed to create render target view. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

// ============================================================================
// render() - versión con depthStencil
// ============================================================================
/**
 * @brief Limpia el RenderTargetView y lo asocia junto con el DepthStencil.
 * @param deviceContext Contexto de render.
 * @param depthStencilView DepthStencilView a asociar.
 * @param numViews Número de vistas a configurar.
 * @param ClearColor Color con el que se limpia el buffer (RGBA).
 */
void
RenderTargetView::render(DeviceContext& deviceContext,
  DepthStencilView& depthStencilView,
  unsigned int numViews,
  const float ClearColor[4]) {
  if (!deviceContext.m_deviceContext) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_renderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }

  // Limpiar el RTV
  deviceContext.m_deviceContext->ClearRenderTargetView(m_renderTargetView, ClearColor);

  // Configurar RTV + DSV
  deviceContext.m_deviceContext->OMSetRenderTargets(numViews,
    &m_renderTargetView,
    depthStencilView.m_depthStencilView);
}

// ============================================================================
// render() - versión sin depthStencil
// ============================================================================
/**
 * @brief Configura el RenderTargetView sin usar DepthStencil.
 * @param deviceContext Contexto de render.
 * @param numViews Número de vistas a configurar.
 */
void
RenderTargetView::render(DeviceContext& deviceContext, unsigned int numViews) {
  if (!deviceContext.m_deviceContext) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }
  if (!m_renderTargetView) {
    ERROR("RenderTargetView", "render", "RenderTargetView is nullptr.");
    return;
  }

  // Configurar RTV
  deviceContext.m_deviceContext->OMSetRenderTargets(numViews,
    &m_renderTargetView,
    nullptr);
}

// ============================================================================
// destroy()
// ============================================================================
/**
 * @brief Libera los recursos del RenderTargetView.
 */
void RenderTargetView::destroy() {
  SAFE_RELEASE(m_renderTargetView);
}
