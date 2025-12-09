/**
 * @file Actor.h
 * @brief Aquí defino la clase Actor, que representa un objeto dentro de la escena.
 *
 * @details
 *  Un Actor básicamente es una entidad que tiene un Transform, una o varias Meshes,
 *  texturas, shaders, buffers y puede actualizarse o renderizarse.
 *  Aquí encapsulo toda la lógica necesaria para mostrar un modelo 3D en pantalla
 *  (y opcionalmente manejar sombras).
 */

#pragma once
#include "Prerequisites.h"
#include "Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "Transform.h"
#include "SamplerState.h"
#include "ShaderProgram.h"

class Device;
class DeviceContext;
class MeshComponent;

/**
 * @class Actor
 * @brief Clase que representa un objeto del mundo (un modelo o cualquier cosa visible).
 *
 * @details
 *  Esta clase hereda de Entity (que ya tiene un Transform), así que cada Actor tiene
 *  su propia posición, rotación y escala.
 *
 *  Además, puedo asignarle una o varias mallas, diferentes texturas, buffers
 *  y shaders específicos para renderizarlo.
 *
 *  La idea es que cada Actor sea autónomo y se pueda actualizar o dibujar por sí solo.
 */
class
  Actor : public Entity {
public:

  /**
   * @brief Constructor por default, no hago nada especial aquí.
   */
  Actor() = default;

  /**
   * @brief Constructor que recibe un `Device` por si necesito inicializar cosas desde el inicio.
   */
  Actor(Device& device);

  /**
   * @brief Destructor virtual por default (importante por la herencia).
   */
  virtual
    ~Actor() = default;

  /**
   * @brief Inicializo el actor. (Aquí no hago nada aún).
   *
   * @details
   *  Podría usar este método para inicializar buffers o cargar algo si lo necesito,
   *  pero por ahora dejo la lógica en otros métodos.
   */
  void
    init() override {}

  /**
   * @brief Actualizo el actor cada frame.
   *
   * @param deltaTime      Tiempo transcurrido desde el frame anterior.
   * @param deviceContext  Contexto que me permite mandar info a la GPU.
   *
   * @details
   *  Aquí actualizo cualquier animación o lógica dependiente del tiempo.
   *  También puedo enviar matrices actualizadas a shaders mediante constant buffers.
   */
  void
    update(float deltaTime, DeviceContext& deviceContext) override;

  /**
   * @brief Renderizo el actor con el contexto de dispositivo.
   *
   * @details
   *  Aquí dibujo cada mesh del actor, asigno sus texturas y buffers,
   *  y llamo al shader correspondiente.
   *  Es básicamente el corazón visual del Actor.
   */
  void
    render(DeviceContext& deviceContext) override;

  /**
   * @brief Libero todos los recursos asociados al Actor.
   */
  void
    destroy();

  /**
   * @brief Asigno las mallas del actor, y creo sus buffers internos.
   *
   * @param device   Dispositivo con el que creo los buffers.
   * @param meshes   Lista de MeshComponent que representan el modelo.
   *
   * @details
   *  Aquí convierto cada MeshComponent en vertex/index buffers listos para la GPU.
   */
  void
    setMesh(Device& device, std::vector<MeshComponent> meshes);

  /**
   * @brief Obtengo el nombre del actor.
   */
  std::string
    getName() { return m_name; }

  /**
   * @brief Cambio el nombre del actor (útil para el inspector/ImGui).
   */
  void
    setName(const std::string& name) { m_name = name; }

  /**
   * @brief Asigno manualmente las texturas de este actor.
   */
  void
    setTextures(std::vector<Texture> textures) { m_textures = textures; }

  /**
   * @brief Activo o desactivo la capacidad de generar sombras.
   */
  void
    setCastShadow(bool v) { castShadow = v; }

  /**
   * @brief Consulto si este actor puede generar sombra.
   */
  bool
    canCastShadow() const { return castShadow; }

  /**
   * @brief Renderizo la forma de sombra del actor (shadow pass).
   *
   * @details
   *  Aquí uso el shader de sombras, los buffers y las matrices para dibujar
   *  la “silhouette” del modelo en un mapa de sombras.
   */
  void
    renderShadow(DeviceContext& deviceContext);

private:

  /// @brief Lista de mallas del actor.
  std::vector<MeshComponent> m_meshes;

  /// @brief Texturas asignadas al actor (una o varias).
  std::vector<Texture> m_textures;

  /// @brief Vertex buffers internos, uno por mesh.
  std::vector<Buffer> m_vertexBuffers;

  /// @brief Index buffers internos, uno por mesh.
  std::vector<Buffer> m_indexBuffers;

  /// @brief Sampler para leer texturas (filtrado, wrapping, etc.).
  SamplerState m_sampler;

  /// @brief Constant buffer para enviar matrices del modelo (world/color).
  CBChangesEveryFrame m_model;

  /// @brief Buffer en GPU para la info del modelo.
  Buffer m_modelBuffer;

  // --------------------------------------------------------------------
  // Shaders y buffers para el pass de sombras
  // --------------------------------------------------------------------

  /// @brief Shader especial para el pass de sombras.
  ShaderProgram m_shaderShadow;

  /// @brief Buffer de shader (usado para constantes del shadow pass).
  Buffer m_shaderBuffer;

  /// @brief Datos de transformación para el pass de sombras.
  CBChangesEveryFrame m_cbShadow;

  /// @brief Posición de la luz (la uso para generar sombras).
  XMFLOAT4 m_LightPos;

  /// @brief Nombre del actor (para debug e inspector).
  std::string m_name = "Actor";

  /// @brief Flag para saber si este actor debe proyectar sombras.
  bool castShadow = true;
};
