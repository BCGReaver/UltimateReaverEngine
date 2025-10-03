#pragma once
/**
 * @file DeviceContext.h
 * @brief Wrapper chill del contexto inmediato de D3D11 para no estar llamando la API cruda a cada rato.
 *
 * @details
 * Este header es parte de **UltimateReaverEngine**. La idea es tener un lugar
 * donde centralizamos las llamadas típicas del pipeline (set shaders, buffers, samplers,
 * viewports, etc.) con validaciones básicas y nombres claritos.
 */

#include "Prerequisites.h"

 /**
  * @class DeviceContext
  * @brief Envoltura del @c ID3D11DeviceContext (el “inmediato”) con helpers con validación.
  *
  * @details
  * Este contexto es el que manda los comandos de dibujo y estados al GPU.
  * Con esta clase evitas repetir código y reduces crashes por nullptrs,
  * porque cada método valida parámetros antes de llamar a D3D11.
  */
class
  DeviceContext {
public:
  /**
   * @brief Ctor por defecto (no crea nada todavía).
   */
  DeviceContext() = default;

  /**
   * @brief Dtor por defecto (no libera solo; usa @ref destroy).
   */
  ~DeviceContext() = default;

  /**
   * @brief Inicializa el contexto del dispositivo.
   *
   * @details
   * Punto de entrada para enganchar el contexto inmediato creado junto al @c ID3D11Device.
   * (En este proyecto lo asocia el @c SwapChain en su init).
   */
  void
    init();

  /**
   * @brief Update de mantenimiento (placeholder).
   * @note Por si luego quieres stats, marcadores, etc.
   */
  void
    update();

  /**
   * @brief Hook para render/debug (placeholder).
   */
  void
    render();

  /**
   * @brief Libera el contexto inmediato.
   * @post @c m_deviceContext == nullptr.
   */
  void
    destroy();

  // -----------------------------
  // Setters de estado y recursos
  // -----------------------------

  /**
   * @brief Configura los viewports en la etapa de rasterización.
   * @param NumViewports Cuántos viewports vas a setear.
   * @param pViewports   Array con los @c D3D11_VIEWPORT.
   */
  void
    RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

  /**
   * @brief Enlaza SRVs (texturas) al Pixel Shader.
   * @param StartSlot  Primer slot.
   * @param NumViews   Cuántas vistas vas a setear.
   * @param ppShaderResourceViews Array de @c ID3D11ShaderResourceView*.
   */
  void
    PSSetShaderResources(unsigned int StartSlot,
      unsigned int NumViews,
      ID3D11ShaderResourceView* const* ppShaderResourceViews);

  /**
   * @brief Define el Input Layout activo (formato de los vértices).
   * @param pInputLayout Pointer al @c ID3D11InputLayout válido.
   */
  void
    IASetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Asigna el Vertex Shader.
   * @param pVertexShader    Shader compilado.
   * @param ppClassInstances Instancias (HLSL classes), opcional.
   * @param NumClassInstances Número de instancias.
   */
  void
    VSSetShader(ID3D11VertexShader* pVertexShader,
      ID3D11ClassInstance* const* ppClassInstances,
      unsigned int NumClassInstances);

  /**
   * @brief Asigna el Pixel Shader.
   * @param pPixelShader     Shader compilado.
   * @param ppClassInstances Instancias (HLSL classes), opcional.
   * @param NumClassInstances Número de instancias.
   */
  void
    PSSetShader(ID3D11PixelShader* pPixelShader,
      ID3D11ClassInstance* const* ppClassInstances,
      unsigned int NumClassInstances);

  /**
   * @brief Copia datos desde CPU a un recurso en GPU (update rápido).
   * @param pDstResource   Recurso destino.
   * @param DstSubresource Subrecurso destino.
   * @param pDstBox        Región (opcional, nullptr = todo).
   * @param pSrcData       Datos fuente.
   * @param SrcRowPitch    Bytes por fila.
   * @param SrcDepthPitch  Bytes por capa.
   */
  void
    UpdateSubresource(ID3D11Resource* pDstResource,
      unsigned int DstSubresource,
      const D3D11_BOX* pDstBox,
      const void* pSrcData,
      unsigned int SrcRowPitch,
      unsigned int SrcDepthPitch);

  /**
   * @brief Enlaza Vertex Buffers al IA (Input Assembler).
   * @param StartSlot       Slot inicial.
   * @param NumBuffers      Cuántos VBs.
   * @param ppVertexBuffers Array de @c ID3D11Buffer*.
   * @param pStrides        Strides (tamaño del vértice) por VB.
   * @param pOffsets        Offsets iniciales por VB.
   */
  void
    IASetVertexBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppVertexBuffers,
      const unsigned int* pStrides,
      const unsigned int* pOffsets);

  /**
   * @brief Enlaza el Index Buffer al IA.
   * @param pIndexBuffer IB.
   * @param Format       Formato (ej. @c DXGI_FORMAT_R16_UINT).
   * @param Offset       Offset inicial.
   */
  void
    IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
      DXGI_FORMAT Format,
      unsigned int Offset);

  /**
   * @brief Enlaza Samplers al Pixel Shader.
   * @param StartSlot   Slot inicial.
   * @param NumSamplers Cuántos samplers.
   * @param ppSamplers  Array de @c ID3D11SamplerState*.
   */
  void
    PSSetSamplers(unsigned int StartSlot,
      unsigned int NumSamplers,
      ID3D11SamplerState* const* ppSamplers);

  /**
   * @brief Setea el Rasterizer State actual.
   * @param pRasterizerState Estado creado con el device.
   */
  void
    RSSetState(ID3D11RasterizerState* pRasterizerState);

  /**
   * @brief Setea el Blend State en el Output Merger.
   * @param pBlendState Estado de blending.
   * @param BlendFactor Factor RGBA (mezcla constante).
   * @param SampleMask  Máscara de muestras.
   */
  void
    OMSetBlendState(ID3D11BlendState* pBlendState,
      const float BlendFactor[4],
      unsigned int SampleMask);

  /**
   * @brief Enlaza Render Targets y Depth/Stencil al OM.
   * @param NumViews            Cuántos RTVs.
   * @param ppRenderTargetViews Array de RTVs.
   * @param pDepthStencilView   DSV opcional.
   */
  void
    OMSetRenderTargets(unsigned int NumViews,
      ID3D11RenderTargetView* const* ppRenderTargetViews,
      ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Define la topología de primitivas (triángulos, líneas, etc.).
   * @param Topology @c D3D11_PRIMITIVE_TOPOLOGY_*.
   */
  void
    IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  // -----------------------------
  // Clears y constant buffers
  // -----------------------------

  /**
   * @brief Limpia un RT con un color.
   * @param pRenderTargetView RTV válido.
   * @param ColorRGBA         Color en RGBA (0..1).
   */
  void
    ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
      const float ColorRGBA[4]);

  /**
   * @brief Limpia un DSV (depth/stencil).
   * @param pDepthStencilView Vista de profundidad/esténcil.
   * @param ClearFlags        @c D3D11_CLEAR_DEPTH / @c D3D11_CLEAR_STENCIL (o ambos).
   * @param Depth             Valor depth (0..1).
   * @param Stencil           Valor stencil.
   */
  void
    ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
      unsigned int ClearFlags,
      float Depth,
      UINT8 Stencil);

  /**
   * @brief Enlaza constant buffers al VS.
   * @param StartSlot        Slot inicial.
   * @param NumBuffers       Cuántos buffers.
   * @param ppConstantBuffers Array de @c ID3D11Buffer*.
   */
  void
    VSSetConstantBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Enlaza constant buffers al PS.
   * @param StartSlot        Slot inicial.
   * @param NumBuffers       Cuántos buffers.
   * @param ppConstantBuffers Array de @c ID3D11Buffer*.
   */
  void
    PSSetConstantBuffers(unsigned int StartSlot,
      unsigned int NumBuffers,
      ID3D11Buffer* const* ppConstantBuffers);

  // -----------------------------
  // Draw calls
  // -----------------------------

  /**
   * @brief Lanza un draw con índices (lo más común).
   * @param IndexCount         Total de índices a dibujar.
   * @param StartIndexLocation Desde qué índice arrancas.
   * @param BaseVertexLocation Offset base para los vértices.
   */
  void
    DrawIndexed(unsigned int IndexCount,
      unsigned int StartIndexLocation,
      int BaseVertexLocation);

public:
  /**
   * @brief Contexto inmediato de D3D11 (el que manda las órdenes al GPU).
   * @details Queda válido tras @ref init y se libera en @ref destroy.
   */
  ID3D11DeviceContext* m_deviceContext = nullptr;
};
