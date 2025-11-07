/**
 * @file ShaderProgram.h
 * @brief Aquí defino la clase ShaderProgram, que es donde manejo los shaders del motor.
 *
 * @details
 *  Esta clase la uso para cargar, compilar y crear los shaders de Direct3D.
 *  Básicamente es el “cerebro visual” del motor, donde defino cómo se van a dibujar las cosas.
 *  Aquí manejo tanto los vertex shaders como los pixel shaders, y también el input layout,
 *  que me sirve para decirle a la GPU cómo leer los vértices.
 */

#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"

class Device;
class DeviceContext;

/**
 * @class ShaderProgram
 * @brief Clase que administra los shaders (vertex y pixel) y su configuración.
 *
 * @details
 *  Esta clase me ayuda a tener el control completo sobre los shaders: los compilo, los creo,
 *  los asigno al pipeline y luego los destruyo cuando ya no se usan.
 *  También me encargo de crear el input layout para que el motor sepa cómo interpretar los datos
 *  que le paso a la GPU (posición, color, textura, etc).
 */
class
  ShaderProgram {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí todavía porque prefiero inicializar todo en `init()`
   *  cuando ya tengo los datos necesarios del shader.
   */
  ShaderProgram() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No destruyo nada directo aquí, eso lo hago con `destroy()`
   *  para tener un cierre controlado de los recursos.
   */
  ~ShaderProgram() = default;

  /**
   * @brief Inicializo el programa de shaders (compilo y creo los objetos).
   *
   * @param device     Dispositivo de Direct3D que uso para crear los shaders.
   * @param fileName   Nombre o ruta del archivo HLSL.
   * @param layout     Descripción del input layout (cómo se leen los vértices).
   *
   * @return HRESULT   `S_OK` si todo salió bien, o código de error si falló.
   *
   * @details
   *  Aquí leo el archivo HLSL, compilo los shaders y creo sus objetos en GPU.
   *  También genero el input layout para que Direct3D sepa cómo mapear los datos del modelo.
   */
  HRESULT
    init(Device& device,
      const std::string& fileName,
      std::vector<D3D11_INPUT_ELEMENT_DESC> layout);

  /**
   * @brief Actualizo el estado del shader program.
   *
   * @details
   *  Normalmente los shaders no cambian cada frame, pero dejo este método
   *  por si más adelante quiero agregar animaciones o recompilaciones dinámicas.
   */
  void
    update();

  /**
   * @brief Renderizo usando los shaders activos.
   *
   * @param deviceContext Contexto donde se aplican los shaders al pipeline.
   *
   * @details
   *  Aquí activo los shaders y dejo todo listo para que los draw calls los usen.
   *  Es la versión general, que activa ambos (vertex y pixel).
   */
  void
    render(DeviceContext& deviceContext);

  /**
   * @brief Renderizo un shader específico (vertex o pixel).
   *
   * @param deviceContext Contexto de dibujo.
   * @param type          Tipo de shader a activar.
   *
   * @details
   *  Este método me da control más fino por si quiero activar solo un tipo de shader,
   *  por ejemplo en un pase de render especial.
   */
  void
    render(DeviceContext& deviceContext, ShaderType type);

  /**
   * @brief Destruyo los shaders y libero toda la memoria asociada.
   *
   * @details
   *  Aquí libero los punteros a los shaders y los blobs usados en la compilación.
   *  Es importante llamar esto antes de cerrar el motor o recargar recursos.
   */
  void
    destroy();

  /**
   * @brief Creo el input layout con la descripción de los vértices.
   *
   * @param device  Dispositivo que uso para crear el layout.
   * @param layout  Vector con la descripción de los elementos del vértice.
   *
   * @return HRESULT `S_OK` si se creó bien, o error si algo falló.
   *
   * @details
   *  Aquí básicamente le digo a la GPU: “así están organizados mis datos de vértices”.
   *  Es clave para que el vertex shader sepa qué datos esperar.
   */
  HRESULT
    CreateInputLayout(Device& device,
      std::vector<D3D11_INPUT_ELEMENT_DESC> layout);

  /**
   * @brief Creo un shader específico (vertex o pixel) sin recompilar archivo.
   *
   * @param device  Dispositivo donde se crea.
   * @param type    Tipo de shader que quiero generar.
   *
   * @return HRESULT `S_OK` si todo bien, o código de error si algo falló.
   *
   * @details
   *  Uso esto cuando ya tengo los datos compilados y solo necesito crear el objeto GPU.
   */
  HRESULT
    CreateShader(Device& device, ShaderType type);

  /**
   * @brief Creo y compilo un shader directamente desde archivo.
   *
   * @param device     Dispositivo donde se crea.
   * @param type       Tipo de shader (vertex o pixel).
   * @param fileName   Nombre del archivo HLSL a compilar.
   *
   * @return HRESULT   `S_OK` si se compiló y creó bien, o error si algo falló.
   *
   * @details
   *  Este método es más completo porque compila y crea el shader desde cero.
   *  Me sirve cuando quiero recargar o probar nuevos shaders en tiempo real.
   */
  HRESULT
    CreateShader(Device& device, ShaderType type, const std::string& fileName);

  /**
   * @brief Compilo un shader desde un archivo HLSL.
   *
   * @param szFileName   Ruta del archivo HLSL.
   * @param szEntryPoint Punto de entrada del shader (por ejemplo "VSMain" o "PSMain").
   * @param szShaderModel Modelo del shader (ej. "vs_5_0" o "ps_5_0").
   * @param ppBlobOut    Puntero donde se guarda el resultado compilado.
   *
   * @return HRESULT     `S_OK` si se compiló bien, o error si algo falló.
   *
   * @details
   *  Aquí es donde realmente compilo el código HLSL a bytecode que entiende la GPU.
   *  Este bytecode luego lo uso para crear el shader en el dispositivo.
   */
  HRESULT
    CompileShaderFromFile(char* szFileName,
      LPCSTR szEntryPoint,
      LPCSTR szShaderModel,
      ID3DBlob** ppBlobOut);

public:

  /// @brief Shader de vértices. Se encarga de procesar las posiciones de los modelos.
  ID3D11VertexShader* m_VertexShader = nullptr;

  /// @brief Shader de píxeles. Es el que pinta el color final en pantalla.
  ID3D11PixelShader* m_PixelShader = nullptr;

  /// @brief Input layout que describe cómo se leen los datos del vértice.
  InputLayout m_inputLayout;

private:

  /// @brief Nombre del archivo HLSL usado por este shader program.
  std::string m_shaderFileName;

  /// @brief Datos binarios del vertex shader (resultado de la compilación).
  ID3DBlob* m_vertexShaderData = nullptr;

  /// @brief Datos binarios del pixel shader (resultado de la compilación).
  ID3DBlob* m_pixelShaderData = nullptr;
};
