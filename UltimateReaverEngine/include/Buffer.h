/**
 * @file Buffer.h
 * @brief Aquí defino la clase Buffer, que se encarga de crear, actualizar y administrar los buffers en GPU.
 *
 * @details
 *  Esta clase la uso para manejar los diferentes tipos de buffers en Direct3D 11:
 *  vertex buffer, index buffer o constant buffer.
 *  Es una clase genérica que centraliza toda la lógica de creación y envío de datos a la GPU.
 *  Gracias a esto, evito repetir el mismo código para cada tipo de buffer en el motor.
 */

#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Clase que administra buffers de Direct3D (vertex, index o constant buffers).
 *
 * @details
 *  Este compa es el encargado de crear los buffers en GPU y mantenerlos sincronizados con los datos del CPU.
 *  También puedo actualizar su contenido y vincularlo al pipeline cuando sea necesario.
 *  Es una parte esencial del motor, porque sin buffers, la GPU no tendría datos para dibujar.
 */
class
  Buffer {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada aquí todavía, ya que la inicialización real del buffer se hace con `init()`.
   */
  Buffer() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No destruyo nada aquí directamente, prefiero hacerlo con `destroy()`
   *  para controlar cuándo libero los recursos en GPU.
   */
  ~Buffer() = default;

  /**
   * @brief Inicializo un buffer a partir de un `MeshComponent`.
   *
   * @param device    Dispositivo de Direct3D que se usa para crear el buffer.
   * @param mesh      Malla de la que obtengo los vértices o índices.
   * @param bindFlag  Tipo de buffer (por ejemplo, `D3D11_BIND_VERTEX_BUFFER` o `D3D11_BIND_INDEX_BUFFER`).
   *
   * @return HRESULT  `S_OK` si se creó correctamente, o código de error si algo falló.
   *
   * @details
   *  Este método lo uso para crear buffers que vienen directamente de los datos de una malla.
   *  Así puedo tener mi geometría lista para mandarla a la GPU sin escribir lógica repetida.
   */
  HRESULT
    init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

  /**
   * @brief Inicializo un buffer vacío con un tamaño específico.
   *
   * @param device     Dispositivo de Direct3D que crea el buffer.
   * @param byteWidth  Tamaño del buffer en bytes.
   *
   * @return HRESULT   `S_OK` si se creó correctamente, o error si algo falló.
   *
   * @details
   *  Uso este método cuando quiero un buffer dinámico o temporal, como un constant buffer.
   *  Puedo llenarlo después con `update()`.
   */
  HRESULT
    init(Device& device, unsigned int byteWidth);

  /**
   * @brief Actualizo los datos de un buffer existente.
   *
   * @param deviceContext  Contexto de dibujo donde aplico la actualización.
   * @param pDstResource   Recurso destino en GPU.
   * @param DstSubresource Subrecurso del buffer que quiero actualizar.
   * @param pDstBox        Región opcional que quiero sobrescribir.
   * @param pSrcData       Puntero a los nuevos datos.
   * @param SrcRowPitch    Paso por fila (en bytes).
   * @param SrcDepthPitch  Paso por profundidad (en bytes).
   *
   * @details
   *  Con esto puedo modificar un buffer que ya existe, como un constant buffer o uno dinámico.
   *  Ideal para cuando los datos cambian cada frame (como matrices de vista o mundo).
   */
  void
    update(DeviceContext& deviceContext,
      ID3D11Resource* pDstResource,
      unsigned int DstSubresource,
      const D3D11_BOX* pDstBox,
      const void* pSrcData,
      unsigned int SrcRowPitch,
      unsigned int SrcDepthPitch);

  /**
   * @brief Vinculo el buffer al pipeline para que la GPU pueda usarlo.
   *
   * @param deviceContext  Contexto de dibujo de Direct3D.
   * @param StartSlot      Slot donde se va a asignar el buffer.
   * @param NumBuffers     Número de buffers a activar.
   * @param setPixelShader Si es true, el buffer se vincula al pixel shader (en lugar del vertex shader).
   * @param format         Formato de los índices (solo si es un index buffer).
   *
   * @details
   *  Aquí le digo a la GPU: “usa este buffer para dibujar”.
   *  Dependiendo del tipo (vertex, index o constant), se enlaza en una etapa diferente del pipeline.
   */
  void
    render(DeviceContext& deviceContext,
      unsigned int StartSlot,
      unsigned int NumBuffers,
      bool setPixelShader = false,
      DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

  /**
   * @brief Destruyo el buffer y libero sus recursos en GPU.
   *
   * @details
   *  Llamo a `Release()` sobre el buffer y limpio los valores.
   *  Siempre llamo a esto antes de cerrar el motor o recargar modelos.
   */
  void
    destroy();

  /**
   * @brief Creo el buffer en GPU con una descripción y datos iniciales personalizados.
   *
   * @param device     Dispositivo que crea el buffer.
   * @param desc       Descripción del buffer (tipo, tamaño, flags, etc.).
   * @param initData   Datos iniciales opcionales.
   *
   * @return HRESULT   `S_OK` si se creó correctamente, o error si falló.
   *
   * @details
   *  Este método es el que realmente llama a `ID3D11Device::CreateBuffer()`.
   *  Lo uso internamente dentro de `init()` para evitar repetir código.
   */
  HRESULT
    createBuffer(Device& device,
      D3D11_BUFFER_DESC& desc,
      D3D11_SUBRESOURCE_DATA* initData);

private:

  /// @brief Puntero al buffer de Direct3D (ya sea de vértices, índices o constantes).
  ID3D11Buffer* m_buffer = nullptr;

  /// @brief Tamaño de un solo elemento del buffer (por ejemplo, tamaño de un vértice).
  unsigned int m_stride = 0;

  /// @brief Offset o desplazamiento inicial del buffer (normalmente 0).
  unsigned int m_offset = 0;

  /// @brief Tipo de buffer según Direct3D (por ejemplo, `D3D11_BIND_VERTEX_BUFFER`).
  unsigned int m_bindFlag = 0;
};
