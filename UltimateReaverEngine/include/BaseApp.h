/**
 * @file BaseApp.h
 * @brief Aquí defino la clase BaseApp, que es básicamente el “núcleo” de mi motor.
 *
 * @details
 *  Esta clase engloba todo el ciclo de vida de mi aplicación: inicialización,
 *  actualización, render y destrucción. Aquí es donde se crea la ventana,
 *  el dispositivo D3D11, el swap chain y todo lo necesario para mostrar modelos
 *  y también usar la interfaz gráfica del editor (ImGui).
 */

#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h";
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"
#include "UserInterface.h"

 /**
  * @class BaseApp
  * @brief Clase principal que maneja la ejecución general de mi motor gráfico.
  *
  * @details
  *  Aquí voy controlando los distintos subsistemas:
  *  - Render (D3D11)
  *  - Ventana
  *  - Swap chain
  *  - Shaders
  *  - Texturas
  *  - Modelos y Actores
  *  - Interfaz de usuario con ImGui
  *
  *  Esta clase sirve como punto de entrada y también como contenedor principal
  *  para todos los objetos centrales del motor.
  */
class
  BaseApp {
public:

  /**
   * @brief Constructor por defecto.
   * @details No hago nada aquí, prefiero inicializar todo en `init()`.
   */
  BaseApp() = default;

  /**
   * @brief Destructor de la aplicación.
   * @details Aquí llamo a destroy() para limpiar recursos.
   */
  ~BaseApp() { destroy(); }

  /**
   * @brief Ejecuta el loop principal de la aplicación.
   *
   * @param hInst     Instancia del programa
   * @param nCmdShow  Modo de visualización
   * @return int      Código de salida
   *
   * @details
   *  Aquí corro todo el ciclo del motor:
   *  - Inicialización
   *  - Mensajes de Windows
   *  - Update
   *  - Render
   */
  int
    run(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Inicializo todos los sistemas del motor.
   * @return HRESULT  S_OK si todo salió bien.
   */
  HRESULT
    init();

  /**
   * @brief Actualización por frame.
   *
   * @param deltaTime tiempo transcurrido desde el frame anterior.
   */
  void
    update(float deltaTime);

  /**
   * @brief Renderizo la escena cada frame.
   */
  void
    render();

  /**
   * @brief Libero todos los recursos del motor.
   *
   * @details
   *  Aquí destruyo:
   *  - Buffers
   *  - Texturas
   *  - Vistas
   *  - UI
   *  - etc.
   */
  void
    destroy();

private:
  /**
   * @brief Procedimiento de la ventana (Win32)
   *
   * @details
   *  Aquí recibo eventos como teclado, redimensionar o cerrar ventana.
   */
  static LRESULT CALLBACK
    wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
  // --- subsistemas base ---
  Window m_window;            ///< Ventana principal
  Device m_device;            ///< Dispositivo DirectX11
  DeviceContext m_deviceContext; ///< Contexto de dispositivo (GPU)
  SwapChain m_swapChain;      ///< Maneja front/back buffer

  // --- targets y depth ---
  Texture m_backBuffer;
  RenderTargetView m_renderTargetView;
  Texture m_depthStencil;
  DepthStencilView m_depthStencilView;

  // --- viewport ---
  Viewport m_viewport;

  // --- shader principal ---
  ShaderProgram m_shaderProgram;

  // --- constant buffers ---
  Buffer m_cbNeverChanges;
  Buffer m_cbChangeOnResize;

  // --- textura del modelo principal (avión) ---
  Texture m_abeBowserAlbedo;

  // --- matrices cámara ---
  XMMATRIX m_View;
  XMMATRIX m_Projection;

  // --- actores de la escena ---
  std::vector<EU::TSharedPointer<Actor>> m_actors;
  EU::TSharedPointer<Actor> m_abeBowser;

  // --- modelo cargado ---
  Model3D* m_model;

  // --- data para constant buffers ---
  CBChangeOnResize cbChangesOnResize;
  CBNeverChanges cbNeverChanges;

  // --- interfaz gráfica ---
  UserInterface m_userInterface;
};
