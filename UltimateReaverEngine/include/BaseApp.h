/**
 * @file BaseApp.h
 * @brief Aquí defino la clase BaseApp, que funciona como el punto base de ejecución del motor.
 *
 * @details
 *  Esta clase es la que maneja todo el ciclo de vida del programa: inicializa Direct3D, crea la ventana,
 *  actualiza la escena, la renderiza y destruye todo al final.
 *  Básicamente, es el “main” del motor, pero encapsulado en una clase ordenada y modular.
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
#include "ModelLoader.h"

 /**
  * @class BaseApp
  * @brief Clase principal del motor que maneja la inicialización, el ciclo de juego y la renderización.
  *
  * @details
  *  Esta clase la uso como base para cualquier aplicación que haga con el motor.
  *  Aquí se junta todo: la ventana, los dispositivos de Direct3D, los shaders, las texturas,
  *  y hasta el modelo que cargo para renderizar.
  *  Contiene el clásico ciclo de vida de una app de render:
  *  1. `init()` → inicializa todo
  *  2. `update()` → actualiza la lógica
  *  3. `render()` → dibuja la escena
  *  4. `destroy()` → limpia los recursos
  */
class
  BaseApp {
public:

  /**
   * @brief Constructor principal de la aplicación base.
   *
   * @param hInst     Instancia principal del programa (la que recibe WinMain).
   * @param nCmdShow  Modo de visualización de la ventana.
   *
   * @details
   *  Aquí guardo los valores de instancia y configuro el entorno de inicio del motor.
   *  También puedo crear la ventana y preparar todo para `init()`.
   */
  BaseApp(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Destructor de la clase.
   *
   * @details
   *  Llamo a `destroy()` automáticamente para asegurarme de que se limpien los recursos.
   */
  ~BaseApp() { destroy(); }

  /**
   * @brief Función principal que ejecuta la aplicación.
   *
   * @param hInst     Instancia del programa.
   * @param nCmdShow  Modo de visualización.
   *
   * @return int      Código de salida de la app.
   *
   * @details
   *  Esta función inicia el loop principal del motor: inicializa, actualiza, renderiza y escucha eventos.
   *  Es el equivalente al `WinMain`, pero encapsulado dentro de la clase.
   */
  int
    run(HINSTANCE hInst, int nCmdShow);

  /**
   * @brief Inicializo todos los sistemas del motor.
   *
   * @return HRESULT  `S_OK` si todo se inicializó correctamente, o error si algo falló.
   *
   * @details
   *  Aquí creo la ventana, el dispositivo, el contexto, el swap chain y los buffers.
   *  También cargo los shaders, las texturas y el modelo que se va a renderizar.
   */
  HRESULT
    init();

  /**
   * @brief Actualizo la lógica del juego o escena.
   *
   * @param deltaTime  Tiempo transcurrido entre frames.
   *
   * @details
   *  Aquí manejo animaciones, transformaciones y cualquier otra lógica del juego.
   *  Se ejecuta una vez por frame.
   */
  void
    update(float deltaTime);

  /**
   * @brief Renderizo la escena.
   *
   * @details
   *  Aquí limpio los buffers, activo los shaders, seteo las texturas y dibujo la geometría.
   *  Es el corazón visual del motor: todo lo que se vea en pantalla pasa por aquí.
   */
  void
    render();

  /**
   * @brief Destruyo y libero todos los recursos del motor.
   *
   * @details
   *  Libero los buffers, shaders, texturas, vistas y demás objetos de Direct3D.
   *  Siempre lo llamo al final para dejar limpio el contexto.
   */
  void
    destroy();

private:

  /**
   * @brief Procedimiento de ventana (WndProc) para manejar mensajes del sistema.
   *
   * @param hWnd     Handle de la ventana.
   * @param message  Tipo de mensaje (input, resize, close, etc.).
   * @param wParam   Parámetro adicional (depende del mensaje).
   * @param lParam   Parámetro adicional (depende del mensaje).
   *
   * @return LRESULT Resultado del procesamiento del mensaje.
   *
   * @details
   *  Aquí proceso los eventos de Windows: cerrar ventana, redimensionar, teclado, etc.
   *  Es una función estática porque Windows la requiere así, pero internamente
   *  puede redirigir los eventos al motor.
   */
  static LRESULT CALLBACK
    wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
  // ------------------------------------------------------------
  // Componentes base del motor
  // ------------------------------------------------------------

  Window m_window;                  ///< Ventana principal del motor.
  Device m_device;                  ///< Dispositivo Direct3D (crea recursos).
  DeviceContext m_deviceContext;    ///< Contexto de dispositivo (ejecuta las órdenes a la GPU).
  SwapChain m_swapChain;            ///< Administra el intercambio de buffers (back/front).
  Texture m_backBuffer;             ///< Textura del back buffer.
  RenderTargetView m_renderTargetView; ///< Render Target principal.
  Texture m_depthStencil;           ///< Textura usada como buffer de profundidad.
  DepthStencilView m_depthStencilView; ///< Vista del depth stencil.
  Viewport m_viewport;              ///< Área de dibujo (viewport activo).
  ShaderProgram m_shaderProgram;    ///< Shaders usados para renderizar.
  MeshComponent m_mesh;             ///< Malla principal del modelo cargado.
  Buffer m_vertexBuffer;            ///< Buffer de vértices.
  Buffer m_indexBuffer;             ///< Buffer de índices.
  Buffer m_cbNeverChanges;          ///< Constant buffer estático (view).
  Buffer m_cbChangeOnResize;        ///< Constant buffer para cambios de tamaño (projection).
  Buffer m_cbChangesEveryFrame;     ///< Constant buffer dinámico (world y color).
  Texture m_textureCube;            ///< Textura del modelo (por ejemplo, cubo).
  SamplerState m_samplerState;      ///< Estado de muestreo de textura.

  // ------------------------------------------------------------
  // Variables de transformación y color
  // ------------------------------------------------------------
  XMMATRIX m_World;                 ///< Matriz de mundo (posición/rotación del modelo).
  XMMATRIX m_View;                  ///< Matriz de vista (cámara).
  XMMATRIX m_Projection;            ///< Matriz de proyección.
  XMFLOAT4 m_vMeshColor;            ///< Color del modelo.

  // ------------------------------------------------------------
  // Constant buffers estructurados (para enviar a shaders)
  // ------------------------------------------------------------
  CBChangeOnResize cbChangesOnResize; ///< Datos de proyección.
  CBNeverChanges cbNeverChanges;      ///< Datos de vista.
  CBChangesEveryFrame cb;             ///< Datos de mundo y color.

  // ------------------------------------------------------------
  // Herramientas auxiliares
  // ------------------------------------------------------------
  ModelLoader m_modelLoader;         ///< Cargador de modelos 3D (.obj, etc).
};
