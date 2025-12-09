/**
 * @file UserInterface.h
 * @brief Aquí defino la clase UserInterface, que se encarga de manejar ImGui dentro del motor.
 *
 * @details
 *  Esta clase la uso como un puente entre mi motor y Dear ImGui.
 *  Aquí inicializo ImGui, creo los frames, dibujo ventanas (como el inspector)
 *  y destruyo todo al final.
 *  También guardo cuál Actor está seleccionado para poder editar sus propiedades
 *  directamente desde la UI (como la posición, rotación, etc.).
 */

#pragma once
#include "Prerequisites.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <imgui_internal.h>
#include "ECS/Actor.h"

 /**
  * @class UserInterface
  * @brief Clase encargada de toda la interfaz gráfica del editor usando ImGui.
  *
  * @details
  *  Esta clase se conecta con la ventana de Win32 y el Device/DeviceContext de DirectX 11
  *  para poder dibujar overlays con ImGui encima de la escena.
  *
  *  Aquí:
  *  - Inicializo ImGui (`init`)
  *  - Actualizo la UI por frame (`update`)
  *  - La dibujo (`render`)
  *  - La destruyo (`destroy`)
  *
  *  Además, manejo un Actor seleccionado para poder mostrar sus datos en un panel tipo inspector.
  */
class
  UserInterface {
public:

  /**
   * @brief Constructor por defecto.
   *
   * @details
   *  No hago nada todavía aquí. Toda la chamba se hace en `init()`.
   */
  UserInterface() = default;

  /**
   * @brief Destructor por defecto.
   *
   * @details
   *  No llamo a nada aquí directamente, la limpieza real se hace en `destroy()`.
   */
  ~UserInterface() = default;

  /**
   * @brief Inicializo ImGui y lo conecto con Win32 y DirectX 11.
   *
   * @param window         Puntero a la ventana (HWND casteado a void*).
   * @param device         Dispositivo de Direct3D 11.
   * @param deviceContext  Contexto de dispositivo de Direct3D 11.
   *
   * @details
   *  Aquí configuro:
   *  - Contexto de ImGui
   *  - Backend Win32 (inputs, teclado, mouse)
   *  - Backend DX11 (render de la UI)
   *  Con esto ya puedo empezar a dibujar paneles de editor encima del juego.
   */
  void
    init(void* window, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

  /**
   * @brief Actualizo la UI en cada frame.
   *
   * @details
   *  Aquí empiezo un nuevo frame de ImGui y defino las ventanas/paneles
   *  que quiero mostrar, como el inspector del Actor seleccionado.
   */
  void
    update();

  /**
   * @brief Renderizo la UI en pantalla.
   *
   * @details
   *  Al final del frame, le digo a ImGui que dibuje todo lo que se definió
   *  en `update()` usando DirectX 11.
   */
  void
    render();

  /**
   * @brief Destruyo y libero todos los recursos asociados a ImGui.
   *
   * @details
   *  Aquí cierro los backends de Win32 y DX11, y destruyo el contexto de ImGui.
   *  Lo llamo cuando cierro el motor para limpiar todo correctamente.
   */
  void
    destroy();

  /**
   * @brief Control personalizado para editar un `vec3` desde la UI.
   *
   * @param label         Etiqueta del control (por ejemplo "Position").
   * @param values        Arreglo de 3 floats (x, y, z) que quiero editar.
   * @param resetValues   Valor al que puedo resetear cada componente (por default 0.0f).
   * @param columnWidth   Ancho de la columna de la etiqueta en la UI.
   *
   * @return true si se modificó algún valor, false si se quedó igual.
   *
   * @details
   *  Esta función la uso para exponer cosas tipo posición/rotación/escala
   *  de manera más cómoda. Normalmente la uso con el Transform del Actor seleccionado.
   */
  bool
    vec3Control(const std::string& label,
      float* values,
      float resetValues = 0.0f,
      float columnWidth = 100.0f);

  /**
   * @brief Defino qué Actor está seleccionado en el editor.
   *
   * @param actor Puntero al Actor que quiero inspeccionar/editar desde la UI.
   *
   * @details
   *  Lo uso para que el panel de inspector sepa qué objeto mostrar y editar.
   */
  void
    setSelectedActor(Actor* actor) { m_selectedActor = actor; }

private:

  /// @brief Actor actualmente seleccionado en el editor (para mostrar info en la UI).
  Actor* m_selectedActor = nullptr;
};
