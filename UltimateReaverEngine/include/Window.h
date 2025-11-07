/**
 * @file Window.h
 * @brief Esta clase la hice para manejar todo lo relacionado con la ventana del motor.
 *
 * @details
 *  Aquí encapsulo todo lo que tiene que ver con crear, actualizar y destruir una ventana de Windows.
 *  Lo hago así para no tener que estar repitiendo el mismo código cada vez que arranco un proyecto.
 *  Básicamente, esta clase se encarga de armar la ventana, mantenerla viva mientras corre el juego,
 *  y cerrarla cuando ya no se necesita.
 */

#pragma once
#include "Prerequisites.h"

 /**
  * @class Window
  * @brief Clase que se encarga de manejar la ventana del motor.
  *
  * @details
  *  Yo diseñé esta clase para tener todo lo del sistema de ventanas centralizado.
  *  Así es más fácil controlarla sin andar peleándome con WinAPI en cada proyecto.
  *  Aquí solo tengo lo esencial: crear, actualizar, renderizar (aunque más simbólico),
  *  y destruir.
  */
class Window {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada todavía aquí porque la ventana se crea después con `init()`.
   *  Básicamente solo existe para tener una instancia lista.
   */
  Window() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No destruyo nada aquí porque eso se hace de forma controlada en `destroy()`.
   *  Pero igual lo dejo declarado para mantener el orden.
   */
  ~Window() = default;

  /**
   * @brief Inicializo la ventana con los parámetros básicos.
   *
   * @param hInstance  Instancia principal del programa.
   * @param nCmdShow   Modo en el que se va a mostrar la ventana.
   * @param wndproc    Procedimiento de ventana (callback para mensajes).
   *
   * @return HRESULT   Devuelve S_OK si todo salió bien, o un código de error si algo falló.
   *
   * @details
   *  Aquí básicamente creo la ventana, la registro, y defino su tamaño.
   *  Este método se encarga de dejar todo listo para que la ventana aparezca.
   */
  HRESULT
    init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief Actualizo la ventana.
   *
   * @details
   *  Aquí normalmente procesaría los mensajes del sistema (teclas, clicks, etc).
   *  La idea es mantener la ventana viva y que el motor no se congele.
   */
  void
    update();

  /**
   * @brief Renderizo el contenido de la ventana.
   *
   * @details
   *  En este punto no dibujo nada directamente, pero dejo el método
   *  porque me sirve para mantener un flujo ordenado dentro del motor.
   */
  void
    render();

  /**
   * @brief Destruyo la ventana y libero sus recursos.
   *
   * @details
   *  Cuando ya no necesito la ventana, llamo a este método para cerrarla bien
   *  y limpiar todo lo que haya quedado asignado.
   */
  void
    destroy();

public:

  /// @brief Handle de la ventana, aquí se guarda el identificador principal (HWND).
  HWND m_hWnd = nullptr;

  /// @brief Ancho de la ventana en píxeles.
  unsigned int m_width;

  /// @brief Alto de la ventana en píxeles.
  unsigned int m_height;

private:

  /// @brief Instancia principal de la aplicación, la uso para crear la ventana.
  HINSTANCE m_hInst = nullptr;

  /// @brief Rectángulo de la ventana (sirve para calcular el área cliente y cosas así).
  RECT m_rect;

  /// @brief Nombre que aparece en la barra de título de la ventana.
  std::string m_windowName = "UltimateReaverEngine";
};
