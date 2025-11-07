/**
 * @file ModelLoader.h
 * @brief Aquí defino la clase ModelLoader, que se encarga de cargar modelos 3D desde archivos.
 *
 * @details
 *  Esta clase la uso para leer archivos de modelos (por ejemplo .obj) y convertir sus datos
 *  en algo que mi motor pueda entender. Básicamente, de aquí saco las posiciones, normales
 *  y coordenadas de textura de cada vértice, y las guardo dentro de un `MeshComponent`.
 */

#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h"

 /**
  * @class ModelLoader
  * @brief Clase que maneja la carga y el parseo de modelos 3D.
  *
  * @details
  *  Esta clase me sirve como una herramienta para interpretar modelos de texto plano (tipo .obj)
  *  y transformarlos en datos estructurados. Aquí separo la lógica para cargar el archivo,
  *  leer las líneas y convertirlas en vectores de `XMFLOAT2` o `XMFLOAT3`.
  *  Así mantengo el código del motor más limpio y modular.
  */
class ModelLoader {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí, solo dejo la clase lista para usarse cuando se necesite cargar un modelo.
   */
  ModelLoader() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No manejo memoria dinámica aquí, así que no necesito liberar nada.
   *  Lo dejo definido por convención.
   */
  ~ModelLoader() = default;

  /**
   * @brief Carga un modelo desde un archivo y lo guarda en un `MeshComponent`.
   *
   * @param fileName  Nombre o ruta del archivo del modelo (por ejemplo "cube.obj").
   * @param outMesh   Referencia al `MeshComponent` donde se guardarán los datos del modelo.
   *
   * @return `true` si la carga fue exitosa, o `false` si algo falló.
   *
   * @details
   *  Aquí abro el archivo, leo sus líneas y saco toda la información necesaria
   *  para generar los vértices, normales y UVs.
   *  Al final, dejo el `MeshComponent` listo para mandarlo al GPU.
   */
  bool
    loadModel(const std::string& fileName, MeshComponent& outMesh);

  /**
   * @brief Parsea un vector 2D desde una línea del archivo.
   *
   * @param streamLine  Línea del archivo que contiene los datos (por ejemplo: "vt 0.5 1.0").
   * @param outVector   Vector donde se van a guardar los resultados en formato `XMFLOAT2`.
   *
   * @details
   *  Uso este método para leer coordenadas de textura (`vt`) en archivos `.obj`.
   *  Básicamente, convierto texto a valores flotantes y los guardo como vectores 2D.
   */
  void
    parseVec2(std::stringstream& streamLine, std::vector<XMFLOAT2>& outVector);

  /**
   * @brief Parsea un vector 3D desde una línea del archivo.
   *
   * @param streamLine  Línea del archivo con los datos (por ejemplo: "v 0.0 1.0 -2.0").
   * @param outVector   Vector donde se guardan los resultados en formato `XMFLOAT3`.
   *
   * @details
   *  Lo uso para leer posiciones o normales (`v` o `vn`) desde archivos `.obj`.
   *  Es igual que el anterior pero en 3D. Me deja los datos listos para generar los vértices.
   */
  void
    parseVec3(std::stringstream& streamLine, std::vector<XMFLOAT3>& outVector);
};
