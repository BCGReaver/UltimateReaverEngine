#pragma once
#include "Prerequisites.h"

/**
 * @file Window.h
 * @brief Ventana Win32 que usamos para arrancar Direct3D y dibujar.
 *
 * @details
 * Esta clase arma una ventana cl�sica de Windows y guarda sus datos b�sicos
 * (HWND, ancho, alto). La idea es que sea un wrapper sencillo para el
 * proyecto **UltimateReaverEngine**.
 *
 * Estilo r�pido:
 * - camelCase para m�todos (init, update, render, destroy).
 * - Miembros privados con prefijo m_.
 * - Indentaci�n de 2 espacios.
 */
class
  Window {
public:
  /**
   * @brief Ctor por default (no hace nada hasta que llames a init()).
   */
  Window() = default;

  /**
   * @brief Dtor por default (si quieres liberar algo, hazlo en destroy()).
   */
  ~Window() = default;

  /**
   * @brief Crea y muestra la ventana de la app.
   *
   * @param hInstance  Handle de instancia que te manda WinMain.
   * @param nCmdShow   C�mo mostrar la ventana (normal, maximizada, etc.).
   * @param wndproc    Procedimiento de ventana (callback para mensajes).
   * @return HRESULT   S_OK si todo ok; alg�n c�digo de error si falla.
   *
   * @details
   * Registra la clase de ventana, crea el HWND, hace Show/Update y calcula
   * el tama�o del cliente (para m_width/m_height).
   */
  HRESULT
    init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief L�gica por frame de la ventana (si necesitas hacer algo aqu�).
   */
  void
    update();

  /**
   * @brief Render por frame (normalmente vac�o, el render �real� va en la app).
   */
  void
    render();

  /**
   * @brief Limpieza de recursos propios de la ventana (si aplica).
   */
  void
    destroy();

public:
  /**
   * @brief Handle de la ventana (lo usamos para crear el swap chain).
   */
  HWND m_hWnd = nullptr;

  /**
   * @brief Ancho del �rea de cliente (en p�xeles).
   */
  unsigned int m_width;

  /**
   * @brief Alto del �rea de cliente (en p�xeles).
   */
  unsigned int m_height;

private:
  /**
   * @brief Instancia del proceso, la guardamos para registrar la clase.
   */
  HINSTANCE m_hInst = nullptr;

  /**
   * @brief Rect de cliente, lo usamos para calcular ancho/alto.
   */
  RECT m_rect;

  /**
   * @brief T�tulo de la ventana (mostrar en la barra de t�tulo).
   */
  std::string m_windowName = "UltimateReaverEngine";
};
