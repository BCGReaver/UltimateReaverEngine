/**
 * @file Entity.h
 * @brief Aquí defino la clase Entity, la base de cualquier objeto en la escena.
 *
 * @details
 *  Una Entity representa algo existente en mi mundo: un actor, una luz,
 *  una cámara, o cualquier objeto que pueda tener componentes.
 *
 *  Esta clase no sabe nada de meshes ni render, solo gestiona componentes
 *  (como Transform, MeshComponent, etc) y define funciones virtuales puras,
 *  para que cada tipo de entidad implemente su propio comportamiento.
 */

#pragma once
#include "Prerequisites.h"
#include "Component.h"

class DeviceContext;

/**
 * @class Entity
 * @brief Clase base abstracta para cualquier objeto del mundo (Actor, cámara, etc).
 *
 * @details
 *  Entity permite agregar componentes usando `addComponent<T>()`,
 *  y obtenerlos con `getComponent<T>()`.
 *
 *  Aquí también defino las funciones virtuales `init()`, `update()`,
 *  `render()`, y `destroy()`, que cada entidad debe implementar según lo que haga.
 *
 *  La idea es que una Entity es como un “contenedor de componentes” configurable.
 */
class
  Entity {
public:

  /// @brief Constructor por default (no hago nada especial).
  Entity() = default;

  /// @brief Destructor virtual para permitir herencia segura.
  virtual
    ~Entity() = default;

  /**
   * @brief Inicializo la entidad.
   *
   * @details
   *  Como es abstracta, no implemento nada aquí. Cada entidad hija define qué hace.
   */
  virtual void
    init() = 0;

  /**
   * @brief Actualizo la entidad cada frame.
   *
   * @param deltaTime Tiempo entre frames.
   * @param deviceContext Contexto de GPU para enviar comandos.
   *
   * @details
   *  Cada entidad define aquí su lógica de actualización.
   */
  virtual void
    update(float deltaTime, DeviceContext& deviceContext) = 0;

  /**
   * @brief Renderizo esta entidad (si es visible).
   *
   * @param deviceContext Contexto para enviar comandos de dibujo.
   */
  virtual void
    render(DeviceContext& deviceContext) = 0;

  /**
   * @brief Libero todos los recursos asociados a esta entidad.
   */
  virtual void
    destroy() = 0;

  /**
   * @brief Agrego un componente de tipo T a esta entidad.
   *
   * @tparam T Tipo del componente (debe heredar de Component).
   * @param component SharedPointer al componente creado.
   *
   * @details
   *  Guardo el componente en un vector interno pero lo casteo a Component
   *  para manejarlo polimórficamente.
   */
  template <typename T>
  void
    addComponent(EU::TSharedPointer<T> component) {

    static_assert(std::is_base_of<Component,
      T>::value,
      "T must be derived from Component");

    m_components.push_back(component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Obtengo un componente específico del tipo T.
   *
   * @tparam T Tipo del componente buscado.
   * @return SharedPointer al componente de tipo T si existe, si no nullptr.
   *
   * @details
   *  Recorro todos los componentes, y si alguno hace cast correcto,
   *  lo retorno inmediatamente.
   */
  template<typename T>
  EU::TSharedPointer<T>
    getComponent() {

    for (auto& component : m_components) {
      EU::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();

      if (specificComponent) {
        return specificComponent;
      }
    }

    return EU::TSharedPointer<T>();
  }

protected:

  /// @brief Bandera para saber si está activa en escena.
  bool m_isActive;

  /// @brief ID único de la entidad (si lo necesitas para administrar).
  int m_id;

  /// @brief Vector de componentes asociados a esta entidad.
  std::vector<EU::TSharedPointer<Component>> m_components;
};
