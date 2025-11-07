#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @file SwapChain.h
 * @brief Wrapper del swap chain de DXGI para presentar en ventana dentro de **UltimateReaverEngine**.
 *
 * @details
 * Este compa maneja el ciclo de buffers (front/back) para que lo que renders se vea en pantalla.
 * Aquí armamos el swap chain, checamos MSAA, agarramos el back buffer y luego presentamos.
 * Todo en plan directo y legible, con camelCase y m_ para miembros.
 */
class
  SwapChain {
public:
  /**
   * @brief Ctor por defecto (no crea nada todavía).
   */
  SwapChain() = default;

  /**
   * @brief Dtor por defecto (recursos se sueltan con destroy()).
   */
  ~SwapChain() = default;

  /**
   * @brief Crea el swap chain y saca el back buffer asociado a una ventana.
   *
   * @param device         Dispositivo D3D11 (con este se crean las cosas).
   * @param deviceContext  Contexto del dispositivo (para binds y state).
   * @param backBuffer     Textura donde cae el render (el back buffer real).
   * @param window         Ventana donde se va a presentar todo.
   * @return S_OK si todo fino; HRESULT de error si truena.
   *
   * @post Si regresa S_OK, m_swapChain != nullptr y el backBuffer ya está listo.
   */
  HRESULT
    init(Device& device,
      DeviceContext& deviceContext,
      Texture& backBuffer,
      Window window);

  /**
   * @brief Hook para actualizar ajustes del swap chain (resize/MSAA, etc).
   * @note Por ahora es un placeholder sin lógica.
   */
  void
    update();

  /**
   * @brief Hook de render del swap chain (útil para debug/sync si lo necesitas).
   * @note De momento no hace nada.
   */
  void
    render();

  /**
   * @brief Libera todas las interfaces relacionadas (swap chain, factory, etc.).
   *
   * @post m_swapChain == nullptr y las DXGI interfaces quedan en nullptr.
   */
  void
    destroy();

  /**
   * @brief Presenta el back buffer en pantalla (el famoso .Present()).
   *
   * @note Si quieres V-Sync, ese parámetro se setea al hacer Present en el .cpp.
   */
  void
    present();

public:
  /** @brief Handler principal del swap chain (DXGI). */
  IDXGISwapChain* m_swapChain = nullptr;

  /** @brief Tipo de driver que se terminó usando (hardware, warp, ref). */
  D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;

private:
  /** @brief Feature level al que quedó el device (ej. 11.0). */
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

  /** @brief Cuántas muestras por píxel para MSAA (4 = 4x). */
  unsigned int m_sampleCount;

  /** @brief Niveles de calidad disponibles para ese MSAA. */
  unsigned int m_qualityLevels;

  /** @brief Interfaz DXGI “vista” de device (para llegar a adapter/factory). */
  IDXGIDevice* m_dxgiDevice = nullptr;

  /** @brief El adapter real (tu GPU) visto desde DXGI. */
  IDXGIAdapter* m_dxgiAdapter = nullptr;

  /** @brief Factory de DXGI (sirve para crear el swap chain). */
  IDXGIFactory* m_dxgiFactory = nullptr;
};
