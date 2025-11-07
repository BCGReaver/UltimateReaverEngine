/**
 * @file RenderTargetView.h
 * @brief Aquí defino la clase RenderTargetView, que se encarga de manejar el buffer donde dibujo la escena.
 *
 * @details
 *  Esta clase la uso para crear y controlar los Render Target Views (RTV), que son básicamente las “pantallas”
 *  en las que la GPU dibuja. En pocas palabras, el RTV es el destino de todo lo que renderizo antes de presentarlo.
 *  Aquí puedo limpiar el color, configurarlo, y vincularlo con el DepthStencilView para dibujar correctamente.
 */

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Clase que administra los render targets, o sea, donde se dibuja la imagen antes de mostrarla.
 *
 * @details
 *  Con esta clase controlo el ciclo de vida de un Render Target View:
 *  - Lo creo desde un back buffer o textura personalizada.
 *  - Lo actualizo o limpio cada frame.
 *  - Lo destruyo cuando ya no se necesita.
 *  También manejo el clear color para borrar el frame antes de dibujar uno nuevo.
 */
class
  RenderTargetView {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada todavía aquí porque prefiero inicializar el RTV en el método `init()`,
   *  cuando ya tengo la textura o el back buffer listos.
   */
  RenderTargetView() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente, eso lo hago con `destroy()` para mantener
   *  el control total del ciclo de vida del recurso en GPU.
   */
  ~RenderTargetView() = default;

  /**
   * @brief Inicializo el Render Target View a partir del back buffer principal.
   *
   * @param device      Dispositivo de Direct3D que uso para crear el RTV.
   * @param backBuffer  Textura base donde se va a dibujar (normalmente la del swap chain).
   * @param Format      Formato de la textura (por ejemplo DXGI_FORMAT_R8G8B8A8_UNORM).
   *
   * @return HRESULT    `S_OK` si se creó correctamente, o código de error si algo falló.
   *
   * @details
   *  Esta función es la más común: crea el render target directamente del back buffer,
   *  o sea, el que se presenta en pantalla. Lo dejo listo para usar en el pipeline.
   */
  HRESULT
    init(Device& device, Texture& backBuffer, DXGI_FORMAT Format);

  /**
   * @brief Inicializo un Render Target View desde una textura personalizada.
   *
   * @param device         Dispositivo de Direct3D para crear el RTV.
   * @param inTex          Textura de entrada (puede ser un render texture o shadow map).
   * @param ViewDimension  Dimensión del RTV (2D, 3D, array, etc).
   * @param Format         Formato de color que va a usar el render target.
   *
   * @return HRESULT       `S_OK` si se configuró bien, o error si algo tronó.
   *
   * @details
   *  Uso esto cuando quiero renderizar a una textura específica, como un framebuffer
   *  intermedio o un mapa de sombras. Así tengo flexibilidad para postprocesos.
   */
  HRESULT
    init(Device& device,
      Texture& inTex,
      D3D11_RTV_DIMENSION ViewDimension,
      DXGI_FORMAT Format);

  /**
   * @brief Actualizo el estado del render target.
   *
   * @details
   *  Normalmente no hay que actualizarlo cada frame, pero dejo este método por consistencia
   *  con el resto del motor. Podría usarse más adelante para cambios de resolución dinámicos.
   */
  void
    update();

  /**
   * @brief Limpio y preparo el render target para empezar a dibujar.
   *
   * @param deviceContext   Contexto de dibujo.
   * @param depthStencilView Vista de profundidad asociada (para limpiar también el z-buffer).
   * @param numViews        Número de RTVs que se van a setear.
   * @param ClearColor      Color de limpieza (RGBA).
   *
   * @details
   *  Aquí uso `ClearRenderTargetView` y `ClearDepthStencilView` para borrar la pantalla.
   *  Es lo primero que hago cada frame antes de dibujar la nueva escena.
   */
  void
    render(DeviceContext& deviceContext,
      DepthStencilView& depthStencilView,
      unsigned int numViews,
      const float ClearColor[4]);

  /**
   * @brief Activo el render target sin limpiar el color.
   *
   * @param deviceContext   Contexto de dibujo.
   * @param numViews        Número de RTVs que quiero activar.
   *
   * @details
   *  Uso este método cuando ya tengo un color renderizado y solo quiero volver a dibujar
   *  encima sin hacer un clear. Es útil para efectos por capas o postprocesado.
   */
  void
    render(DeviceContext& deviceContext,
      unsigned int numViews);

  /**
   * @brief Destruyo el render target y libero sus recursos.
   *
   * @details
   *  Aquí libero el `m_renderTargetView` para no dejar memoria colgando en la GPU.
   *  Siempre llamo a esto antes de cerrar el motor o cambiar de escena.
   */
  void
    destroy();

private:

  /// @brief Puntero al Render Target View de Direct3D. Es donde realmente dibujo la imagen antes del swap.
  ID3D11RenderTargetView* m_renderTargetView = nullptr;
};
