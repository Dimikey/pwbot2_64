#include "console.h"

void console_error(const char* text) {
    printf("ERROR: %s Error code: %d\n", text, (int)(GetLastError()));
    getchar();
}

void console_log(const char* text) {
    printf("INFO: %s\n", text);
}

void console_confirm(const char* text) {
    printf("ASK: %s Press any key to continue...\n", text);
    getchar();
}

void console_log_1num(const char* text, int a) {
    printf("INFO: %s: %d\n", text, a);
}

void console_log_1str(const char* text, const char* a) {
    printf("INFO: %s: %s\n", text, a);
}


void console_log_1num_hex(const char* text, LONGLONG a) {
    printf("INFO: %s: 0x%llx\n", text, a);
}

void console_log_2num(const char* text, int a, int b) {
    printf("INFO: %s: %d, %d\n", text, a, b);
}

void console_print_char_stats() {
    //printf("console_print_char_stats() dummy function");
    //printf("HP: %d/%d, MP: %d/%d, Target: 0x%x\n", char_get_hp(), char_get_maxhp(), char_get_mp(), char_get_maxmp(), char_get_target());
    printf("HP: %d, MP: %d, Target: 0x%x, Money: %d, Exp: %d, XYZ: %4.3f %4.3f %4.3f, LVL: %d\n",
           char_get_hp_perc(),
           char_get_mp_perc(),
           char_get_target(),
           char_get_money(), char_get_exp(),
           char_get_pos_x(), char_get_pos_y(), char_get_pos_z(),
           char_get_lvl());
}

int console_get_input() {
    int result;
    printf("Your input: ");
    scanf("%d", &result);
    printf("\n");
    return result;
}
