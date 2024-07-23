#include <gtk/gtk.h>


/*
char arreglo[] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'Á', 'É', 'Í', 'Ó', 'Ú', 'Ñ',
    'á', 'é', 'í', 'ó', 'ú', 'ñ',
    '.', ':', ',', ';', '"', '(', ')', '[', ']', '?', '!',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '/', '*', '+', '-', '=', '¿', '¡', '-'};
*/
// Tu arreglo y función encriptado aquí...
int size, nuevo;
int encriptado2[100][3];
char cadena[100];
char arreglo[] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int bandera[3][3] = {
    {1, -2, 2},
    {-1, 1, -3},
    {1, -1, -4}};

int encriptado(char *cadena, int size)
{
    char temp[100] = ""; // Variable temporal para almacenar los índices
    char str_indice[4];  // Para almacenar el índice y los corchetes
    int div;             // para matriz
    int igual;           //

    div = (size + 2) / 3; // Calcular el número de filas necesarias

    int encriptado[100][3];

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
        printf("SE PROCEDERÁ A ENCRIPTAR...\n");
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
        for (int i = 0; i < 100; i++)
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
                    encriptado[i / 3][i % 3] = igual; // se divide entre 3 para saber en qué fila lo vas a guardar y por mod para la columna
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

    nuevo = div; // Asegúrate de que 'nuevo' no sea mayor que 3
    if (nuevo > 3)
    {
        nuevo = 3;
    }

    // Multiplicar cada fila generada por cada palabra por la matriz bandera
    for (int i = 0; i < div; i++)
    {

        for (int j = 0; j < 3; j++)
        {

            encriptado2[i][j] = 0; // Reiniciar el valor encriptado2 en cada iteración

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
            printf(" (%d) ", encriptado2[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void on_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkTextView *text_view = GTK_TEXT_VIEW(user_data);
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    char *text;

    // Obtén el buffer del text view
    buffer = gtk_text_view_get_buffer(text_view);

    // Obtén los iteradores para el inicio y el final del texto en el buffer
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    // Obtén el texto del buffer
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Llama a tu función encriptado con el texto obtenido
    encriptado(text, strlen(text));

    // Aquí es donde debes actualizar el TextView con el texto encriptado.
    // Supongamos que tu función encriptado modifica el texto in situ y lo encripta.
    // Entonces puedes simplemente volver a establecer el texto del buffer con el texto encriptado.
    gtk_text_buffer_set_text(buffer, text, -1);

    // Libera la memoria del texto
    g_free(text);
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *text_view;
    GtkWidget *button;
    GtkWidget *vbox;

    gtk_init();

    window = gtk_window_new();
    text_view = gtk_text_view_new();
    button = gtk_button_new_with_label("Encriptar");
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), text_view);

    gtk_box_append(GTK_BOX(vbox), text_view);
    gtk_box_append(GTK_BOX(vbox), button);

    gtk_window_set_child(GTK_WINDOW(window), vbox);

    gtk_widget_set_visible(window);

    gtk_main();

    return 0;
}
