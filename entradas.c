/*
 * Compile me with:
 *   gcc -o entradas entradas.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0)
 */
 
#include "support.h"

//#define UI_FILE "entradas.glade"

void on_treeselectionProveedores_changed( GtkTreeSelection *treeselectionProveedores, Data *data );

int
main( int    argc,
      char **argv )
{
	//gint result;
	Data	data;
	GtkBuilder *builder;
	//GtkWidget  *window;
	GError     *error = NULL;
        char uifile[128];
        
        homedir = getenv("HOME");  // Directorio Home del usuario
        strcpy(uifile, homedir);
        strcat(uifile, "/.carnesbecerra/entradas.glade"); // Archivo donde se define la interface
        
	/* Init GTK+ */
	gtk_init( &argc, &argv );
 
	/* Create new GtkBuilder object */
	builder = gtk_builder_new();
	/* Load UI from file. If error occurs, report it and quit application.
	* Replace "tut.glade" with your saved project. */
	if( ! gtk_builder_add_from_file( builder, uifile, &error ) )
	{
		g_warning( "%s", error->message );
		g_free( error );
		return( 1 );
	}
 
	data.principal = GTK_WIDGET( gtk_builder_get_object( builder, "principal" ) );
	data.txtcodigo = GTK_WIDGET( gtk_builder_get_object( builder, "txtcodigo" ) );
	data.liststoreArticulos = GTK_LIST_STORE( gtk_builder_get_object( builder, "liststoreArticulos" ) );
	data.treeview = GTK_WIDGET( gtk_builder_get_object( builder, "treeview" ) );
	data.dialogLimpiar = GTK_WIDGET( gtk_builder_get_object( builder, "dialogLimpiar" ) );
	data.liststoreProveedores = GTK_LIST_STORE( gtk_builder_get_object( builder, "liststoreProveedores" ) );
	data.dialogLogin = GTK_WIDGET( gtk_builder_get_object( builder, "dialogLogin" ) );
	data.statusbar = GTK_WIDGET( gtk_builder_get_object( builder, "statusbar" ) );
	data.txtproveedor = GTK_WIDGET( gtk_builder_get_object( builder, "txtproveedor" ) );
	data.btnBuscarProveedor = GTK_WIDGET( gtk_builder_get_object( builder, "btnBuscarProveedor" ) );
	data.treeviewProveedores = GTK_WIDGET( gtk_builder_get_object( builder, "treeviewProveedores" ) );
	data.btnAgregar = GTK_WIDGET( gtk_builder_get_object( builder, "btnAgregar" ) );
	data.lblcodigo = GTK_WIDGET( gtk_builder_get_object( builder, "lblcodigo" ) );
	data.treeselectionProveedores = GTK_TREE_SELECTION( gtk_builder_get_object( builder, "treeselectionProveedores" ) );
	data.windowBuscarArticulo = GTK_WIDGET( gtk_builder_get_object( builder, "windowBuscarArticulo" ) );
	data.liststoreBuscarArticulos = GTK_LIST_STORE( gtk_builder_get_object( builder, "liststoreBuscarArticulos" ) );
	data.txtBuscarArticulo = GTK_WIDGET( gtk_builder_get_object( builder, "txtBuscarArticulo" ) );
	data.treeviewBuscarArticulo = GTK_WIDGET( gtk_builder_get_object( builder, "treeviewBuscarArticulo" ) );
	data.btnAceptarBuscarArticulo = GTK_WIDGET( gtk_builder_get_object( builder, "btnAceptarBuscarArticulo" ) );
	data.dialogGuardar = GTK_WIDGET( gtk_builder_get_object( builder, "dialogGuardar" ) );
	data.windowLogin = GTK_WIDGET( gtk_builder_get_object( builder, "windowLogin" ) );
	data.txtusuario = GTK_WIDGET( gtk_builder_get_object( builder, "txtusuario" ) );
	data.txtpassword = GTK_WIDGET( gtk_builder_get_object( builder, "txtpassword" ) );
	data.btnGuardar = GTK_WIDGET( gtk_builder_get_object( builder, "btnGuardar" ) );
	data.stock3Dias = GTK_WIDGET( gtk_builder_get_object( builder, "stock3Dias" ) );
	data.dialogImprimir = GTK_WIDGET( gtk_builder_get_object( builder, "dialogImprimir" ) );
	
	//data.treeselectionProveedores = gtk_tree_view_get_selection (GTK_TREE_VIEW (data.treeviewProveedores));
	gtk_tree_selection_set_mode (data.treeselectionProveedores, GTK_SELECTION_SINGLE);

	/* Connect signals */
	gtk_builder_connect_signals( builder, &data );
 
	/* Destroy builder, since we don't need it anymore */
	g_object_unref( G_OBJECT( builder ) );

	gtk_widget_show( data.windowLogin );

	gtk_main();

    return( 0 );
}
