#pragma once
/**
 * @file Device.h
 * @brief Wrapper del @c ID3D11Device para crear recursos de D3D11 sin pelearte con la API cruda.
 *
 * Parte de **UltimateReaverEngine**. Con esta clase creas texturas, shaders, buffers y vistas
 * con helpers que ya validan parámetros y regresan @c HRESULT limpio.
 */

#include "Prerequisites.h"

 /**
  * @class Device
  * @brief Contiene el @c ID3D11Device y expone helpers para fabricar recursos (RTV, DSV, shaders, etc.).
  *
  * @details
  * Piensa en esto como la “fábrica” de objetos de GPU. Aquí pides texturas, buffers, estados
  * y shaders ya listos, y tú te enfocas en usarlos, no en la verbosidad de D3D11.
  */
class
  Device {
public:
  Device() = default;
  ~Device() = default;

  /**
   * @brief Engancha/crea el @c ID3D11Device (si aplica).
   */
  void
    init();

  /**
   * @brief Tick de mantenimiento (placeholder).
   */
  void
    update();

  /**
   * @brief Hook de render/debug (placeholder).
   */
  void
    render();

  /**
   * @brief Libera el @c ID3D11Device.
   */
  void
    destroy();

  // -----------------------------
  // Creación de vistas / recursos
  // -----------------------------

  /**
   * @brief Crea una Render Target View (RTV) desde un recurso.
   * @param pResource   Recurso base (normalmente una textura).
   * @param pDesc       Descriptor (o @c nullptr para default).
   * @param ppRTView    Salida: puntero a la RTV creada.
   * @return @c S_OK si todo ok; @c HRESULT de error si falla.
   */
  HRESULT
    CreateRenderTargetView(ID3D11Resource* pResource,
      const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
      ID3D11RenderTargetView** ppRTView);

  /**
   * @brief Crea una textura 2D.
   * @param pDesc        Descriptor completo de la textura.
   * @param pInitialData Datos iniciales (opcional).
   * @param ppTexture2D  Salida: puntero a la textura creada.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Crea una Depth Stencil View (DSV).
   * @param pResource          Textura con @c D3D11_BIND_DEPTH_STENCIL.
   * @param pDesc              Descriptor (o @c nullptr).
   * @param ppDepthStencilView Salida: DSV creada.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateDepthStencilView(ID3D11Resource* pResource,
      const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
      ID3D11DepthStencilView** ppDepthStencilView);

  /**
   * @brief Crea un Vertex Shader.
   * @param pShaderBytecode Bytecode compilado.
   * @param BytecodeLength  Tamaño del bytecode.
   * @param pClassLinkage   Linkage (opcional).
   * @param ppVertexShader  Salida: VS creado.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateVertexShader(const void* pShaderBytecode,
      unsigned int BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11VertexShader** ppVertexShader);

  /**
   * @brief Crea un Input Layout (formato de vértices).
   * @param pInputElementDescs Array de elementos de entrada.
   * @param NumElements        Cuántos elementos hay.
   * @param pShaderBytecodeWithInputSignature Bytecode con la firma.
   * @param BytecodeLength     Tamaño del bytecode.
   * @param ppInputLayout      Salida: layout creado.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
      unsigned int NumElements,
      const void* pShaderBytecodeWithInputSignature,
      unsigned int BytecodeLength,
      ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Crea un Pixel Shader.
   * @param pShaderBytecode Bytecode compilado.
   * @param BytecodeLength  Tamaño del bytecode.
   * @param pClassLinkage   Linkage (opcional).
   * @param ppPixelShader   Salida: PS creado.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreatePixelShader(const void* pShaderBytecode,
      unsigned int BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11PixelShader** ppPixelShader);

  /**
   * @brief Crea un buffer (vertex/index/constant/structured…).
   * @param pDesc        Descriptor del buffer.
   * @param pInitialData Datos iniciales (opcional).
   * @param ppBuffer     Salida: buffer creado.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Buffer** ppBuffer);

  /**
   * @brief Crea un Sampler State.
   * @param pSamplerDesc  Descriptor del sampler.
   * @param ppSamplerState Salida: sampler creado.
   * @return @c S_OK si ok; @c HRESULT si falla.
   */
  HRESULT
    CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
      ID3D11SamplerState** ppSamplerState);

public:
  /**
   * @brief Dispositivo D3D11 real. Se crea en @ref init y se libera en @ref destroy.
   */
  ID3D11Device* m_device = nullptr;
};
