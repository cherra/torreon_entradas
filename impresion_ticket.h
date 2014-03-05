/* Funcion que imprime Ticket ya sean de contado o credito
*
*
*/

#include <stdarg.h>

gboolean imprimiendo = FALSE; /* Bandera de impresion !!! */

#define MAXFIL 9999

MYSQL mysql;
MYSQL_RES *resultado, *resultado2, *resultado3;
MYSQL_ROW row, row2, row3;
FILE *fpt, *fpt2, *fptlogo, *fpttemp; //Declaracion del archivo

char tipoPago[20]="";
char c[1500]; //Aqui se guarda las cadenas a imprimir
int fac_nombre[99][4];
int impresora_nombre[99][4];

char Errores[200];

//Variables del ARRAY
int fila, nfilas, ncols;
char *matrix[MAXFIL];
char retVar[500]; //Cadena con la cantidad con letra

int abrir_cajon (char puerto[50]);


//Traduce
void traduce(double numero, char retVar[500]){
// numero   -> Numero a traducir
// retVar     -> Cadena con la traduccion
	int i, j, a;
	int bandera=0;
	int veces;
	char numero_char[20]; //Numero en cadena
	char num[20];
	int num_num, num_num2, num_num3; //Numero Temporales
	char num_tmp[20]; //Temporal de numero

	char respnu[20]; //Respaldo num
	char centavos[20];
	int grande; //Tamao de la cadena
	int largo;
	int reslarg; //Respaldo

	int inicio;
	int fin;
	int inicio_fin;
	char tango[20];
	char pesos[10];


	char unidt[50]; //Unidades
	char decet[50]; //Decenas
	char centt[50]; //Centenas
	char millt[50]; //Millardos
	char decmillt[50]; //Decenamillardos
	char ciecentt[50]; //Cientmillardos
	char veintes[50];
	char textt[300]; //AQUI SE GUARDA LA CADENA

	char *unidad[10];
	char *decena[10];
	char *centena[10];
	char *millar[10];

	unidad[0] = "cero";
	unidad[1] = "un";
	unidad[2] = "dos";
	unidad[3] = "tres";
	unidad[4] = "cuatro";
	unidad[5] = "cinco";
	unidad[6] = "seis";
	unidad[7] = "siete";
	unidad[8] = "ocho";
	unidad[9] = "nueve";

	decena[1] = "diez";
	decena[2] = "veinte";
	decena[3] = "treinta";
	decena[4] = "cuarenta";
	decena[5] = "cincuenta";
	decena[6] = "sesenta";
	decena[7] = "setenta";
	decena[8] = "ochenta";
	decena[9] = "noventa";

	centena[1] = "ciento";
	centena[2] = "doscientos";
	centena[3] = "trescientos";
	centena[4] = "cuatrocientos";
	centena[5] = "quinientos";
	centena[6] = "seiscientos";
	centena[7] = "setecientos";
	centena[8] = "ochocientos";
	centena[9] = "novecientos";

	millar[1] = "mil";
	millar[2] = "dos mil";
	millar[3] = "tres mil";
	millar[4] = "cuatro mil";
	millar[5] = "cinco mil";
	millar[6] = "seis mil";
	millar[7] = "siete mil";
	millar[8] = "ocho mil";
	millar[9] = "nueve mil";

	strcpy(unidt,""); //Unidades
	strcpy(decet,""); //Decenas
	strcpy(centt,""); //Centenas
	strcpy(millt,""); //Millardos
	strcpy(decmillt,""); //Decenamillardos
	strcpy(ciecentt,""); //Cientmillardos

	sprintf(numero_char, "%.2f", numero); //Convierte a cadena
	printf("\nNUMERO A CONVERTIR: %s\n", numero_char);
	grande = strlen(numero_char);
	strcpy(num,"");
	strcpy(centavos,"");
	for ( a=0; a< grande; a++ ) {
		//printf("\nA: %c",numero_char[a]);
		if (numero_char[a] == '.' ) {  //substr(a, 1 );
				//num = numero.substr(0, a );
				strncat(num,numero_char,a);
				if (a < grande - 1){
						j=0;
						for (i=a+1;i<grande;i++) //centavos = numero.substr(a + 1, 2);
						{
							if(j<2){
								centavos[j] = numero_char[i];
								centavos[j+1] = '\0';
							}else
								break;
							j++;
						}

				} else {
						strcpy(centavos,"00");
				}
				/*
				if (parseInt(numero.substr(a + 1, 2)) < 10 && (parseInt(numero.substr( a + 1, 1))) > 0 )
					centavos = centavos + "0";
				*/
				bandera = 99;
		}
	}

	if ((bandera != 99 )) {
		strcpy(num,numero_char);
		strcpy(centavos,"00");
	}
	largo = strlen(num);
	veces = 1;

	if (largo > 6 ) {
			strcpy(respnu,num);
			reslarg = largo;
			strcpy(num_tmp,"");
			strncat(num_tmp,num,largo-6);
			strcpy(num, num_tmp);
			//num = num.substr(0,largo - 6);
			largo = largo - 6;
			veces = 2;
	}

	for ( a=1; a<= veces; a++ ) {

		if (a == 2 ) {
				j=0;
				for(i=largo;i<strlen(respnu);i++){ 		//num = respnu.substring(respnu.length - 6 , respnu.length)
					num[j] = respnu[i];
					num[j+1] = '\0';
					j++;
				}
				largo = 6;
		}



		sprintf(num_tmp, "%s", num); //Convierte a cadena
		num_num = atoi(num_tmp); //Convierte a numero
		if (num_num % 10 != 0 || numero == 0){
				//unidt = unidad[num.substr(largo - 1, 1)];
				sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
				num_num = atoi(num_tmp);
				strcpy(unidt,unidad[num_num]);
			} else {
				strcpy(unidt,"");
		}




		if (largo > 1 ) {
			sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 0 ){
				sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
				num_num = atoi(num_tmp);
				if(num_num > 0 ){
					sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decet,decena[num_num]);
					strcat(decet," y ");
				} else {
					sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decet,decena[num_num]);
				}
			} else {
				strcpy(decet,"");
			}
		}

		if (largo > 2 ) {

			sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
				num_num = atoi(num_tmp);
				strcpy(centt,centena[num_num]);
				strcat(centt," ");
			} else {

				sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
				num_num = atoi(num_tmp);

				if(num_num == 0 ){
					strcpy(centt," ");
				}


				//if (parseInt(num.substr( largo - 3, 1)) == 1 && (parseInt(num.substr(2,num.length))) > 0 )
				//	centt = centena[parseInt(num.substr(largo - 3, 1))] + " ";
				sprintf(num_tmp, "%c", num[largo-3]); //Primera parte del IF
				num_num = atoi(num_tmp);

				j=2;
				for(i=1;i<strlen(num);i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num2 = atoi(num_tmp);
				if(num_num == 1 && num_num2 > 0){
					sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(centt,centena[num_num]);
					strcat(centt," ");
				}


				//if (parseInt(num.substr( largo - 3, 1)) == 1 && (parseInt(num.substr(2,num.length))) == 0 )
				//	centt = "cien";

				sprintf(num_tmp, "%c", num[largo-3]); //Primera parte del IF
				num_num = atoi(num_tmp);

				if(num_num == 1 && num_num2 == 0){
					strcpy(centt,"cien");

				}

			}

		}


		if (largo > 3 ) {

			sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				strcpy(millt,unidad[num_num]);
				strcat(millt," mil ");

			} else {
				if(num_num == 1 ){
					strcpy(millt,"un mil ");
				}
				if(num_num == 0 ){
					strcpy(millt," mil ");
				}
			}
		}


		if (largo > 4 ) {
			sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 0 ){
				sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
				num_num = atoi(num_tmp);
				if(num_num > 0 ){
					sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decmillt,decena[num_num]);
					strcat(decmillt," y ");
				} else {
					sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decmillt,decena[num_num]);
				}

				} else {
					strcpy(decmillt,"");
				}

		}

		if (largo > 5 ) {
			sprintf(num_tmp, "%c", num[largo-6]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				strcpy(ciecentt,centena[num_num]);
				strcat(ciecentt," ");
			} else {
				if(num_num == 0 ){
					strcpy(ciecentt, "");
				}

				j=largo-5;
				for(i=1;i<2;i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num2 = atoi(num_tmp);
				if(num_num == 1 && num_num2 > 0){
					strcpy(ciecentt,centena[num_num]);
					strcat(ciecentt," ");
				}
					if(num_num == 1 && num_num2 == 0){
					strcpy(ciecentt,"cien ");
				}
			}
		}

		if (strcmp(decet,"diez y ") == 0) {
			sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
			num_num = atoi(num_tmp);

			switch ( num_num ){
			case 1:
				strcpy(veintes,"once");
				break;
			case 2:
				strcpy(veintes,"doce");
				break;
			case 3:
				strcpy(veintes,"trece");
				break;
			case 4:
				strcpy(veintes,"catorce");
				break;
			case 5:
				strcpy(veintes,"quince");
				break;
			case 6:
				strcpy(veintes,"dieciseis");
				break;
			case 7:
				strcpy(veintes,"diecisiete");
				break;
			case 8:
				strcpy(veintes,"dieciocho");
				break;
			case 9:
				strcpy(veintes,"diecinueve");
				break;
			case 0:
				strcpy(veintes,"diez");

				break;
   			}
			strcpy(decet,veintes);
			strcpy(unidt,"");
		}


	if (strcmp(decet,"veinte y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"veintiun");
			break;
		case 2:
			strcpy(veintes,"veintidos");
			break;
		case 3:
			strcpy(veintes,"veintitres");
			break;
		case 4:
			strcpy(veintes,"veinticuatro");
			break;
		case 5:
			strcpy(veintes,"veinticinco");
			break;
		case 6:
			strcpy(veintes,"veintiseis");
			break;
		case 7:
			strcpy(veintes,"veintisiete");
			break;
		case 8:
			strcpy(veintes,"veintiocho");
			break;
		case 9:
			strcpy(veintes,"veintinueve");
			break;
		case 0:
			strcpy(veintes,"veinte");
			break;
		}
		strcpy(decet,veintes);
		strcpy(unidt,"");
		}


	if (strcmp(decet,"diez y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
			case 1:
				strcpy(veintes,"once mil");
				break;
			case 2:
				strcpy(veintes,"doce mil");
				break;
			case 3:
				strcpy(veintes,"trece mil");
				break;
			case 4:
				strcpy(veintes,"catorce mil");
				break;
			case 5:
				strcpy(veintes,"quince mil");
				break;
			case 6:
				strcpy(veintes,"dieciseis mil");
				break;
			case 7:
				strcpy(veintes,"diecisiete mil");
				break;
			case 8:
				strcpy(veintes,"dieciocho mil");
				break;
			case 9:
				strcpy(veintes,"diecinueve mil");
				break;
			case 0:
				strcpy(veintes,"diez mil");
				break;
		}
		strcpy(decmillt,veintes);
		strcpy(millt,"");
		}

	if (strcmp(decet,"veinte y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"veintiun mil");
			break;
		case 2:
			strcpy(veintes,"veintidos mil");
			break;
		case 3:
			strcpy(veintes,"veintitres mil");
			break;
		case 4:
			strcpy(veintes,"veinticuatro mil");
			break;
		case 5:
			strcpy(veintes,"veinticinco mil");
			break;
		case 6:
			strcpy(veintes,"veintiseis mil");
			break;
		case 7:
			strcpy(veintes,"veintisiete mil");
			break;
		case 8:
			strcpy(veintes,"veintiocho mil");
			break;
		case 9:
			strcpy(veintes,"veintinueve mil");
			break;
		case 0:
			strcpy(veintes,"veinte");
			break;
		}
		strcpy(decmillt,veintes);
		strcpy(millt,"");
		}

		if (a == 1 && veces == 2 ) {
				strcpy(textt,ciecentt); //Se concatenan todas las variables
				strcat(textt,decmillt);
				strcat(textt,millt);
				strcat(textt,centt);
				strcat(textt,decet);
				strcat(textt,unidt);


				j=0;
				for(i=1;i<2;i++){
					num_tmp[i-1] = textt[j];
					num_tmp[i] = '\0';
					j++;
				}

				if (strcmp(num_tmp,"un") == 0) {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"milln");
				} else {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"millones");
				}
		}


		if (veces > 1 ) {
			inicio=strlen(num) - 6;
			fin=strlen(num);

			//substring
			if(inicio == fin)
				strcpy(num_tmp,"");
			else{
				if(inicio < 0)
					inicio = 0;
				if(fin >= strlen(num))
					fin = strlen(num)-1;
				if(inicio > fin){
					inicio_fin = inicio;
					inicio = fin;
					fin = inicio_fin;
				}
				j=0;
				for(i=inicio;i<fin;i++){
					num_tmp[j] = textt[i];
					num_tmp[j+1] = '\0';
					j++;
				}
			}
			//Fin funcion substring
			strcpy(tango, num_tmp);

			sprintf(num_tmp, "%c", tango[0]); //Convierte a cadena
			num_num = atoi(num_tmp);
			sprintf(num_tmp, "%c", tango[1]); //Convierte a cadena
			num_num2 = atoi(num_tmp);
			sprintf(num_tmp, "%c", tango[2]); //Convierte a cadena
			num_num3 = atoi(num_tmp);

			if((num_num == 0) && (num_num2 == 0) && (num_num3 == 0)){
				strcpy(millt,"");
			}
		}


		if (a == 1 && veces == 1 ){
			strcpy(textt,ciecentt); //Se concatenan todas las variables
			strcat(textt,decmillt);
			strcat(textt,millt);
			strcat(textt,centt);
			strcat(textt,decet);
			strcat(textt,unidt);
		}

		if (a == 2 && veces == 2 ){
			strcpy(textt,ciecentt); //Se concatenan todas las variables
			strcat(textt,decmillt);
			strcat(textt,millt);
			strcat(textt,centt);
			strcat(textt,decet);
			strcat(textt,unidt);
			}
	}


	if (numero >= 1 && numero < 2){
		strcpy(pesos," peso ");
	} else {
		strcpy(pesos," pesos ");
	}


	num_num = atoi(respnu);
		strcpy(retVar,"");
		//strcpy(retVar,textt);
		//strcat(retVar,centavos);
		//strcat(retVar,"/100 M.N. ");

	if (num_num >= 1000000  && (num_num % 1000000)==0){
		strcpy(retVar,textt);
		strcat(retVar," de pesos ");
		strcat(retVar,centavos);
		strcat(retVar,"/100 M.N. ");
	} else {
		strcpy(retVar,textt);
		strcat(retVar,pesos);
		strcat(retVar,centavos);
		strcat(retVar,"/100 M.N. ");
	}
printf("\nTEXTO: %s",retVar);



}


//Inicializa la MATRIX
void inicializar(){
 	int fila, col, n;
	n = 2;
	for(col=0;col<ncols;++col){
		for(fila=0; fila <=nfilas; ++fila){
			matrix[fila][col] = ' ';
			if(fila == nfilas)
				matrix[fila][col] = '\n';
		}
	}
}


void imprimir(char c[1000], int nX){
	int i, j;
        char cTMP[1000];
		//printf ("### CAD = %s \n",c);
        strcpy(cTMP,""); //Inicializacion
        i=0;
        j=1;
        while(i <= strlen(c)){
				//printf ("# I = %d \n",i);
				cTMP[i] = c[i];
                if(j == nX){
                        printf("\n");
                        fputs("\n",fpt);
                        j=0;
                }
		if (cTMP[i] == '\n')
		{
                        printf("\n");
                        //fputs("\n",fpt);
                        j=0;
		}
		
                printf("%c",cTMP[i]);
                fputc(cTMP[i],fpt);
                i++;
                j++;
        }
}


void genera_codigo_barras(char destino[18], char fuente[14])
{
	destino[0]=29;
	destino[1]=107;
	destino[2]=67;	/* 2 = EAN13 */
	destino[3]=13;  

	strcat(destino, fuente);
}


int imprimirticket(char *id_venta_char, char tipo[20], double pago_num, ...){
	int id_venta_num;
	//va_list lst_param;
	
	printf ("\n################################## \nInicia el proceso de impresión\n##################################\nid_entrada: %s\ntipo: %s\n\n", id_venta_char, tipo);
	printf ("\n");
	
	int i, j, k, m;
        int nX; //TamaÃ±o del papel de largo
/*        int numVenta; //Numero de venta*/
	char temp[9] = "         ";
	char temp3[40] = "                                        ";
	char sql[2000]; 
	char id_venta[10], num_articulos[300], cambio[10];
	char num_tmp[6];
	char cadconf[50];

	int err; //Variable de error de la consulta SQL

	//Temporales
	int num_numero_int = 0;
	char cad_temporal[255], cad_temporal2[255];

	//Para calculo de stock
	double venta_pasada = 0;
	double venta_actual = 0;
	double diferencia = 0;
	double total_3_dias = 0;
	double stock_sugerido = 0;
	double total_faltante = 0;
	double existencias = 0;


//CONFIGURACION IMPRESORA
	//int arr_impresora_entradas_almacen = 0;

	//char impresora_entradas_almacen[50];

//MINIPRINTER EPSON
	static char salto[4];
	static char reversa[4];
	static char color_rojo[4];
	static char color_negro[4];
	static char defecto[4],cancela[4];
	static char subraya_s[4],negrita_grande[4],negrita_subraya[4],negrita_subraya1[4],negrita[4],negrita1[4],subraya_s1[4], negrita_media[4];
	static char alinea_d[4],alinea_i[4],alinea_c[4];
	static char tamano16[4],tamano1[4], tamano2[4];
	static char resetea[3],corta_papel[3], corta_papel_modo[4];
	static char codigo_barras[18], altura_codigo_barras[4], cadena_codigo[18];
	
	static char espaciado[4];
	
	imprimiendo = TRUE;
	
	id_venta_num = atoi (id_venta_char);
	
	espaciado[0]=27;
	espaciado[1]=51;
	espaciado[2]=19;
	espaciado[3]='\0';

//MINIPRINTER
	tamano16[0] = 27;
	tamano16[1] = 33;
	tamano16[2] = 16;
	tamano16[3] = '\0';
	
	tamano1[0] = 27;
	tamano1[1] = 33;
	tamano1[2] = 1;
	tamano1[3] = '\0';
	
	tamano2[0] = 27;
	tamano2[1] = 33;
	tamano2[2] = 4;
	tamano2[3] = '\0';
	
	salto[0]=27;
	salto[1]=100;
	salto[2]=1;
	salto[3]='\0';
	
	color_rojo[0]=27;
	color_rojo[1]=114;
	color_rojo[2]=1;
	color_rojo[3]='\0';
	
	color_negro[0]=27;
	color_negro[1]=114;
	color_negro[2]=0;
	color_negro[3]='\0';
	
	reversa[0]=27;
	reversa[1]=101;
	reversa[2]=2;
	reversa[3]='\0';

	corta_papel_modo[0]=29;
	corta_papel_modo[1]=86;
	corta_papel_modo[2]=48;
	corta_papel_modo[3]='\0';

	corta_papel[0]=27;
	corta_papel[1]=109;
	corta_papel[2]='\0';
	
	espaciado[0]=27;
	espaciado[1]=51;
	espaciado[2]=20;
	espaciado[3]='\0';

	alinea_d[0]=27;
	alinea_d[1]=97;
	alinea_d[2]=2;
	alinea_d[3]='\0';
	alinea_i[0]=27;
	alinea_i[1]=97;
	alinea_i[2]=0;
	alinea_i[3]='\0';
	alinea_c[0]=27;
	alinea_c[1]=97;
	alinea_c[2]=1;
	alinea_c[3]='\0';

	subraya_s[0]=27;
	subraya_s[1]=33;
	subraya_s[2]=128;
	subraya_s[3]='\0';
	negrita_grande[0]=27;
	negrita_grande[1]=33;
	negrita_grande[2]=24;
	negrita_grande[3]='\0';
	
	negrita_subraya[0]=27;
	negrita_subraya[1]=33;
	negrita_subraya[2]=136;
	negrita_subraya[3]='\0';
	
	negrita[0]=27;
	negrita[1]=33;
	negrita[2]=8;
	negrita[3]='\0';
	
	negrita1[0]=27;
	negrita1[1]=33;
	negrita1[2]=9;
	negrita1[3]='\0';
	
	negrita_media[0]=27;
	negrita_media[1]=33;
	negrita_media[2]=10;
	negrita_media[3]='\0';
	
	negrita_subraya1[0]=27;
	negrita_subraya1[1]=33;
	negrita_subraya1[2]=137;
	negrita_subraya1[3]='\0';
	subraya_s1[0]=27;
	subraya_s1[1]=33;
	subraya_s1[2]=129;
	subraya_s1[3]='\0';

	/* CODIGOS DE BARRAS*/
	altura_codigo_barras[0]=29;
	altura_codigo_barras[1]=104;
	altura_codigo_barras[2]=81;
	altura_codigo_barras[3]='\0';

	defecto[0]=27;
	defecto[1]=33;
	defecto[2]=1;
	defecto[3]='\0';
	cancela[0]=27;
	cancela[1]=33;
	cancela[2]=0;
	cancela[3]='\0';

	resetea[0]=27;
	resetea[1]=64;
	resetea[2]='\0';


	int anchoticket = 0;   //Bandera para saber si esta definido el ancho del ticket en el archivo de configuracion de impresoras.
	nX = 50; //Tamaño del ticket de ancho en caracteres

	sprintf(id_venta, "%d", id_venta_num);

/*CONFIGURACION DE LA IMPRESORA (SE OBTIENE EL ANCHO DEL TICKET)
*/
		fpt2 = fopen(ImpresoraConfig,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
			imprimiendo = FALSE;
			return (1);
		}else{
			m=1;
			while(fgets(c, 255, fpt2) != NULL){
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				printf("CADENA: %s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' '){
							j=0;
							if(k==0){
								//Guarda la posicion de las variables.
								if(strcmp(cadconf,"anchoticket") == 0) anchoticket = 1;

								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if(anchoticket == 1) 
								{
									nX = atoi(cadconf);
									anchoticket = 0;
								}
								printf ("El ancho del ticket = %d\n", nX);
								//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
							}
							strcpy(cadconf,"");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}
				m++;
			}
			fclose(fpt2);
		}
	
/*
TERMINA LA CONFIGURACION DE LA IMPRESORA*/

	if(strcmp(tipo, "entrada_almacen") == 0){

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
			imprimir(resetea,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita_grande,nX);
			imprimir("ENTRADA DE ALMACEN",nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			sprintf(sql, "SELECT DATE_FORMAT(Entrada_Almacen.fecha, \"%%d/%%m/%%Y  %%H:%%i:%%s\"), Usuario.nombre, Proveedor.razon_social FROM Entrada_Almacen INNER JOIN Usuario ON Entrada_Almacen.id_usuario = Usuario.id_usuario INNER JOIN Proveedor ON Entrada_Almacen.id_proveedor = Proveedor.id_proveedor WHERE Entrada_Almacen.id_entrada = %s",id_venta);
			//sprintf(sql, "SELECT  DATE_FORMAT(Pedido.fecha,\"%%d/%%m/%%Y\"), Pedido.hora, Pedido.HoraEntrega, DATE_FORMAT(Pedido.FechaEntrega,\"%%d/%%m/%%Y\"), Cliente.nombre, Cliente.telefono, Usuario.nombre, Cliente.domicilio, Cliente.id_cliente, Cliente.entre_calles, Cliente.ciudad_estado, Cliente.colonia, Pedido.observacion_pedido, Pedido.servicio, FORMAT(Pedido.servicio,2), Lista.nombre FROM Pedido INNER JOIN Cliente ON Cliente.id_cliente = Pedido.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Usuario ON Pedido.id_usuario=Usuario.id_usuario WHERE Pedido.id_pedido=%s", id_venta);

			printf("\nConsulta: %s\n", sql);

			if(conecta_bd() == 1)
			{
				//printf("\nTicket para el carnicero\n\n\n");
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							if((row=mysql_fetch_row(resultado)))
							{
								imprimir(salto,nX);
								strcpy(c,"Folio: ");
								strcat(c, id_venta);
								imprimir(negrita,nX);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								strcpy(c,"Fecha: ");
								imprimir(c,nX);
								strcpy(c, row[0]);
								imprimir(c,nX);
								imprimir(salto,nX);
								strcpy(c,"Usuario: ");
								imprimir(c,nX);
								strcpy(c, row[1]);
								imprimir(c,nX);
								imprimir(salto,nX);
								strcpy(c,"Proveedor:  ");
								strcat(c, row[2]);
								imprimir(c,nX);
								imprimir(salto,nX);
								
							}else{
								printf("\nError al imprimir la entrada de almacen");
							}
						}
						sprintf(sql, "SELECT CONCAT(LEFT(CONCAT(Subproducto.codigo,Articulo.codigo,' ',Articulo.nombre),%d), SPACE( %d-LENGTH( LEFT( CONCAT( Subproducto.codigo,Articulo.codigo,' ',Articulo.nombre),%d ) ) )), FORMAT(Entrada_Almacen_Articulo.cantidad,3), Articulo.tipo FROM Entrada_Almacen_Articulo INNER JOIN Articulo ON Entrada_Almacen_Articulo.id_articulo = Articulo.id_articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto WHERE Entrada_Almacen_Articulo.id_entrada = %s ORDER BY Entrada_Almacen_Articulo.id_entrada_almacen_articulo", nX-12, nX-12, nX-12, id_venta);
						printf("\nConsulta2: %s\n", sql);
      						err = mysql_query(&mysql, sql);
						if(err == 0)
						{
							resultado = mysql_store_result(&mysql);
							if(resultado)
							{
								imprimir(alinea_c, nX);
								sprintf(c, "                                      ");
								imprimir(subraya_s1,nX);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								if (mysql_num_rows(resultado) > 0){
									while((row=mysql_fetch_row(resultado)))
									{
										strcpy(c, "");
										if(strcmp(row[2], "peso")==0)
											strcpy(num_tmp,"kg");
										else
											strcpy(num_tmp,"  ");

										imprimir(alinea_i, nX);
										imprimir(negrita1,nX);
										imprimir(row[0],nX);
										
										strcpy(cad_temporal,"");
										strcpy(c,"");
										sprintf(cad_temporal, "%s%s", row[1],num_tmp);	//Cantidad
										strncat(c, temp3, 12-strlen(cad_temporal));
										strcat(c, cad_temporal);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(cancela,nX);
										imprimir(defecto,nX);
									}
									imprimir(alinea_c, nX);
									sprintf(c, "                                      ");
									imprimir(subraya_s1,nX);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(cancela,nX);
									imprimir(defecto,nX);
									imprimir(alinea_i, nX);
									num_numero_int = mysql_num_rows(resultado);
									sprintf(cambio, "%d",num_numero_int);
									sprintf(c, "TOTAL ARTICULOS: %s\n",cambio); imprimir(c,nX);

									//Calcula el Total de Cantidad
									sprintf(sql, "SELECT TRUNCATE(SUM(Entrada_Almacen_Articulo.cantidad),3) FROM Entrada_Almacen_Articulo INNER JOIN Articulo ON Entrada_Almacen_Articulo.id_articulo = Articulo.id_articulo WHERE Entrada_Almacen_Articulo.id_entrada = %s AND Articulo.tipo = 'peso'  GROUP BY Entrada_Almacen_Articulo.id_entrada",id_venta);
									printf("\nConsulta3 (a): %s\n", sql);
									if(!(err = mysql_query(&mysql, sql)))
										if((resultado = mysql_store_result(&mysql)))
											if((row=mysql_fetch_row(resultado))){
												sprintf(c, "TOTAL KILOS:  %skg\n",row[0]); imprimir(c,nX);
											}else
												printf("Error al calcular la cantidad de los articulos");
										else
											printf("Error al calcular la cantidad de los articulos");
									else
										printf("Error al calcular la cantidad de los articulos");
								}
							}
						}
						else
						{
							sprintf(Errores,"%s",mysql_error(&mysql));
							printf("%s\n",Errores);
						}
						imprimir(salto,nX);
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					printf("%s\n",Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				printf("%s\n",Errores);
			}
			mysql_close(&mysql);
			
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir("servicios TI",nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);


//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");

		//system(cad_temporal);
		if (manda_imprimir (TicketImpresion,"entrada_almacen") != 0)
		{
			printf ("Error de impresion\n");
		}

		imprimiendo = FALSE;
		return (1);
		}

	}
	else if(strcmp(tipo, "stock_3_dias") == 0){

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
			imprimir(resetea,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita_grande,nX);
			imprimir("STOCK SUGERIDO",nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(negrita,nX);
			imprimir("3 DIAS",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir("PRESENTACIONES",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			if(conecta_bd() == 1)
			{
				sprintf(sql, "SELECT DATE_FORMAT(DATE_ADD(CURDATE(), INTERVAL 1 DAY),'%%d-%%m-%%Y'), CONCAT(DATE_FORMAT(CURDATE(),'%%d-%%m-%%Y'),' ',CURTIME())");
				printf("\nConsulta: %s\n", sql);
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							if((row=mysql_fetch_row(resultado))){
								imprimir(alinea_c, nX);
								sprintf(c,"Para el:");
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(c,"%s%s",negrita,row[0]);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);
								sprintf(cad_temporal2,"%s",row[1]);
							}
						}
					}
				}else{
					printf("Error en el SQL: %s\n",mysql_error(&mysql));
				}

				imprimir(salto,nX);

				sprintf(sql, "SELECT Articulo.id_articulo, IF(Articulo.codigo IS NOT NULL,LEFT(CONCAT(Subproducto.codigo, Articulo.codigo,' ',Articulo.nombre),16),LEFT(CONCAT('     ',Articulo.nombre),16)) AS Codigo FROM Articulo INNER JOIN Subproducto USING (id_subproducto) WHERE Articulo.inventariado = 's' ORDER BY Articulo.nombre");
				printf("\nConsulta: %s\n", sql);
				//printf("\nTicket para el carnicero\n\n\n");
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							imprimir(alinea_i, nX);
							sprintf(c, "Presentacion       Stock   VP  Sug Falta");
							imprimir(negrita_subraya1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(cancela,nX);
							imprimir(defecto,nX);
							i=0;
							while((row=mysql_fetch_row(resultado)))
							{
								if(i%2 == 0)
									imprimir(negrita1,nX);
								i++;
								total_3_dias = 0;
								strcpy(cad_temporal,"");
								strcpy(c,"");
								sprintf(cad_temporal, "%s", row[1]);	//Codigo
								strcat(c, cad_temporal);
								strncat(c, temp3, 16-strlen(cad_temporal));

								// Existencia

								sprintf(sql,"SELECT IFNULL(MAX(Inventario_Inicial_Presentacion.fecha),MIN(Entrada_Almacen.fecha)) as fecha, CONCAT(DATE(MAX(Inventario_Inicial_Presentacion.fecha)),' / ', TIME(MAX(Inventario_Inicial_Presentacion.fecha))), CONCAT(Subproducto.codigo,Articulo.codigo) as codigo FROM Articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto LEFT JOIN Inventario_Inicial_Presentacion ON Articulo.id_articulo = Inventario_Inicial_Presentacion.id_articulo LEFT JOIN Entrada_Almacen_Articulo ON Articulo.id_articulo = Entrada_Almacen_Articulo.id_articulo LEFT JOIN Entrada_Almacen ON Entrada_Almacen_Articulo.id_entrada = Entrada_Almacen.id_entrada WHERE 1 AND Articulo.id_articulo = %s AND (Articulo.id_articulo = ANY ( SELECT id_articulo FROM Entrada_Almacen_Articulo GROUP BY id_articulo ) OR Articulo.id_articulo = ANY ( SELECT id_articulo FROM Inventario_Inicial_Presentacion GROUP BY id_articulo )) GROUP BY Articulo.id_articulo ORDER BY Articulo.nombre",row[0]);
								//printf("\nConsulta: %s\n", sql);
								err = mysql_query(&mysql, sql);
								if(err == 0)
								{
									resultado2 = mysql_store_result(&mysql);
									if(resultado2){
										if((row2 = mysql_fetch_row(resultado2))){
											sprintf(sql,"SELECT IFNULL((SELECT cantidad FROM Inventario_Inicial_Presentacion WHERE id_articulo = %s ORDER BY fecha DESC LIMIT 1),0), IFNULL((SELECT SUM(Entrada_Almacen_Articulo.cantidad) FROM Entrada_Almacen_Articulo INNER JOIN Entrada_Almacen USING(id_entrada) WHERE Entrada_Almacen_Articulo.id_articulo = %s AND Entrada_Almacen.fecha BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND Entrada_Almacen.cancelada = 'n'),0), (SELECT IFNULL((SELECT SUM(Venta_Articulo.cantidad) FROM Venta_Articulo INNER JOIN Venta USING(id_venta) WHERE CONCAT(Venta.fecha,' ',Venta.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND Venta_Articulo.id_articulo = %s AND Venta.cancelada = 'n'),0) + IFNULL((SELECT SUM(SalidasVarias_Articulo.cantidad) FROM SalidasVarias_Articulo INNER JOIN SalidasVarias USING(id_venta) WHERE CONCAT(SalidasVarias.fecha,' ',SalidasVarias.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND SalidasVarias_Articulo.id_articulo = %s AND SalidasVarias.cancelada = 'n'),0) + IFNULL((SELECT SUM(Pruebas_Articulo.cantidad) FROM Pruebas_Articulo INNER JOIN Pruebas USING(id_venta) WHERE CONCAT(Pruebas.fecha,' ',Pruebas.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND Pruebas_Articulo.id_articulo = %s AND Pruebas.cancelada = 'n'),0))",row[0],row[0],row2[0],row2[0],row[0],row2[0],row[0],row2[0],row[0]);
											//printf("\nConsulta: %s\n", sql);
											err = mysql_query(&mysql,sql);
											if(err == 0){
												resultado3 = mysql_store_result(&mysql);
												if(resultado3){
													if((row3 = mysql_fetch_row(resultado3))){
														strcpy(cad_temporal,"");
														existencias = atof(row3[0]) + atof(row3[1]) - atof(row3[2]);
														sprintf(cad_temporal,"%.2f",existencias);
														strncat(c, temp3, 8-strlen(cad_temporal));
														strcat(c, cad_temporal);
													}
												}
											}else{
												printf("Error en el SQL: %s\n",mysql_error(&mysql));
											}
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}


								// Venta promedio año pasado
								sprintf(sql,"SELECT SUM(Venta_Articulo.cantidad), SUM(Venta_Articulo.cantidad)/56 FROM Venta_Articulo INNER JOIN Venta ON Venta_Articulo.id_venta = Venta.id_venta WHERE Venta_Articulo.id_articulo = %s AND Venta.cancelada = 'n' AND Venta.fecha BETWEEN DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 60 WEEK) AND DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 52 WEEK) GROUP BY id_articulo ORDER BY Venta.fecha",row[0]);
								//printf("\nConsulta: %s\n", sql);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										if((row2 = mysql_fetch_row(resultado2))){
											venta_pasada = atof(row2[1]);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}

								// Venta promedio actual
								sprintf(sql,"SELECT SUM(Venta_Articulo.cantidad), SUM(Venta_Articulo.cantidad)/56, FORMAT(SUM(Venta_Articulo.cantidad)/56,2) FROM Venta_Articulo INNER JOIN Venta ON Venta_Articulo.id_venta = Venta.id_venta WHERE Venta_Articulo.id_articulo = %s AND Venta.cancelada = 'n' AND Venta.fecha BETWEEN DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 8 WEEK) AND DATE_SUB(CURDATE(), INTERVAL 1 DAY) GROUP BY id_articulo ORDER BY Venta.fecha",row[0]);
								//printf("\nConsulta: %s\n", sql);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										if((row2 = mysql_fetch_row(resultado2))){
											venta_actual = atof(row2[1]);
											strcpy(cad_temporal,"");
											sprintf(cad_temporal,"%.0f",venta_actual);
											strncat(c, temp3, 5-strlen(cad_temporal));
											strcat(c, cad_temporal);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}
								
								// Porcentaje de diferencia en la venta comparada con el año pasado
								diferencia = (venta_actual - venta_pasada) / venta_pasada;


								//Stock sugerido
								sprintf(sql,"SELECT SUM(Venta_Articulo.cantidad) FROM Venta_Articulo INNER JOIN Venta ON Venta_Articulo.id_venta = Venta.id_venta WHERE Venta_Articulo.id_articulo = %s AND Venta.cancelada = 'n' AND Venta.fecha BETWEEN DATE_SUB(DATE_ADD(CURDATE(), INTERVAL 1 DAY), INTERVAL 52 WEEK) AND DATE_SUB(DATE_ADD(CURDATE(), INTERVAL 3 DAY), INTERVAL 52 WEEK) GROUP BY Venta.fecha ORDER BY Venta.fecha",row[0]);
								//printf("\nConsulta: %s\n", sql);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										while((row2 = mysql_fetch_row(resultado2))){
											total_3_dias += atof(row2[0]);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}

								
								//printf("Venta pasada: %.2f\nVenta actual: %.2f\nDiferencia: %.2f%\nStock sugerido: %.2f\n",venta_pasada,venta_actual,diferencia,total_3_dias * (1+diferencia) * 1.2);


								// Sugiere un stock + un 20% de reserva
								stock_sugerido = total_3_dias * (1+diferencia) * 1.2;
								strcpy(cad_temporal,"");
								sprintf(cad_temporal,"%.0f",stock_sugerido);
								strncat(c, temp3, 5-strlen(cad_temporal));
								strcat(c, cad_temporal);
								
								strcpy(cad_temporal,"");
								//printf("Se han calculado los stocks...\nStock sugerido: %.0f\nExistencia: %.0f\n",stock_sugerido,existencias);
								if(stock_sugerido > existencias){
									//printf("Si hay stock sugerido...\n");
									//strcat(c,negrita1);
									sprintf(cad_temporal,"%.0f",stock_sugerido - existencias);
									total_faltante += stock_sugerido - existencias;
								}else{
									sprintf(cad_temporal,"%.0f",0.00);
									//printf("No hay stock sugerido...\n");
								}
								strncat(c, temp3, 6-strlen(cad_temporal));
								strcat(c, cad_temporal);

								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
							}
							sprintf(c,"                                        ");
							imprimir(subraya_s1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);

							imprimir(alinea_d,nX);
							imprimir(negrita1,nX);
							sprintf(c,"TOTAL: %.0fkg",total_faltante);
							imprimir(c,nX);
							imprimir(salto,nX);

							imprimir(cancela,nX);
							imprimir(defecto,nX);
							imprimir(salto,nX);
							imprimir(alinea_d,nX);
							imprimir(cad_temporal2,nX);
							imprimir(salto,nX);
						}else{
							printf("No hubo rows\n");
						}

					}else{
						printf("No hubo resultado\n");
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					printf("%s\n",Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				printf("%s\n",Errores);
			}
			mysql_close(&mysql);
			
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir("servicios TI",nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);


//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");

		//system(cad_temporal);
		if (manda_imprimir (TicketImpresion,"entrada_almacen") != 0)
		{
			printf ("Error de impresion\n");
		}

		imprimiendo = FALSE;
		return (1);
		}

	}
	else if(strcmp(tipo, "stock_3_dias_subproductos") == 0){

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
			imprimir(resetea,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita_grande,nX);
			imprimir("STOCK SUGERIDO",nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(negrita,nX);
			imprimir("3 DIAS",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir("SUBPRODUCTOS",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			if(conecta_bd() == 1)
			{
				sprintf(sql, "SELECT DATE_FORMAT(DATE_ADD(CURDATE(), INTERVAL 1 DAY),'%%d-%%m-%%Y'), CONCAT(DATE_FORMAT(CURDATE(),'%%d-%%m-%%Y'),' ',CURTIME())");
				printf("\nConsulta: %s\n", sql);
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							if((row=mysql_fetch_row(resultado))){
								imprimir(alinea_c, nX);
								sprintf(c,"Para el:");
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(c,"%s%s",negrita,row[0]);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);
								sprintf(cad_temporal2,"%s",row[1]);
							}
						}
					}
				}else{
					printf("Error en el SQL: %s\n",mysql_error(&mysql));
				}

				imprimir(salto,nX);

				sprintf(sql, "SELECT Subproducto.id_subproducto, LEFT(CONCAT(Subproducto.codigo,' ',Subproducto.nombre),16) AS Codigo FROM Articulo INNER JOIN Subproducto USING (id_subproducto) WHERE Articulo.inventariado = 's' GROUP BY Subproducto.id_subproducto ORDER BY Subproducto.nombre");
//				printf("\nConsulta: %s\n", sql);
				//printf("\nTicket para el carnicero\n\n\n");
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							imprimir(alinea_i, nX);
							sprintf(c, "Subproducto        Stock   VP  Sug Falta");
							imprimir(negrita_subraya1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(cancela,nX);
							imprimir(defecto,nX);
							i=0;
							while((row=mysql_fetch_row(resultado)))
							{
								if(i%2 == 0)
									imprimir(negrita1,nX);
								i++;
								total_3_dias = 0;
								strcpy(cad_temporal,"");
								strcpy(c,"");
								sprintf(cad_temporal, "%s", row[1]);	//Codigo
								strcat(c, cad_temporal);
								strncat(c, temp3, 16-strlen(cad_temporal));

								// Existencia

								sprintf(sql,"SELECT IFNULL(MAX(Inventario_Inicial_Presentacion.fecha),MIN(Entrada_Almacen.fecha)) as fecha, CONCAT(DATE(MAX(Inventario_Inicial_Presentacion.fecha)),' / ', TIME(MAX(Inventario_Inicial_Presentacion.fecha))), Subproducto.codigo FROM Articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto LEFT JOIN Inventario_Inicial_Presentacion ON Articulo.id_articulo = Inventario_Inicial_Presentacion.id_articulo LEFT JOIN Entrada_Almacen_Articulo ON Articulo.id_articulo = Entrada_Almacen_Articulo.id_articulo LEFT JOIN Entrada_Almacen ON Entrada_Almacen_Articulo.id_entrada = Entrada_Almacen.id_entrada WHERE 1 AND Subproducto.id_subproducto = %s AND (Articulo.id_articulo = ANY ( SELECT id_articulo FROM Entrada_Almacen_Articulo GROUP BY id_articulo ) OR Articulo.id_articulo = ANY ( SELECT id_articulo FROM Inventario_Inicial_Presentacion GROUP BY id_articulo )) GROUP BY Subproducto.id_subproducto ORDER BY Subproducto.nombre",row[0]);
								err = mysql_query(&mysql, sql);
								if(err == 0)
								{
									resultado2 = mysql_store_result(&mysql);
									if(resultado2){
										if((row2 = mysql_fetch_row(resultado2))){
											sprintf(sql,"SELECT IFNULL((SELECT SUM(i.cantidad) FROM Inventario_Inicial_Presentacion AS i INNER JOIN Articulo AS a ON i.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE s.id_subproducto = %s ORDER BY i.fecha DESC LIMIT 1),0), IFNULL((SELECT SUM(eaa.cantidad) FROM Entrada_Almacen_Articulo AS eaa INNER JOIN Entrada_Almacen AS ea ON eaa.id_entrada = ea.id_entrada INNER JOIN Articulo AS a ON eaa.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE s.id_subproducto = %s AND ea.fecha BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND ea.cancelada = 'n'),0), (SELECT IFNULL((SELECT SUM(va.cantidad) FROM Venta_Articulo AS va INNER JOIN Venta AS v ON va.id_venta = v.id_venta INNER JOIN Articulo AS a ON va.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE CONCAT(v.fecha,' ',v.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND s.id_subproducto = %s AND v.cancelada = 'n'),0) + IFNULL((SELECT SUM(sva.cantidad) FROM SalidasVarias_Articulo AS sva INNER JOIN SalidasVarias AS sv ON sva.id_venta = sv.id_venta INNER JOIN Articulo AS a ON a.id_articulo = sva.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE CONCAT(sv.fecha,' ',sv.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND s.id_subproducto = %s AND sv.cancelada = 'n'),0) + IFNULL((SELECT SUM(pa.cantidad) FROM Pruebas_Articulo AS pa INNER JOIN Pruebas AS p ON pa.id_venta = p.id_venta INNER JOIN Articulo AS a ON pa.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE CONCAT(p.fecha,' ',p.hora) BETWEEN '%s' AND CONCAT(CURDATE(),' 23:59:59') AND s.id_subproducto = %s AND p.cancelada = 'n'),0))",row[0],row[0],row2[0],row2[0],row[0],row2[0],row[0],row2[0],row[0]);
											err = mysql_query(&mysql,sql);
											if(err == 0){
												resultado3 = mysql_store_result(&mysql);
												if(resultado3){
													if((row3 = mysql_fetch_row(resultado3))){
														strcpy(cad_temporal,"");
														existencias = atof(row3[0]) + atof(row3[1]) - atof(row3[2]);
														sprintf(cad_temporal,"%.2f",existencias);
														strncat(c, temp3, 8-strlen(cad_temporal));
														strcat(c, cad_temporal);
													}
												}
											}else{
												printf("Error en el SQL: %s\n",mysql_error(&mysql));
											}
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}


								// Venta promedio año pasado
								sprintf(sql,"SELECT SUM(va.cantidad), SUM(va.cantidad)/56 FROM Venta_Articulo AS va INNER JOIN Venta AS v ON va.id_venta = v.id_venta INNER JOIN Articulo AS a ON va.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE s.id_subproducto = %s AND v.cancelada = 'n' AND v.fecha BETWEEN DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 60 WEEK) AND DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 52 WEEK) GROUP BY s.id_subproducto ORDER BY v.fecha",row[0]);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										if((row2 = mysql_fetch_row(resultado2))){
											venta_pasada = atof(row2[1]);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}

								// Venta promedio actual
								sprintf(sql,"SELECT SUM(va.cantidad), SUM(va.cantidad)/56, FORMAT(SUM(va.cantidad)/56,2) FROM Venta_Articulo AS va INNER JOIN Venta AS v ON va.id_venta = v.id_venta INNER JOIN Articulo AS a ON va.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE s.id_subproducto = %s AND v.cancelada = 'n' AND v.fecha BETWEEN DATE_SUB(DATE_SUB(CURDATE(), INTERVAL 1 DAY), INTERVAL 8 WEEK) AND DATE_SUB(CURDATE(), INTERVAL 1 DAY) GROUP BY s.id_subproducto ORDER BY v.fecha",row[0]);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										if((row2 = mysql_fetch_row(resultado2))){
											venta_actual = atof(row2[1]);
											strcpy(cad_temporal,"");
											sprintf(cad_temporal,"%.0f",venta_actual);
											strncat(c, temp3, 5-strlen(cad_temporal));
											strcat(c, cad_temporal);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}
								
								// Porcentaje de diferencia en la venta comparada con el año pasado
								diferencia = (venta_actual - venta_pasada) / venta_pasada;


								//Stock sugerido
								sprintf(sql,"SELECT SUM(va.cantidad) FROM Venta_Articulo AS va INNER JOIN Venta AS v ON va.id_venta = v.id_venta INNER JOIN Articulo AS a ON va.id_articulo = a.id_articulo INNER JOIN Subproducto AS s ON a.id_subproducto = s.id_subproducto WHERE s.id_subproducto = %s AND v.cancelada = 'n' AND v.fecha BETWEEN DATE_SUB(DATE_ADD(CURDATE(), INTERVAL 1 DAY), INTERVAL 52 WEEK) AND DATE_SUB(DATE_ADD(CURDATE(), INTERVAL 3 DAY), INTERVAL 52 WEEK) GROUP BY v.fecha ORDER BY v.fecha",row[0]);
								err = mysql_query(&mysql, sql);
								if(err == 0){
									resultado2 = mysql_store_result(&mysql);
									if(resultado){
										while((row2 = mysql_fetch_row(resultado2))){
											total_3_dias += atof(row2[0]);
										}
									}
								}else{
									printf("Error en el SQL: %s\n",mysql_error(&mysql));
								}

								
								//printf("Venta pasada: %.2f\nVenta actual: %.2f\nDiferencia: %.2f%\nStock sugerido: %.2f\n",venta_pasada,venta_actual,diferencia,total_3_dias * (1+diferencia) * 1.2);


								// Sugiere un stock + un 20% de reserva
								stock_sugerido = total_3_dias * (1+diferencia) * 1.2;
								strcpy(cad_temporal,"");
								sprintf(cad_temporal,"%.0f",stock_sugerido);
								strncat(c, temp3, 5-strlen(cad_temporal));
								strcat(c, cad_temporal);

								strcpy(cad_temporal,"");
								if(stock_sugerido > existencias){
									//strcat(c,negrita1);
									sprintf(cad_temporal,"%.0f",stock_sugerido - existencias);
									total_faltante += stock_sugerido - existencias;
								}else
									sprintf(cad_temporal,"%.0f",0.00);
								strncat(c, temp3, 6-strlen(cad_temporal));
								strcat(c, cad_temporal);

								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
							}
							
							sprintf(c,"                                        ");
							imprimir(subraya_s1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);

							imprimir(alinea_d,nX);
							imprimir(negrita1,nX);
							sprintf(c,"TOTAL: %.0fkg",total_faltante);
							imprimir(c,nX);
							imprimir(salto,nX);

							imprimir(cancela,nX);
							imprimir(defecto,nX);
							imprimir(salto,nX);
							imprimir(alinea_d,nX);
							imprimir(cad_temporal2,nX);
							imprimir(salto,nX);				
						}else{
							printf("No hubo rows\n");
						}

					}else{
						printf("No hubo resultado\n");
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					printf("%s\n",Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				printf("%s\n",Errores);
			}
			mysql_close(&mysql);
			
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir("servicios TI",nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);


//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");

		//system(cad_temporal);
		if (manda_imprimir (TicketImpresion,"entrada_almacen") != 0)
		{
			printf ("Error de impresion\n");
		}

		imprimiendo = FALSE;
		return (1);
		}

	}
	else
		printf("No se imprimió\n");
	printf("\n");
/*Bandera de bloqueo del cajon !#####################3*/
	imprimiendo = FALSE;
	return (0);
}


int abrir_cajon (char puerto[50])
{
        char comando[6];
        FILE *impresora;
                                                                                                 
        comando[0]=27;
        comando[1]='p';
        comando[2]=0;
        comando[3]=25;
        comando[4]=250;
        comando[5]='\0';
        if((impresora = fopen(puerto,"w")))
	{
        	fputc (comando[0],impresora);
        	fputc (comando[1],impresora);
        	fputc (comando[2],impresora);
        	fputc (comando[3],impresora);
        	fputc (comando[4],impresora);
        	fputc (comando[5],impresora);
		fclose (impresora);
	}
                                                                                                 
        return 0;
}


int checar_puerto_serie(){
//char nombrearchivo[50] = "prueba.txt"; //Nombre del archivo
//char *consulta = "default"; //Nombre de la impresora
//char impresora[100];

char ImpresoraConfig[] = "impresion_conf/impresoras.conf.txt"; //Nombre del archivo de configuracion
char c[1000]; //Aqui se guarda las cadenas a imprimir
char cadconf[50];
int i, j, k, m;
int error = 0;
//Parametros.
int par1 = -1;
int par2 = -1;
char par3[50];
char par4[50];
FILE *fpt;
//printf("\nSacando informacion de la configuracion Serial");
/*CONFIGURACION DE LA IMPRESORA
*/
		fpt = fopen(ImpresoraConfig,"r");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
			return (1);
		}else{
			m=1;
			while(fgets(c, 255, fpt) != NULL){
				printf("M=%d\n",m);
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				//printf("\n\nCADENA: %s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' ' && (strcmp(cadconf," ") != 0)){
							j=0;
							quitar_escape(cadconf);
							trim(cadconf);
							//printf("\n	Cadenas de conf: %s",cadconf);
							if(k==0){
								if(strcmp(cadconf, "sincajon") == 0)
									return(-1);
								else if(strcmp(cadconf, "contado") == 0)
									par1 = 0; //Indica que se metio
								else
									break; //Se sale del for
								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if( (par2 = buscar_arreglo(cadconf))  == -1){
									printf("\nNo encuentro nada %s",cadconf);
									break;
								}
								if(strcmp(cadconf,"paralelo") == 0)
									return (-1);
								//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
							}else if(k==2){
								strcpy(par3,cadconf);
								//printf("->Puerto Serial %s\n<-",cadconf);
								printf ("\n ############# Serial ########### \n puerto = %d \n\n",atoi(cadconf));
								return (atoi(cadconf));
							}else if(k==3){
								//impresiones = atoi(cadconf);
								printf("Numero de copias: %s\n<-",cadconf);
							}else if(k==4){
								/*trim(cadconf);
								if(strlen(cadconf) > 0){
									printf("\nVarias impresiones\n\n");
									impresiones = atoi(cadconf);
								}*/
								strcpy(par4,cadconf);
							}
							strcpy(cadconf," ");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}
				m++;
				if(error==1){ //Si existe un error se sale del while
					break;
				}
			}
			fclose(fpt);
		}
		/*if(par1 != -1){
			switch(par2){
				case 0: //Serial
					strcpy(impresora,"cat ");
					strcat(impresora,nombrearchivo);
					strcat(impresora," > /dev/");
					strcat(impresora,"ttyS");
					strcat(impresora,par3);
				break;
				case 1: //Paralela
					strcpy(impresora,"cat ");
					strcat(impresora,nombrearchivo);
					strcat(impresora," > /dev/");
					strcat(impresora,"lp");
					strcat(impresora,par3);
				break;
				case 2: //cups
					strcpy(impresora,"lp.cups -d ");
					strcat(impresora,par3);
					if (strcmp (consulta,"default") == 0)
					{
						strcat(impresora," -h ");
					}
					else
					{
						strcat(impresora," -o raw -h ");
					}
					strcat(impresora,par4);
				    	strcat(impresora,"  ");
					strcat(impresora,nombrearchivo);
				break;
				case 3: //print file 32
					strcpy(impresora,"prfile32 ");
					strcat(impresora,nombrearchivo);
				break;
				default:
					error = 1;
				break;
			}
		}*/
		
	/*if(error != 1){
		printf("\nCadena de la Impresion: ->%s<--",impresora);
		printf("\n");
		system(impresora);
		return(0);
	}else{
		printf("\nNo se puede imprimir, verifica el archivo de configuracion");
		printf("\n");
		return(-1);
	}*/
}
