/**
 * @file Prerequisites.h
 * @brief En este header junto todo lo básico que necesito del proyecto: includes, macros, structs y enums.
 *
 * @details
 *  Aquí centralizo las dependencias de Windows y Direct3D 11 para no estar repitiendo en cada archivo.
 *  También dejo algunos macros de utilidad (como `SAFE_RELEASE`) y constantes para loggear rápido.
 *  Además, defino los structs de vértices y los constant buffers que uso con los shaders,
 *  y enums para distinguir tipos de archivos y tipos de shader. Todo directo y listo para reusar.
 */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>
#include <map>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include "Resource.h"
#include "resource.h"

 /**
  * @brief Macro de ayuda para liberar interfaces de COM de forma segura.
  *
  * @details
  *  Yo lo uso para evitar memory leaks en GPU/COM. Si el puntero no es null, llamo `Release()`
  *  y luego lo dejo en `nullptr`. Me ahorro escribir el if a cada rato.
  */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

  /**
   * @brief Macro para loguear mensajes de creación de recursos en el OutputDebug.
   *
   * @param classObj  Nombre de la clase (texto wide).
   * @param method    Método donde estoy logueando.
   * @param state     Estado o mensaje corto (por ejemplo: "OK" o "FAILED").
   *
   * @details
   *  Yo lo uso para tener trazas rápidas cuando creo cosas de D3D (buffers, shaders, etc.)
   *  y así ver en el Output de Visual Studio cómo va el flujo.
   */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << L"::" << method << L" : " \
       << L"[CREATION OF RESOURCE : " << state << L"]\n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

   /**
    * @brief Macro para loguear errores en el OutputDebug de forma segura.
    *
    * @param classObj  Nombre de la clase (wide).
    * @param method    Método donde ocurrió el error.
    * @param errorMSG  Mensaje descriptivo del error.
    *
    * @details
    *  En caso de que algo truene, lo mando a OutputDebug. Envuelvo en try/catch por si el stream falla.
    *  Así no se me cae el programa por intentar loguear un error, sería el colmo.
    */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

    /**
     * @struct SimpleVertex
     * @brief Vértice básico con posición y coordenadas de textura.
     *
     * @details
     *  Este lo uso para geometría sencilla: posición en 3D y UV para texturas.
     *  Perfecto para probar shaders o dibujar modelos simples.
     */
struct SimpleVertex {
  /// @brief Posición del vértice en espacio 3D.
  XMFLOAT3 Pos;
  /// @brief Coordenadas de textura (u, v).
  XMFLOAT2 Tex;
};

/**
 * @struct CBNeverChanges
 * @brief Constant buffer para datos que casi nunca cambian.
 *
 * @details
 *  Aquí meto la vista (mView). Normalmente la cargo una vez y rara vez la toco.
 */
struct CBNeverChanges {
  /// @brief Matriz de vista (cámara).
  XMMATRIX mView;
};

/**
 * @struct CBChangeOnResize
 * @brief Constant buffer para cosas que cambian cuando redimensiono la ventana.
 *
 * @details
 *  Al cambiar el tamaño de la ventana, recalculo la proyección y la guardo aquí.
 */
struct CBChangeOnResize {
  /// @brief Matriz de proyección (perspectiva u ortográfica).
  XMMATRIX mProjection;
};

/**
 * @struct CBChangesEveryFrame
 * @brief Constant buffer para datos que sí cambian cada frame.
 *
 * @details
 *  Aquí pongo el mundo (mWorld) y un color por malla, ideal para animaciones o efectos por objeto.
 */
struct CBChangesEveryFrame {
  /// @brief Matriz de mundo (transformación del objeto).
  XMMATRIX mWorld;
  /// @brief Color del mesh (RGBA), útil para tintes o debug.
  XMFLOAT4 vMeshColor;
};

/**
 * @enum ExtensionType
 * @brief Tipos de extensión de textura que manejo al cargar.
 *
 * @details
 *  Lo uso para decidir el pipeline de carga (DDS nativo, o imágenes como PNG/JPG).
 */
enum ExtensionType {
  DDS = 0, ///< Textura DDS (ideal para GPU, soporta compresión/ mipmaps).
  PNG = 1, ///< Imagen PNG (sin pérdida, buena para UI).
  JPG = 2  ///< Imagen JPG (con pérdida, ligera para difusas).
};

/**
 * @enum ShaderType
 * @brief Tipo de shader que quiero crear/usar en el pipeline.
 *
 * @details
 *  Por ahora manejo vertex y pixel shaders. Si luego agrego geometry/hull/domain, los extiendo aquí.
 */
enum ShaderType {
  VERTEX_SHADER = 0, ///< Vertex Shader (transforma vértices).
  PIXEL_SHADER = 1  ///< Pixel Shader (calcula el color final).
};
