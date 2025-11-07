/**
 * @file MeshComponent.h
 * @brief Aquí defino la clase MeshComponent, que representa la malla o geometría básica de un modelo.
 *
 * @details
 *  Esta clase la uso para guardar los vértices, índices y el nombre de cada malla del motor.
 *  Básicamente, es la estructura que contiene toda la info necesaria para que un modelo se pueda renderizar.
 *  No maneja materiales ni shaders directamente, solo los datos crudos de geometría.
 */

#pragma once
#include "Prerequisites.h"

class DeviceContext;

/**
 * @class MeshComponent
 * @brief Clase encargada de almacenar y administrar los datos de una malla.
 *
 * @details
 *  Aquí guardo todos los vértices (`m_vertex`), los índices (`m_index`) y el nombre de la malla.
 *  Es la base de cualquier modelo 3D: sin esta parte, el motor no sabría qué dibujar.
 *  También tiene métodos para inicializar, actualizar y renderizar la malla con el `DeviceContext`.
 */
class
  MeshComponent {
public:

  /**
   * @brief Constructor que inicializa los contadores de vértices e índices en cero.
   *
   * @details
   *  Cuando creo una malla nueva, empiezo con `m_numVertex` y `m_numIndex` en 0.
   *  Luego, cuando cargo un modelo (por ejemplo con el ModelLoader), los lleno con los datos reales.
   */
  MeshComponent() : m_numVertex(0), m_numIndex(0) {}

  /**
   * @brief Destructor virtual por default.
   *
   * @details
   *  Lo dejo virtual por si después hago herencia (por ejemplo, StaticMesh o SkinnedMesh),
   *  para que se libere bien todo al destruirse.
   */
  virtual
    ~MeshComponent() = default;

  /**
   * @brief Inicializo la malla.
   *
   * @details
   *  Aquí normalmente creo los buffers (vertex buffer, index buffer, etc.) en GPU.
   *  Ahorita puede estar vacío si solo la estoy estructurando, pero la idea es dejarla lista para renderizar.
   */
  void
    init();

  /**
   * @brief Actualizo la malla cada frame.
   *
   * @param deltaTime  Tiempo transcurrido desde el último frame.
   *
   * @details
   *  Este método lo uso si quiero animar vértices o hacer algo dinámico con la malla (como rotaciones).
   *  En general, las mallas estáticas no se actualizan cada frame, pero dejo esto por consistencia.
   */
  void
    update(float deltaTime);

  /**
   * @brief Renderizo la malla usando el contexto de dispositivo.
   *
   * @param deviceContext  Contexto de dibujo de Direct3D.
   *
   * @details
   *  Aquí mando los vértices e índices a la GPU y hago la llamada de dibujo.
   *  Es donde realmente el modelo se pinta en pantalla.
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Destruyo y limpio la malla.
   *
   * @details
   *  Libero cualquier recurso asociado (buffers o vectores) para no dejar nada colgando.
   *  Siempre llamo a esto antes de cerrar el motor o cambiar de escena.
   */
  void
    destroy();

public:

  /// @brief Nombre de la malla (me sirve para identificarla o debuguear).
  std::string m_name;

  /// @brief Lista de vértices de la malla (posición, UV, etc).
  std::vector<SimpleVertex> m_vertex;

  /// @brief Lista de índices que indican el orden de los vértices al dibujar.
  std::vector<unsigned int> m_index;

  /// @brief Número total de vértices que tiene la malla.
  int m_numVertex;

  /// @brief Número total de índices que tiene la malla.
  int m_numIndex;
};
