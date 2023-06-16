#include <stdio.h>

    #define ANSI_RESET       "\x1b[0m"    

    #define ANSI_BLACK      "\e[30m" 
    #define ANSI_RED        "\e[31m"
    #define ANSI_GREEN      "\e[32m"
    #define ANSI_YELLOW     "\e[33m"
    #define ANSI_BLUE       "\e[34m"
    #define ANSI_MAGENTA    "\e[35m"
    #define ANSI_CYAN       "\e[36m"
    #define ANSI_WHITE      "\e[37m"

    #define ANSI_BRIGHT_BLACK       "\e[1;30m"
    #define ANSI_BRIGHT_RED         "\e[1;31m"
    #define ANSI_BRIGHT_GREEN       "\e[1;32m"
    #define ANSI_BRIGHT_YELLOW      "\e[1;33m"
    #define ANSI_BRIGHT_BLUE        "\e[1;34m"
    #define ANSI_BRIGHT_MAGENTA     "\e[1;35m"
    #define ANSI_BRIGHT_CYAN        "\e[1;36m"
    #define ANSI_BRIGHT_WHITE       "\e[1;37m"

    #define ANSI_BACKGROUND_BLACK       "\e[40m"
    #define ANSI_BACKGROUND_RED         "\e[41m"
    #define ANSI_BACKGROUND_GREEN       "\e[42m"
    #define ANSI_BACKGROUND_YELLOW      "\e[43m"
    #define ANSI_BACKGROUND_BLUE        "\e[44m"
    #define ANSI_BACKGROUND_MAGENTA     "\e[45m"
    #define ANSI_BACKGROUND_CYAN        "\e[46m"
    #define ANSI_BACKGROUND_WHITE       "\e[47m"

    #define ANSI_UNDERLINE_BLACK    "\e[4;30m"        
    #define ANSI_UNDERLINE_RED      "\e[4;31m"
    #define ANSI_UNDERLINE_GREEN    "\e[4;32m"
    #define ANSI_UNDERLINE_YELLOW   "\e[4;33m"
    #define ANSI_UNDERLINE_BLUE     "\e[4;34m"
    #define ANSI_UNDERLINE_MAGENTA  "\e[4;35m"
    #define ANSI_UNDERLINE_CYAN     "\e[4;36m"
    #define ANSI_UNDERLINE_WHITE    "\e[4;37m"

/* usage
   printf(ANSI_BLACK   "%s Black\n"    ANSI_RESET, sFarben);   // Black    / Schwarz
    printf(ANSI_RED     "%s Red\n"      ANSI_RESET, sFarben);   // Red      / Rot
    printf(ANSI_GREEN   "%s Green\n"    ANSI_RESET, sFarben);   // Green    / Grün
    printf(ANSI_YELLOW  "%s Yellow\n"   ANSI_RESET, sFarben);   // Yellow   / Gelb
    printf(ANSI_BLUE    "%s Blue\n"     ANSI_RESET, sFarben);   // Blue     / Blau
    printf(ANSI_MAGENTA "%s Magenta\n"  ANSI_RESET, sFarben);   // Magenta  / Magenta
    printf(ANSI_CYAN    "%s Cyan\n"     ANSI_RESET, sFarben);   // Cyan     / Cyan
    printf(ANSI_WHITE   "%s White\n"    ANSI_RESET, sFarben);   // White    / Weiss
*/