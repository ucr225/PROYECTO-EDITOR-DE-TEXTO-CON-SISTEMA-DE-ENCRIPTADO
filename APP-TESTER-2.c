#include <gtk/gtk.h>

int size, nuevo;
int encriptado2[100000][3];
int cuadro1[100000][3];
int cuadro2[100000][3];
int cuadro3[100000][3];
int cuadro4[100000][3];
char cadena[100000];
char mensaje[100000];
int totales;
char descifrado[100000];
char descifrado_individual[100000][4]; // Array para almacenar el descifrado de cada GtkTextView
char buffer[100000];                   // Buffer temporal para formatear cada nÃºmero
GtkWidget *text_views[4];              // Crea un nuevo arreglo de GtkTextViews
GtkTextBuffer *buffers[4];

char arreglo[] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'Ã ', 'Ã‰', 'Ã ', 'Ã“', 'Ãš', 'Ã‘',
    'Ã¡', 'Ã©', 'Ã­', 'Ã³', 'Ãº', 'Ã±',
    '.', ':', ',', ';', '"', '(', ')', '[', ']', '?', '!',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '/', '*', '+', '-', '=', 'Â¿', 'Â¡', '-', '\n'

};

int bandera[3][3] = {
    {1, -2, 2},
    {-1, 1, 3},
    {1, -1, -4}

};

void llamadaCifrada(GtkWidget *widget, gpointer user_data);
int cifrado(char *cadena, int size);
int desencriptado(int encriptado2[100000][3]);
void llamada2(GtkButton *button, gpointer user_data);

static void activate(GtkApplication *app, gpointer user_data)
{

    // CREACION DE WIDGETS
    GtkWidget *window;              // VENTANA
    GtkWidget *encrypt_button;      // BOTÓN DE ENCRIPTACIÓN
    GtkWidget *desencriptar_button; // BOTÓN DE ENCRIPTACIÓN

    GtkWidget *scrolled_windows[4];

    // Creamos una nueva ventana de aplicación
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "APP TESTER BY UC");
    gtk_window_set_default_size(GTK_WINDOW(window), 955, 755);
    gtk_widget_set_size_request(window, 900, 700);

    // Creamos una caja para contener los cuadros de texto
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(box, 500, 500); // DIMENSIONES EN PX DEL CUADRO DE TEXTO

    // Crea un nuevo botón con la etiqueta "Encriptar"
    encrypt_button = gtk_button_new_with_label("Encriptar");
    desencriptar_button = gtk_button_new_with_label("Desencriptar");

    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(llamadaCifrada), NULL);
    g_signal_connect(desencriptar_button, "clicked", G_CALLBACK(llamada2), text_views[1]);

    // Añade el botón a la caja
    gtk_box_append(GTK_BOX(box), encrypt_button);
    gtk_box_append(GTK_BOX(box), desencriptar_button);

    for (int i = 0; i < 4; i++)
    {
        text_views[i] = gtk_text_view_new();
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_views[i]), GTK_WRAP_WORD_CHAR); // PARA LO DE LA LINEA INFINITA
        gtk_widget_set_size_request(text_views[i], 100, 100);                          // DIMENSIONES EN PX DEL CUADRO DE TEXTO

        gtk_box_append(GTK_BOX(box), text_views[i]);
        buffers[i] = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_views[i])); // Obtiene el buffer de cada GtkTextView

        scrolled_windows[i] = gtk_scrolled_window_new();
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_windows[i]), text_views[i]);
        gtk_box_append(GTK_BOX(box), scrolled_windows[i]); // Añade el GtkScrolledWindow a la caja

        gtk_widget_show(text_views[i]);
        gtk_widget_show(scrolled_windows[i]);
    }

    gtk_window_set_child(GTK_WINDOW(window), box);

    // Mostramos la ventana
    gtk_widget_show(window);
}

int main(int argc, char **argv)
{

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

void llamadaCifrada(GtkWidget *widget, gpointer user_data)
{

    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    char *text;

    for (int i = 0; i < 4; i++)
    {

        memset(mensaje, 0, sizeof(mensaje));

        // Obtén el buffer del text view
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_views[i]);

        // Obtén los iteradores para el inicio y el final del texto en el buffer
        gtk_text_buffer_get_bounds(buffer, &start, &end);

        // Obtén el texto del buffer
        text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        // Si el texto no está vacío, lo encriptas
        if (strlen(text) > 0)
        {
            cifrado(text, strlen(text));
            switch (i)
            {
            case 0:
                memcpy(cuadro1, encriptado2, sizeof(encriptado2));

                break;
            case 1:
                memcpy(cuadro2, encriptado2, sizeof(encriptado2));
                break;
            case 2:
                memcpy(cuadro3, encriptado2, sizeof(encriptado2));
                break;
            case 3:
                memcpy(cuadro4, encriptado2, sizeof(encriptado2));
                break;
            default:
                // código para el caso por defecto
                break;
            }

            // Establece el texto encriptado en el buffer del text view
            gtk_text_buffer_set_text(buffer, mensaje, -1);
        }

        g_free(text);
    }
}

int cifrado(char *cadena, int size)
{

    char temp[100000] = ""; // Variable temporal para almacenar los Ã­ndices
    char str_indice[4];     // Para almacenar el Ã­ndice y los corchetes
    int div;                // para matriz
    int igual;              //

    div = (size + 2) / 3; // Calcular el nÃºmero de filas necesarias

    int encriptado[100000][3];

    int rev = 1;
    char *token;

    char espacio[2] = " ";
    int diff = 0;

    size = strlen(cadena);
    printf("SIZE 1  %d \n", size);
    while (size % 3 != 0)
    {
        strcat(cadena, espacio); // Agrega un espacio al final de la cadena
        size = strlen(cadena);   // Actualiza el tamaÃ±o de la cadena
        printf("SIZE %d \n", size);
    }
    if (size % 3 == 0)
    {
        nuevo = size;
    }

    for (int i = 0; i < size; i++)
    {
        int found = 0;
        for (int j = 0; j < sizeof(arreglo); j++)
        {
            if (cadena[i] == arreglo[j])
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            rev = 0;
            break;
        }
    }

    if (rev == 1)
    {
        printf("TODO EL CONTENIDO ES ENCRIPTABLE, NO HAY CARACTERES ESPECIALES\n");
        printf("SE PROCEDERÃ A ENCRIPTAR...\n");
        printf("\n");
        printf("\n");
        printf("\n");

        printf("\n");

        printf("\n");
    }
    else
    {
        printf("TODO EL CONTENIDO (NO) ES ENCRIPTABLE, HAY CARACTERES ESPECIALES\n");
    }

    if (rev == 1) // si se puede encriptar...
    {
        // Inicializar la matriz con un valor predeterminado
        for (int i = 0; i < 100000; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                encriptado[i][j] = 0;
                encriptado2[i][j] = 0;
                        }
        }

        for (int i = 0; i < size; i++) // recorremos la cadena
        {
            for (int j = 0; j < sizeof(arreglo); j++) // el arreglo que almacena los espacios
            {
                if (cadena[i] == arreglo[j]) // si hay coincidencia..
                {
                    igual = j;
                    encriptado[i / 3][i % 3] = igual; // se divide entre 3 para saber en quÃ© fila lo vas a guardar y por mod para la columna
                }
            }
        }

        printf("La matriz es:\n");
        for (int i = 0; i < div; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf(" (%d) -", encriptado[i][j]);
            }
            printf("\n");
        }
    }

    nuevo = div; // AsegÃºrate de que 'nuevo' no sea mayor que 3
    if (nuevo > 3)
    {
        nuevo = 3;
    }

    // Multiplicar cada fila generada por cada palabra por la matriz bandera
    for (int i = 0; i < div; i++)
    {

        for (int j = 0; j < 3; j++)
        {

            encriptado2[i][j] = 0; // Reiniciar el valor encriptado2 en cada iteraciÃ³n
            totales++;
            for (int k = 0; k < 3; k++)
            {

                encriptado2[i][j] += encriptado[i][k] * bandera[k][j];
            }
        }
    }

    printf("\n");

    printf("AJA AHORA SI A VER QUE TAL\n");

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            encriptado[i][j] = 0;
            printf(" (%d) ", encriptado2[i][j]);
            sprintf(buffer, " (%d) ", encriptado2[i][j]);
            strcat(mensaje, buffer);

            // Verificamos el tamaño del búfer mensaje
            if (strlen(mensaje) >= sizeof(mensaje) - 1)
            {
                fprintf(stderr, "Error: Desbordamiento de búfer en mensaje\n");
                return -1; // Indicamos un error
            }
        }
    }

    return 0;
}

int desencriptado(int matriz[100000][3])
{

    int regreso[100000][3]; // Declare and initialize the regreso variable
    int div = totales / 3;  // Calculate the number of required rows
    int inversa[3][3] = {
        {-1, -10, -8},
        {-1, -6, -5},
        {0, -1, -1}

    };

    printf("  DIVVVV %d \n\n", div);

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            regreso[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                regreso[i][j] += matriz[i][k] * inversa[k][j];
            }
        }
    }

    printf("CHEQUEANDO  SI DA LO MISMO  :))))\n");

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("   (%d)   ", regreso[i][j]);
        }
        printf("\n");
    }
    printf("CREANDO CADENA NUEVA :))))\n");

    int num = sizeof(arreglo) / sizeof(arreglo[0]);
    descifrado[100000] = '\0';
    int tamano = 0;
    int valor;

    char mientras[100];

    char cadena[100000]; // Declare and initialize the cadena variable

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (regreso[i][j] >= 0 && regreso[i][j] < num)
            {
                int valor = regreso[i][j];
                char temp[200] = {arreglo[valor], '\0'}; // Create a temporary string with the character from arreglo

                strcat(descifrado, temp); // Append the temporary string to descifrado
            }
        }
    }

    tamano = strlen(descifrado);
    printf("DESCIFRADO size %d \n", tamano);
    printf("%s \n", descifrado);

    // Asignar el valor predeterminado a cada elemento de la matriz
    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            encriptado2[i][j] = 0; // Asignar el valor 0
            regreso[i][j] = 0;     // Asignar el valor 0
            cuadro1[i][j] = 0;
            cuadro2[i][j] = 0;
            cuadro3[i][j] = 0;
            cuadro4[i][j] = 0;
        }
    }
    size = 0;
    totales = 0;
    strcpy(buffer, "");

    return 0;
}

void llamada2(GtkButton *button, gpointer user_data)
{

    for (int i = 0; i < 4; i++)
    {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_views[i]);

        // Obtén el texto actual del buffer
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        // Llama a tu función desencriptado solo si hay texto encriptado
        if (strlen(text) > 0)
        {
            switch (i)
            {
            case 0:
                desencriptado(cuadro1);
                // Almacena el descifrado en el array individual
                strcpy(descifrado_individual[0], descifrado);
                // Actualiza el TextView con la cadena generada solo si hay texto encriptado
                gtk_text_buffer_set_text(buffer, descifrado_individual[0], -1);
                break;
            case 1:
                desencriptado(cuadro2);
                strcpy(descifrado_individual[1], descifrado);
                // Actualiza el TextView con la cadena generada solo si hay texto encriptado
                gtk_text_buffer_set_text(buffer, descifrado_individual[1], -1);
                break;
            case 2:
                desencriptado(cuadro3);
                strcpy(descifrado_individual[2], descifrado);
                // Actualiza el TextView con la cadena generada solo si hay texto encriptado
                gtk_text_buffer_set_text(buffer, descifrado_individual[2], -1);
                break;
            case 3:
                desencriptado(cuadro4);
                strcpy(descifrado_individual[3], descifrado);
                // Actualiza el TextView con la cadena generada solo si hay texto encriptado
                gtk_text_buffer_set_text(buffer, descifrado_individual[3], -1);
                break;
            default:
                break;
            }
            // desencriptado(encriptado2);

            // Almacena el descifrado en el array individual
            // strcpy(descifrado_individual[i], descifrado);

            // Actualiza el TextView con la cadena generada solo si hay texto encriptado
            // gtk_text_buffer_set_text(buffer, descifrado_individual[i], -1);
        }

        g_free(text);
    }
}