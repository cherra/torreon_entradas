#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Macros para obtener objetos del archivo UI */
#define CH_GET_OBJECT( builder, name, type, data ) \
	data->name = type( gtk_builder_get_object( builder, #name ) )
#define CH_GET_WIDGET( builder, name, data ) \
	CH_GET_OBJECT( builder, name, GTK_WIDGET, data )

char *homedir;  // Directorio HOME del usuario
char file_db_config[128];  // Variable para almacenar la ruta del archivo de configuración para la base de datos

// Variables de configuración para el módulo de impresión.
char ImpresoraConfig[256], TicketImpresion[256];

/* Definición principal de la esctructura de datos */
typedef struct _Data Data;
struct _Data
{
	GtkWidget *principal;
	GtkWidget *txtcodigo;
	GtkListStore *liststoreArticulos;
	GtkWidget *treeview;
	GtkWidget *dialogLimpiar;
	GtkListStore *liststoreProveedores;
	GtkWidget *dialogLogin;
	GtkWidget *statusbar;
	GtkWidget *btnBuscarProveedor;
	GtkWidget *txtproveedor;
	GtkWidget *treeviewProveedores;
	GtkWidget *btnAgregar;
	GtkWidget *lblcodigo;
	GtkTreeSelection *treeselectionProveedores;
	GtkWidget *windowBuscarArticulo;
	GtkListStore *liststoreBuscarArticulos;
	GtkWidget *txtBuscarArticulo;
	GtkWidget *treeviewBuscarArticulo;
	GtkWidget *btnAceptarBuscarArticulo;
	GtkWidget *dialogGuardar;
	GtkWidget *windowLogin;
	GtkWidget *txtusuario;
	GtkWidget *txtpassword;
	GtkWidget *btnGuardar;
	GtkWidget *stock3Dias;
	GtkWidget *dialogImprimir;
};

#endif /*  __SUPPORT_H__ */
