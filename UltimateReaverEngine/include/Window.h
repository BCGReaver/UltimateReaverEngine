#pragma once
#include "Prerequisites.h"

/**
 * @file Window.h
 * @brief Ventana Win32 que usamos para arrancar Direct3D y dibujar.
 *
 * @details
 * Esta clase arma una ventana clásica de Windows y guarda sus datos básicos
 * (HWND, ancho, alto). La idea es que sea un wrapper sencillo para el
 * proyecto **UltimateReaverEngine**.
 *
 * Estilo rápido:
 * - camelCase para métodos (init, update, render, destroy).
 * - Miembros privados con prefijo m_.
 * - Indentación de 2 espacios.
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
   * @param nCmdShow   Cómo mostrar la ventana (normal, maximizada, etc.).
   * @param wndproc    Procedimiento de ventana (callback para mensajes).
   * @return HRESULT   S_OK si todo ok; algún código de error si falla.
   *
   * @details
   * Registra la clase de ventana, crea el HWND, hace Show/Update y calcula
   * el tamaño del cliente (para m_width/m_height).
   */
  HRESULT
    init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief Lógica por frame de la ventana (si necesitas hacer algo aquí).
   */
  void
    update();

  /**
   * @brief Render por frame (normalmente vacío, el render “real” va en la app).
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
   * @brief Ancho del área de cliente (en píxeles).
   */
  unsigned int m_width;

  /**
   * @brief Alto del área de cliente (en píxeles).
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
   * @brief Título de la ventana (mostrar en la barra de título).
   */
  std::string m_windowName = "UltimateReaverEngine";
};
