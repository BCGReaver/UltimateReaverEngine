/**
 * @file Texture.cpp
 * @brief Implementación de la clase Texture dentro del motor UltimateReaverEngine.
 *
 * @details
 * Este archivo maneja la creación, actualización, render y destrucción de texturas
 * usando DirectX 11. Básicamente es como la parte gráfica que pinta imágenes en pantalla,
 * ya sea cargadas desde archivo o creadas desde cero.
 */

#include "Texture.h"
#include "Device.h"
#include "DeviceContext.h"

 // ============================================================================
 // Inicializar con nombre de archivo (no implementado todavía)
 // ============================================================================
 /**
  * @brief Inicializa una textura desde un archivo (por ahora no está implementado).
  * @param device Referencia al Device de DirectX.
  * @param textureName Nombre del archivo de textura (sin extensión).
  * @param extensionType Tipo de extensión (png, jpg, etc.).
  * @return E_NOTIMPL porque aún no está armado.
  */
HRESULT
Texture::init(Device& device,
  const std::string& textureName,
  ExtensionType extensionType) {
  return E_NOTIMPL;
}

// ============================================================================
// Inicializar textura "vacía" con parámetros (ej: depth, render targets, etc.)
// ============================================================================
/**
 * @brief Crea una textura en memoria con las dimensiones y formato especificados.
 * @param device El Device de DirectX.
 * @param width Ancho de la textura.
 * @param height Alto de la textura.
 * @param Format Formato DXGI de la textura (ej: RGBA, Depth).
 * @param BindFlags Flags de cómo se va a usar (render target, depth, shader resource...).
 * @param sampleCount Multisample count (para anti-aliasing).
 * @param qualityLevels Calidad del multisample.
 * @return S_OK si salió bien, error HRESULT si algo truena.
 *
 * @details
 * Esta función es para cuando quieres una textura desde cero (no un .png),
 * como un buffer de profundidad o un render target. Si algo falla, suelta un
 * mensaje en consola y regresa el error.
 */
HRESULT
Texture::init(Device& device,
  unsigned int width,
  unsigned int height,
  DXGI_FORMAT Format,
  unsigned int BindFlags,
  unsigned int sampleCount,
  unsigned int qualityLevels) {
  if (!device.m_device) {
    ERROR("Texture", "init", "Device is null.");
    return E_POINTER;
  }
  if (width == 0 || height == 0) {
    ERROR("Texture", "init", "Width and height must be greater than 0");
    E_INVALIDARG;
  }

  // Configuración de la textura
  D3D11_TEXTURE2D_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = Format;
  desc.SampleDesc.Count = sampleCount;
  desc.SampleDesc.Quality = qualityLevels;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = BindFlags;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  HRESULT hr = device.CreateTexture2D(&desc, nullptr, &m_texture);

  if (FAILED(hr)) {
    ERROR("Texture", "init",
      ("Failed to create texture with specified params. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

// ============================================================================
// Inicializar desde otra textura (ej: para crear SRV de una textura existente)
// ============================================================================
/**
 * @brief Crea un Shader Resource View (SRV) desde otra textura ya creada.
 * @param device Device de DirectX.
 * @param textureRef Referencia a la textura base.
 * @param format Formato DXGI para la vista.
 * @return S_OK si se arma bien, error HRESULT si falla.
 *
 * @details
 * Esto sirve cuando ya tienes una textura (ej: depth buffer, render target)
 * y quieres que el pixel shader la lea como si fuera una imagen.
 */
HRESULT
Texture::init(Device& device, Texture& textureRef, DXGI_FORMAT format) {
  if (!device.m_device) {
    ERROR("Texture", "init", "Device is null.");
    return E_POINTER;
  }
  if (!textureRef.m_texture) {
    ERROR("Texture", "init", "Texture is null.");
    return E_POINTER;
  }
  // Crear Shader Resource View
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
  srvDesc.Format = format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = 1;
  srvDesc.Texture2D.MostDetailedMip = 0;

  HRESULT hr = device.m_device->CreateShaderResourceView(textureRef.m_texture,
    &srvDesc,
    &m_textureFromImg);

  if (FAILED(hr)) {
    ERROR("Texture", "init",
      ("Failed to create shader resource view for PNG textures. HRESULT: " + std::to_string(hr)).c_str());
    return hr;
  }

  return S_OK;
}

// ============================================================================
// Update (no hace nada de momento)
// ============================================================================
/**
 * @brief Actualiza la textura (placeholder).
 *
 * @details
 * Aquí podrías meter lógica si la textura cambiara cada frame,
 * por ahora está vacío.
 */
void
Texture::update() {
}

// ============================================================================
// Render (bind de la textura al pixel shader)
// ============================================================================
/**
 * @brief Hace bind de la textura al pixel shader para poder usarla en render.
 * @param deviceContext Contexto de render.
 * @param StartSlot Primer slot donde se va a bindear la textura.
 * @param NumViews Número de vistas que se van a asignar (normalmente 1).
 *
 * @details
 * Básicamente: si la textura existe, la pasamos al shader
 * para que pueda dibujar con ella.
 */
void
Texture::render(DeviceContext& deviceContext,
  unsigned int StartSlot,
  unsigned int NumViews) {
  if (!deviceContext.m_deviceContext) {
    ERROR("Texture", "render", "Device Context is null.");
    return;
  }

  if (m_textureFromImg) {
    deviceContext.PSSetShaderResources(StartSlot, NumViews, &m_textureFromImg);
  }
}

// ============================================================================
// Destroy (libera memoria de la textura)
// ============================================================================
/**
 * @brief Libera la memoria de la textura y limpia recursos de DirectX.
 *
 * @details
 * Muy importante: si no llamas a destroy, vas a tener leaks de memoria.
 */
void
Texture::destroy() {
  if (m_texture != nullptr) {
    SAFE_RELEASE(m_texture);
  }
  else if (m_textureFromImg != nullptr) {
    SAFE_RELEASE(m_textureFromImg);
  }
}
