/**
 * @file InputLayout.h
 * @brief Aquí defino la clase InputLayout, que se encarga de decirle a la GPU cómo están estructurados los vértices.
 *
 * @details
 *  Esta clase la uso para crear el “input layout” de Direct3D, que básicamente le indica al vertex shader
 *  cómo debe interpretar los datos que le mando (posición, color, UVs, normales, etc.).
 *  Es una parte súper importante del pipeline, porque si el layout no coincide con el shader, nada se dibuja bien.
 */

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class InputLayout
 * @brief Clase que administra el diseño de entrada de los vértices para la GPU.
 *
 * @details
 *  Aquí configuro cómo se leen los datos de un vértice cuando llegan al vertex shader.
 *  Por ejemplo, le puedo decir que la primera parte del buffer son posiciones (`POSITION`),
 *  la segunda son coordenadas de textura (`TEXCOORD`), etc.
 *  Este layout debe coincidir exactamente con la estructura del shader para evitar errores.
 */
class InputLayout {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí todavía, prefiero inicializar el layout en `init()`
   *  cuando ya tengo el shader compilado y la descripción del input.
   */
  InputLayout() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No destruyo nada aquí directamente, lo hago en `destroy()` para mantener
   *  un control total sobre cuándo libero recursos de GPU.
   */
  ~InputLayout() = default;

  /**
   * @brief Inicializo el input layout con la información del shader y la descripción del vértice.
   *
   * @param device             Dispositivo de Direct3D usado para crear el layout.
   * @param layout             Vector con las descripciones (`D3D11_INPUT_ELEMENT_DESC`) del vértice.
   * @param vertexShaderData   Datos compilados del vertex shader (ID3DBlob) que se usa para validar el layout.
   *
   * @return HRESULT           `S_OK` si se creó correctamente, o un código de error si algo falló.
   *
   * @details
   *  Aquí es donde se crea realmente el `ID3D11InputLayout`.
   *  Lo importante es que el layout que defina coincida exactamente con las variables de entrada
   *  del vertex shader, o Direct3D marcará error.
   */
  HRESULT
    init(Device& device,
      std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
      ID3DBlob* vertexShaderData);

  /**
   * @brief Actualizo el estado del input layout.
   *
   * @details
   *  Normalmente no hace falta actualizar el layout una vez creado, pero dejo este método
   *  por consistencia con el resto del ciclo del motor. Lo puedo usar para debug o reconfiguración.
   */
  void
    update();

  /**
   * @brief Activo el input layout en el pipeline gráfico.
   *
   * @param deviceContext  Contexto de dibujo donde se aplica el layout.
   *
   * @details
   *  Aquí básicamente le digo a la GPU “usa este formato para leer los vértices”.
   *  Es un paso obligatorio antes de renderizar cualquier modelo.
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Destruyo el input layout y libero su memoria en GPU.
   *
   * @details
   *  Libero el puntero `m_inputLayout` con `Release()` para evitar fugas de memoria.
   *  Siempre llamo a esto cuando cierro el motor o cambio de shader.
   */
  void
    destroy();

public:

  /// @brief Puntero al objeto de Direct3D que define la estructura del vértice.
  ID3D11InputLayout* m_inputLayout = nullptr;
};
