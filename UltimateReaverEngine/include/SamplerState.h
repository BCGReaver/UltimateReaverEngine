/**
 * @file SamplerState.h
 * @brief Aquí defino la clase SamplerState, que básicamente controla cómo se muestrean las texturas.
 *
 * @details
 *  Esta clase la uso para decirle a la GPU cómo quiero que se lean las texturas.
 *  Por ejemplo, si quiero filtrado lineal, repetición, clamp, etc.
 *  Es una parte chiquita del pipeline, pero súper importante para que las texturas se vean bien.
 */

#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class SamplerState
 * @brief Clase encargada de crear y manejar el estado de muestreo de texturas (Sampler).
 *
 * @details
 *  Con esta clase defino el comportamiento del muestreo de texturas en el motor.
 *  El sampler básicamente le dice al pixel shader cómo debe leer una textura:
 *  si interpola los colores, si repite los bordes o si corta, etc.
 *  Lo mantengo separado para poder reutilizarlo con diferentes materiales o efectos.
 */
class
  SamplerState {
public:

  /**
   * @brief Constructor por default.
   *
   * @details
   *  No hago nada todavía aquí. Prefiero inicializar el sampler con `init()`
   *  cuando tengo acceso al device de Direct3D.
   */
  SamplerState() = default;

  /**
   * @brief Destructor por default.
   *
   * @details
   *  No libero nada aquí directamente, eso lo hago en `destroy()` para tener
   *  el control de cuándo se destruyen los recursos.
   */
  ~SamplerState() = default;

  /**
   * @brief Inicializo el sampler state.
   *
   * @param device  Dispositivo de Direct3D que se usa para crear el sampler.
   * @return HRESULT `S_OK` si se creó correctamente, o un código de error si falló.
   *
   * @details
   *  Aquí configuro cómo se van a muestrear las texturas: el tipo de filtrado (por ejemplo lineal o anisotrópico),
   *  y el modo de dirección (wrap, clamp, mirror). Es decir, aquí le digo al motor “así vas a leer tus texturas”.
   */
  HRESULT
    init(Device& device);

  /**
   * @brief Actualizo el estado del sampler.
   *
   * @details
   *  Normalmente el sampler no necesita actualizarse cada frame,
   *  pero dejo este método por consistencia con el ciclo del motor (por si después lo uso dinámicamente).
   */
  void
    update();

  /**
   * @brief Activo el sampler dentro del pipeline gráfico.
   *
   * @param deviceContext  Contexto de dibujo de Direct3D.
   * @param startSlot      Slot donde empiezo a bindear el sampler.
   * @param numSamplers    Número de samplers que quiero activar.
   *
   * @details
   *  Aquí básicamente le digo al shader “usa este sampler para las texturas”.
   *  Esto es lo que hace que el filtrado se aplique cuando renderizo.
   */
  void
    render(DeviceContext& deviceContext,
      unsigned int startSlot,
      unsigned int numSamplers);

  /**
   * @brief Libero los recursos del sampler.
   *
   * @details
   *  Cuando ya no lo necesito, libero el objeto `m_sampler` para evitar fugas de memoria en la GPU.
   *  Siempre llamo a esto antes de cerrar el motor o recargar recursos.
   */
  void
    destroy();

public:

  /// @brief Puntero al objeto ID3D11SamplerState, que define cómo la GPU muestrea las texturas.
  ID3D11SamplerState* m_sampler = nullptr;
};
