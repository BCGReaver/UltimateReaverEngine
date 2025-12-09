/**
 * @file Main.cpp
 * @brief Punto de entrada principal de la aplicación.
 *
 * @details
 *  Aquí es donde arranca todo el programa. Windows siempre busca esta función (`wWinMain`)
 *  cuando se inicia una aplicación gráfica en lugar del clásico `main`.
 *
 *  Lo que hago aquí es simplemente crear una instancia de mi `BaseApp`,
 *  y luego llamo a su método `run()` para ejecutar el ciclo de vida completo del motor
 *  (inicializar, actualizar, renderizar y destruir).
 */

#include "BaseApp.h"

 /**
  * @brief Punto de entrada principal de una app Windows (versión wide con Unicode).
  *
  * @param hInstance       Instancia principal de la aplicación.
  * @param hPrevInstance   (No se usa, legado de Windows para versiones antiguas).
  * @param lpCmdLine       Argumentos de línea de comandos en Unicode.
  * @param nCmdShow        Cómo se va a mostrar la ventana al iniciar.
  *
  * @return int            Código de salida de la aplicación.
  *
  * @details
  *  Windows utiliza esta función como arranque cuando compilo como aplicación gráfica.
  *  Aquí creo mi motor (`BaseApp`) y simplemente llamo a `run()` para dejar que el motor
  *  maneje todo el ciclo (ventana, render, entrada, etc.).
  *
  *  Si cierro la ventana, `run()` terminará el ciclo, destruirá recursos y regresará aquí
  *  para cerrar el programa correctamente.
  */
int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

  // Creo mi aplicación base (el motor).
  // Nota: puedo pasar los parámetros aquí o directamente en run().
  BaseApp app;

  // Inicio la app llamando a su ciclo principal
  return app.run(hInstance, nCmdShow);
}
