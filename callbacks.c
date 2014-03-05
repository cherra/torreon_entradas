#include "support.h"
#include "funciones.h"
#include "imprimir.c"
#include "impresion_ticket.h"


MYSQL mysql;
MYSQL_ROW row,row2,row3;
MYSQL_RES *res,*res2;
int er=0;
gboolean ya_codigo = FALSE;
GtkTreeIter iter_articulos;

G_MODULE_EXPORT
void inicializa_variables( GtkWidget *widget, Data *data )
{
    printf("Inicializando variables....\n");
    
    strcpy(file_db_config, homedir);
    strcat(file_db_config, "/.carnesbecerra/configuracionbd.dat");
    /*
     * Se inicializan las variables de configuración para impresiones
     */
    strcpy(ImpresoraConfig, homedir);
    strcat(ImpresoraConfig, "/.carnesbecerra/impresoras.conf");
    strcpy(TicketImpresion, homedir);
    strcat(TicketImpresion, "/.carnesbecerra/impresion/impresiones-entradas-tmp.txt");
    
    printf("Listo!\n");
        
}

int conecta_bd()
{
	FILE *fconfiguracionbd;
	int i=0;
	int j=0;
	char tipo[50]="";
	char dato[50]="";
	char tmp;
	char ip[50]="";
	char usuario[50]="";
	char contrasena[50]="";
	char bd[50]="";

        
	if((fconfiguracionbd = fopen(file_db_config,"r")))
	{
		while(!feof(fconfiguracionbd))
		{
			tmp = fgetc(fconfiguracionbd);
			if(tmp != ' ')
			{
				tipo[i] = tmp;
				i++;
			}
			else
			{
				while(tmp != '\n')
				{
					tmp = fgetc(fconfiguracionbd);
					dato[j]=tmp;
					j++;
				}
				dato[j-1] = '\0';
				j=0;
				if(strcmp(tipo,"ip") == 0)
					strcpy(ip, dato);
				if(strcmp(tipo,"usuario") == 0)
					strcpy(usuario, dato);
				if(strcmp(tipo,"contrasena") == 0)
					strcpy(contrasena, dato);
				if(strcmp(tipo,"bd") == 0)
					strcpy(bd, dato);
				strcpy(tipo,"");
				for(i=0;i<30;i++)
					tipo[i]='\0';
				i=0;
			}
		}
		fclose(fconfiguracionbd);

		mysql_init(&mysql);
		if(!(mysql_real_connect(&mysql, ip, usuario, contrasena, bd, 0, NULL, 0)))
			return -1;
		else
			return 1;
	}
	else
	{
		printf("No se pudo abrir el archivo de configuracion: %s\n", file_db_config);
		return -1;
	}
}

gboolean dialogo_error( const char *mensaje, Data *data )
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new( GTK_WINDOW( data->principal ), GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", mensaje );
	gtk_dialog_run( GTK_DIALOG( dialog ) );
	gtk_widget_destroy( dialog );
}

gboolean dialogo_operacion_bien( const char *mensaje, Data *data )
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new( GTK_WINDOW( data->principal ), GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s", mensaje );
	gtk_dialog_run( GTK_DIALOG( dialog ) );
	gtk_widget_destroy( dialog );
}


G_MODULE_EXPORT
void on_txtpassword_activate( GtkWidget *widget, Data *data )
{
	char sql[142];
	const gchar *usuario, *password;
	gchar *nombre;

	usuario = gtk_entry_get_text( GTK_ENTRY( data->txtusuario ) );
	password = gtk_entry_get_text( GTK_ENTRY( data->txtpassword ) );
	
	sprintf( sql, "SELECT id_usuario, nombre FROM Usuario WHERE username = '%s' AND password = '%s' AND eliminado = 'n'", usuario, password);
	//g_print("El SQL para obtener lista de proveedores: %s\n", sql);

	if( conecta_bd() == -1 )
    	{
      		g_print( "No se pudo conectar a la base de datos. Error: %s\n", mysql_error( &mysql ) );
    	}
	else
	{
		er = mysql_query( &mysql,sql );
		if(er != 0)
			g_print( "Error en el query de articulos: %s\n%s\n", mysql_error( &mysql ),sql );
		else
		{
			res2 = mysql_store_result( &mysql );
			if(res2)
			{
				if( mysql_num_rows(res2) > 0 )
				{
					gtk_widget_destroy( data->windowLogin );
					if( row2 = mysql_fetch_row(res2) )
					{
						nombre = g_strdup_printf("Bienvenido %s", row2[1]);
						gtk_statusbar_push( GTK_STATUSBAR( data->statusbar ), 1, nombre );
						g_free( nombre );
					}
					else
						printf("Error en el row: %s\n", mysql_error(&mysql));
					mysql_close(&mysql);
					gtk_widget_show( data->principal );
				}
				else
					dialogo_error("El usuario o la contraseña no son válidos.\nIntentalo de nuevo.", data );
			}
		}
	}
}


gboolean agrega_articulo( const char *codigo, Data *data )
{
	gchar *barcode;
	const char *cantidad;
	char codigo_subproducto[4], codigo_articulo[2], kilos[4], bascula[3], gramos[3], vendedor[3], peso[6], tipo_codigo[2];
	char sqlarticulo[200];


	if( strlen( codigo ) > 0 )
	{	
		if( strlen( codigo ) < 13 )
		{
			if( !(ya_codigo) )
			{
				barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 0, 3);
			        strcpy(codigo_subproducto, barcode);
			        barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 3, 4);
			        strcpy(codigo_articulo, barcode);
				gtk_label_set_markup(GTK_LABEL( data->lblcodigo ), "Cantidad: ");
				ya_codigo = TRUE;
			}
			else
			{
				cantidad = gtk_entry_get_text( GTK_ENTRY( data->txtcodigo ) );
				sprintf(peso,"%.2f",atof(cantidad));
				gtk_list_store_set( data->liststoreArticulos, &iter_articulos, 3, peso , -1 );
				ya_codigo = FALSE;
				gtk_label_set_markup(GTK_LABEL( data->lblcodigo ), "Código: ");
				gtk_entry_set_text( GTK_ENTRY( data->txtcodigo ), "");
				gtk_widget_set_sensitive( GTK_WIDGET( data->btnGuardar ), TRUE );
			}
		}
		else
		{
			barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 0, 1);
			strcpy(tipo_codigo, barcode);
			barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 1, 4);
			strcpy(codigo_subproducto, barcode);
			barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 4, 5);
			strcpy(codigo_articulo, barcode);
			if(strcmp(tipo_codigo,"9") == 0){
				barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 5, 8);
				strcpy(kilos, barcode);
				barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 8, 10);
				strcpy(gramos, barcode);
			}else{
				barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 5, 7);
                                strcpy(kilos, barcode);
                                barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 7, 9);
                                strcpy(gramos, barcode);
			}
			barcode = gtk_editable_get_chars(GTK_EDITABLE(data->txtcodigo), 10, 12);
			strcpy(vendedor, barcode);
			sprintf(peso, "%d.%s", atoi(kilos), gramos);
			printf("El codigo: %s%s\n", codigo_subproducto, codigo_articulo);
			
			ya_codigo = TRUE;
		}
		if(ya_codigo)
		{
			if(conecta_bd() == 1)
			{
				printf("Se conecta a la base de datos\n");
				sprintf(sqlarticulo,"SELECT Articulo.nombre, Articulo.id_articulo FROM Articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto WHERE Articulo.codigo = '%s' AND Subproducto.codigo = '%s'", codigo_articulo, codigo_subproducto);
				printf("Cadena SQL: %s\n", sqlarticulo);
				er = mysql_query(&mysql, sqlarticulo);
				if(er == 0)
				{
					res = mysql_store_result(&mysql);
					if(res)
					{
						if(mysql_num_rows(res)>0)
						{
							printf("Bien el SQL\n");
							strcat(codigo_subproducto, codigo_articulo);
							row = mysql_fetch_row(res);
							gtk_list_store_append( data->liststoreArticulos, &iter_articulos);
							if( strlen( codigo ) < 13 )
							{
								gtk_list_store_set( data->liststoreArticulos, &iter_articulos, 0, row[1], 1, codigo_subproducto, 2, row[0], -1);
								gtk_widget_set_sensitive( GTK_WIDGET( data->btnGuardar ), FALSE );
							}
							else
							{
								gtk_list_store_set( data->liststoreArticulos, &iter_articulos, 0, row[1], 1, codigo_subproducto, 2, row[0], 3, peso, -1);
								ya_codigo = FALSE;
							}
						}
						else
							ya_codigo = FALSE;
					}
				}
				mysql_close(&mysql);
			}
			else
			{
				printf("Error al conectar al servidor: %s\n", mysql_error(&mysql));
				dialogo_error( "Error al conectar a la base de datos.", data );
			}
			gtk_entry_set_text( GTK_ENTRY( data->txtcodigo ), "");
			//g_free(barcode);
		}
	}
	else
	{
		if(ya_codigo)
			dialogo_error( "Es necesario insertar una cantidad.", data );
		else
			dialogo_error( "Es necesario insertar un código.", data );
	}
}


G_MODULE_EXPORT
void on_btnQuitar_clicked( GtkButton *btnQuitar, Data *data )
{
	GtkTreeSelection *seleccion;
	GtkTreeIter iter;
	GtkTreeModel *model;

	seleccion = gtk_tree_view_get_selection( GTK_TREE_VIEW(data->treeview) );
	if( gtk_tree_selection_get_selected( GTK_TREE_SELECTION( seleccion ), &model, &iter ) )
	{
		gtk_list_store_remove( data->liststoreArticulos, &iter );
	}
	gtk_widget_grab_focus( data->txtcodigo );
}

gboolean limpiar_lista_articulos( Data *data )
{
	gint result;

	/*g_print("Si funciona el signal\n");
	model = gtk_tree_view_get_model( GTK_TREE_VIEW( data->treeview ) );
	if( gtk_tree_model_get_iter_first( model, &iter ) == FALSE )
	{
		g_print("Entro al if\n");
		return;
	}*/
	result = gtk_dialog_run( GTK_DIALOG( data-> dialogLimpiar ) );
	switch ( result )
	{
		case 1:
			gtk_list_store_clear( data->liststoreArticulos );
			break;
		default:
			break;
	}
	gtk_widget_hide( data->dialogLimpiar );
	gtk_widget_grab_focus( data->txtcodigo );
}


G_MODULE_EXPORT
void on_btnLimpiar_clicked( GtkButton *btnLimpiar, Data *data )
{
	limpiar_lista_articulos( data );
}

G_MODULE_EXPORT
void on_comboProveedor_changed( GtkComboBox *comboProveedor, Data *data )
{
	gtk_widget_grab_focus( data->txtcodigo );
}


gboolean carga_proveedores( const char *filtro, Data *data )
{
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(data->treeviewProveedores) );
	GtkTreeIter iter;
	char sql[256];
	gchararray razon_social;
	gchararray id_proveedor;
	GError *error;

	sprintf( sql, "SELECT id_proveedor, razon_social FROM Proveedor WHERE razon_social LIKE '%%%s%%' ORDER BY razon_social LIMIT 100",filtro );
	//g_print("El SQL para obtener lista de proveedores: %s\n", sql);

	if( conecta_bd() == -1 )
    	{
      		g_print( "No se pudo conectar a la base de datos. Error: %s\n", mysql_error( &mysql ) );
    	}
	else
	{
		er = mysql_query( &mysql,sql );
		if(er != 0)
			g_print( "Error en el query de proveedores: %s\n%s\n", mysql_error( &mysql ),sql );
		else
		{
			res = mysql_store_result( &mysql );
			if(res)
			{
				gtk_list_store_clear( data->liststoreProveedores );
				while( ( row=mysql_fetch_row( res ) ) )
				{
					error = NULL;
					id_proveedor = row[0];
					razon_social = g_locale_to_utf8( row[1], -1, NULL, NULL, &error );
					if( error != NULL )
					{
						g_print( "Error al agregar un proveedor a la lista: %s\n%s\n", row[1], error->message );
						razon_social = row[1];
					}
					if( strlen( razon_social ) > 0 )
					{
						gtk_list_store_append( data->liststoreProveedores, &iter);
						gtk_list_store_set( data->liststoreProveedores, &iter, 0, id_proveedor, 1, razon_social, -1);
					}
				}
				gtk_tree_selection_select_path(selection, gtk_tree_path_new_first());
			}
		}
		mysql_close(&mysql);
	}
}

G_MODULE_EXPORT
void on_principal_show( GtkWidget *principal, Data *data )
{
	carga_proveedores( "", data );
	gtk_widget_grab_focus( data->txtproveedor );
}


G_MODULE_EXPORT
void on_txtcodigo_activate( GtkEntry *txtcodigo, Data *data )
{
	GtkTreeIter iterProveedores;
	GtkTreeSelection *seleccion;
	GtkTreeModel *model;
	
	seleccion = gtk_tree_view_get_selection( GTK_TREE_VIEW(data->treeviewProveedores) );
	if( gtk_tree_selection_get_selected( GTK_TREE_SELECTION( seleccion ), &model, &iterProveedores ) )
	{
		agrega_articulo( gtk_entry_get_text( GTK_ENTRY( data->txtcodigo ) ), data );
	}
	else
		dialogo_error( "Para agregar un articulo es necesario seleccionar un proveedor de la lista.", data );

	gtk_widget_grab_focus( data->txtcodigo );
}


G_MODULE_EXPORT
void on_txtproveedor_changed( GtkEntry *txtproveedor, Data *data )
{
	carga_proveedores( gtk_entry_get_text( GTK_ENTRY( data->txtproveedor ) ), data );
}

gboolean carga_articulos( const char *filtro, Data *data )
{
	GtkTreeSelection *selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW(data->treeviewBuscarArticulo) );
	GtkTreeIter iter;
	char sql[384];
	gchararray codigo;
	gchararray nombre;
	GError *error;

	sprintf( sql, "SELECT CONCAT(Subproducto.codigo, Articulo.codigo), Articulo.nombre FROM Articulo INNER JOIN Subproducto USING(id_subproducto) WHERE Articulo.nombre LIKE '%%%s%%' AND Articulo.id_linea NOT IN( 0, 10 ) AND Articulo.codigo IS NOT NULL ORDER BY Articulo.nombre LIMIT 100",filtro );
	//g_print("El SQL para obtener lista de proveedores: %s\n", sql);

	if( conecta_bd() == -1 )
    	{
      		g_print( "No se pudo conectar a la base de datos. Error: %s\n", mysql_error( &mysql ) );
    	}
	else
	{
		er = mysql_query( &mysql,sql );
		if(er != 0)
			g_print( "Error en el query de articulos: %s\n%s\n", mysql_error( &mysql ),sql );
		else
		{
			res = mysql_store_result( &mysql );
			if(res)
			{
				gtk_list_store_clear( data->liststoreBuscarArticulos );
				while( ( row=mysql_fetch_row( res ) ) )
				{
					error = NULL;
					codigo = row[0];
					nombre = g_locale_to_utf8( row[1], -1, NULL, NULL, &error );
					if( error != NULL )
					{
						g_print( "Error al agregar un articulo a la lista: %s\n%s\n", row[1], error->message );
						nombre = row[1];
					}
					if( strlen( nombre ) > 0 )
					{
						gtk_list_store_append( data->liststoreBuscarArticulos, &iter);
						gtk_list_store_set( data->liststoreBuscarArticulos, &iter, 0, codigo, 1, nombre, -1);
					}
				}
				gtk_tree_selection_select_path(selection, gtk_tree_path_new_first());
			}
		}
		mysql_close(&mysql);
	}
}

G_MODULE_EXPORT
void on_windowBuscarArticulo_show( GtkWidget *windowBuscarArticulo, Data *data )
{
	carga_articulos( gtk_entry_get_text( GTK_ENTRY( data->txtBuscarArticulo ) ), data );
	gtk_widget_grab_focus( data->txtBuscarArticulo );
}


G_MODULE_EXPORT
void on_txtBuscarArticulo_changed( GtkEntry *txtBuscarArticulo, Data *data )
{
	carga_articulos( gtk_entry_get_text( GTK_ENTRY( txtBuscarArticulo ) ), data );
}


G_MODULE_EXPORT
gboolean on_windowBuscarArticulo_key_release_event( GtkWidget *windowBuscarArticulo, GdkEvent *event, Data *data )
{
	if(event->key.keyval == 65307)
	{
		gtk_widget_hide(windowBuscarArticulo);
		gtk_widget_grab_focus( data->txtcodigo );
	}
}


G_MODULE_EXPORT
on_btnAceptarBuscarArticulo_clicked( GtkButton *btnAceptarBuscarArticulo, Data *data )
{
	GtkTreeIter iter;
	GtkTreeSelection *seleccion;
	GtkTreeModel *model;
	gchar *codigo;

  	seleccion = gtk_tree_view_get_selection(GTK_TREE_VIEW( data->treeviewBuscarArticulo ));
  	if (gtk_tree_selection_get_selected( seleccion, &model, &iter )) {
    		gtk_tree_model_get(model, &iter, 0, &codigo, -1);
		printf("Se seleccionó algo: %s\n", codigo);
		gtk_entry_set_text( GTK_ENTRY( data->txtcodigo), codigo );
		g_signal_emit_by_name( data->txtcodigo, "activate" );
		g_free( codigo );
  	}
	//gtk_entry_set_text( GTK_ENTRY(data->txtcodigo), codigo );
	gtk_widget_hide( data->windowBuscarArticulo );
	gtk_widget_grab_focus( data->txtcodigo );
}


G_MODULE_EXPORT
void on_treeviewBuscarArticulo_row_activated( GtkTreeView *treeviewBuscarArticulo, GtkTreePath *path, GtkTreeViewColumn *column, Data *data )
{
	g_signal_emit_by_name( data->btnAceptarBuscarArticulo, "clicked" );
}


gboolean leer_fila( GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer user_data )
{
	gchar *id_articulo, *codigo, *descripcion, *cantidad, *tree_path_str;
	char sql[200];
	long int id_entrada;

	gtk_tree_model_get( model, iter, 0, &id_articulo, 1, &codigo, 2, &descripcion, 3, &cantidad, -1 );
	tree_path_str = gtk_tree_path_to_string( path );

	g_print( "\nEl pointer = %d\n\n", GPOINTER_TO_INT( user_data ) );

	sprintf( sql, "INSERT INTO Entrada_Almacen_Articulo ( id_entrada, id_articulo, cantidad ) VALUES( %d, %s, %s )", GPOINTER_TO_INT( user_data ), id_articulo, cantidad );
	g_print("El SQL para insertar el artículo: %s\n", sql);

	if( conecta_bd() == -1 )
    	{
      		g_print( "No se pudo conectar a la base de datos. Error: %s\n", mysql_error( &mysql ) );
    	}
	else
	{
		er = mysql_query( &mysql,sql );
		if(er != 0)
			g_print( "Error en el query de entrada de articulos: %s\n%s\n", mysql_error( &mysql ),sql );
		else
		{
			res = mysql_store_result( &mysql );
			if(res)
			{
				g_print( "Se insertó el artículo %s\n", codigo );
			}
		}
		mysql_close(&mysql);
	}
	
	g_free( tree_path_str );
	g_free( descripcion );
	g_free( cantidad );

	return FALSE;
}


G_MODULE_EXPORT
void on_btnGuardar_clicked( GtkButton *btnGuardar, Data *data )
{
	char sql[200];
	gint result, id_entrada;
	GtkTreeIter iter;
	GtkTreeSelection *seleccion;
	GtkTreeModel *model, *model2;
	gchar *id_proveedor;
	gint numRows;
	char id_entrada_char[10];

	/* Obtengo el número de filas en la lista de articulos */
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->treeview));
	numRows = gtk_tree_model_iter_n_children(model, NULL);

	if(numRows > 0)
	{
		result = gtk_dialog_run( GTK_DIALOG( data-> dialogGuardar ) );
		switch ( result )
		{
			case 1:
			{
				seleccion = gtk_tree_view_get_selection(GTK_TREE_VIEW( data->treeviewProveedores ));
  				if (gtk_tree_selection_get_selected( seleccion, &model, &iter )) {
    					gtk_tree_model_get(model, &iter, 0, &id_proveedor, -1);
					printf("Proveedor seleccionado: %s\n", id_proveedor);
  				}
				sprintf( sql, "INSERT INTO Entrada_Almacen ( id_usuario, id_proveedor, fecha ) VALUES( 1, %s, NOW() )", id_proveedor );
	
				if( conecta_bd() == -1 )
   			 	{
    			  		g_print( "No se pudo conectar a la base de datos. Error: %s\n", mysql_error( &mysql ) );
			    	}
				else
				{
					er = mysql_query( &mysql,sql );
					if(er != 0)
						g_print( "Error en el query de entrada de articulos: %s\n%s\n", mysql_error( &mysql ),sql );
					else
					{
						id_entrada = mysql_insert_id( &mysql );
						sprintf(id_entrada_char,"%d",id_entrada);
						gtk_tree_model_foreach( GTK_TREE_MODEL( data->liststoreArticulos ), leer_fila, GINT_TO_POINTER( id_entrada ) );
						gtk_list_store_clear( data->liststoreArticulos );
						gtk_widget_hide( data->dialogGuardar );
						imprimirticket(id_entrada_char, "entrada_almacen", 0);
						dialogo_operacion_bien("Entrada a almacén registrada con éxito", data );
					}
					mysql_close( &mysql );
				}
				break;
			}
			default:
				break;
		}
		gtk_widget_grab_focus( data->txtproveedor );
	}
	else
		dialogo_error("La lista de artículos está vacía", data );
}


G_MODULE_EXPORT
void on_treeviewProveedores_cursor_changed( GtkWidget *treeviewProveedores, Data *data )
{
	GtkTreeIter iter;
	GtkTreeSelection *seleccion;
	GtkTreeModel *model;
	gchar *id_proveedor;

  	seleccion = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewProveedores));
  	if (gtk_tree_selection_get_selected(seleccion, &model, &iter)) {
    		gtk_tree_model_get(model, &iter, 0, &id_proveedor, -1);
		printf("Se seleccionó algo: %s\n", id_proveedor);
		g_free(id_proveedor);
  	}
}


G_MODULE_EXPORT
void on_stock3Dias_activate( GtkMenuItem *stock3Dias, Data *data )
{
	gint result;

	g_print("Si funciona el signal\n");
	
	result = gtk_dialog_run( GTK_DIALOG( data-> dialogImprimir ) );
	switch ( result )
	{
		case 1:
			imprimirticket("0", "stock_3_dias", 0);
			break;
		default:
			break;
	}
	gtk_widget_hide( data->dialogImprimir );
	gtk_widget_grab_focus( data->txtcodigo );
}

G_MODULE_EXPORT
void on_stock3DiasSubproductos_activate( GtkMenuItem *stock3DiasSubproductos, Data *data )
{
	gint result;

	g_print("Si funciona el signal\n");
	
	result = gtk_dialog_run( GTK_DIALOG( data-> dialogImprimir ) );
	switch ( result )
	{
		case 1:
			imprimirticket("0", "stock_3_dias_subproductos", 0);
			break;
		default:
			break;
	}
	gtk_widget_hide( data->dialogImprimir );
	gtk_widget_grab_focus( data->txtcodigo );
}
