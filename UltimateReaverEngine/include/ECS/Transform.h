/**
 * @file Transform.h
 * @brief Aquí defino el componente Transform, encargado de la posición,
 *        rotación y escala de cualquier entidad en la escena.
 *
 * @details
 *  Este componente es parte fundamental de mi sistema de componentes, ya que
 *  define dónde está un objeto en el mundo, cómo está rotado y cuál es su tamaño.
 *  Además, calcula la matriz final (World Matrix) que se usará al momento de renderizar.
 */

#pragma once
#include "Prerequisites.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "Component.h"

 /**
  * @class Transform
  * @brief Componente encargado de manejar posición, rotación, escala y matriz del objeto.
  *
  * @details
  *  Este componente se añade a cualquier Entity que necesite transformaciones dentro del mundo.
  *  Durante `update()` recalculo la matriz usando scaling, rotation y translation,
  *  para así obtener la World Matrix final lista para enviarla al shader.
  */
class
  Transform : public Component {
public:

  /**
   * @brief Constructor del Transform.
   *
   * @details
   *  Inicializo los vectores como (0,0,0), la escala por default es 1,
   *  y marco el tipo del componente como TRANSFORM.
   */
  Transform() : position(),
    rotation(),
    scale(),
    matrix(),
    Component(ComponentType::TRANSFORM) {
  }

  /**
   * @brief Inicializo valores por defecto.
   *
   * @details
   *  Pongo la escala a (1,1,1) y la matriz en identidad para arrancar limpio.
   */
  void
    init() {
    scale.one();
    matrix = XMMatrixIdentity();
  }

  /**
   * @brief Actualizo la matriz transform cada frame.
   *
   * @param deltaTime  Tiempo transcurrido desde el último frame.
   *
   * @details
   *  Aquí construyo la matriz usando:
   *  1. Escala
   *  2. Rotación
   *  3. Translación
   *  y al final las multiplico para obtener la World Matrix completa.
   */
  void
    update(float deltaTime) override {

    XMMATRIX scaleMatrix = XMMatrixScaling(scale.x,
      scale.y,
      scale.z);

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x,
      rotation.y,
      rotation.z);

    XMMATRIX translationMatrix = XMMatrixTranslation(position.x,
      position.y,
      position.z);

    // Matrix final (World)
    matrix = scaleMatrix * rotationMatrix * translationMatrix;
  }

  /**
   * @brief El Transform normalmente no dibuja nada, así que no hago nada aquí.
   */
  void
    render(DeviceContext& deviceContext) override {}

  /**
   * @brief Aquí también normalmente no tengo recursos que destruir.
   */
  void
    destroy() {}

  /// @brief Obtengo la posición actual.
  const EU::Vector3&
    getPosition() const { return position; }

  /// @brief Cambio la posición completa.
  void
    setPosition(const EU::Vector3& newPos) { position = newPos; }

  /// @brief Obtengo la rotación actual.
  const EU::Vector3&
    getRotation() const { return rotation; }

  /// @brief Cambio la rotación completa.
  void
    setRotation(const EU::Vector3& newRot) { rotation = newRot; }

  /// @brief Obtengo el valor actual de escala.
  const EU::Vector3&
    getScale() const { return scale; }

  /// @brief Cambio la escala completa.
  void
    setScale(const EU::Vector3& newScale) { scale = newScale; }

  /**
   * @brief Set completo de transformación (posición, rotación y escala).
   *
   * @param newPos Nueva posición
   * @param newRot Nueva rotación
   * @param newSca Nueva escala
   *
   * @details
   *  Es muy útil cuando quiero mover el objeto de golpe sin modificar cada valor por separado.
   */
  void
    setTransform(const EU::Vector3& newPos,
      const EU::Vector3& newRot,
      const EU::Vector3& newSca) {
    position = newPos;
    rotation = newRot;
    scale = newSca;
  }

  /**
   * @brief Muevo (acumulo) la posición actual usando un vector.
   */
  void
    translate(const EU::Vector3& translation);

private:

  /// @brief Posición del objeto en el mundo.
  EU::Vector3 position;

  /// @brief Rotación del objeto (en radianes).
  EU::Vector3 rotation;

  /// @brief Escala del objeto.
  EU::Vector3 scale;

public:

  /// @brief Matriz final (World Matrix) lista para enviar al shader.
  XMMATRIX matrix;
};
