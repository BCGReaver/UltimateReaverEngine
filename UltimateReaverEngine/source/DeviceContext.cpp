/**
 * @file DeviceContext.cpp
 * @brief Wrappers del ID3D11DeviceContext para UltimateReaverEngine (con checks y mensajes claros).
 *
 * @details
 * Aquí centralizamos todas las llamadas típicas del contexto de Direct3D 11
 * (set de shaders, buffers, RTs, limpiar, dibujar, etc.) pero con validaciones
 * básicas para no crashear si algo llega nulo y para loguear qué falló.
 *
 * Notita: seguimos camelCase para métodos y prefijo m_ para miembros privados.
 */

#include "DeviceContext.h"

 // ============================================================================
 // destroy
 // ============================================================================
 /**
  * @brief Libera el contexto de dispositivo de D3D11.
  */
void
DeviceContext::destroy() {
  SAFE_RELEASE(m_deviceContext);
}

// ============================================================================
// RSSetViewports
// ============================================================================
/**
 * @brief Setea uno o más viewports en el rasterizer stage.
 * @param NumViewports Cuántos viewports vas a setear.
 * @param pViewports Arreglo de viewports (no puede ser nullptr).
 */
void
DeviceContext::RSSetViewports(unsigned int NumViewports,
  const D3D11_VIEWPORT* pViewports) {
  if (!pViewports) {
    ERROR("DeviceContext", "RSSetViewports", "pViewports is nullptr");
    return;
  }
  m_deviceContext->RSSetViewports(NumViewports, pViewports);
}

// ============================================================================
// PSSetShaderResources
// ============================================================================
/**
 * @brief Asigna SRVs (textures, etc.) al Pixel Shader.
 * @param StartSlot Slot inicial.
 * @param NumViews Número de SRVs.
 * @param ppShaderResourceViews Arreglo con los SRVs (no puede ser nullptr).
 */
void
DeviceContext::PSSetShaderResources(unsigned int StartSlot,
  unsigned int NumViews,
  ID3D11ShaderResourceView* const* ppShaderResourceViews) {
  if (!ppShaderResourceViews) {
    ERROR("DeviceContext", "PSSetShaderResources", "ppShaderResourceViews is nullptr");
    return;
  }
  m_deviceContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

// ============================================================================
// IASetInputLayout
// ============================================================================
/**
 * @brief Define el InputLayout para el Input Assembler.
 * @param pInputLayout Input layout ya creado (no puede ser nullptr).
 */
void
DeviceContext::IASetInputLayout(ID3D11InputLayout* pInputLayout) {
  if (!pInputLayout) {
    ERROR("DeviceContext", "IASetInputLayout", "pInputLayout is nullptr");
    return;
  }
  m_deviceContext->IASetInputLayout(pInputLayout);
}

// ============================================================================
// VSSetShader
// ============================================================================
/**
 * @brief Activa el Vertex Shader.
 * @param pVertexShader Shader a setear (no puede ser nullptr).
 * @param ppClassInstances Arreglo opcional de clases (puede ser nullptr).
 * @param NumClassInstances Número de clases en ppClassInstances.
 */
void
DeviceContext::VSSetShader(ID3D11VertexShader* pVertexShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  if (!pVertexShader) {
    ERROR("DeviceContext", "VSSetShader", "pVertexShader is nullptr");
    return;
  }
  m_deviceContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

// ============================================================================
// PSSetShader
// ============================================================================
/**
 * @brief Activa el Pixel Shader.
 * @param pPixelShader Shader a setear (no puede ser nullptr).
 * @param ppClassInstances Arreglo opcional de clases (puede ser nullptr).
 * @param NumClassInstances Número de clases en ppClassInstances.
 */
void
DeviceContext::PSSetShader(ID3D11PixelShader* pPixelShader,
  ID3D11ClassInstance* const* ppClassInstances,
  unsigned int NumClassInstances) {
  if (!pPixelShader) {
    ERROR("DeviceContext", "PSSetShader", "pPixelShader is nullptr");
    return;
  }
  m_deviceContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

// ============================================================================
// UpdateSubresource
// ============================================================================
/**
 * @brief Sube datos a un recurso de GPU (constant buffers, texturas, etc.).
 * @param pDstResource Recurso destino (no puede ser nullptr).
 * @param DstSubresource Subrecurso destino.
 * @param pDstBox Región destino (puede ser nullptr para todo).
 * @param pSrcData Puntero a los datos a copiar (no puede ser nullptr).
 * @param SrcRowPitch Pitch por fila.
 * @param SrcDepthPitch Pitch por profundidad.
 */
void
DeviceContext::UpdateSubresource(ID3D11Resource* pDstResource,
  unsigned int DstSubresource,
  const D3D11_BOX* pDstBox,
  const void* pSrcData,
  unsigned int SrcRowPitch,
  unsigned int SrcDepthPitch) {
  if (!pDstResource || !pSrcData) {
    ERROR("DeviceContext", "UpdateSubresource",
      "Invalid arguments: pDstResource or pSrcData is nullptr");
    return;
  }
  m_deviceContext->UpdateSubresource(pDstResource,
    DstSubresource,
    pDstBox,
    pSrcData,
    SrcRowPitch,
    SrcDepthPitch);
}

// ============================================================================
// IASetVertexBuffers
// ============================================================================
/**
 * @brief Asigna los vertex buffers al Input Assembler.
 * @param StartSlot Slot de inicio.
 * @param NumBuffers Cantidad de buffers.
 * @param ppVertexBuffers Arreglo de buffers (no puede ser nullptr).
 * @param pStrides Arreglo con los strides por buffer (no puede ser nullptr).
 * @param pOffsets Arreglo con los offsets por buffer (no puede ser nullptr).
 */
void
DeviceContext::IASetVertexBuffers(unsigned int StartSlot,
  unsigned int NumBuffers,
  ID3D11Buffer* const* ppVertexBuffers,
  const unsigned int* pStrides,
  const unsigned int* pOffsets) {
  if (!ppVertexBuffers || !pStrides || !pOffsets) {
    ERROR("DeviceContext", "IASetVertexBuffers",
      "Invalid arguments: ppVertexBuffers, pStrides, or pOffsets is nullptr");
    return;
  }
  m_deviceContext->IASetVertexBuffers(StartSlot,
    NumBuffers,
    ppVertexBuffers,
    pStrides,
    pOffsets);
}

// ============================================================================
// IASetIndexBuffer
// ============================================================================
/**
 * @brief Define el index buffer a usar por el Input Assembler.
 * @param pIndexBuffer Buffer de índices (no puede ser nullptr).
 * @param Format Formato de índice (R16/R32).
 * @param Offset Offset inicial en el buffer.
 */
void
DeviceContext::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
  DXGI_FORMAT Format,
  unsigned int Offset) {
  if (!pIndexBuffer) {
    ERROR("DeviceContext", "IASetIndexBuffer", "pIndexBuffer is nullptr");
    return;
  }
  m_deviceContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

// ============================================================================
// PSSetSamplers
// ============================================================================
/**
 * @brief Asigna estados de muestreo al Pixel Shader.
 * @param StartSlot Slot inicial.
 * @param NumSamplers Cantidad de samplers.
 * @param ppSamplers Arreglo de samplers (no puede ser nullptr).
 */
void
DeviceContext::PSSetSamplers(unsigned int StartSlot,
  unsigned int NumSamplers,
  ID3D11SamplerState* const* ppSamplers) {
  if (!ppSamplers) {
    ERROR("DeviceContext", "PSSetSamplers", "ppSamplers is nullptr");
    return;
  }
  m_deviceContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

// ============================================================================
// RSSetState
// ============================================================================
/**
 * @brief Setea el rasterizer state (culling, fill mode, etc.).
 * @param pRasterizerState Estado a activar (no puede ser nullptr).
 */
void
DeviceContext::RSSetState(ID3D11RasterizerState* pRasterizerState) {
  if (!pRasterizerState) {
    ERROR("DeviceContext", "RSSetState", "pRasterizerState is nullptr");
    return;
  }
  m_deviceContext->RSSetState(pRasterizerState);
}

// ============================================================================
// OMSetBlendState
// ============================================================================
/**
 * @brief Define el blend state para la etapa de Output Merger.
 * @param pBlendState Estado de blending (no puede ser nullptr).
 * @param BlendFactor Factor de blend (RGBA, puede ser nullptr).
 * @param SampleMask Máscara de sampleo.
 */
void
DeviceContext::OMSetBlendState(ID3D11BlendState* pBlendState,
  const float BlendFactor[4],
  unsigned int SampleMask) {
  if (!pBlendState) {
    ERROR("DeviceContext", "OMSetBlendState", "pBlendState is nullptr");
    return;
  }
  m_deviceContext->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
}

// ============================================================================
// OMSetRenderTargets
// ============================================================================
/**
 * @brief Asigna render targets y depth-stencil al Output Merger.
 * @param NumViews Cantidad de RTVs.
 * @param ppRenderTargetViews Lista de RTVs (puede ser nullptr si NumViews=0).
 * @param pDepthStencilView DSV a usar (puede ser nullptr si no usas depth).
 */
void
DeviceContext::OMSetRenderTargets(unsigned int NumViews,
  ID3D11RenderTargetView* const* ppRenderTargetViews,
  ID3D11DepthStencilView* pDepthStencilView) {
  if (!ppRenderTargetViews && !pDepthStencilView) {
    ERROR("DeviceContext", "OMSetRenderTargets",
      "Both ppRenderTargetViews and pDepthStencilView are nullptr");
    return;
  }

  if (NumViews > 0 && !ppRenderTargetViews) {
    ERROR("DeviceContext", "OMSetRenderTargets",
      "ppRenderTargetViews is nullptr, but NumViews > 0");
    return;
  }

  m_deviceContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

// ============================================================================
// IASetPrimitiveTopology
// ============================================================================
/**
 * @brief Define la topología de primitivas (triangles, lines, etc.).
 * @param Topology Topología a usar (no puede ser UNDEFINED).
 */
void
DeviceContext::IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) {
  if (Topology == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {
    ERROR("DeviceContext", "IASetPrimitiveTopology",
      "Topology is D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED");
    return;
  }
  m_deviceContext->IASetPrimitiveTopology(Topology);
}

// ============================================================================
// ClearRenderTargetView
// ============================================================================
/**
 * @brief Limpia un RenderTargetView con un color.
 * @param pRenderTargetView RTV a limpiar (no puede ser nullptr).
 * @param ColorRGBA Color (RGBA) de limpieza (no puede ser nullptr).
 */
void
DeviceContext::ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
  const float ColorRGBA[4]) {
  if (!pRenderTargetView) {
    ERROR("DeviceContext", "ClearRenderTargetView", "pRenderTargetView is nullptr");
    return;
  }
  if (!ColorRGBA) {
    ERROR("DeviceContext", "ClearRenderTargetView", "ColorRGBA is nullptr");
    return;
  }
  m_deviceContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

// ============================================================================
// ClearDepthStencilView
// ============================================================================
/**
 * @brief Limpia el depth-stencil con las banderas indicadas.
 * @param pDepthStencilView DSV a limpiar (no puede ser nullptr).
 * @param ClearFlags Banderas (D3D11_CLEAR_DEPTH / D3D11_CLEAR_STENCIL).
 * @param Depth Valor de profundidad para limpiar.
 * @param Stencil Valor de stencil para limpiar.
 */
void
DeviceContext::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
  unsigned int ClearFlags,
  float Depth,
  UINT8 Stencil) {
  if (!pDepthStencilView) {
    ERROR("DeviceContext", "ClearDepthStencilView",
      "pDepthStencilView is nullptr");
    return;
  }
  if ((ClearFlags & (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL)) == 0) {
    ERROR("DeviceContext", "ClearDepthStencilView",
      "Invalid ClearFlags: must include D3D11_CLEAR_DEPTH or D3D11_CLEAR_STENCIL");
    return;
  }
  m_deviceContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

// ============================================================================
// VSSetConstantBuffers
// ============================================================================
/**
 * @brief Asigna constant buffers al Vertex Shader.
 * @param StartSlot Slot inicial.
 * @param NumBuffers Cantidad de buffers.
 * @param ppConstantBuffers Arreglo de buffers (no puede ser nullptr).
 */
void
DeviceContext::VSSetConstantBuffers(unsigned int StartSlot,
  unsigned int NumBuffers,
  ID3D11Buffer* const* ppConstantBuffers) {
  if (!ppConstantBuffers) {
    ERROR("DeviceContext", "VSSetConstantBuffers", "ppConstantBuffers is nullptr");
    return;
  }
  m_deviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

// ============================================================================
// PSSetConstantBuffers
// ============================================================================
/**
 * @brief Asigna constant buffers al Pixel Shader.
 * @param StartSlot Slot inicial.
 * @param NumBuffers Cantidad de buffers.
 * @param ppConstantBuffers Arreglo de buffers (no puede ser nullptr).
 */
void
DeviceContext::PSSetConstantBuffers(unsigned int StartSlot,
  unsigned int NumBuffers,
  ID3D11Buffer* const* ppConstantBuffers) {
  if (!ppConstantBuffers) {
    ERROR("DeviceContext", "PSSetConstantBuffers", "ppConstantBuffers is nullptr");
    return;
  }
  m_deviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

// ============================================================================
// DrawIndexed
// ============================================================================
/**
 * @brief Dibuja usando índice (DrawIndexed de toda la vida).
 * @param IndexCount Número de índices a dibujar (no puede ser 0).
 * @param StartIndexLocation Índice inicial.
 * @param BaseVertexLocation Offset base del vértice.
 */
void
DeviceContext::DrawIndexed(unsigned int IndexCount,
  unsigned int StartIndexLocation,
  int BaseVertexLocation) {
  if (IndexCount == 0) {
    ERROR("DeviceContext", "DrawIndexed", "IndexCount is zero");
    return;
  }
  m_deviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}
