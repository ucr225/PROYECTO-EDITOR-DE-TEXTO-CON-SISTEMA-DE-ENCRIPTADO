// Función para encriptar el contenido del text view
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <gtk/gtk.h>

int size, nuevo;
int encriptado2[100][3];
char cadena[100];
char arreglo[] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
int bandera[3][3] = {
    {2, 3, 5},
    {1, 4, 7},
    {6, 8, 9}};

void encriptar_text_view(GtkButton *button, GtkTextView *text_view)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);

    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Lógica de encriptación aquí
    // ...


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





    // Reemplazar el contenido del text view con el texto encriptado
    gtk_text_buffer_set_text(buffer, text_encriptado, -1);

    g_free(text);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Crear la ventana y otros widgets necesarios
    // ...

    // Crear el text view
    GtkTextView *text_view = GTK_TEXT_VIEW(gtk_text_view_new());

    // Crear el botón
    GtkButton *button = GTK_BUTTON(gtk_button_new_with_label("Encriptar"));

    // Conectar la señal "clicked" del botón a la función encriptar_text_view
    g_signal_connect(button, "clicked", G_CALLBACK(encriptar_text_view), text_view);

    // Agregar el text view y el botón a la ventana
    // ...

    gtk_widget_show_all(window);

    gtk_main();



    return 0;
}