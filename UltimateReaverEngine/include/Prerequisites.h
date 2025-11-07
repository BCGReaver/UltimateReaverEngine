#pragma once
/**
 * @file Prerequisites.h
 * @brief Todo lo básico que usa el motor (UltimateReaverEngine): includes de Win/DirectX, macros útiles y structs compartidos.
 *
 * @details
 * Aquí juntamos:
 * - Librerías estándar y de Windows.
 * - Headers de DirectX 11 (D3D11, D3DX11, HLSL compiler).
 * - Macros para liberar recursos COM y loguear cosas rápido.
 * - Structs típicos para los constant buffers y el layout de vértice.
 */

 // -----------------------------
 // Librerías STD / Windows
 // -----------------------------
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>
#include <map>

// -----------------------------
// DirectX 11
// -----------------------------
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

// Recursos (iconos, ids, etc.)
#include "Resource.h"
#include "resource.h"

/**
 * @def SAFE_RELEASE(x)
 * @brief Libera un recurso COM de manera segura y lo deja en nullptr.
 *
 * @param x Puntero COM (por ejemplo, @c ID3D11Buffer*, @c ID3D11Texture2D*).
 *
 * @note Esta macro chequea @p x por nullptr antes de llamar @c Release().
 * Úsala siempre que termines con un recurso de DirectX.
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

 /**
  * @def MESSAGE(classObj, method, state)
  * @brief Log de mensajito simple al OutputDebug con el estado de creación/acción.
  *
  * @param classObj Nombre de la clase (string o literal wide).
  * @param method   Nombre del método que reporta.
  * @param state    Texto libre para describir qué pasó (por ejemplo, "OK", "created", etc.).
  *
  * @details
  * Se imprime algo tipo: `Clase::metodo : [CREATION OF RESOURCE : OK]`
  * Útil para debug rápido sin ensuciar consola.
  */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << L"::" << method << L" : " \
       << L"[CREATION OF RESOURCE : " << state << L"]\n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

  /**
   * @def ERROR(classObj, method, errorMSG)
   * @brief Loguea un error en el OutputDebug con formato fijo.
   *
   * @param classObj Nombre de la clase (wide).
   * @param method   Nombre del método (wide).
   * @param errorMSG Mensaje de error ya en wide o literal L"...".
   *
   * @details
   * No tira excepción ni corta la app, solo deja el mensaje en el debugger.
   * Ej: `ERROR : Device::CreateBuffer : descripción...`
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

   // ============================================================================
   // Estructuras compartidas (coinciden con lo que espera el shader)
   // ============================================================================

   /**
    * @struct SimpleVertex
    * @brief Layout de vértice básico: posición y UV.
    *
    * @var SimpleVertex::Pos
    * Posición del vértice en espacio del objeto (x,y,z).
    *
    * @var SimpleVertex::Tex
    * Coordenadas de textura (u,v) en [0..1].
    */
struct SimpleVertex {
  XMFLOAT3 Pos;
  XMFLOAT2 Tex;
};

/**
 * @struct CBNeverChanges
 * @brief Constant buffer para cosas que casi no cambian (ej: View).
 *
 * @var CBNeverChanges::mView
 * Matriz de vista (cámara), transpuesta cuando se manda al shader.
 */
struct CBNeverChanges {
  XMMATRIX mView;
};

/**
 * @struct CBChangeOnResize
 * @brief Constant buffer pensado para cuando cambias tamaño de ventana.
 *
 * @var CBChangeOnResize::mProjection
 * Matriz de proyección (perspectiva/ortográfica), transpuesta para el shader.
 */
struct CBChangeOnResize {
  XMMATRIX mProjection;
};

/**
 * @struct CBChangesEveryFrame
 * @brief Constant buffer que sí se actualiza cada frame.
 *
 * @var CBChangesEveryFrame::mWorld
 * Matriz world (transform del objeto), transpuesta.
 *
 * @var CBChangesEveryFrame::vMeshColor
 * Color del mesh (RGBA) para tintar o debuggear rápido.
 */
struct CBChangesEveryFrame {
  XMMATRIX mWorld;
  XMFLOAT4 vMeshColor;
};

/**
 * @enum ExtensionType
 * @brief Tipos de archivo soportados al cargar texturas desde disco.
 *
 * @var ExtensionType::DDS
 * Formato DDS (ideal para GPU, soporta mipmaps y compresión).
 *
 * @var ExtensionType::PNG
 * PNG con alpha (común para UI, sprites, etc.).
 *
 * @var ExtensionType::JPG
 * JPG (ligero, sin alpha, ojo con artefactos).
 */
enum ExtensionType {
  DDS = 0,
  PNG = 1,
  JPG = 2
};

enum ShaderType {
  VERTEX_SHADER = 0, ///< Vertex shader type.
  PIXEL_SHADER = 1   ///< Pixel shader type.
};
