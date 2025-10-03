#pragma once
/**
 * @file DepthStencilView.h
 * @brief Wrapper del @c ID3D11DepthStencilView para enchufar el depth/stencil al pipeline sin dramas.
 *
 * Parte de **UltimateReaverEngine**.
 * Aquí creas la vista de profundidad/esténcil desde una textura válida, la enlazas al contexto
 * cuando vas a renderizar y la sueltas al final. Directo y sin ruido.
 */

#include "Prerequisites.h"

 // Forward declarations
class Device;
class DeviceContext;
class Texture;

/**
 * @class DepthStencilView
 * @brief Maneja la vida de un @c ID3D11DepthStencilView (crear, usar y destruir).
 */
class
  DepthStencilView {
public:
  /**
   * @brief ctor vacío (no crea nada todavía).
   */
  DepthStencilView() = default;

  /**
   * @brief dtor por defecto (llama destroy() tú mismo).
   */
  ~DepthStencilView() = default;

  /**
   * @brief Crea la vista de depth/stencil desde una textura de profundidad válida.
   *
   * @param device       Dispositivo que fabrica el recurso.
   * @param depthStencil Textura con @c D3D11_BIND_DEPTH_STENCIL.
   * @param format       Formato DXGI de la vista (ej. @c DXGI_FORMAT_D24_UNORM_S8_UINT).
   * @return @c S_OK si salió bien; @c HRESULT de error si algo falló.
   *
   * @post Si retorna @c S_OK, @ref m_depthStencilView queda listo para usarse.
   */
  HRESULT
    init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

  /**
   * @brief Hook de actualización (placeholder por si luego quieres reprovisionar estados).
   */
  void
    update() {}

  /**
   * @brief Limpia el depth/stencil y lo deja activo en el Output-Merger.
   *
   * @param deviceContext Contexto donde se va a usar.
   *
   * @pre @ref m_depthStencilView debe existir (init() ok).
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Suelta el @c ID3D11DepthStencilView.
   *
   * @post @ref m_depthStencilView == nullptr.
   */
  void
    destroy();

public:
  /**
   * @brief Handler COM del depth/stencil view.
   */
  ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
