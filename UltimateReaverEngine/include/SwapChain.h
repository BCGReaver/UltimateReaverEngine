/**
 * @file SwapChain.h
 * @brief Aquí defino la clase SwapChain, que básicamente es la encargada de manejar los buffers de pantalla.
 *
 * @details
 *  Esta clase la uso para conectar el render del motor con la ventana del sistema operativo.
 *  El swap chain es el que se encarga de presentar lo que dibujo en la GPU hacia la pantalla.
 *  Aquí manejo la inicialización, actualización y el clásico “present” que intercambia los buffers.
 */

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @class SwapChain
 * @brief Administro el ciclo de presentación de imágenes en pantalla (double buffering).
 *
 * @details
 *  La idea del swap chain es sencilla: tengo un buffer donde dibujo (back buffer)
 *  y otro que está en pantalla (front buffer). Cada frame, los intercambio con `present()`.
 *  Esta clase me ayuda a mantener ese proceso encapsulado y limpio, sin tener que tocar
 *  directamente las APIs de DXGI.
 */
class
  SwapChain {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí porque la creación real del swap chain se hace en `init()`.
   *  Prefiero mantener el constructor liviano para evitar inicializaciones forzadas.
   */
  SwapChain() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente, lo hago en `destroy()`
   *  para tener control total del ciclo de vida del objeto.
   */
  ~SwapChain() = default;

  /**
   * @brief Inicializo el swap chain conectándolo con la ventana del motor.
   *
   * @param device          Dispositivo de Direct3D (lo uso para crear el swap chain).
   * @param deviceContext   Contexto del dispositivo, necesario para operaciones gráficas.
   * @param backBuffer      Referencia al buffer donde se dibuja (el back buffer).
   * @param window          Ventana destino donde se mostrará el contenido.
   *
   * @return HRESULT        `S_OK` si se creó correctamente, o código de error si algo falló.
   *
   * @details
   *  Aquí creo el swap chain usando las interfaces de DXGI (Device, Adapter, Factory).
   *  Básicamente dejo todo configurado para que la GPU pueda presentar frames en la ventana.
   */
  HRESULT
    init(Device& device,
      DeviceContext& deviceContext,
      Texture& backBuffer,
      Window window);

  /**
   * @brief Actualizo el estado del swap chain.
   *
   * @details
   *  Normalmente no hace mucho cada frame, pero dejo el método por consistencia
   *  (en caso de querer sincronizar opciones o manejar cambios de ventana dinámica).
   */
  void
    update();

  /**
   * @brief Renderizo el contenido (a nivel swap chain).
   *
   * @details
   *  En esta etapa normalmente no dibujo nada, pero la incluyo para mantener
   *  el flujo clásico del motor (update → render → present). Aquí podría ir
   *  cualquier cosa relacionada con postprocesado o debug visual del swap chain.
   */
  void
    render();

  /**
   * @brief Libero los recursos asociados al swap chain.
   *
   * @details
   *  Cuando ya no necesito el swap chain (por ejemplo, al cerrar la app),
   *  libero las interfaces y dejo todo limpio para evitar leaks.
   */
  void
    destroy();

  /**
   * @brief Presento el frame actual en pantalla.
   *
   * @details
   *  Esta es la parte clave: aquí intercambio el back buffer con el front buffer
   *  para que lo que dibujé se vea realmente en la ventana.
   *  Es la función que hace que la imagen “salte” a pantalla cada frame.
   */
  void
    present();

public:

  /// @brief Puntero principal al swap chain de DXGI, donde se manejan los buffers.
  IDXGISwapChain* m_swapChain = nullptr;

  /// @brief Tipo de driver de Direct3D (hardware, software, referencia, etc).
  D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;

private:

  /// @brief Nivel de características de Direct3D (por defecto uso 11.0).
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

  /// @brief Cantidad de samples para MSAA (antialiasing).
  unsigned int m_sampleCount;

  /// @brief Nivel de calidad para MSAA (0 si no se usa).
  unsigned int m_qualityLevels;

  /// @brief Dispositivo DXGI, lo uso para obtener adaptadores y crear la fábrica.
  IDXGIDevice* m_dxgiDevice = nullptr;

  /// @brief Adaptador de DXGI (la GPU física o virtual que estoy usando).
  IDXGIAdapter* m_dxgiAdapter = nullptr;

  /// @brief Fábrica DXGI, necesaria para crear el swap chain.
  IDXGIFactory* m_dxgiFactory = nullptr;
};
