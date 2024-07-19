/*
 *	Licencia Apache, Version 2.0 con Modificacion
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentacion asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificacion, fusion, publicacion, distribucion, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluiran en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTiA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLiCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTiAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPoSITO PARTICULAR Y NO INFRACCIoN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERaN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAnO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIoN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIoN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Ademas, cualquier modificacion realizada por terceros se considerara propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explicitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones especificas de codigo 
 *	modificado.
 */

#ifndef __DEBUG_C__
#define __DEBUG_C__
#include "debug_c.h"

#ifdef _WIN32
#ifndef _ExceptionHandler_WIN_
#define _ExceptionHandler_WIN_
LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS *ExceptionInfo) {
    printf("Se ha producido una excepcion (codigo %lx) en la direccion %p\n",
        ExceptionInfo->ExceptionRecord->ExceptionCode,
        ExceptionInfo->ExceptionRecord->ExceptionAddress);
    return EXCEPTION_EXECUTE_HANDLER; // Manejar la excepcion
}
#endif
#endif

void __attribute__((constructor)) __constructor_debug_c__(){
    //debug_set_log_file("debug_log.txt");
    //open_file(&Log_debug_file, NAME_DEFAULT_LOG_DEBUG, READ_WRITE );
    #ifdef _WIN32
    #ifndef _ExceptionHandler_WIN_
    SetUnhandledExceptionFilter(ExceptionHandler);
    #endif
    #endif
    DEBUG_PRINT(DEBUG_LEVEL_INFO, "#{FG:white}[#{FG:red}DEBUG INIT#{FG:white}]");
    
    /*if (Log_debug_file.archivo == OPEN_ERROR) {
        printf("Error al abrir el archivo " NAME_DEFAULT_LOG_DEBUG "\n");
        return 1;
    }*/

}
void __attribute__((destructor)) __destructor_debug_c__(){
    /*if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }*/
    /*close_file(&Log_debug_file);*/
}

const char* get_level_debug(DebugLevel level) {
    switch(level){

    case DEBUG_LEVEL_INFO:
        return ErrorLevelStrings[0];
        break;
    case DEBUG_LEVEL_WARNING:
        return ErrorLevelStrings[1];
        break;
    case DEBUG_LEVEL_ERROR:
        return ErrorLevelStrings[2];
        break;
    default:
        return "UNKNOW";
        break;
    };
}  

/*void debug_set_log_file(const char *filename)
{
    if (logFile != NULL)
    {
        fclose(logFile);
        logFile = NULL;
    }

    logFile = fopen(filename, "w");
}*/

void debug_set_level(DebugLevel level)
{
    currentLevel = level;
}

void debug_print(DebugLevel level, const char *fmt, ...)
{
    if (fmt == (const char *)NULL) return;
    if (level < currentLevel)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = (vsnprintf(NULL, 0, fmt, args_copy) + 1) * sizeof(char);
    va_end(args_copy);
    va_copy(args_copy, args);

    vprintf_color( fmt, args_copy);
    va_end(args_copy);

    /*if (logFile != NULL)
    {
        vfprintf(logFile, fmt, args);
        fprintf(logFile, "\n");
    }*/

    /*unsigned char *formatted_buffer = (unsigned char *)malloc(size);
    vsprintf(formatted_buffer, fmt, args);

    write_file(&Log_debug_file, formatted_buffer);
    if(Log_debug_file.size == WRITE_ERROR){
        printf("Error al escribir el archivo\n");
    }*/

    va_end(args);

    if (level == DEBUG_LEVEL_ERROR)
    {
        va_start(args, fmt);
        char *message = malloc(size * sizeof(char));
        vsnprintf(message, size, fmt, args);
        va_end(args);
        
        ERROR_C(ERROR_LEVEL_ERROR, message);
    }
}

char *get_addr_to_encoder_x86(uint64_t addr, uint8_t encoder_val) {
    /*
     *  
     * get_addr_to_encoder_x86(uint64_t addr, encoder_x86 encoder_val):
     * Esta funcion devuelve un string con la direccion de memoria formateada en 16, 32 o 64bits
     * 
     * Se espera recibir addr el cual es la direccion de memoria a formatear,
     * se espera recibir encoder_val, el cual indica si formatear la direcion a 16, 32 o 64bits.
     *      Posibles valores para encoder_x86 encoder_val:
     *          - ENCODER_IN_16bits = 0 : para 16bits
     *          - ENCODER_IN_32bits = 1 : para 32bits
     *          - ENCODER_IN_64bits = 2 : para 64bits
     * 
     * En caso de que malloc devuelva error, la funcion devuelve NULL
     *  
     */
    size_t size;
    char *buffer_Position_memory;
    switch (encoder_val) {
        case 0:
            size = (snprintf(NULL, 0, "%04x", (uint16_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%04x", (uint16_t)addr);
            break;
        case 1:
            size = (snprintf(NULL, 0, "%08x", (uint32_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%08x", (uint32_t)addr);
            break;
        case 2:
            size = (snprintf(NULL, 0, "%016llx", (uint64_t)addr) + 1) * sizeof(char);
            buffer_Position_memory = (char *)malloc(size);
            if (buffer_Position_memory == NULL) return NULL;
            sprintf(buffer_Position_memory, "%016llx", (uint64_t)addr);
            break;
        default: return NULL; // error
    }
    return buffer_Position_memory;
}


void print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, uint8_t encoder_val) {
    /*
     *  
     * print_table_hex(char *string_init, char *string_text_for_printing, size_t size_string_text_for_printing, encoder_x86 encoder_val):
     * Esta funcion imprime un "string_text_for_printing" en forma de tabla hexadecimal de tamaño de X * 0xf(16 en decimal).
     * Donde X es size_string_text_for_printing / 16. 
     * 
     * Se espera recibir un string_init, el cual es un string que se imprimira al inicio de cada fila.
     * Se espera recibir un string_text_for_printing el cual sea la cadena o secuencia de bytes a imprimir en formao hexadecimal
     * Se espera recibir un size_string_text_for_printing el cual sea el tamaño del string  o secuencia de bytes a imprimir.
     * Se espera a recibir un encoder_val el cual indica el tamaño de las direcciones de memoria a imprimir:
     *      Posibles valores para encoder_x86 encoder_val:
     *          - ENCODER_IN_16bits = 0 : para 16bits
     *          - ENCODER_IN_32bits = 1 : para 32bits
     *          - ENCODER_IN_64bits = 2 : para 64bits
     * 
     * En caso de que string_init no sea un puntero valido o NULL se usara una cadena vacia por defecto.
     * En caso de que string_text_for_printing no sea un puntero valido o NULL, la funcion no imprimira nada.
     * En caso de que size_string_text_for_printing sea 0 no se imprimira nada.
     *  
     */
    char my_string_default[] = "";
    if ((string_text_for_printing == NULL) || (size_string_text_for_printing == 0)) return; // error
    if (string_init == NULL) string_init = my_string_default; // no se ingreso un valor de incio que usar para imprimir cada fila

    size_t size = 0;
    char *buffer_Position_memory = get_addr_to_encoder_x86(0, encoder_val);
    uint32_t level_space = strlen(buffer_Position_memory);

    
    printf_color("\n%s ", string_init);
    for (uint32_t i = 0; i < level_space; i++) putchar(' ');
    #ifndef __DISABLE_COLORS_FORE_BACK_GROUND__ 
        unsigned int random_color = jenkins_hash(string_text_for_printing[0], level_space, 0, 1, 2, 3, 4);
        // imprimir primera fila. (empezamos desde 0x23 para tener unos colores agradables)
        for (uint16_t r = 0x23; r < 0x33; r++) printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%02x#{FG:reset}", r >> 2, ((uint8_t)(r-0x33)) - 0xf0 ); 
        printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"%s #{FG:reset}", string_init, ((((uint8_t)random_color >> 2)) & 0b1110111) | 0b00001001, buffer_Position_memory);
    #else
        for (uint16_t r = 0x23; r < 0x33; r++) printf_color("|%02x#{FG:reset}", r >> 2); 
        printf_color("|\n%s%s #{FG:reset}", string_init, buffer_Position_memory);
    #endif
    free(buffer_Position_memory);
    for (uint32_t i = 0; i < size_string_text_for_printing; i++)
    {
        #ifndef __DISABLE_COLORS_FORE_BACK_GROUND__
            // imprimir el resto de filas
            unsigned int Avalue1, Avalue2, Avalue3, seed, values[] = {
                (unsigned int)size_string_text_for_printing, 
                size, level_space,  0xa0, 0xe1, string_text_for_printing[i]
            };
            // generar 3 valores apartir de una semilla de entrada y 6 valores(pueden ser constantes o variables)
            seed = (unsigned int)(string_text_for_printing[i]);
            Avalue1 = jenkins_hash(seed,    values[0], values[1], values[2], values[3], values[4], values[5]);
            Avalue2 = jenkins_hash(Avalue1, values[0], values[1], values[2], values[3], values[4], values[5]);
            Avalue3 = jenkins_hash(Avalue2, values[0], values[1], values[2], values[3], values[4], values[5]);
            // mediante la operacion ((((uint8_t)valor >> 2)) & 0b1110111) | 0b00001001
            // de puede obtener colores claros
            
                printf_color("|"FOREGROUND_COLOR_CUSTOM("%d")"%.2X#{BG:reset}", 
                    ((((uint8_t)string_text_for_printing[i] >> 2)) & 0b1110111) | 0b00001001, 
                    (uint8_t)string_text_for_printing[i]);
        #else
            printf_color("|%.2X", 
                (uint8_t)string_text_for_printing[i]);
        #endif
        // imrpimir en X * 16
        if ((i+1) % ((256*4)/8 / 8) == 0){
            // imprimir la siguiente filas, despues de imprimir 16 bytes
            buffer_Position_memory = get_addr_to_encoder_x86(i+1, encoder_val);
            #ifndef __DISABLE_COLORS_FORE_BACK_GROUND__ 
                random_color = jenkins_hash(string_text_for_printing[i], Avalue1, Avalue2, Avalue3, 2, 3, 4);
                printf_color("|\n%s"FOREGROUND_COLOR_CUSTOM("%d")"%s #{FG:reset}", string_init, ((((uint8_t)random_color >> 2)) & 0b1111111) | 0b00001001, buffer_Position_memory, i+1);
            #else
                printf_color("|\n%s%s #{FG:reset}", string_init, buffer_Position_memory, i+1);
            #endif
            free(buffer_Position_memory);
        }
    }
    printf("|\n");
}

#endif