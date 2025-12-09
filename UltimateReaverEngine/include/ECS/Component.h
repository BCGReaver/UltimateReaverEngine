/**
 * @file Component.h
 * @brief Aquí defino la clase base Component para mi sistema de componentes.
 *
 * @details
 *  Un Component es como una pieza modular que agrego a un Actor o una Entity.
 *  Por ejemplo: MeshComponent, Transform, Collider, etc.
 *  La idea es que cada componente tenga su propia lógica de inicialización,
 *  actualización, render y destrucción.
 *
 *  Esta clase es abstracta, así que nunca instancio Component directamente,
 *  sino que heredo de ella para crear componentes reales.
 */

#pragma once
#include "Prerequisites.h"

class DeviceContext;

/**
 * @class Component
 * @brief Clase base pura para todos los componentes del motor.
 *
 * @details
 *  Cada componente va a tener un tipo (`ComponentType`) para identificar qué es.
 *  Y también define funciones virtuales puras (init/update/render/destroy)
 *  que obligan a las clases hijas a implementar su comportamiento.
 *
 *  También me sirve para poder manejar todos mis componentes de forma polimórfica.
 */
class
  Component {
public:

  /**
   * @brief Constructor por defecto, no hago nada especial aquí.
   */
  Component() = default;

  /**
   * @brief Constructor alternativo para asignar el tipo del componente.
   *
   * @param type Tipo del componente (Transform, Mesh, etc).
   */
  Component(const ComponentType type) : m_type(type) {}

  /**
   * @brief Destructor virtual para soportar herencia correctamente.
   */
  virtual
    ~Component() = default;

  /**
   * @brief Inicializo el componente.
   *
   * @details
   *  Como es una clase abstracta, aquí no implemento nada,
   *  pero obligo a las clases hijas a definir cómo se inicializa.
   */
  virtual void
    init() = 0;

  /**
   * @brief Actualizo la lógica del componente.
   *
   * @param deltaTime Tiempo desde el último frame.
   *
   * @details
   *  Aquí cada componente define su propio comportamiento relacionado con el tiempo.
   */
  virtual void
    update(float deltaTime) = 0;

  /**
   * @brief Renderizo el componente si es necesario.
   *
   * @param deviceContext Contexto usado para enviar comandos a la GPU.
   *
   * @details
   *  No todos los componentes necesitan renderizarse,
   *  pero dejo la función virtual por si alguna clase hija la necesita.
   */
  virtual void
    render(DeviceContext& deviceContext) = 0;

  /**
   * @brief Libero cualquier recurso del componente.
   *
   * @details
   *  Cada componente define su propia manera de limpiar memoria o GPU.
   */
  virtual void
    destroy() = 0;

  /**
   * @brief Obtengo el tipo del componente.
   */
  ComponentType
    getType() const { return m_type; }

protected:

  /// @brief Tipo del componente (enum).
  ComponentType m_type;
};
