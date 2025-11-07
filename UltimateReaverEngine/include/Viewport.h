/**
 * @file Viewport.h
 * @brief Aquí dejo todo lo del viewport de Direct3D: básicamente defino el área donde se dibuja.
 *
 * @details
 *  Esta clase la uso para configurar el D3D11_VIEWPORT de forma limpia y rápida.
 *  La idea es que yo pueda inicializarlo ya sea con una ventana (para tomar su tamaño)
 *  o con dimensiones específicas cuando quiero algo custom (por ejemplo, render targets off-screen).
 *  Mantengo el ciclo con update/render/destroy para que se integre bonito al motor.
 */

#pragma once
#include "Prerequisites.h"

class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Administro el viewport que le dice a D3D dónde y de qué tamaño voy a dibujar.
 *
 * @details
 *  Con este compa ajusto el rectángulo de dibujo: posición (TopLeftX/Y), ancho y alto,
 *  y el rango de profundidad (MinDepth/MaxDepth). Normalmente lo pongo a pantalla completa,
 *  pero a veces lo cambio para depurar o para efectos (split-screen, mini-mapas, etc.).
 */
class
  Viewport {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí porque prefiero inicializar el viewport con `init(...)`
   *  cuando ya sé las dimensiones reales.
   */
  Viewport() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  La estructura D3D11_VIEWPORT no necesita liberar nada, pero igual mantengo
   *  el flujo con `destroy()` por si en el futuro agrego recursos asociados.
   */
  ~Viewport() = default;

  /**
   * @brief Inicializo el viewport tomando tamaño desde la ventana.
   *
   * @param window  Referencia a la ventana de la que saco ancho y alto del área cliente.
   * @return HRESULT `S_OK` si quedó bien configurado, o un error si algo no cuadró.
   *
   * @details
   *  Aquí configuro `m_viewport` con el ancho/alto de la ventana y valores default
   *  de profundidad (0.0f a 1.0f). Es la ruta “rápida” para empezar a dibujar a pantalla completa.
   */
  HRESULT
    init(const Window& window);

  /**
   * @brief Inicializo el viewport con dimensiones específicas.
   *
   * @param width   Ancho del viewport en píxeles.
   * @param height  Alto del viewport en píxeles.
   * @return HRESULT `S_OK` si todo salió bien, o un código de error si no.
   *
   * @details
   *  Uso esto cuando quiero controlar el tamaño manualmente (por ejemplo, al renderizar a una textura
   *  o cuando hago vistas divididas). También dejo el rango de profundidad de 0.0f a 1.0f.
   */
  HRESULT
    init(unsigned int width, unsigned int height);

  /**
   * @brief Actualizo el estado del viewport si hiciera falta.
   *
   * @details
   *  Normalmente el viewport no cambia cada frame, pero dejo este método
   *  para mantener el patrón del motor (y por si luego sincronizo con cambios de ventana).
   */
  void
    update();

  /**
   * @brief Enlazo el viewport al pipeline de D3D para que lo use en el draw.
   *
   * @param deviceContext  Contexto de dispositivo donde aplico el `RSSetViewports`.
   *
   * @details
   *  Aquí es donde realmente le digo a Direct3D: “oye, dibuja usando este rectángulo”.
   *  Básicamente seteo `m_viewport` en el rasterizer state del `deviceContext`.
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Limpio o reseteo el viewport si fuera necesario.
   *
   * @details
   *  La estructura en sí no tiene recursos, pero dejo este método para mantener
   *  el ciclo de vida claro y por si luego agrego estado extra relacionado al viewport.
   */
  void
    destroy();

public:

  /// @brief Viewport de D3D11 que realmente usa la GPU para saber dónde dibujar.
  D3D11_VIEWPORT m_viewport;
};
