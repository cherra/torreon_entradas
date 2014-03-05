/*
 * Compile me with:
 *   gcc -o entradas entradas.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0)
 */
 
/*#include "support.h"*/

#define UI_FILE "entradas.glade"
 
typedef struct _Data Data;
struct _Data
{
	GtkWidget *principal;
	GtkWidget *txtcodigo;
};

int
main( int    argc,
      char **argv )
{
	Data	data;
	GtkBuilder *builder;
	//GtkWidget  *window;
	GError     *error = NULL;
 
	/* Init GTK+ */
	gtk_init( &argc, &argv );
 
	/* Create new GtkBuilder object */
	builder = gtk_builder_new();
	/* Load UI from file. If error occurs, report it and quit application.
	* Replace "tut.glade" with your saved project. */
	if( ! gtk_builder_add_from_file( builder, UI_FILE, &error ) )
	{
		g_warning( "%s", error->message );
		g_free( error );
		return( 1 );
	}
 
	/* Almacena la estructura de datos */
	data = g_slice_new( ChData );

	/* Obtiene los objects de la UI */
#define GW( name ) CH_GET_WIDGET( builder, name, data )
	GW( principal );
#undef GW

	/* Get main window pointer from UI (antes de agregar support.h) */
	//window = GTK_WIDGET( gtk_builder_get_object( builder, "principal" ) );
 
	/* Connect signals */
	gtk_builder_connect_signals( builder, NULL );
 
	/* Destroy builder, since we don't need it anymore */
	g_object_unref( G_OBJECT( builder ) );
 
	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show( data->principal );
 
	/* Start main loop */
	gtk_main();

	/* Libera memoria */
	g_slice_free( ChData, data );
 
    return( 0 );
}
