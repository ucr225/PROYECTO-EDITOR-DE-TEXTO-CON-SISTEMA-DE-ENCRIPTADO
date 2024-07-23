/*
PROYECTO N#2-LAB DE LENGUAJE DE PROGRAMACION
REALIZADO POR URIEL CACERES V-29664183
FECHA DE INICIO 11-12-2023

VERSION 2.0
CUADRO DE TEXTO QUE PERMITA HACER CORRECION ORTOGRAFICA Y ENCRIPTACION

*/

#include <gtk/gtk.h>           //  GTK
#include <hunspell/hunspell.h> //LIBRERIA DE EDICION DE TEXTO
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>



// VARIABLES GLOBALES

// PARA CARACTERES ESPECIALES

//  Crear un arreglo para almacenar los contadores de caracteres
gint total_len = 0;
GtkWidget *text_view[4];
GtkWidget *frame[4];
// GtkTextBuffer *buffer; // bufer del cuadro de texto anterior
GtkEventController *key_controller;
int cuadros = 0; // CONTADOR DE CUADROS
// gboolean was_period = FALSE;

// Variable global o estática para el contador de clics
static int n_press = 0; // ES PARA LOS CLICs

GtkWidget *button1; // BOTONES
GtkWidget *button2;
GtkWidget *button3;
GMenu *menu;

// ENCRIPTACION-VARIABLES

int size, nuevo;
char descifrado_individual[4][100000]; // Array para almacenar el descifrado de cada GtkTextView

int encriptado[100000][3];
int encriptado2[100000][3];
int cuadro1[100000][3];
int cuadro2[100000][3];
int cuadro3[100000][3];
int cuadro4[100000][3];
char cadena[100000];
char mensaje[100000];
int totales;
char descifrado[100000];
char buffer[100000]; // Buffer temporal para formatear cada nÃºmero
// GtkWidget *text_views[4];              // Crea un nuevo arreglo de GtkTextViews
GtkTextBuffer *buffers[4];

/*
char arreglo[] = {
    32, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
    193, 201, 205, 211, 218, 220, 225, 233, 237, 243, 250, 252,
    46, 58, 44, 59, 34, 40, 41, 91, 93, 63, 33,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
    47, 42, 43, 45, 61, 191, 161, 45, 10};
*/

char arreglo[] = {
    ' ', 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'o', 'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z', 'Á', 'É', 'Í', 'Ó', 'Ú', 'Ñ', 'á', 'é', 'í', 'ó', 'ú', 'ñ', '¿', '¡', '.', ':', ',', ';', '"', '(', ')', '[', ']', '?', '!', '/', '*', '+', '-', '=', '-', '\n', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int bandera[3][3] = {
    {1, -2, 2},
    {-1, 1, 3},
    {1, -1, -4}

};

// PROTOTIPOS DE FUNCIONES
static void activate(GtkApplication *app, gpointer user_data);                                                                  // Mi constructor
void lineas(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data);                                // Manejo de lineas
gboolean tabulacion(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data); // Tab
void desbloqueo(GtkButton *button, gpointer user_data);                                                                         // reinicio al limpiar
static void borrado(GtkButton *button, gpointer user_data);                                                                     // limpio todo
static void informacion(GtkButton *button, gpointer user_data);                                                                 // ventana con informacion del programa
void portapapeles(GObject *source_object, GAsyncResult *res, gpointer user_data);                                               // texto pegado
void gestionPegar(GtkTextView *text_view, GtkTextBuffer *buffer);
static void saving(GtkButton *button, gpointer user_data);                   // archivo txt normal
static gboolean mirada(gpointer user_data);                                  // scroll automatico
void actualizacion(GtkTextBuffer *buffer, gpointer user_data);               // guardado automatico
static void Confirmacion(GtkButton *button, gpointer user_data);             // txt ventana
static void ConfirmacionEncriptacion(GtkButton *button, gpointer user_data); // ventana cifrar
static void ConfirmacionDescifrado(GtkButton *button, gpointer user_data);   // ventana descifrar
// CORRECTOR
gboolean worter(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data); // click cambio palabra
void lineaRoja(GtkTextBuffer *buffer, gpointer user_data);                                      // subrayado
void revision(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end, gpointer data);
void sugerencia(GtkListBox *box, GtkListBoxRow *row, gpointer data);
void modificacion(GtkTextBuffer *buffer, gpointer user_data);

// CIFRADO
int cifrado(char *cadena, int size);
void llamadaCifrada(GtkWidget *widget, gpointer user_data); // llama la funcion y tomo el buffer
void Desencriptar(GtkButton *button, gpointer user_data);

// FUNCIONES

//  MAIN-AQUI SE EJECUTA TODO
int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

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
  GtkWidget *window;    // VENTANA
  GtkWidget *header;    // ENCABEZADO DE LA VENTANA
  GtkWidget *watermark; // marca personal
  GtkGesture *gesture;  // Para el click

  GtkCssProvider *provider; // PARA MI FRONT END
  provider = gtk_css_provider_new();

  GtkWidget *scrolled_window; // VENTANA DINAMICA

  GtkTextBuffer *buffer; // BUFFER
  gint limite = 22;      // LIMITE DE LINEAS

  // Creamos una nueva ventana con desplazamiento
  scrolled_window = gtk_scrolled_window_new();
  gtk_window_set_default_size(GTK_WINDOW(scrolled_window), 950, 850); // TAMANO DE LA VENTANA AL CREARSE
  gtk_widget_set_size_request(scrolled_window, 900, 800);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "EDITOR");
  gtk_window_set_default_size(GTK_WINDOW(window), 950, 850);
  gtk_widget_set_size_request(window, 900, 800);

  // Creamos un nuevo encabezado
  header = gtk_header_bar_new();
  gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE); // VISIBILIDAD DEL HEADER
  gtk_window_set_titlebar(GTK_WINDOW(window), header);                 // POSICIONAR EL HEADER EN LA VENTANA CON JERARQUIA DE OBJETOS

  // Crea un GMenu con las opciones para el GtkMenuButton
  menu = g_menu_new();
  // Establece el nombre del icono para el botón del menú
  g_menu_append(menu, "Guardar... ", "app.saving");
  g_menu_append(menu, "Ayuda", "app.informacion");

  // En tu función principal o de inicialización
  GSimpleAction *factory_action = g_simple_action_new("saving", NULL);
  g_signal_connect(factory_action, "activate", G_CALLBACK(saving), NULL);
  g_signal_connect(factory_action, "activate", G_CALLBACK(Confirmacion), NULL);
  g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(factory_action));

  GSimpleAction *subway_action = g_simple_action_new("informacion", NULL);
  g_signal_connect(subway_action, "activate", G_CALLBACK(informacion), NULL);
  g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(subway_action));

  // Crea un GtkMenuButton y asigna el modelo de menú
  GtkWidget *menu_button = gtk_menu_button_new();
  gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_button), G_MENU_MODEL(menu));
  gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button), "emblem-system");

  gtk_widget_show(menu_button); // Asegúrate de que el botón del menú sea visible
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), menu_button);

  // Crea un nuevo GtkLabel
  GtkWidget *label = gtk_label_new("<=  MENU");
  gtk_widget_add_css_class(label, "label"); // PASO LA CLASE DEL CSS

  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), label);
  // Muestra el GtkLabel
  gtk_widget_show(label);

  button1 = gtk_button_new_with_label("LIMPIAR");
  g_signal_connect_swapped(button1, "clicked", G_CALLBACK(borrado), window);
  gtk_widget_add_css_class(button1, "button"); // lE PASAS EL CSS AL BOTON

  button2 = gtk_button_new_with_label("ENCRIPTAR"); // TITULO DEL BOTON
  gtk_widget_add_css_class(button2, "button2");     // PASO LA CLASE DEL CSS
  g_signal_connect(button2, "clicked", G_CALLBACK(llamadaCifrada), text_view);
  g_signal_connect(button2, "clicked", G_CALLBACK(ConfirmacionEncriptacion), text_view);
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button1); // PASAR UBICACION AL BOTON
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button2); // LO PONGO EN EL HEADER

  button3 = gtk_button_new_with_label("DESENCRIPTAR "); // NOMBRE DEL BOTON
  g_signal_connect(button3, "clicked", G_CALLBACK(Desencriptar), text_view);
  g_signal_connect(button3, "clicked", G_CALLBACK(ConfirmacionDescifrado), text_view);
  gtk_widget_set_sensitive(button3, FALSE);
  gtk_widget_add_css_class(button3, "button3");               // CLASE DE CSS
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button3); // LO PONGO EN EL HEADER DE LA VENTANA

  // Creamos una caja para contener los cuadros de texto
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  // Cargamos las reglas CSS-CON ESO PASO TODO LO REFERENTE A PERSONALIZACION
  gtk_css_provider_load_from_data(
      provider,

      //"window { background-color: #219ebc  }"
      "window { background: url('file:///C:/Users/uriel/OneDrive/Imágenes/patron.jpg');background - size : cover;background - position : center}"
      ".label { color: #3d348b; font-weight: bold; font-family: roboto; font-size: 14px; }"
      ".button {  color:#a4161a ; font-weight: bold;font family:roboto;font-size:14px;}"
      ".button2 {  color:#1d3557 ; font-weight: bold;font family:monospace;font-size:14px;}"
      ".button3 {  color:green; font-weight: bold;font family:monospace;font-size:14px;}"
      ".watermark { color: #cae9ff; background-color: #003f88; font-family: roboto, monospace; font-size: 17px;font-weight: bold; border-top-left-radius: 15px;border-bottom-right-radius: 15px;}"
      ".text_view{ background-color:#f6fff8 ;border: 7px solid #03045e; font-family: roboto ; font-size:18px ; color: #003049;text-decoration: noneline-height: 1.8; }",
      -1);

  // CSS AL WIDGET

  // Aplicamos el proveedor de CSS al contexto de estilo del GtkLabel
  gtk_style_context_add_provider(gtk_widget_get_style_context(label),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button1),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button2),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider(gtk_widget_get_style_context(button3),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // Aplicamos las reglas CSS a la ventana
  gtk_style_context_add_provider(gtk_widget_get_style_context(window),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  // CENTRO LA CAJA DE LOS CUADROS DE TEXTO PA QUE SE GENEREN CENTRADOS

  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_box_set_spacing(GTK_BOX(box), 20);
  // BUCLE QUE ME GENERA MIS CUADROS DE TEXTO,ES UN ARREGLO

  GtkWidget *sInicio = gtk_label_new("");
  gtk_widget_set_size_request(sInicio, -1, 15); // Ajusta el '15' al tamaNo de espacio que desees
  gtk_box_append(GTK_BOX(box), sInicio);

  for (int i = 0; i < 4; i++)
  {
    // Creamos un nuevo cuadro de texto
    text_view[i] = gtk_text_view_new();
    int *index_ptr = g_new(int, 1);
    *index_ptr = i;

    key_controller = gtk_event_controller_key_new();                // CONTROLADOR DE LAS TECLAS
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[i])); // CREO UN BUFFER PARA CADA CUADRO
    gtk_text_buffer_set_text(buffer, "", 0);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[i]));

    g_signal_connect_data(buffer, "insert-text", G_CALLBACK(lineas), index_ptr, (GClosureNotify)g_free, 0);

    // CORRECTOR!!
    gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), GDK_BUTTON_PRIMARY);
    g_signal_connect(gesture, "pressed", G_CALLBACK(worter), text_view[i]);
    gtk_widget_add_controller(text_view[i], GTK_EVENT_CONTROLLER(gesture));

    // CONECTANDO FUNCIONES..
    g_signal_connect(buffer, "changed", G_CALLBACK(modificacion), NULL);
    g_signal_connect(text_view[i], "paste-clipboard", G_CALLBACK(gestionPegar), buffer);
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(tabulacion), NULL);
    gtk_widget_add_controller(GTK_WIDGET(text_view[i]), key_controller);            // Controlador a la tecla
    gtk_text_view_set_justification(GTK_TEXT_VIEW(text_view[i]), GTK_JUSTIFY_FILL); // JUSTIFICAR TEXTO
    g_signal_connect(buffer, "changed", G_CALLBACK(actualizacion), text_view[i]);   // ESTO PERMITE EL GUARDADO AUTOMATICO
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view[i]), GTK_WRAP_WORD_CHAR);   // PARA LO DE LA LINEA INFINITA

    gtk_widget_set_size_request(text_view[i], 765, 75); // DIMENSIONES EN PX DEL CUADRO DE TEXTO

    // MARCO QUE CONTIENE LOS CUADROS DE TEXTO
    frame[i] = gtk_frame_new(NULL);

    // Establecemos un margen superior de 10 unidades
    gtk_widget_set_margin_top(frame[i], 15);
    gtk_frame_set_child(GTK_FRAME(frame[i]), text_view[i]);
    gtk_box_append(GTK_BOX(box), frame[i]); // METES EL MARCO  enCAJA

    GtkStyleContext *style_context = gtk_widget_get_style_context(text_view[i]);
    gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_widget_add_css_class(text_view[i], "text_view"); // CLASE DEL CSS

    if (i != 0)
    {
      gtk_widget_set_visible(text_view[i], FALSE); // HAGO INVISIBLE EL CUADRO
      gtk_widget_set_visible(frame[i], FALSE);     // HAGO INVISIBLE EL MARCO
    }
  }

  GtkWidget *sFinal = gtk_label_new(""); // ASI NO SE SALEN PEGADOS
  gtk_widget_set_size_request(sFinal, -1, 20);

  watermark = gtk_label_new("PROYECTO #2 LAB-LENGUAJE DE PROGRAMACIÓN\nProf. JOSÉ RAMÍREZ\n ENERO 2024\n\n V-2.0 \n\n AUTOR: CACERES RIOS URIEL JOSE CONCEPCION\nV.29664183\n ");
  gtk_widget_add_css_class(watermark, "watermark");

  gtk_label_set_justify(GTK_LABEL(watermark), GTK_JUSTIFY_CENTER);
  gtk_label_set_xalign(GTK_LABEL(watermark), 0.5);
  gtk_label_set_yalign(GTK_LABEL(watermark), 0.5);
  gtk_box_append(GTK_BOX(box), watermark);
  gtk_box_append(GTK_BOX(box), sFinal);

  // Aplicamos las reglas CSS a la ventana
  gtk_style_context_add_provider(gtk_widget_get_style_context(watermark),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);
  gtk_window_set_child(GTK_WINDOW(window), scrolled_window);

  // Mostramos la ventana
  gtk_widget_show(window);
}

void lineas(GtkTextBuffer *buffer, GtkTextIter *location, gchar *text, gint len, gpointer data)
{
  GtkTextIter start, end;
  int cuadros = *(int *)data;
  int cont = 0;
  gtk_text_buffer_get_start_iter(buffer, &start);
  // Inicializa el iterador 'end' al inicio del texto.
  end = start;
  GtkTextView *current_text_view = GTK_TEXT_VIEW(text_view[cuadros]);
  GtkWidget *scrolled_window = gtk_widget_get_ancestor(GTK_WIDGET(current_text_view), GTK_TYPE_SCROLLED_WINDOW);
  // Desplazamos el GtkScrolledWindow hasta la parte inferior.
  g_idle_add(mirada, scrolled_window);
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
    GtkWidget *emergencia = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(emergencia), "ALERTA!");
    gtk_window_set_default_size(GTK_WINDOW(emergencia), 300, 200);
    gtk_window_set_resizable(GTK_WINDOW(emergencia), FALSE);

    // Crear una etiqueta con el mensaje
    GtkWidget *label = gtk_label_new("ESTIMADO USUARIO \n\n USTED ALCANZO EL LIMITE DE LINEAS DISPONIBLE EN EL PROGRAMA  \nPOR MOTIVOS DE SEGURIDAD\nSE LIMPIARA TODO EL CONTENIDO\nPROCURE ESCRIBIR 22 LINEAS POR PÁGINA \n\n");
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

    GtkWidget *ok_button = gtk_button_new_with_label("OK");
    g_signal_connect(ok_button, "clicked", G_CALLBACK(desbloqueo), text_view);
    g_signal_connect(ok_button, "clicked", G_CALLBACK(borrado), text_view);
    g_signal_connect_swapped(ok_button, "clicked", G_CALLBACK(gtk_window_destroy), emergencia);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    // meto la etiqueta al grid
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    // meto el botón "OK" al grid
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 1, 1, 1);
    gtk_widget_set_halign(ok_button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(ok_button, GTK_ALIGN_END);
    gtk_window_set_child(GTK_WINDOW(emergencia), grid);

    // Aplicar CSS para cambiar el color de fondo
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    ".emergencia { background-color: #ef233c; font-weight: bold;color: #22181c ;font:condensed 120% sans-serif;font-size:20px  }"
                                    "* {font-size: 16px; font-family:roboto}",
                                    -1);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_add_css_class(emergencia, "emergencia");

    // Mostrar la ventana
    gtk_widget_show(emergencia);
  }
}

// BLOQUEO EL TABULADOR PARA EVITAR DESTRUIR MI CUADRO DE TEXTO
gboolean tabulacion(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data)
{
  // Verificar si la tecla presionada es Tab
  if (keyval == GDK_KEY_Tab || keyval == GDK_KEY_ISO_Left_Tab)
  {
    return TRUE;
  }
  return FALSE;
}

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

static void saving(GtkButton *button, gpointer user_data)
{

  char informacion[600];
  char informacion2[600];

  sprintf(informacion, "C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\ARCHIVO .txt\\informacion.txt");
  // sprintf(informacion2, "C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\ARCHIVO .txt\\informacion2.txt");

  FILE *file = fopen(informacion, "w");
  // FILE *file2 = fopen(informacion2, "w");

  for (int j = 0; j < 4; j++)
  {
    // Comienza un nuevo cuadro para cada GtkTextView
    for (int i = 0; i < 85; i++)
    {
      fputc('+', file);
      // fputc('^', file2);
    }

    for (int i = 0; i < 85; i++)
    {
      fputc(' ', file);
      // fputc(' ', file2);
    }
    fputc('\n', file);
    // fputc('\n', file2);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view[j]));
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
      fputc('+', file);
      fputc(' ', file);
      // fputc('^', file2);
      // fputc(' ', file2);

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
            // fputc(' ', file2);
          }
          fputc('+', file);
          fputc('\n', file);
          // fputc('^', file2);
          // fputc('\n', file2);
          char_count = 0;
          fputc('+', file);
          fputc(' ', file);
          // fputc('^', file2);
          // fputc(' ', file2);
        }
        else
        {
          fputc(text[j], file);
          // fputc(text[j], file2);
          char_count++;
        }
      }

      for (int k = char_count; k < max_line_length; k++)
      {
        fputc(' ', file);
        // fputc(' ', file2);
      }
      fputc('  +', file);
      fputc('\n', file);
      // fputc('  ^', file2);
      // fputc('\n', file2);
      char_count = 0;
    }

    g_free(text);

    // Termina el cuadro para cada GtkTextView
    for (int i = 0; i < 85; i++)
    {
      fputc('+', file);
      // fputc('^', file2);
    }

    for (int i = 0; i < 85; i++)
    {
      fputc(' ', file);
      // fputc(' ', file2);
    }
    fputc('\n', file);
    // fputc('\n', file2);

    // Agrega dos saltos de línea después de cada cuadro
    fputc('\n', file);
    fputc('\n', file);
    // fputc('\n', file2);
    // fputc('\n', file2);
  }

  fclose(file);
  // fclose(file2);
}
// Esta función desplaza el GtkScrolledWindow hasta la parte inferior.
static gboolean mirada(gpointer user_data)
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
    buffers[i] = NULL;
  }
  // encriptado2[100000][3] = {0};
  // cuadro1[100000][3] = {0};
  // cuadro2[100000][3] = {0};
  // cuadro3[100000][3] = {0};
  // cuadro4[100000][3] = {0};
  size = 0;
  nuevo = 0;
  totales = 0;

  memset(cadena, 0, sizeof(cadena));
  memset(mensaje, 0, sizeof(mensaje));
  memset(descifrado, 0, sizeof(descifrado));
  memset(descifrado_individual, 0, sizeof(descifrado_individual));
  memset(buffer, 0, sizeof(buffer));
  return 0;
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
                                "3) Al presionar el botón 'Guardar', se estará creando un archivo en formato .txt  con el nombre 'informacion' \n "
                                "Este contiene lo escrito por usted.El archivo 'cifrado' se creara al presionar el boton de encriptar mientras que el archivo 'informacion' tiene guardado automatico.\n\n"
                                "4) El presente editor de texto solo hace correcciones ortográficas en español.\n\n"
                                "5) El texto siempre será justificado.\n\n"
                                "6) Después de escribir un punto (.), la siguiente letra estará en mayúscula de manera automática.\n\n"
                                "7) Los nombres propios no tienen corrección ortográfica.\n\n"
                                "8) Solo puede pegar texto mientras no exceda 22 lineas y 4 paginas;\n\n "
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

void portapapeles(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
  GdkClipboard *clipboard = GDK_CLIPBOARD(source_object);
  GtkTextBuffer *buffer = GTK_TEXT_BUFFER(user_data);
  GError *error = NULL;

  gchar *text = gdk_clipboard_read_text_finish(clipboard, res, &error);
  if (error != NULL)
  {
    // g_printerr("Error al conseguir el texto del portapapeles  %s\n", error->message);
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
    int cuadros = 0;

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

void gestionPegar(GtkTextView *text_view, GtkTextBuffer *buffer)
{

  GdkClipboard *clipboard = gtk_widget_get_clipboard(GTK_WIDGET(text_view));
  gdk_clipboard_read_text_async(clipboard, NULL, portapapeles, buffer);
  g_signal_stop_emission_by_name(text_view, "paste-clipboard");
}

void llamadaCifrada(GtkWidget *widget, gpointer user_data)
{

  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  char *text;

  gtk_widget_set_sensitive(button2, FALSE);
  gtk_widget_set_visible(button2, FALSE);

  gtk_widget_set_visible(button1, FALSE);
  gtk_widget_set_sensitive(button3, TRUE);

  for (int i = 0; i < 4; i++)
  {
    gtk_widget_set_sensitive(text_view[i], FALSE);
  }

  for (int i = 0; i < 4; i++)
  {

    memset(mensaje, 0, sizeof(mensaje));

    // Obtén el buffer del text view
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view[i]);

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


  }
}

int cifrado(char *cadena, int size)
{

  char temp[100000] = ""; // Variable temporal para almacenar los Ã­ndices
  char str_indice[4];     // Para almacenar el Ã­ndice y los corchetes
  int div;                // para matriz
  int igual;              //

  div = (size + 2) / 3; // Calcular el nÃºmero de filas necesarias

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
    printf("TODO EL CONTENIDO ES ENCRIPTABLE\n");
  }
  else
  {
    printf("TODO EL CONTENIDO (NO) ES ENCRIPTABLE, HAY CARACTERES ESPECIALES\n");
  }

  if (rev == 1) // si se puede encriptar...
  {

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

    printf("MATRIZ!:\n");
    for (int i = 0; i < div; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        printf(" %d ", encriptado[i][j]);
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

      //encriptado2[i][j] = 0; // Reiniciar el valor encriptado2 en cada iteraciÃ³n
      totales++;
      for (int k = 0; k < 3; k++)
      {

        encriptado2[i][j] += encriptado[i][k] * bandera[k][j];
      }
    }
  }

  printf("\n");

  printf("COMPRUEBO...\n");

  for (int i = 0; i < div; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      encriptado[i][j] = 0;
      printf(" %d ", encriptado2[i][j]);
      sprintf(buffer, " %d ", encriptado2[i][j]);
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

void Desencriptar(GtkButton *button, gpointer user_data)
{

  gtk_widget_set_sensitive(button2, TRUE);
  gtk_widget_set_visible(button1, TRUE);
  gtk_widget_set_visible(button2, TRUE);

  for (int i = 0; i < 4; i++)
  {
    gtk_widget_set_sensitive(text_view[i], TRUE);
  }

  for (int i = 0; i < 4; i++)
  {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view[i]);

    // Obtén el texto actual del buffer
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    if (strlen(text) > 0)
    {
      int(*matriz)[3];
      switch (i)
      {
      case 0:
        matriz = cuadro1;
        break;
      case 1:
        matriz = cuadro2;
        break;
      case 2:
        matriz = cuadro3;
        break;
      case 3:
        matriz = cuadro4;
        break;
      }

      // Aquí comienza la función desencriptado

      int regreso[100000][3];
      int div = totales / 3;
      int inversa[3][3] = {
          {-1, -10, -8},
          {-1, -6, -5},
          {0, -1, -1}};

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

      char descifrado[100000] = "";
      int num = sizeof(arreglo) / sizeof(arreglo[0]);

      for (int i = 0; i < div; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          if (regreso[i][j] >= 0 && regreso[i][j] < num)
          {
            int valor = regreso[i][j];
            char temp[200] = {arreglo[valor], '\0'};
            strcat(descifrado, temp);
          }
        }
      }

      // ...

      int len = strlen(descifrado);
      int count = 0;
      for (int i = len - 1; i >= 0; i--)
      {
        if (isspace(descifrado[i]))
        {
          count++;
        }
        else
        {
          break;
        }
      }

      descifrado[len - count] = '\0';

      // Remove everything after the last space
      char *lastSpace = strrchr(descifrado, ' ');
      if (lastSpace != NULL)
      {
        *lastSpace = '\0';
      }

      // Aquí termina la función desencriptado

      strcpy(descifrado_individual[i], descifrado);
      gtk_text_buffer_set_text(buffer, descifrado_individual[i], -1);
    }

    g_free(text);
  }

  memset(cadena, 0, sizeof(cadena));
  memset(mensaje, 0, sizeof(mensaje));
  memset(descifrado, 0, sizeof(descifrado));

  memset(descifrado_individual, 0, sizeof(descifrado_individual));

  memset(encriptado, 0, sizeof(encriptado));
  memset(encriptado2, 0, sizeof(encriptado2));

  memset(cuadro1, 0, sizeof(cuadro1));
  memset(cuadro2, 0, sizeof(cuadro2));
  memset(cuadro3, 0, sizeof(cuadro3));
  memset(cuadro4, 0, sizeof(cuadro4));
}

void actualizacion(GtkTextBuffer *buffer, gpointer user_data)
{
  // Llama a la función archivo para guardar el archivo automáticamente //
  saving(NULL, user_data);
}

// ESTA ME MUESTRA UNA VENTANA QUE ME INDICA QUE SE CREO MI ARCHIVO TXT CON EXITO+RUTA DE UBICACION
static void Confirmacion(GtkButton *button, gpointer user_data)
{
  GtkWidget *box_main, *box_etiquetas; // CAJAS
  GtkWidget *message_label;            // MENSAJE
  GtkWidget *mensaje2;                 // OTRO MENSAJE
  GtkWidget *instrucciones;            // VENTANA
  GtkCssProvider *provider;            // PARA FRONT END

  // CREO LA VENTANA
  instrucciones = gtk_dialog_new();

  // LE PASO EL PROVEEDOR DE CSS PARA PERSONALIZAR
  provider = gtk_css_provider_new();

  // CLASE DE CSS
  gtk_css_provider_load_from_data(provider, ".miClase { background-color: #70e000; font-weight: bold;color: #012a4a;font:condensed 120% roboto ;font-size:18px }", -1);

  // PASO CSS A MI VENTANA DE DIALOGO
  gtk_widget_add_css_class(instrucciones, "miClase");
  // PASO EL PROVIDER DE CSS AL WIDGET PARA QUE SE APLIQUEN LOS CAMBIOS
  gtk_style_context_add_provider(gtk_widget_get_style_context(instrucciones),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_set_default_size(GTK_WINDOW(instrucciones), 400, 200);          // SIZE DE MI VENTANA
  gtk_window_set_resizable(GTK_WINDOW(instrucciones), FALSE);                // BLOQUEO EL REDIMENCIONAR
  gtk_window_set_icon_name(GTK_WINDOW(instrucciones), "dialog-information"); // ICONO PERO NO SIRVE XD

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ARCHIVO CREADO CON EXITO!! "); // TITULO PARA MI VENTANA

  box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE Y

  box_etiquetas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE X - CON AMBAS PUEDO CENTRAR TODO

  mensaje2 = gtk_label_new("Estimado usuario... \nSU ARCHIVO HA SIDO CREADO SATISFACTORIAMENTE \n POR FAVOR REVISE EL DIRECTORIO"); // IMPRIMO MI MENSAJE EN LA VENTANA
  gtk_label_set_justify(GTK_LABEL(mensaje2), GTK_JUSTIFY_CENTER);                                                                   // JUSTIFICO AL CENTRO MI TEXTO DE LA ETIQUETA

  gtk_box_append(GTK_BOX(box_etiquetas), mensaje2); // METO LA ETIQUETA EN LA CAJA,ASI SE PUEDE CENTRAR

  message_label = gtk_label_new("\n \n C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\ARCHIVO.txt\\informacion.txt \n\n NOTA: \n\n EL Guardado AUTOMATICO HA SIDO ACTIVADO");
  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_FILL); // LO MISMO ACA

  // Agregar espacio alrededor de la etiqueta
  gtk_widget_set_margin_start(message_label, 15);  // Espacio a la izquierda
  gtk_widget_set_margin_end(message_label, 15);    // Espacio a la derecha
  gtk_widget_set_margin_top(message_label, 15);    // Espacio arriba
  gtk_widget_set_margin_bottom(message_label, 25); // Espacio abajo

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ALERTA"); // TITULO PARA MI VENTANA

  gtk_box_append(GTK_BOX(box_etiquetas), message_label); // METO LA ETIQUETA EN LA CAJA

  gtk_box_append(GTK_BOX(box_main), box_etiquetas);  // RELACIONO LOS WIDGETS UNO DENTRO DE OTROS
  gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER); // ALINEO AL CENTRO HORIZONTAL
  gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER); // IGUAL PERO AL EJE VERTICAL

  gtk_window_set_child(GTK_WINDOW(instrucciones), box_main); // METO UN WIDGET DENTRO DE OTRO

  gtk_widget_show(instrucciones); // APARECE MI VENTANA
}

static void ConfirmacionEncriptacion(GtkButton *button, gpointer user_data)
{
  GtkWidget *box_main, *box_etiquetas; // CAJAS
  GtkWidget *message_label;            // MENSAJE
  GtkWidget *mensaje2;                 // OTRO MENSAJE
  GtkWidget *instrucciones;            // VENTANA
  GtkCssProvider *provider;            // PARA FRONT END

  // CREO LA VENTANA
  instrucciones = gtk_dialog_new();

  // LE PASO EL PROVEEDOR DE CSS PARA PERSONALIZAR
  provider = gtk_css_provider_new();

  // CLASE DE CSS
  gtk_css_provider_load_from_data(provider, ".miClase { background-color: #007f5f; font-weight: bold;color:black ;font: roboto ;font-size:18px }", -1);

  // PASO CSS A MI VENTANA DE DIALOGO
  gtk_widget_add_css_class(instrucciones, "miClase");
  // PASO EL PROVIDER DE CSS AL WIDGET PARA QUE SE APLIQUEN LOS CAMBIOS
  gtk_style_context_add_provider(gtk_widget_get_style_context(instrucciones),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_set_default_size(GTK_WINDOW(instrucciones), 400, 200);          // SIZE DE MI VENTANA
  gtk_window_set_resizable(GTK_WINDOW(instrucciones), FALSE);                // BLOQUEO EL REDIMENCIONAR
  gtk_window_set_icon_name(GTK_WINDOW(instrucciones), "dialog-information"); // ICONO PERO NO SIRVE XD

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ARCHIVO CREADO CON EXITO!! "); // TITULO PARA MI VENTANA

  box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE Y

  box_etiquetas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE X - CON AMBAS PUEDO CENTRAR TODO

  mensaje2 = gtk_label_new("Estimado usuario... \nSU CONTENIDO HA SIDO ENCRIPTADO SATISFACTORIAMENTE \n POR FAVOR REVISE EL DIRECTORIO  "); // IMPRIMO MI MENSAJE EN LA VENTANA
  gtk_label_set_justify(GTK_LABEL(mensaje2), GTK_JUSTIFY_CENTER);                                                                           // JUSTIFICO AL CENTRO MI TEXTO DE LA ETIQUETA

  gtk_box_append(GTK_BOX(box_etiquetas), mensaje2); // METO LA ETIQUETA EN LA CAJA,ASI SE PUEDE CENTRAR

  message_label = gtk_label_new("EL ENCRIPTADO HA SIDO REALIZADO SATISFACTORIAMENTE \n\n NOTA: \n\n FUNCIONES COMO ENTRADA DE TEXTO,LIMIAR TEXTO Y GUARDADO AUTOMATICO HAN SIDO DESACTIVADAS \n\n EL ARCHIVO ENCRIPTADO ESTA EN LA UBICACION: \n\n  C:\\Users\\uriel\\OneDrive\\Documentos\\LABORATORIO DE LENGUAJE DE PROGRAMACION\\PROYECTO ENCRIPTACION\\ARCHIVO .txt");
  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_FILL); // LO MISMO ACA

  // Agregar espacio alrededor de la etiqueta
  gtk_widget_set_margin_start(message_label, 15);  // Espacio a la izquierda
  gtk_widget_set_margin_end(message_label, 15);    // Espacio a la derecha
  gtk_widget_set_margin_top(message_label, 15);    // Espacio arriba
  gtk_widget_set_margin_bottom(message_label, 25); // Espacio abajo

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ALERTA"); // TITULO PARA MI VENTANA

  gtk_box_append(GTK_BOX(box_etiquetas), message_label); // METO LA ETIQUETA EN LA CAJA

  gtk_box_append(GTK_BOX(box_main), box_etiquetas);  // RELACIONO LOS WIDGETS UNO DENTRO DE OTROS
  gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER); // ALINEO AL CENTRO HORIZONTAL
  gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER); // IGUAL PERO AL EJE VERTICAL

  gtk_window_set_child(GTK_WINDOW(instrucciones), box_main); // METO UN WIDGET DENTRO DE OTRO

  gtk_widget_show(instrucciones); // APARECE MI VENTANA
}

static void ConfirmacionDescifrado(GtkButton *button, gpointer user_data)
{
  GtkWidget *box_main, *box_etiquetas; // CAJAS
  GtkWidget *message_label;            // MENSAJE
  GtkWidget *mensaje2;                 // OTRO MENSAJE
  GtkWidget *instrucciones;            // VENTANA
  GtkCssProvider *provider;            // PARA FRONT END

  // CREO LA VENTANA
  instrucciones = gtk_dialog_new();

  // LE PASO EL PROVEEDOR DE CSS PARA PERSONALIZAR
  provider = gtk_css_provider_new();

  // CLASE DE CSS
  gtk_css_provider_load_from_data(provider, ".miClase { background-color: #007f5f; font-weight: bold;color:black ;font: roboto ;font-size:18px }", -1);

  // PASO CSS A MI VENTANA DE DIALOGO
  gtk_widget_add_css_class(instrucciones, "miClase");
  // PASO EL PROVIDER DE CSS AL WIDGET PARA QUE SE APLIQUEN LOS CAMBIOS
  gtk_style_context_add_provider(gtk_widget_get_style_context(instrucciones),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_window_set_default_size(GTK_WINDOW(instrucciones), 400, 200);          // SIZE DE MI VENTANA
  gtk_window_set_resizable(GTK_WINDOW(instrucciones), FALSE);                // BLOQUEO EL REDIMENCIONAR
  gtk_window_set_icon_name(GTK_WINDOW(instrucciones), "dialog-information"); // ICONO PERO NO SIRVE XD

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ARCHIVO CREADO CON EXITO!! "); // TITULO PARA MI VENTANA

  box_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE Y

  box_etiquetas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); // CAJA EN EJE X - CON AMBAS PUEDO CENTRAR TODO

  mensaje2 = gtk_label_new("Estimado usuario... \nSU CONTENIDO HA SIDO DESCIFRADO SATISFACTORIAMENTE  "); // IMPRIMO MI MENSAJE EN LA VENTANA
  gtk_label_set_justify(GTK_LABEL(mensaje2), GTK_JUSTIFY_CENTER);                                         // JUSTIFICO AL CENTRO MI TEXTO DE LA ETIQUETA

  gtk_box_append(GTK_BOX(box_etiquetas), mensaje2); // METO LA ETIQUETA EN LA CAJA,ASI SE PUEDE CENTRAR

  message_label = gtk_label_new("EL DESCIFRADO HA SIDO REALIZADO SATISFACTORIAMENTE,YA PUEDE LEER SU CONTENIDO NUEVAMENTE \n\n");
  gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_FILL); // LO MISMO ACA

  // Agregar espacio alrededor de la etiqueta
  gtk_widget_set_margin_start(message_label, 15);  // Espacio a la izquierda
  gtk_widget_set_margin_end(message_label, 15);    // Espacio a la derecha
  gtk_widget_set_margin_top(message_label, 15);    // Espacio arriba
  gtk_widget_set_margin_bottom(message_label, 25); // Espacio abajo

  gtk_window_set_title(GTK_WINDOW(instrucciones), "ALERTA"); // TITULO PARA MI VENTANA

  gtk_box_append(GTK_BOX(box_etiquetas), message_label); // METO LA ETIQUETA EN LA CAJA

  gtk_box_append(GTK_BOX(box_main), box_etiquetas);  // RELACIONO LOS WIDGETS UNO DENTRO DE OTROS
  gtk_widget_set_halign(box_main, GTK_ALIGN_CENTER); // ALINEO AL CENTRO HORIZONTAL
  gtk_widget_set_valign(box_main, GTK_ALIGN_CENTER); // IGUAL PERO AL EJE VERTICAL

  gtk_window_set_child(GTK_WINDOW(instrucciones), box_main); // METO UN WIDGET DENTRO DE OTRO

  gtk_widget_show(instrucciones); // APARECE MI VENTANA
}

// Comienzo del programa con las funciones para el corrector  // -- ACHTUNG!!!
// Función que modifica el contenido de un GtkTextBuffer
void modificacion(GtkTextBuffer *buffer, gpointer user_data)
{
  // Declaración de iteradores para el inicio y el final del buffer
  GtkTextIter start, end;
  // Obtiene los límites del buffer
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  // Obtiene el texto del buffer
  char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  // Variable para controlar el inicio de una nueva oración
  bool new_sentence = true;

  // Obtiene la longitud del texto
  size_t len = strlen(text);

  // Recorre el texto
  for (size_t i = 0; i < len; ++i)
  {
    // Si es el inicio de una nueva oración y el carácter no es un espacio o un salto de línea
    if (new_sentence && text[i] != ' ' && text[i] != '\n')
    {
      // Convierte el carácter a mayúsculas
      text[i] = toupper(text[i]);
      // Indica que ya no es el inicio de una nueva oración
      new_sentence = false;
    }
    // Si el carácter es un punto
    else if (text[i] == '.')
    {
      // Indica que es el inicio de una nueva oración
      new_sentence = true;
    }
  }

  // Bloquea las señales del buffer para evitar un bucle infinito
  g_signal_handlers_block_by_func(buffer, modificacion, user_data);

  // Establece el nuevo texto en el buffer
  gtk_text_buffer_set_text(buffer, text, -1);

  // Desbloquea las señales del buffer
  g_signal_handlers_unblock_by_func(buffer, modificacion, user_data);

  // Llama a la función lineaRoja (no se proporcionó el código de esta función)
  lineaRoja(buffer, user_data);

  // Libera la memoria del texto
  g_free(text);
}

void sugerencia(GtkListBox *box, GtkListBoxRow *row, gpointer data)
{
  // Obtiene el primer hijo del widget 'row' y lo convierte a un GtkLabel.
  GtkLabel *label = GTK_LABEL(gtk_widget_get_first_child(GTK_WIDGET(row)));

  // Obtiene el texto del GtkLabel.
  const gchar *suggestion = gtk_label_get_text(label);

  // Obtiene el buffer de texto asociado con el GtkListBox.
  GtkTextBuffer *buffer = g_object_get_data(G_OBJECT(box), "buffer");

  // Obtiene los offsets de inicio y fin del texto seleccionado en el buffer.
  gint start_offset = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(box), "start_offset"));
  gint end_offset = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(box), "end_offset"));

  GtkTextIter start, end;

  // Obtiene los iteradores correspondientes a los offsets de inicio y fin.
  gtk_text_buffer_get_iter_at_offset(buffer, &start, start_offset);
  gtk_text_buffer_get_iter_at_offset(buffer, &end, end_offset);

  // Elimina el texto seleccionado en el buffer.
  gtk_text_buffer_delete(buffer, &start, &end);

  // Obtiene el iterador correspondiente al offset de inicio.
  gtk_text_buffer_get_iter_at_offset(buffer, &start, start_offset);

  // Inserta la sugerencia en el buffer de texto en la posición del iterador de inicio.
  gtk_text_buffer_insert(buffer, &start, suggestion, -1);

  // Obtiene el widget de diálogo que es el ancestro del GtkListBox.
  GtkWidget *dialog = gtk_widget_get_ancestor(GTK_WIDGET(box), GTK_TYPE_WINDOW);

  // Solicita que el diálogo sea redibujado.
  gtk_widget_queue_draw(dialog);

  // Destruye el diálogo.
  gtk_window_destroy(GTK_WINDOW(dialog));
}

void revision(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end, gpointer data)
{

  Hunhandle *hunspell = Hunspell_create("sugerencias.aff", "palabras.dic");

  if (hunspell == NULL)
  {

    g_print("Error .\n");

    return;
  }

  gchar *word = gtk_text_buffer_get_text(buffer, start, end, FALSE);

  GtkWidget *window = GTK_WIDGET(data);

  if (Hunspell_spell(hunspell, word) == 0)
  {

    char **slist;

    int ns = Hunspell_suggest(hunspell, &slist, word);

    if (ns > 0)
    {

      GtkWidget *dialog = gtk_window_new();
      GtkCssProvider *provider;

      // Creamos un nuevo proveedor de CSS
      provider = gtk_css_provider_new();
      // Definir una clase CSS
      gtk_css_provider_load_from_data(provider, ".miClase { background-color: #F0F3BD; font-weight: bold; color: #012a4a; font-family: sans-serif; font-size: 16px; }", -1);

      // Aplicar la clase CSS a la ventana de di?logo
      gtk_widget_add_css_class(dialog, "miClase");

      gtk_style_context_add_provider(gtk_widget_get_style_context(dialog),
                                     GTK_STYLE_PROVIDER(provider),
                                     GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

      gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));

      gtk_window_set_title(GTK_WINDOW(dialog), "Posibles sugerencias...  ");

      gtk_window_set_deletable(GTK_WINDOW(dialog), FALSE);
      gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
      gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

      gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

      GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

      GtkWidget *list_box = gtk_list_box_new();

      g_object_set_data(G_OBJECT(list_box), "buffer", buffer);

      g_object_set_data(G_OBJECT(list_box), "start_offset", GINT_TO_POINTER(gtk_text_iter_get_offset(start)));

      g_object_set_data(G_OBJECT(list_box), "end_offset", GINT_TO_POINTER(gtk_text_iter_get_offset(end)));

      g_signal_connect(list_box, "row-activated", G_CALLBACK(sugerencia), NULL);

      for (int i = 0; i < ns; i++)
      {

        GtkWidget *label = gtk_label_new(slist[i]);

        gtk_list_box_insert(GTK_LIST_BOX(list_box), label, -1);
      }
      gtk_box_append(GTK_BOX(box), list_box);

      GtkWidget *button = gtk_button_new_with_label("IGNORAR SUGERENCIA...");

      g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_destroy), dialog);

      gtk_box_append(GTK_BOX(box), button);

      gtk_window_set_child(GTK_WINDOW(dialog), box);

      gtk_widget_show(dialog);
    }

    Hunspell_free_list(hunspell, &slist, ns);
  }

  if (word != NULL)
  {

    g_free(word);
  }

  Hunspell_destroy(hunspell);
}

void lineaRoja(GtkTextBuffer *buffer, gpointer user_data)
{

  static Hunhandle *hunspell = NULL;

  if (hunspell == NULL)
  {

    hunspell = Hunspell_create("sugerencias.aff", "palabras.dic");

    if (hunspell == NULL)
    {

      g_print("Error al inicializar Hunspell.\n");

      return;
    }
  }

  GtkTextIter start, end;

  gtk_text_buffer_get_start_iter(buffer, &start);

  end = start;

  int error_count = 0;

  while (!gtk_text_iter_is_end(&end))
  {

    if (!gtk_text_iter_inside_word(&end))
    {

      if (!gtk_text_iter_equal(&start, &end))
      {

        gchar *word = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (Hunspell_spell(hunspell, word) == 0)
        {

          if (error_count < 30)
          {

            GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL, "underline", PANGO_UNDERLINE_ERROR, NULL);

            gtk_text_buffer_apply_tag(buffer, tag, &start, &end);

            error_count++; // Incrementa el contador de errores
          }
        }

        g_free(word);
      }

      start = end;
    }

    gtk_text_iter_forward_char(&end);
  }
}

gboolean worter(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data)

{

  if (gtk_gesture_single_get_button(GTK_GESTURE_SINGLE(gesture)) == GDK_BUTTON_PRIMARY)
  {

    GtkTextView *text_view = GTK_TEXT_VIEW(user_data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

    GtkTextIter iter;

    gtk_text_view_get_iter_at_location(text_view, &iter, x, y); // Obtén el iterador en la ubicación del clic

    GtkTextIter start = iter, end = iter;

    gtk_text_iter_backward_word_start(&start); // Encuentra el inicio de la palabra

    gtk_text_iter_forward_word_end(&end); // Encuentra el final de la palabra

    revision(buffer, &start, &end, user_data); // Verifica la palabra en la que se hizo clic

    return TRUE;
  }

  return FALSE;
}