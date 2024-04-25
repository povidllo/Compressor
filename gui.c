#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
// #include "compressor.h"

#if defined(_WIN32) || defined(_WIN64)
    #define OS_NAME "Windows"
#elif defined(__linux__)
    #define OS_NAME "Linux"
#else
    #define OS_NAME "Unknown"
#endif

// gcc -O2 -Wl,-rpath=. -o app gui.c compressor.so `pkg-config --cflags --libs gtk+-3.0` -rdynamic
// gcc -o app gui.c `pkg-config --cflags --libs gtk+-3.0` -rdynamic



char file_name[330];


G_MODULE_EXPORT void file_set(GtkFileChooser *filechsr)
{
    gchar *uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(filechsr));
    if (uri != NULL) {

        gchar *file_name_with_prefix = g_filename_from_uri(uri, NULL, NULL);
        g_free(uri);

        snprintf(file_name, sizeof(char) * 330, "%s", file_name_with_prefix);
        g_free(file_name_with_prefix);

        printf("File selected: %s\n", file_name);
    }
}


// Function to handle the compress button click event
G_MODULE_EXPORT void Compress_btn_clicked(GtkButton *btn, gpointer data)
{
    // Add your logic here to handle compression

    // For example, you can check if a file is selected before compressing
    if (strlen(file_name) > 0)
    {
        char* command = (char*)calloc(1000, sizeof(char));
        if(strcmp(OS_NAME, "Linux") == 0)
        {
            strcat(command, "./compressor ");
            strcat(command, file_name);
            strcat(command, " zip");
            system(command);
            printf("%s\n", command);
            printf("%s\n", command);
        }
        // printf("")
        // Compressor(3, file_name, mode);
        // File is selected, proceed with compression
        // printf("Compressing file: %s\n", file_name);

        // Call your compression function or perform compression logic here
        // For example:
        // compress_file(file_name);

        // After compression, you might want to show a message or update UI accordingly
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_INFO,
                                                    GTK_BUTTONS_OK,
                                                    "File compressed successfully!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
        // No file selected, show error message
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                                    GTK_DIALOG_MODAL,
                                                    GTK_MESSAGE_ERROR,
                                                    GTK_BUTTONS_OK,
                                                    "No file selected for compression!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


G_MODULE_EXPORT void Zip_choose_clicked(GtkButton * btn, gpointer data)  //open zip_menu window
{
    GtkBuilder * ui_builder;
    GError * err = NULL;
    ui_builder = gtk_builder_new();
    if(!gtk_builder_add_from_file(ui_builder, "UI/gui_compressor.glade", &err)) {
        g_critical ("Не вышло загрузить файл с UI : %s", err->message);
        g_error_free (err);
    }
    GtkWidget * window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "Zip_menu"));
    gtk_builder_connect_signals(ui_builder, NULL);
    gtk_widget_show_all(window);
}

int main(int argc, char * argv[]) {

    //Инициализация gtk
    gtk_init(&argc, &argv);

    // Этот объект будет считывать данные из формы
    // и создавать интерфейс на их основе
    GtkBuilder * ui_builder;
    // Если будут какие-либо ошибки, то они появятся здесь
    GError * err = NULL;

    // Инициализируем GtkBuilder
    ui_builder = gtk_builder_new();
    //Загрузка файла с UI в GtkBuilder
    if(!gtk_builder_add_from_file(ui_builder, "UI/gui_compressor.glade", &err)) {
        g_critical ("Не вышло загрузить файл с UI : %s", err->message);
        g_error_free (err);
    }

    //Теперь получаем виджет из Builder
    // Помните мы указывали ID? Вот по нему мы и ищем нужный
    // В данном случае ищем виджет окна
    GtkWidget * window = GTK_WIDGET(gtk_builder_get_object(ui_builder, "Choose_menu"));

    //Таким же образом можно получить и другие виджеты
    // но нам они не понадобятся

    //Подключаем сигналы)
    gtk_builder_connect_signals(ui_builder, NULL);

    // Разрешаем отображение
    gtk_widget_show_all(window);
    //Пошла программа
    gtk_main();


    return 0;
}