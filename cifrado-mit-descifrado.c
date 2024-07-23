#include <stdio.h>
#include <string.h>

#include <stdlib.h>
int size, nuevo;
int encriptado2[100][3];
// int regreso[100][3];
char cadena[100];
char descifrado[100];

char arreglo[] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'Á', 'É', 'Í', 'Ó', 'Ú', 'Ñ',
    'á', 'é', 'í', 'ó', 'ú', 'ñ',
    '.', ':', ',', ';', '"', '(', ')', '[', ']', '?', '!',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '/', '*', '+', '-', '=', '¿', '¡', '-'};

int bandera[3][3] = {
    {1, -2, 2},
    {-1, 1, 3},
    {1, -1, -4}};

int encriptado(char *cadena, int size)
{
    char temp[100] = ""; // Variable temporal para almacenar los índices
    char str_indice[4];  // Para almacenar el índice y los corchetes

    int igual;            //
    int div;              // para matriz
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

int desencriptado(int encriptado2[100][3])
{
    int regreso[100][3];      // Declare and initialize the regreso variable
    int div = (size + 2) / 3; // Calculate the number of required rows
    int inversa[3][3] = {
        {-1, -10, -8},
        {-1, -6, -5},
        {0, -1, -1}};

    printf("%d \n\n", size);

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            regreso[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                regreso[i][j] += encriptado2[i][k] * inversa[k][j];
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
    descifrado[0] = '\0';
    int tamano = 0;
    int valor;

    char mientras[1];

    char cadena[100]; // Declare and initialize the cadena variable

    for (int i = 0; i < div; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (regreso[i][j] >= 0 && regreso[i][j] < num)
            {
                int valor = regreso[i][j];
                char temp[2] = {arreglo[valor], '\0'}; // Create a temporary string with the character from arreglo
                strcat(descifrado, temp);              // Append the temporary string to descifrado
            }
        }
    }

    tamano = strlen(descifrado);
    printf("DESCIFRADO size %d \n", tamano);
    printf("%s \n", descifrado);

    return 0;
}

int main()
{
    float bandera[3][3] = {
        {2, -2, 2},
        {2, 1, 0},
        {3, -2, 2}};

    printf("Mete un msj: ");
    // gets(cadena);
    fgets(cadena, sizeof(cadena), stdin);
    cadena[strcspn(cadena, "\n")] = 0;

    size = strlen(cadena);
    printf("\n");
    printf("SIZE %d \n", size);
    printf("NUEVO %d \n", nuevo);
    printf("UPPER %s \n", cadena);
    printf("==================\n");
    encriptado(&cadena, size);

    printf("PARTE 2");
    desencriptado(encriptado2);

    getche();
}
