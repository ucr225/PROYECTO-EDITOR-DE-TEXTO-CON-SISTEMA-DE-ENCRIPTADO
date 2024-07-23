/*
PROYECTO N#2-LAB DE LENGUAJE DE PROGRAMACION
REALIZADO POR URIEL CACERES V-29664183
FECHA DE INICIO 11-12-2023

VERSION 2.0

CUADRO DE TEXTO QUE PERMITA HACER CORRECION ORTOGRAFICA Y ENCRIPTACION



*/

#include <gtk/gtk.h>           // Incluye la biblioteca GTK
#include <hunspell/hunspell.h> //LIBRERIA DE EDICION DE TEXTO
#include <string.h>

// VARIABLES GLOBALES

// Crear un arreglo para almacenar los contadores de caracteres
gint total_len = 0;
GtkWidget *text_view[4];
GtkWidget *frame[4];
GtkTextBuffer *buffer; // bufer del cuadro de texto anterior
GtkEventController *key_controller;
int cuadros = 0; // CONTADOR DE CUADROS
gboolean was_period = FALSE;
GtkTextIter start, end;
// Variable global o estática para el contador de clics
static int n_press = 0; // ES PARA LOS CLICs

// Variable global para el contador de archivos
int totalArchivos = 1;

// VARIABLES PARA ENCRIPTADO
int size, nuevo;
int encriptado2[100][3];
char cadena[100];
char mensaje[4090];
int totales;
char descifrado[100];
char bufferr[500]; // Buffer temporal para formatear cada nÃºmero
// char arreglo[] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char arreglo[] = {
    ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'Ã ', 'Ã‰', 'Ã ', 'Ã“', 'Ãš', 'Ã‘',
    'Ã¡', 'Ã©', 'Ã­', 'Ã³', 'Ãº', 'Ã±',
    '.', ':', ',', ';', '"', '(', ')', '[', ']', '?', '!',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '/', '*', '+', '-', '=', 'Â¿', 'Â¡', '-', '\n'};
int bandera[3][3] = {
    {1, -2, 2},
    {-1, 1, 3},
    {1, -1, -4}};

// PROTOTIPOS DE FUNCIONES
static void activate(GtkApplication *app, gpointer user_data);                                                                  // ESTA FUNCION ES LA QUE ABRE LA VENTANA PRINCIPAL DE MI PROGRAMA
void lineas(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data);                                // PARA LAS LINEAS
static void mayuscula(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer user_data);                 // MAYUSCULAS
gboolean tabulacion(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data); // TABULADOR
void desbloqueo(GtkButton *button, gpointer user_data);
void regreso(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data);
static void borrado(GtkButton *button, gpointer user_data);
void prevent_pegado(GtkTextView *text_view, gpointer user_data);
static void informacion(GtkButton *button, gpointer user_data);
void paste_received(GObject *source_object, GAsyncResult *res, gpointer user_data);
static void on_backspace(GtkWidget *widget, gpointer data);

void handle_paste(GtkTextView *text_view, GtkTextBuffer *buffer);

void korrectur(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer user_data);
static void worter(GtkButton *button, gpointer user_data);
static void saving(GtkButton *button, gpointer user_data);
static gboolean scroll_to_bottom(gpointer user_data);

static void data(GtkButton *button, gpointer user_data);

int cifrado(char *cadena, int size);
static void llamadaCifrada(GtkButton *button, gpointer user_data);
int desencriptado(int encriptado2[100][3]);
void llamada2(GtkButton *button, gpointer user_data);

// FUNCIONES

// Función de callback para la acción "Factory"
static void factory_callback(GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
  // Tu código aquí
  g_print("La opción 'Factory' fue seleccionada.\n");
}

// Función de callback para la acción "Home"
static void home_callback(GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
  // Tu código aquí
  g_print("La opción 'Home' fue seleccionada.\n");
}

// Función de callback para la acción "Subway"
static void subway_callback(GSimpleAction *simple, GVariant *parameter, gpointer user_data)
{
  // Tu código aquí
  g_print("La opción 'Subway' fue seleccionada.\n");
}

//  MAIN-AQUI SE EJECUTA TODO
int main(int argc, char **argv)
{
  GtkApplication *app; // Declara un puntero a una aplicaci n
  int status;          // Declara una variable para el estado de salida de la aplicaci n

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS); // INICIA MI APP
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);             // CONECTO EL ACTIVADOR DE LA VENTANA CON LA FUNCION CON ESE NOMBRE
  status = g_application_run(G_APPLICATION(app), argc, argv);                // EJECUTO LA APP Y GUARDO EL ESTADO DE SALIDA
  g_object_unref(app);                                                       // LIBERO MEMORIA DE LA APP

  return status; // RETORNO DE ESTATUS
}
// ESTA FUNCION ES LA QUE ABRE LA VENTANA PRINCIPAL DE MI PROGRAMA
static void activate(GtkApplication *app, gpointer user_data)
{
  // CREACION DE WIDGETS
  GtkWidget *window; // VENTANA
  GtkWidget *header; // ENCABEZADO DE LA VENTANA
  GtkWidget *button; // BOTONES
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *watermark;

  GtkCssProvider *provider; // PARA MI FRONT END
                            // Creamos un nuevo proveedor de CSS
  provider = gtk_css_provider_new();
  GtkWidget *scrolled_window; // VENTANA DINAMICA

  GtkTextBuffer *buffer; // BUFFER
  gint limite = 22;      // LIMITE DE LINEAS

  // Creamos una nueva ventana con desplazamiento
  scrolled_window = gtk_scrolled_window_new();
  gtk_window_set_default_size(GTK_WINDOW(scrolled_window), 950, 850); // TAMANO DE LA VENTANA AL CREARSE
  gtk_widget_set_size_request(scrolled_window, 900, 800);             // TAMANO MINIMO AL QUE SE PUEDE MINIMIZAR

  // Creamos una nueva ventana de aplicaciON
  window = gtk_application_window_new(app);
  // Establecemos el t?tulo de la ventana
  gtk_window_set_title(GTK_WINDOW(window), "EDITOR");
  // Establecemos el tama?o por defecto de la ventana
  gtk_window_set_default_size(GTK_WINDOW(window), 950, 850);
  gtk_widget_set_size_request(window, 900, 800);

  // Creamos un nuevo encabezado
  header = gtk_header_bar_new();
  gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE); // VISIBILIDAD DEL HEADER
  gtk_window_set_titlebar(GTK_WINDOW(window), header);                 // POSICIONAR EL HEADER EN LA VENTANA CON JERARQUIA DE OBJETOS

  // Crea un GMenu con las opciones para el GtkMenuButton
  GMenu *menu = g_menu_new();
  // Establece el nombre del icono para el botón del menú
  g_menu_append(menu, "Guardar... ", "app.saving");
  g_menu_append(menu, "Ayuda", "app.informacion");
  g_menu_append(menu, "Inf.Encriptacion", "app.data");
  g_menu_append(menu, "PRUEBA NUMERO 2", "app.home");
  // g_menu_append(menu, "PRUEBA NUMERO 3", "app.home");
  // g_menu_append(menu, "PRUEBA NUMERO 4", "app.home");
  // g_menu_append(menu, "PRUEBA NUMERO 5", "app.home");

  // En tu función principal o de inicialización
  GSimpleAction *factory_action = g_simple_action_new("saving", NULL);
  g_signal_connect(factory_action, "activate", G_CALLBACK(saving), NULL);
  g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(factory_action));

  GSimpleAction *home_action = g_simple_action_new("data", NULL);
  g_signal_connect(home_action, "activate", G_CALLBACK(data), NULL);
  g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(home_action));

  GSimpleAction *subway_action = g_simple_action_new("informacion", NULL);
  g_signal_connect(subway_action, "activate", G_CALLBACK(informacion), NULL);
  g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(subway_action));

  // Crea un GtkMenuButton y asigna el modelo de menú
  GtkWidget *menu_button = gtk_menu_button_new();
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu));
  gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "emblem-system");
  // Ajusta el tamaño de la imagen
  gtk_image_set_pixel_size(GTK_IMAGE(menu_button), 32); // Cambia 32 por el tamaño que desees

  gtk_widget_show(menu_button); // Asegúrate de que el botón del menú sea visible
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), menu_button);
  // Crea un nuevo GtkLabel
  GtkWidget *label = gtk_label_new("<=  MENU");
  gtk_widget_add_css_class(label, "label"); // PASO LA CLASE DEL CSS

  // Agrega el GtkLabel al GtkHeaderBar
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), label);

  // Muestra el GtkLabel
  gtk_widget_show(label);

  // Creamos un nuevo boton con la etiqueta "SALIR"
  button = gtk_button_new_with_label("LIMPIAR");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(borrado), window); // CUANDO CLICKEAS SE CIERRA LA VENTANA

  gtk_widget_add_css_class(button, "button"); // lE PASAS EL CSS AL BOTON
  // Creamos un nuevo bot?n con la etiqueta "AYUDA"
  button2 = gtk_button_new_with_label("ENCRIPTAR"); // TITULO DEL BOTON
  gtk_widget_add_css_class(button2, "button2");     // PASO LA CLASE DEL CSS
  g_signal_connect(button2, "clicked", G_CALLBACK(llamadaCifrada), text_view);

  // g_signal_connect(button, "clicked", G_CALLBACK(informacion), text_view); // ACCION AL BOTON QUE EJECUTA ESA FUNCION
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button); // PASAR UBICACION AL BOTON

  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button2); // LO PONGO EN EL HEADER
  button3 = gtk_button_new_with_label("DESENCRIPTAR ");       // NOMBRE DEL BOTON
  g_signal_connect(button3, "clicked", G_CALLBACK(llamada2), text_view);
  // A?adimos la clase 'button3' al cuadro de texto
  gtk_widget_add_css_class(button3, "button3"); // CLASE DE CSS
  // g_signal_connect(button3, "clicked", G_CALLBACK(archivo), text_view);      // FUNCION QUE ME CREA EL ARCHIVO
  // g_signal_connect(button3, "clicked", G_CALLBACK(Confirmacion), text_view); // FUNCION QUE ME SALTA EL MENSAJE DE CONFIRMACION
  //  gtk_widget_add_css_class(button3, "my-button");
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button3); // LO PONGO EN EL HEADER DE LA VENTANA
  // Creamos una caja para contener los cuadros de texto
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  /////////////

  // Cargamos las reglas CSS-CON ESO PASO TODO LO REFERENTE A PERSONALIZACION
  gtk_css_provider_load_from_data(
      provider,
      //"* {font-size: 16px; font-family:roboto}"
      //"window { background-color: #219ebc  }"
      "window { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/patron.jpg');background - size : cover;background - position : center}"
      //"window { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/chaewon-lee-VfhoKbFv16Y-unsplash.jpg');background - size : cover;background - position : center;*font-family: monospace}"
      ".label { color: #3d348b; font-weight: bold; font-family: roboto; font-size: 14px; }"
      ".button {  color:#a4161a ; font-weight: bold;font family:roboto;font-size:14px;}"
      ".button2 {  color:#1d3557 ; font-weight: bold;font family:monospace;font-size:14px;}"
      ".button3 {  color:green; font-weight: bold;font family:monospace;font-size:14px;}"
      ".watermark { color: #cae9ff; background-color: #003f88; font-family: roboto, monospace; font-size: 16px;font-weight: bold; border-top-left-radius: 15px;border-bottom-right-radius: 15px;}"
      ".text_view{ background-color:#f6fff8 ;border: 7px solid #03045e; font-family: roboto ; font-size:18px ; color: #003049;text-decoration: noneline-height: 1.8; }",
      -1);
  // ACA LE PASO EL CSS AL WIDGET

  // Aplicamos el proveedor de CSS al contexto de estilo del GtkLabel
  gtk_style_context_add_provider(gtk_widget_get_style_context(label),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button2),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button3),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // gtk_style_context_add_provider(gtk_widget_get_style_context(new_text_view),
  //  GTK_STYLE_PROVIDER(provider),
  // GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // Aplicamos las reglas CSS a la ventana
  gtk_style_context_add_provider(gtk_widget_get_style_context(window),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  // CENTRO LA CAJA DE LOS CUADROS DE TEXTO PA QUE SE GENEREN CENTRADOS

  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_box_set_spacing(GTK_BOX(box), 10);
  // BUCLE QUE ME GENERA MIS CUADROS DE TEXTO,ES UN ARREGLO
  // Agregamos un espacio al final del ?ltimo cuadro de texto
  GtkWidget *sInicio = gtk_label_new("");      // ASI NO SE SALEN PEGADOS
  gtk_widget_set_size_request(sInicio, -1, 8); // Ajusta el '15' al tamaNo de espacio que desees
  gtk_box_append(GTK_BOX(box), sInicio);

  for (int i = 0; i < 4; i++)
  {
    // Creamos un nuevo cuadro de texto
    text_view[i] = gtk_text_view_new();
    int *index_ptr = g_new(int, 1);
    *index_ptr = i;

    key_controller = gtk_event_controller_key_new(); // LE PASO EL CONTROLADOR DE LAS TECLAS

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[i])); // CREO UN BUFFER PARA CADA CUADRO

    gtk_text_buffer_set_text(buffer, "", 0); // LO INICIO EN CERO

    // Obtiene el buffer del GtkTextView.
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[i]));

    // Conecta la señal "insert-text" al buffer con los datos adicionales y la función de limpieza.
    g_signal_connect_data(buffer, "insert-text", G_CALLBACK(lineas), index_ptr, (GClosureNotify)g_free, 0);
    g_signal_connect(G_OBJECT(text_view[cuadros]), "backspace", G_CALLBACK(regreso), &cuadros);

    g_signal_connect(buffer, "insert-text", G_CALLBACK(korrectur), NULL); // FUNCION PARA MI CORRECTOR
    g_signal_connect(text_view[i], "paste-clipboard", G_CALLBACK(handle_paste), buffer);
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(tabulacion), NULL); // BLOQUEO DEL TABULADOR
    // g_signal_connect(text_view[i], "paste-clipboard", G_CALLBACK(prevent_pegado), NULL); // BLOQUEO DE PEGADO DE TEXTO
    g_signal_connect(buffer, "insert-text", G_CALLBACK(mayuscula), NULL); // FUNCION MAYUSCULA DESPUES DEL PUNTO
    g_signal_connect(G_OBJECT(text_view[i]), "backspace", G_CALLBACK(on_backspace), NULL);
    gtk_widget_add_controller(GTK_WIDGET(text_view[i]), key_controller); // CONTROLADOR DE LAS TECLAS

    gtk_text_view_set_justification(GTK_TEXT_VIEW(text_view[i]), GTK_JUSTIFY_FILL); // JUSTIFICAR TEXTO

    gtk_style_context_add_provider(gtk_widget_get_style_context(frame[i]),
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION); // PROVEEDOR DE CSS

    gtk_widget_add_css_class(text_view[i], "text_view"); // CLASE DEL CSS

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view[i]), GTK_WRAP_WORD_CHAR); // PARA LO DE LA LINEA INFINITA

    gtk_widget_set_size_request(text_view[i], 765, 75); // DIMENSIONES EN PX DEL CUADRO DE TEXTO

    // MARCO QUE CONTIENE LOS CUADROS DE TEXTO
    frame[i] = gtk_frame_new(NULL);

    // Establecemos un margen superior de 10 unidades
    gtk_widget_set_margin_top(frame[i], 10);
    // ANadimos el cuadro de texto al marco
    gtk_frame_set_child(GTK_FRAME(frame[i]), text_view[i]);
    gtk_widget_add_css_class(frame[i], "frame"); // Agrega esta linea para el css del frame,no sirvio pero lo deje por si acaso

    gtk_box_append(GTK_BOX(box), frame[i]); // METES EL MARCO  A LA CAJA,POR LO TANTO,ASSI TIENES EL TEXT VIEW DENTRO DE LA CAJA

    GtkStyleContext *style_context = gtk_widget_get_style_context(text_view[i]); // CSS DEL CUADRO DE TEXTO,LE ESTA PASANDO EL ESTILO
    gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    if (i != 0)
    {
      // gtk_widget_set_visible(text_view[i], FALSE); // HAGO INVISIBLE EL CUADRO
      // gtk_widget_set_visible(frame[i], FALSE);     // HAGO INVISIBLE EL MARCO
    }
  }

  // Agregamos un espacio al final del ?ltimo cuadro de texto
  GtkWidget *sFinal = gtk_label_new("");       // ASI NO SE SALEN PEGADOS
  gtk_widget_set_size_request(sFinal, -1, 20); // Ajusta el '15' al tamaNo de espacio que desees

  // Agregamos el texto de "CREDITOS" como una marca de agua
  watermark = gtk_label_new("PROYECTO #2 LAB-LENGUAJE DE PROGRAMACIÓN\nProf. JOSÉ RAMÍREZ\n ENERO 2024\n\n V-2.0 \n\n AUTOR: CACERES RIOS URIEL JOSE CONCEPCION\nV.29664183\n ");
  gtk_widget_add_css_class(watermark, "watermark");
  // Establecer la alineación del label a 0.5 para centrarlo horizontalmente
  gtk_label_set_justify(GTK_LABEL(watermark), GTK_JUSTIFY_CENTER);
  gtk_label_set_xalign(GTK_LABEL(watermark), 0.5);

  // Establecer la alineación del label a 0.5 para centrarlo verticalmente
  gtk_label_set_yalign(GTK_LABEL(watermark), 0.5);
  gtk_box_append(GTK_BOX(box), watermark);
  gtk_box_append(GTK_BOX(box), sFinal);
  // Aplicamos las reglas CSS a la ventana
  gtk_style_context_add_provider(gtk_widget_get_style_context(watermark),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  // A?adimos el cuadro de texto a la ventana con desplazamiento
  // gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), text_view);
  // A?adimos la caja a la ventana con desplazamiento
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);

  // A?adimos la ventana con desplazamiento a la ventana
  gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

  // Mostramos la ventana
  gtk_widget_show(window);
}

void lineas(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data)
{
  // Declara dos iteradores para el buffer de texto.

  // Obtiene el índice del GtkTextView actual a partir de los datos del usuario.
  int cuadros = *(int *)data;
  // Declara un contador para llevar la cuenta de las líneas dep texto.
  int cont = 0;

  // Obtiene un iterador al inicio del texto en el buffer.
  gtk_text_buffer_get_start_iter(buffer, &start);
  // Inicializa el iterador 'end' al inicio del texto.
  end = start;

  // para el scroll automatico
  //  Obtienemos el GtkTextView desde los datos del usuario.
  GtkTextView *current_text_view = GTK_TEXT_VIEW(text_view[cuadros]);

  // Obtienemos el GtkScrolledWindow que contiene el GtkTextView.
  GtkWidget *scrolled_window = gtk_widget_get_ancestor(GTK_WIDGET(current_text_view), GTK_TYPE_SCROLLED_WINDOW);
  printf("%d\n", end);
  printf("%d\n", start);
  printf("%d\n", cuadros + 1);

  // Desplazamos el GtkScrolledWindow hasta la parte inferior.
  g_idle_add(scroll_to_bottom, scrolled_window);

  // Este bucle recorre todo el texto en el buffer.
  while (!gtk_text_iter_is_end(&end))
  {
    // Si el iterador 'end' está al inicio de una línea de visualización, incrementa el contador.
    if (gtk_text_view_starts_display_line(GTK_TEXT_VIEW(text_view[cuadros]), &end))
    {
      cont++;
    }
    // Avanza el iterador 'end' un carácter hacia adelante.
    gtk_text_iter_forward_char(&end);
  }

  // Si el contador es mayor o igual a 22, es decir, si hay 22 líneas de texto o más...
  if (cont == 22)
  {
    // Si no es el último GtkTextView, obtener el siguiente GtkTextView en el arreglo
    if (cuadros < 3)
    {
      // Incrementa el índice para el siguiente GtkTextView.
      cuadros++;
      // Hace visible el siguiente GtkTextView y su marco.
      gtk_widget_set_visible(text_view[cuadros], TRUE);
      gtk_widget_set_visible(frame[cuadros], TRUE);
      // Mueve el foco al siguiente GtkTextView.
      gtk_widget_grab_focus(text_view[cuadros]);
    }
  }
  if (cont == 23 && cuadros == 3)
  {

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view[3]), FALSE);
    // gtk_widget_grab_focus(text_view[cuadros]);

    // Crear una ventana emergente con el mensaje "HOLA"
    GtkWidget *emergencia = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(emergencia), "ALERTA!");
    gtk_window_set_default_size(GTK_WINDOW(emergencia), 400, 300);
    gtk_window_set_resizable(GTK_WINDOW(emergencia), FALSE);

    // Crear una etiqueta con el mensaje
    GtkWidget *label = gtk_label_new("ESTIMADO USUARIO \n\n USTED ALCANZO EL LIMITE DE LINEAS DISPONIBLE EN EL PROGRAMA  \nPOR MOTIVOS DE SEGURIDAD\nSE LIMPIARA TODO EL CONTENIDO\nPROCURE ESCRIBIR 22 LINEAS POR PÁGINA \n\n");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

    // Crear un botón "OK"
    GtkWidget *ok_button = gtk_button_new_with_label("OK");
    g_signal_connect(ok_button, "clicked", G_CALLBACK(desbloqueo), text_view);
    g_signal_connect(ok_button, "clicked", G_CALLBACK(borrado), text_view);
    g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(gtk_window_destroy), emergencia);

    // Crear un GtkGrid para contener la etiqueta y el botón
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    // Añadir la etiqueta al grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    // Añadir el botón "OK" al grid
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 1, 1, 1);

    // Centrar el botón "OK" horizontalmente y alinear al final verticalmente
    gtk_widget_set_halign(ok_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(ok_button, GTK_ALIGN_END);

    // Establecer el grid como el hijo de la ventana emergente
    gtk_window_set_child(GTK_WINDOW(emergencia), grid);

    // Aplicar CSS para cambiar el color de fondo
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    ".emergencia { background-color: #ef233c; font-weight: bold;color: #22181c ;font:condensed 120% sans-serif;font-size:18px  }"
                                    "* {font-size: 16px; font-family:roboto}",
                                    -1);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_add_css_class(emergencia, "emergencia");

    // Mostrar la ventana
    gtk_widget_show(emergencia);
  }
}

// ESTA FUNCION ME PONE EL TEXTO EN MAYUSCULA DESPUES DE UN PUNTO
static void mayuscula(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer user_data)
{
  if (was_period && g_ascii_islower(text[0])) // SI LA ULTIMA TECLA PRESIONADA ES UN PUNTO Y LA OTRA ES UNA LETRA,MAYUS
  {
    text[0] = g_ascii_toupper(text[0]);
    was_period = FALSE; // REINICIA  PARA PODER SEGUIR USANDO LA FUNCION CON MAS PALABRAS
  }

  if (len == 1 && text[0] == '.')
  {
    was_period = TRUE; // SE PONE TRUE PARA INDICAR EL ULTIMO CARACTER INGRESADO
  }
}

// BLOQUEO EL TABULADOR PARA EVITAR DESTRUIR MI CUADRO DE TEXTO
gboolean tabulacion(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
  // Verificar si la tecla presionada es Tab
  if (keyval == GDK_KEY_Tab || keyval == GDK_KEY_ISO_Left_Tab)
  {
    // Bloquear la propagaci?n del evento
    return TRUE;
  }

  // Permitir la propagaci?n del evento para otras teclas
  return FALSE;
}

// ESTA ES PARA DESBLOQUEAR LA ESCRIURA EN LOS ARCHIVOS DE TEXTO
void desbloqueo(GtkButton *button, gpointer user_data)
{

  GtkTextView **text_view = (GtkTextView **)user_data;

  gtk_text_view_set_editable(text_view[0], TRUE); // DESBLOQUEA LA ESCRITURA PARA EL CUADRO DE TEXTO
  gtk_text_view_set_editable(text_view[1], TRUE); // DESBLOQUEA LA ESCRITURA PARA EL CUADRO DE TEXTO

  gtk_text_view_set_editable(text_view[2], TRUE); // DESBLOQUEA LA ESCRITURA PARA EL CUADRO DE TEXTO
  gtk_text_view_set_editable(text_view[3], TRUE); // DESBLOQUEA LA ESCRITURA PARA EL CUADRO DE TEXTO
  gtk_widget_grab_focus(text_view[0]);

  cuadros = 0;
  gtk_widget_set_visible(text_view[1], FALSE); // HAGO INVISIBLE EL CUADRO
  gtk_widget_set_visible(frame[1], FALSE);     // HAGO INVISIBLE EL MARCO
  gtk_widget_set_visible(text_view[2], FALSE); // HAGO INVISIBLE EL CUADRO
  gtk_widget_set_visible(frame[2], FALSE);     // HAGO INVISIBLE EL MARCO
  gtk_widget_set_visible(text_view[3], FALSE); // HAGO INVISIBLE EL CUADRO
  gtk_widget_set_visible(frame[3], FALSE);     // HAGO INVISIBLE EL MARCO
}

void regreso(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data)
{
  // Declara dos iteradores para el buffer de texto.
  GtkTextIter start, end;
  // Obtiene el índice del GtkTextView actual a partir de los datos del usuario.
  int cuadros = *(int *)data;

  // Obtiene un iterador al inicio del texto en el buffer.
  gtk_text_buffer_get_start_iter(buffer, &start);
  // Inicializa el iterador 'end' al inicio del texto.
  end = start;

  // Verifica si el cursor está al inicio del cuadro.
  gboolean cursor_al_inicio = gtk_text_iter_equal(&start, location);

  // Si el cursor está al inicio del cuadro y el cuadro actual no es el primero...
  if (cursor_al_inicio && cuadros > 0)
  {
    // Decrementa el índice para el cuadro anterior.
    cuadros--;
    // Mueve el foco al cuadro anterior.
    gtk_widget_grab_focus(text_view[cuadros]);
  }
}

// ESTA FUNCION ES LA QUE HACE EL CORRECTOR ORTOGRAFICO USANDO EL HUNSPELL

void korrectur(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer user_data)
{

  if (g_ascii_isspace(text[0]) || g_ascii_ispunct(text[0]) || text[0] == '\n') // VERIFICA SI SE METE UN ESPACIO PARA REVISAR SI LA PALABRA TIENE UN ERROR
  {
    // Obtener el inicio y el final de la palabra

    GtkTextIter start = *location; // INICIO DE LA PALABRA
    GtkTextIter end = *location;   // FINAL

    // gtk_text_iter_backward_word_start(&start);
    // gtk_text_iter_forward_word_end(&end);
    // Mover 'start' al inicio de la palabra
    if (!gtk_text_iter_starts_word(&start))
    {
      gtk_text_iter_backward_word_start(&start);
    }

    // Mover 'end' al final de la palabra
    if (!gtk_text_iter_ends_word(&end))
    {
      gtk_text_iter_forward_word_end(&end);
    }
    // Calcular la longitud de la palabra
    gint word_length = gtk_text_iter_get_offset(&end) - gtk_text_iter_get_offset(&start);
    if (word_length > 1)
    {
      // Obtener la palabra
      gchar *word = gtk_text_iter_get_text(&start, &end);

      // Inicializar Hunspell
      Hunhandle *hunspell = Hunspell_create("sugerencias.aff", "palabras.dic"); // ASI SE PASA EL ARCHIVO DE SUGERENCIAS Y  DE PALABRAS

      // REVISAMOS SI TIENE ERRORES LA PALABRA O NO
      if (Hunspell_spell(hunspell, word) == 0)
      {
        // SI LA PALABRA ESTA MAL ESCRITA OBTENEMOS LAS SUGERENCIAS
        char **suggestions;

        int num_suggestions = Hunspell_suggest(hunspell, &suggestions, word);                                 // LAS METEMOS EN UNA VARIABLE ENTERA
        GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL, "underline", PANGO_UNDERLINE_ERROR, NULL); // PARA QUE LA PALABRA SE SUBRAYE
        gtk_text_buffer_apply_tag(buffer, tag, &start, &end);                                                 // SE APLICA

        if (num_suggestions > 0) // SI HAY SUGERENCIAS,CREO LA VENTANA QUE ME IMPRIME LAS SUGERENCIAS
        {

          GtkWidget *message_window = gtk_window_new();
          GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
          GtkCssProvider *provider;

          gtk_window_set_title(GTK_WINDOW(message_window), "Sugerencias....");
          // Establecemos el tama?o por defecto de la ventana
          gtk_window_set_default_size(GTK_WINDOW(message_window), 225, 50);
          gtk_window_set_resizable(GTK_WINDOW(message_window), FALSE);

          // Creamos un nuevo proveedor de CSS
          provider = gtk_css_provider_new();

          // Definir una clase CSS
          // gtk_css_provider_load_from_data(provider, ".miClase { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/c11.jpg');background-size: cover;background-position: center; }", -1);
          // gtk_css_provider_load_from_data(provider, ".miClase { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/patron3.jpg');background-size: cover;background-position: center; }", -1);
          gtk_css_provider_load_from_data(provider, ".miClase { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/magicpattern-8h_tctpq4h0-unsplash.jpg');background-size: cover;background-position: center; }", -1);

          // Aplicar la clase CSS a la ventana de di?logo
          gtk_widget_add_css_class(message_window, "miClase");

          gtk_style_context_add_provider(gtk_widget_get_style_context(message_window),
                                         GTK_STYLE_PROVIDER(provider),
                                         GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

          for (int i = 0; i < num_suggestions; i++)
          {
            // Crear un nuevo botón con la sugerencia como etiqueta
            GtkWidget *suggestion_button = gtk_button_new_with_label(suggestions[i]);
            // Creamos un nuevo proveedor de CSS
            provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, ".button {  font-weight: bold;color: #14213d;font-family: roboto;font-size:18px; margin-top: 5px; margin-bottom: 5px; border: 4px solid red;}", -1);

            gtk_widget_add_css_class(suggestion_button, "button"); // PASO LA CLASE DEL CSS
            gtk_style_context_add_provider(gtk_widget_get_style_context(suggestion_button),
                                           GTK_STYLE_PROVIDER(provider),
                                           GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
            // Conectar la señal "clicked" al botón de sugerencia
            // g_signal_connect(suggestion_button, "clicked", G_CALLBACK(on_suggestion_button_clicked), buffer);
            // Primero, conectas la señal "clicked" del botón a la función 'suggestion_clicked_cb'
            g_signal_connect(suggestion_button, "clicked", G_CALLBACK(worter), buffer);
            g_signal_connect_swapped(suggestion_button, "clicked", G_CALLBACK(gtk_window_destroy), message_window); // CUANDO CLICKEAS SE CIERRA LA VENTANA

            // Añadir el botón al box
            gtk_box_append(GTK_BOX(box), suggestion_button);

            gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
            gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
          }

          gtk_window_set_child(GTK_WINDOW(message_window), box);
          gtk_widget_show(message_window);
          // g_timeout_add_seconds(35, close_window, message_window);
        }

        // Liberar las sugerencias
        Hunspell_free_list(hunspell, &suggestions, num_suggestions);
      }
      else
      {
        // La palabra estÃƒÂ¡ escrita correctamente
        printf("PALABRA CORRECTA: %s\n", word);
      }

      // Liberar la palabra y el objeto Hunspell
      g_free(word);
      Hunspell_destroy(hunspell);
    }
  }
}

// CAMBIA NOMBRE CHAMO
static void worter(GtkButton *button, gpointer user_data)
{
  // Obtener la sugerencia del botón
  const gchar *suggestion = gtk_button_get_label(button);

  // Obtener el buffer de texto
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(user_data);

  // Obtener la posición del cursor
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));

  // Mover el cursor hacia atrás hasta el inicio de la última palabra
  if (!gtk_text_iter_starts_word(&iter))
  {
    gtk_text_iter_backward_word_start(&iter);
  }

  // Guardar la posición de inicio de la última palabra
  GtkTextIter start_iter = iter;

  // Mover el cursor hacia adelante hasta el final de la última palabra
  gtk_text_iter_forward_word_end(&iter);

  // Comprobar si el siguiente carácter es un espacio
  GtkTextIter next_iter = iter;
  gtk_text_iter_forward_char(&next_iter);
  if (!gtk_text_iter_is_end(&next_iter) && gtk_text_iter_get_char(&next_iter) == ' ')
  {
    // Si la palabra está seguida por un espacio, no hacer nada
    return;
  }

  // Guardar la posición de final de la última palabra
  GtkTextIter end_iter = iter;

  // Eliminar la palabra incorrecta
  gtk_text_buffer_delete(buffer, &start_iter, &end_iter);

  // Insertar la sugerencia en la posición de inicio de la última palabra
  gtk_text_buffer_insert(buffer, &start_iter, suggestion, -1);

  // Mover el cursor al final de la palabra insertada
  gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
  gtk_text_iter_forward_chars(&iter, g_utf8_strlen(suggestion, -1));
  gtk_text_buffer_place_cursor(buffer, &iter);
}
/*
static void saving(GtkButton *button, gpointer user_data)
{
  GtkWidget *VISOR_TXT = (GtkWidget *)user_data;
  char guardar_archivo[600];
  sprintf(guardar_archivo, "C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\archivo.txt");

  FILE *guardado = fopen(guardar_archivo, "w");

  for (int i = 0; i < 85; i++)
  {
    fputc('*', guardado);
  }

  for (int i = 0; i < 85; i++)
  {
    fputc(' ', guardado);
  }
  fputc('\n', guardado);

  for (int x = 0; x < 4; x++)
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[x]));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(buffer, &start_iter);
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gchar *text = gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, FALSE);

    int char_count = 0;
    int text_length = strlen(text);
    int max_line_length = 82;
    int num_lines = (text_length + max_line_length - 1) / max_line_length;

    for (int i = 0; i < num_lines; i++)
    {
      fputc('*', guardado);
      fputc(' ', guardado);

      int line_start_index = i * max_line_length;
      int line_end_index = line_start_index + max_line_length;

      if (line_end_index > text_length)
      {
        line_end_index = text_length;
      }

      for (int j = line_start_index; j < line_end_index; j++)
      {
        if (text[j] == '\n')
        {
          for (int k = char_count; k < max_line_length; k++)
          {
            fputc(' ', guardado);
          }
          fputc('*', guardado);
          fputc('\n', guardado);
          char_count = 0;
          fputc('*', guardado);
          fputc(' ', guardado);
        }
        else
        {
          fputc(text[j], guardado);
          char_count++;
        }
      }

      for (int k = char_count; k < max_line_length; k++)
      {
        fputc(' ', guardado);
      }
      fputc('  *', guardado);
      fputc('\n', guardado);
      char_count = 0;
    }

    g_free(text);
  }

  for (int x = 0; x < 85; x++)
  {
    fputc('*', guardado);
  }

  for (int x = 0; x < 85; x++)
  {
    fputc(' ', guardado);
  }

  fputc('\n', guardado);
  fclose(guardado);
}
*/

static void saving(GtkButton *button, gpointer user_data)
{
  // GtkWidget **text_view = (GtkWidget **)user_data;
  char informacion[600];
  sprintf(informacion, "C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\ARCHIVO .txt\\informacion.txt");

  FILE *file = fopen(informacion, "w");

  for (int x = 0; x < 4; x++)
  {
    // Comienza un nuevo cuadro para cada GtkTextView
    for (int i = 0; i < 85; i++)
    {
      fputc('^', file);
    }

    for (int i = 0; i < 85; i++)
    {
      fputc(' ', file);
    }
    fputc('\n', file);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[x]));
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(buffer, &start_iter);
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gchar *text = gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, FALSE);

    int char_count = 0;
    int text_length = strlen(text);
    int max_line_length = 82;
    int num_lines = (text_length + max_line_length - 1) / max_line_length;

    for (int i = 0; i < num_lines; i++)
    {
      fputc('^', file);
      fputc(' ', file);

      int line_start_index = i * max_line_length;
      int line_end_index = line_start_index + max_line_length;

      if (line_end_index > text_length)
      {
        line_end_index = text_length;
      }

      for (int j = line_start_index; j < line_end_index; j++)
      {
        if (text[j] == '\n')
        {
          for (int k = char_count; k < max_line_length; k++)
          {
            fputc(' ', file);
          }
          fputc('^', file);
          fputc('\n', file);
          char_count = 0;
          fputc('^', file);
          fputc(' ', file);
        }
        else
        {
          fputc(text[j], file);
          char_count++;
        }
      }

      for (int k = char_count; k < max_line_length; k++)
      {
        fputc(' ', file);
      }
      fputc('  ^', file);
      fputc('\n', file);
      char_count = 0;
    }

    g_free(text);

    // Termina el cuadro para cada GtkTextView
    for (int i = 0; i < 85; i++)
    {
      fputc('^', file);
    }

    for (int i = 0; i < 85; i++)
    {
      fputc(' ', file);
    }
    fputc('\n', file);

    // Agrega dos saltos de línea después de cada cuadro
    fputc('\n', file);
    fputc('\n', file);
  }

  fclose(file);
}
// Esta función desplaza el GtkScrolledWindow hasta la parte inferior.
static gboolean scroll_to_bottom(gpointer user_data)
{
  GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(user_data);
  GtkAdjustment *vadjustment = gtk_scrolled_window_get_vadjustment(scrolled_window);
  gtk_adjustment_set_value(vadjustment, gtk_adjustment_get_upper(vadjustment));

  return FALSE;
}

static void borrado(GtkButton *button, gpointer user_data)
{
  // LIMPIA LOS CUADROS
  for (int i = 0; i < 4; i++)
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[i]));
    gtk_text_buffer_set_text(buffer, "", 0);
    cuadros = 0;

    gtk_widget_set_visible(text_view[1], FALSE); // HAGO INVISIBLE EL CUADRO
    gtk_widget_set_visible(frame[1], FALSE);     // HAGO INVISIBLE EL MARCO
    gtk_widget_set_visible(text_view[2], FALSE); // HAGO INVISIBLE EL CUADRO
    gtk_widget_set_visible(frame[2], FALSE);     // HAGO INVISIBLE EL MARCO
    gtk_widget_set_visible(text_view[3], FALSE); // HAGO INVISIBLE EL CUADRO
    gtk_widget_set_visible(frame[3], FALSE);     // HAGO INVISIBLE EL MARCO
  }
}

// ESTA FUNCION ME CREA UNA VENTANA CON EL TUTORIAL DE MI PROGRAMA
static void informacion(GtkButton *button, gpointer user_data)
{
  GtkWidget *box_main, *box_etiquetas; // CAJAS
  GtkWidget *message_label;            // MENSAJE
  GtkWidget *mensaje2;                 // MENSAJE
  GtkWidget *instrucciones;            // VENTANA
  GtkCssProvider *provider;            // PARA EL FRONT END

  // CREO LA VENTANA
  instrucciones = gtk_dialog_new();

  // PARA EL CSS
  provider = gtk_css_provider_new();

  // Definir una clase CSS
  gtk_css_provider_load_from_data(provider, ".miClase { background-color: #a3cef1; font-weight: bolder ;color: #012a4a;font-family: roboto;font-size:16px }", -1);

  // Aplicar la clase CSS a la ventana de di?logo
  gtk_widget_add_css_class(instrucciones, "miClase");

  gtk_style_context_add_provider(gtk_widget_get_style_context(instrucciones),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_set_default_size(GTK_WINDOW(instrucciones), 500, 250);
  gtk_window_set_resizable(GTK_WINDOW(instrucciones), FALSE);
  gtk_window_set_icon_name(GTK_WINDOW(instrucciones), "dialog-information");

  // TITULO DE MI VENTANA
  gtk_window_set_title(GTK_WINDOW(instrucciones), " INSTRUCCIONES DE USO");

  // Crear una caja vertical principal para organizar los elementos
  box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Crear una caja vertical para las etiquetas
  box_etiquetas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Mensaje directo (etiqueta) en la parte superior
  mensaje2 = gtk_label_new("Estimado usuario...");
  gtk_label_set_justify(GTK_LABEL(mensaje2), GTK_JUSTIFY_CENTER);

  // MENSAJE A LA ETIQUETA
  gtk_box_append(GTK_BOX(box_etiquetas), mensaje2);
  message_label = gtk_label_new("\n\n                   =====Por favor, lea con atención las siguientes instrucciones:\n\n"
                                "1) El presente editor de texto tiene como objetivo almacenar hasta un total de 4 páginas de 22 líneas cada una incluyendo funcion de encriptado/desencriptado de informacion. "
                                "Este límite no se puede aumentar.\n\n"
                                "2)Al usted alcanzar el limite,el contenido se va a limpiar y se perdera,procure no pasar el limite.\n\n"
                                "3) Al presionar el botón 'Guardar', se estará creando un archivo en formato .txt  con el nombre 'informacion'/'cifrado' \n "
                                "Este contiene lo escrito por usted.El archivo 'cifrado' se creara al presionar el boton de encriptar mientras que el archivo 'informacion' tiene guardado automatico.\n\n"
                                "4) El presente editor de texto solo hace correcciones ortográficas en español.\n\n"
                                "5) El texto siempre será justificado.\n\n"
                                "6) Después de escribir un punto (.), la siguiente letra estará en mayúscula de manera automática.\n\n"
                                "7) Los nombres propios no tienen corrección ortográfica.\n\n"
                                "8) Solo puede pegar texto mientras no exceda 22 lineas y 4 paginas; "
                                "9) Algunos nombres propios no tienen corrección ortográfica.\n\n"
                                "10) El boton borrar limpia la entrada que esta ingresando en el usuario en los cuadros de texto y te redirecciona al primer cuadro para que inicies la escritura desde cero.\n\n"
                                "=====FAQ (DUDAS FRECUENTES)=====\n\n"
                                "A) Funcionamiento del corrector: La palabra incorrecta va a estar marcada en color rojo para que el usuario vea el listado de sugerencias,el usuario debe presionar entre las sugerencias para asi sustituir la palabra incorrecta\n\n"
                                "B) Si cierro el programa sin haber presionado GUARDAR ¿Pierdo mi texto? No,existe guardado automatico.\n\n"
                                "CONTACTO DEL CREADOR DEL PROGRAMA: caceresuriel22j@gmail.com\n\n\n"
                                "AUTOR: URIEL CACERES V-29664183\n\n"
                                "ESTE PROGRAMA FUE CREADO CON FINES ACADÉMICOS ");

  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_FILL); // JUSTIFICO EL TEXTO

  // Agregar espacio alrededor de la etiqueta
  gtk_widget_set_margin_start(message_label, 15);  // Espacio a la izquierda
  gtk_widget_set_margin_end(message_label, 15);    // Espacio a la derecha
  gtk_widget_set_margin_top(message_label, 15);    // Espacio arriba
  gtk_widget_set_margin_bottom(message_label, 25); // Espacio abajo

  gtk_window_set_title(GTK_WINDOW(instrucciones), "Instrucciones de Uso");

  // A?adir la etiqueta a la caja de etiquetas
  gtk_box_append(GTK_BOX(box_etiquetas), message_label);

  // A?adir la caja de etiquetas a la caja principal
  gtk_box_append(GTK_BOX(box_main), box_etiquetas);
  gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER);

  // Establecer la caja principal como hijo de la ventana
  gtk_window_set_child(GTK_WINDOW(instrucciones), box_main);

  // Mostrar la nueva ventana
  gtk_widget_show(instrucciones);
}

// ESTA FUNCION ME CREA UNA VENTANA CON EL TUTORIAL DE MI ENCRIPTADO
static void data(GtkButton *button, gpointer user_data)
{
  GtkWidget *box_main, *box_etiquetas; // CAJAS
  GtkWidget *message_label;            // MENSAJE
  GtkWidget *mensaje2;                 // MENSAJE
  GtkWidget *data;                     // VENTANA
  GtkCssProvider *provider;            // PARA EL FRONT END

  // CREO LA VENTANA
  data = gtk_dialog_new();

  // PARA EL CSS
  provider = gtk_css_provider_new();

  // Definir una clase CSS
  gtk_css_provider_load_from_data(provider, ".miClase { background-color: #a3cef1; font-weight: bolder ;color: #012a4a;font-family: roboto;font-size:16px }", -1);

  // Aplicar la clase CSS a la ventana de di?logo
  gtk_widget_add_css_class(data, "miClase");

  gtk_style_context_add_provider(gtk_widget_get_style_context(data),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_set_default_size(GTK_WINDOW(data), 500, 250);
  gtk_window_set_resizable(GTK_WINDOW(data), FALSE);
  gtk_window_set_icon_name(GTK_WINDOW(data), "dialog-information");

  // TITULO DE MI VENTANA
  gtk_window_set_title(GTK_WINDOW(data), " INSTRUCCIONES DE USO");

  // Crear una caja vertical principal para organizar los elementos
  box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Crear una caja vertical para las etiquetas
  box_etiquetas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Mensaje directo (etiqueta) en la parte superior
  mensaje2 = gtk_label_new("Estimado usuario...");
  gtk_label_set_justify(GTK_LABEL(mensaje2), GTK_JUSTIFY_CENTER);

  // MENSAJE A LA ETIQUETA
  gtk_box_append(GTK_BOX(box_etiquetas), mensaje2);
  message_label = gtk_label_new("hola esto es una prueba");

  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_FILL); // JUSTIFICO EL TEXTO

  // Agregar espacio alrededor de la etiqueta
  gtk_widget_set_margin_start(message_label, 15);  // Espacio a la izquierda
  gtk_widget_set_margin_end(message_label, 15);    // Espacio a la derecha
  gtk_widget_set_margin_top(message_label, 15);    // Espacio arriba
  gtk_widget_set_margin_bottom(message_label, 25); // Espacio abajo

  gtk_window_set_title(GTK_WINDOW(data), "FUNCIONAMIENTO DE LA ENCRIPTACION");

  // A?adir la etiqueta a la caja de etiquetas
  gtk_box_append(GTK_BOX(box_etiquetas), message_label);

  // A?adir la caja de etiquetas a la caja principal
  gtk_box_append(GTK_BOX(box_main), box_etiquetas);
  gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER);

  // Establecer la caja principal como hijo de la ventana
  gtk_window_set_child(GTK_WINDOW(data), box_main);

  // Mostrar la nueva ventana
  gtk_widget_show(data);
}

void paste_received(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
  // printf("se llama a paste");
  GdkClipboard *clipboard = GDK_CLIPBOARD(source_object);
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(user_data);
  GError *error = NULL;

  gchar *text = gdk_clipboard_read_text_finish(clipboard, res, &error);
  if (error != NULL)
  {
    g_printerr("Error reading clipboard text: %s\n", error->message);
    g_clear_error(&error);
    return;
  }

  if (text != NULL)
  {
    // Obtiene el número de líneas existentes en el buffer antes de pegar
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *existing_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    gchar **existing_lines = g_strsplit(existing_text, "\n", -1);
    int existing_line_count = g_strv_length(existing_lines);
    g_strfreev(existing_lines);
    g_free(existing_text);

    gchar **lines = g_strsplit(text, "\n", -1);
    int cuadros = 0; // Añade un contador para las líneas

    for (int i = 0; lines[i] != NULL; i++)
    {
      // Si se han insertado 22 líneas en el cuarto cuadro, deja de insertar líneas
      if ((cuadros + existing_line_count) / 22 == 4)
      {
        break;
      }
      gtk_text_buffer_insert_at_cursor(buffer, lines[i], -1);
      if (lines[i + 1] != NULL)
      {
        gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
      }
      cuadros++; // Incrementa el contador después de cada línea
      if ((cuadros + existing_line_count) % 22 == 0 && lines[i + 1] != NULL)
      { // Si se han insertado 22 líneas y aún hay más líneas...
        if ((cuadros + existing_line_count) / 22 < 4)
        { // Verifica que el índice esté dentro de los límites del array
          // Hace visible el siguiente GtkTextView y su marco.
          gtk_widget_set_visible(text_view[(cuadros + existing_line_count) / 22], TRUE);
          gtk_widget_set_visible(frame[(cuadros + existing_line_count) / 22], TRUE);
          // Mueve el foco al siguiente GtkTextView.
          gtk_widget_grab_focus(text_view[(cuadros + existing_line_count) / 22]);
          // Cambia el buffer al del siguiente GtkTextView.
          buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[(cuadros + existing_line_count) / 22]));
        }
      }
      if ((cuadros + existing_line_count) % 22 == 1 && (cuadros + existing_line_count) / 22 == 4)
      { // Si se han insertado 23 líneas en el cuarto cuadro...
        // Bloquea la inserción de texto
        gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view[4]), FALSE);
      }
    }
    g_strfreev(lines);
    g_free(text);
  }
}

void handle_paste(GtkTextView *text_view, GtkTextBuffer *buffer)
{
  printf("se llama a handle\n");
  GdkClipboard *clipboard = gtk_widget_get_clipboard(GTK_WIDGET(text_view));
  gdk_clipboard_read_text_async(clipboard, NULL, paste_received, buffer);
  g_signal_stop_emission_by_name(text_view, "paste-clipboard");
}

static void on_backspace(GtkWidget *widget, gpointer data)
{
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
  GtkTextIter start_iter, end_iter;
  gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);

  // Obtiene el texto del buffer para verificar si está vacío
  gchar *text = gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, FALSE);

  // Verifica si el cursor está al principio del buffer
  gboolean cursor_at_start = gtk_text_iter_get_offset(&start_iter) == 0;

  // Verifica si el buffer está vacío y si no es la primera página
  if (cursor_at_start && g_strcmp0(text, "") == 0 && cuadros > 0)
  {
    // Oculta la página actual
    gtk_widget_set_visible(text_view[cuadros], FALSE);

    // Disminuye el contador de páginas
    cuadros--;

    // Mueve el foco a la página anterior
    gtk_widget_grab_focus(text_view[cuadros]);

    // Coloca el cursor al final del buffer de la página anterior
    GtkTextBuffer *prev_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[cuadros]));
    gtk_text_buffer_get_end_iter(prev_buffer, &end_iter);
    gtk_text_buffer_place_cursor(prev_buffer, &end_iter);
  }
}

int cifrado(char *cadena, int size, GtkTextBuffer *buffer)
{
  char temp[100] = ""; // Variable temporal para almacenar los Ã­ndices
  char str_indice[4];  // Para almacenar el Ã­ndice y los corchetes
  int div;             // para matriz
  int igual;           //

  div = (size + 2) / 3; // Calcular el nÃºmero de filas necesarias

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
      printf(" (%d) ", encriptado2[i][j]);
      sprintf(buffer, " (%d) ", encriptado2[i][j]); // Formatea el número
      strcat(mensaje, buffer);                      // Concatena el número formateado a la cadena de mensaje[i]
    }
  }
  g_free(mensaje);
  return 0;
}

static void llamadaCifrada()
{
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  char *text;
  memset(mensaje, 0, sizeof(mensaje));

  for (int i = 0; i < 4; i++)
  {
    // Obtén el buffer del text view
    buffer = gtk_text_view_get_buffer(text_view[i]);

    // Obtén los iteradores para el inicio y el final del texto en el buffer
    gtk_text_buffer_get_bounds(buffer, &start, &end);

    // Obtén el texto del buffer
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // Llama a tu función encriptado con el texto obtenido y el buffer
    cifrado(text, strlen(text), buffer);

    // Reinicia el mensaje para el siguiente índice
    memset(mensaje, 0, sizeof(mensaje));
  }
}

int desencriptado(int encriptado2[100][3])
{
  int regreso[100][3];   // Declare and initialize the regreso variable
  int div = totales / 3; // Calculate the number of required rows
  int inversa[3][3] = {
      {-1, -10, -8},
      {-1, -6, -5},
      {0, -1, -1}};

  printf("  DIVVVV %d \n\n", div);

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

        strcat(descifrado, temp); // Append the temporary string to descifrado
      }
    }
  }

  tamano = strlen(descifrado);
  printf("DESCIFRADO size %d \n", tamano);
  printf("%s \n", descifrado);

  return 0;
}

void llamada2(GtkButton *button, gpointer user_data)
{
  GtkTextView *text_view = GTK_TEXT_VIEW(user_data);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

  // Llama a tu funciÃ³n desencriptado aquÃ­ y guarda la cadena generada en 'descifrado'
  desencriptado(encriptado2);

  // Actualiza el TextView con la cadena generada
  gtk_text_buffer_set_text(buffer, descifrado, -1);
}
