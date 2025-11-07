/**
 * @file DeviceContext.h
 * @brief Aquí defino la clase DeviceContext, que se encarga de manejar el contexto de dibujo de Direct3D.
 *
 * @details
 *  Esta clase la uso para acceder a todas las funciones de renderizado de Direct3D 11.
 *  Es la que realmente le da órdenes a la GPU: setear shaders, buffers, texturas, limpiar la pantalla y dibujar.
 *  En pocas palabras, el `DeviceContext` es el que ejecuta todo lo que preparan las demás clases del motor.
 */

#pragma once
#include "Prerequisites.h"

 /**
  * @class DeviceContext
  * @brief Clase encargada de controlar el contexto de dispositivo de Direct3D 11.
  *
  * @details
  *  El DeviceContext es básicamente el “control remoto” que le da órdenes a la GPU.
  *  Todo lo que implique dibujar, cambiar de shaders, limpiar buffers o actualizar constantes pasa por aquí.
  *  Por eso, esta clase tiene un montón de funciones que envuelven las llamadas nativas de Direct3D,
  *  pero de forma más limpia y centralizada.
  */
class
  DeviceContext {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí todavía, ya que el contexto viene del `Device` y se asigna más adelante.
   */
  DeviceContext() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente, lo hago en `destroy()` para mantener control sobre el ciclo de vida.
   */
  ~DeviceContext() = default;

  /**
   * @brief Inicializo el contexto de dispositivo.
   *
   * @details
   *  Normalmente no lo creo desde cero, sino que lo obtengo del `Device` principal.
   *  Pero este método está aquí por consistencia dentro del motor.
   */
  void
    init();

  /**
   * @brief Actualizo el estado del contexto si fuera necesario.
   *
   * @details
   *  En la mayoría de los casos el contexto no cambia cada frame, pero dejo esto por estructura
   *  y por si quiero agregar debug o sincronización más adelante.
   */
  void
    update();

  /**
   * @brief Ejecuto las operaciones de render.
   *
   * @details
   *  Este método sería el que coordina todas las llamadas de dibujo dentro del frame.
   *  En motores más grandes, desde aquí se lanzarían las draw calls.
   */
  void
    render();

  /**
   * @brief Destruyo el contexto de dispositivo.
   *
   * @details
   *  Suelto el puntero de `m_deviceContext` y limpio memoria.
   *  Importante hacerlo antes de cerrar Direct3D.
   */
  void
    destroy();

  // ====== Métodos para configurar el pipeline gráfico ======

  /**
   * @brief Configuro los viewports activos en el rasterizer.
   */
  void
    RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

  /**
   * @brief Asigno recursos de textura al pixel shader.
   */
  void
    PSSetShaderResources(unsigned int StartSlot,
      unsigned int NumViews,
      ID3D11ShaderResourceView* const* ppShaderResourceViews);

  /**
   * @brief Asigno el input layout actual (estructura de los vértices).
   */
  void
    IASetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Activo un vertex shader.
   */
  void
    VSSetShader(ID3D11VertexShader* pVertexShader,
      ID3D11ClassInstance* const* ppClassInstances,
      unsigned int NumClassInstances);

  /**
   * @brief Activo un pixel shader.
   */
  void
    PSSetShader(ID3D11PixelShader* pPixelShader,
      ID3D11ClassInstance* const* ppClassInstances,
      unsigned int NumClassInstances);

  /**
   * @brief Actualizo un recurso en la GPU con datos nuevos desde CPU.
   */
  void
    UpdateSubresource(ID3D11Resource* pDstResource,
      unsigned int DstSubresource,
      const D3D11_BOX* pDstBox,
      const void* pSrcData,
      unsigned int SrcRowPitch,
      unsigned int SrcDepthPitch);

  /**
   * @brief Asigno buffers de vértices al Input Assembler.
   */
  void
    IASetVertexBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppVertexBuffers,
      const unsigned int* pStrides,
      const unsigned int* pOffsets);

  /**
   * @brief Asigno el buffer de índices al Input Assembler.
   */
  void
    IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
      DXGI_FORMAT Format,
      unsigned int Offset);

  /**
   * @brief Activo los samplers para el pixel shader.
   */
  void
    PSSetSamplers(unsigned int StartSlot,
      unsigned int NumSamplers,
      ID3D11SamplerState* const* ppSamplers);

  /**
   * @brief Configuro el estado del rasterizador (cómo se dibujan los triángulos).
   */
  void
    RSSetState(ID3D11RasterizerState* pRasterizerState);

  /**
   * @brief Configuro el estado de mezcla (blending) del pipeline.
   */
  void
    OMSetBlendState(ID3D11BlendState* pBlendState,
      const float BlendFactor[4],
      unsigned int SampleMask);

  /**
   * @brief Asigno los render targets activos.
   */
  void
    OMSetRenderTargets(unsigned int NumViews,
      ID3D11RenderTargetView* const* ppRenderTargetViews,
      ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Defino la topología de los triángulos (por ejemplo, lista, strip, etc.).
   */
  void
    IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  /**
   * @brief Limpio un render target con un color específico.
   */
  void
    ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
      const float ColorRGBA[4]);

  /**
   * @brief Limpio el depth stencil (profundidad y stencil buffer).
   */
  void
    ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
      unsigned int ClearFlags,
      float Depth,
      UINT8 Stencil);

  /**
   * @brief Asigno constant buffers al vertex shader.
   */
  void
    VSSetConstantBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Asigno constant buffers al pixel shader.
   */
  void
    PSSetConstantBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Ejecuto un dibujo indexado en GPU.
   *
   * @details
   *  Esta es la llamada clásica de dibujo: usa el buffer de índices para renderizar los triángulos.
   *  Aquí es donde realmente se pinta todo en pantalla.
   */
  void
    DrawIndexed(unsigned int IndexCount,
      unsigned int StartIndexLocation,
      int BaseVertexLocation);

public:

  /// @brief Puntero al contexto de dispositivo de Direct3D, el que realmente manda los comandos a la GPU.
  ID3D11DeviceContext* m_deviceContext = nullptr;
};
