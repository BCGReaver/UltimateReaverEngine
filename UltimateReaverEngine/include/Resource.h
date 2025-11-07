/**
 * @file Resource.h
 * @brief Archivo donde se definen los identificadores (IDs) de los recursos de la app.
 *
 * @details
 *  Este archivo lo uso para asignar nombres simbólicos a los recursos del proyecto.
 *  Por ejemplo, íconos, menús, cuadros de diálogo, botones, etc.
 *  Es lo típico de los proyectos Win32: aquí simplemente se guardan las constantes
 *  que usa el sistema para referenciar los recursos.
 */

#pragma once

 /// @brief Título principal de la aplicación (nombre que aparece en la barra superior).
#define IDS_APP_TITLE            103   

/// @brief Identificador del frame principal (la ventana principal del programa).
#define IDR_MAINFRAME            128   

/// @brief ID del diálogo principal del tutorial.
#define IDD_TUTORIAL1_DIALOG     102   

/// @brief ID del cuadro de “Acerca de…” (About Box).
#define IDD_ABOUTBOX             103   

/// @brief ID del comando del menú para abrir el About Box.
#define IDM_ABOUT                104   

/// @brief ID del comando del menú para salir de la aplicación.
#define IDM_EXIT                 105   

/// @brief ID del ícono grande de la aplicación.
#define IDI_TUTORIAL1            107   

/// @brief ID del ícono pequeño de la aplicación (por ejemplo, el que sale en la barra de tareas).
#define IDI_SMALL                108   

/// @brief Identificador general del control principal de la app.
#define IDC_TUTORIAL1            109   

/// @brief Identificador del ícono del programa (usado por algunos menús o diálogos).
#define IDC_MYICON               2     


#ifndef IDC_STATIC
/// @brief ID genérico que se usa para elementos estáticos en los diálogos.
#define IDC_STATIC               -1
#endif


#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
/// @brief Configuración por defecto que usa Visual Studio cuando genera recursos nuevos.
#define _APS_NO_MFC              130
#define _APS_NEXT_RESOURCE_VALUE 129
#define _APS_NEXT_COMMAND_VALUE  32771
#define _APS_NEXT_CONTROL_VALUE  1000
#define _APS_NEXT_SYMED_VALUE    110
#endif
#endif
