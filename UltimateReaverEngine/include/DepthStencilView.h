/**
 * @file DepthStencilView.h
 * @brief Aquí defino la clase DepthStencilView, que se encarga de manejar el buffer de profundidad y stencil.
 *
 * @details
 *  Esta clase la uso para crear y administrar la vista de profundidad del motor.
 *  En pocas palabras, el Depth Stencil es el que evita que los objetos que están detrás
 *  de otros se dibujen encima (controla qué está al frente y qué no).
 *  También puede manejar el stencil buffer, que se usa para efectos especiales o máscaras.
 */

#pragma once
#include "Prerequisites.h"

 // Forward declarations
class Device;
class DeviceContext;
class Texture;

/**
 * @class DepthStencilView
 * @brief Clase que administra la vista de profundidad y stencil de Direct3D.
 *
 * @details
 *  Esta clase se encarga de crear y controlar el `ID3D11DepthStencilView`,
 *  que es el buffer donde la GPU guarda la información de profundidad (Z-buffer)
 *  y, si hace falta, el stencil buffer.
 *  Lo uso junto al RenderTargetView para asegurar que los objetos se dibujen
 *  en el orden correcto dentro de la escena.
 */
class
  DepthStencilView {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada todavía aquí, prefiero inicializar la vista en `init()`
   *  cuando ya tengo el dispositivo y la textura listos.
   */
  DepthStencilView() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente, eso lo hago en `destroy()` para tener
   *  control completo sobre cuándo libero recursos en GPU.
   */
  ~DepthStencilView() = default;

  /**
   * @brief Inicializo la vista de profundidad (Depth Stencil View).
   *
   * @param device        Dispositivo de Direct3D que crea el recurso.
   * @param depthStencil  Textura que se va a usar como buffer de profundidad.
   * @param format        Formato del buffer (por ejemplo DXGI_FORMAT_D24_UNORM_S8_UINT).
   *
   * @return HRESULT      `S_OK` si todo salió bien, o código de error si algo falló.
   *
   * @details
   *  Aquí creo el objeto `ID3D11DepthStencilView` a partir de la textura base.
   *  Básicamente le digo a la GPU: “usa esta textura para controlar la profundidad de los objetos”.
   *  Si no tuviera esto, los modelos se dibujarían encima sin importar su posición.
   */
  HRESULT
    init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

  /**
   * @brief Actualizo el estado del depth stencil.
   *
   * @details
   *  En este caso no hace falta actualizarlo cada frame, así que lo dejo vacío por ahora.
   *  Pero lo mantengo definido por consistencia con el ciclo del motor.
   */
  void
    update() {}

  /**
   * @brief Activo la vista de profundidad en el pipeline de render.
   *
   * @param deviceContext  Contexto de dibujo donde aplico la vista.
   *
   * @details
   *  Aquí vinculo la vista de profundidad para que la GPU la use durante el render.
   *  Es lo que permite que Direct3D sepa dónde detener el dibujo de objetos que están detrás de otros.
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Destruyo la vista de profundidad y libero los recursos.
   *
   * @details
   *  Aquí libero el puntero `m_depthStencilView` con `Release()` para evitar fugas de memoria.
   *  Siempre lo llamo antes de cerrar el motor o cambiar de render target.
   */
  void
    destroy();

public:

  /// @brief Puntero al objeto Depth Stencil View, encargado de manejar la profundidad y el stencil buffer.
  ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
