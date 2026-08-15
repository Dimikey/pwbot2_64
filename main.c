#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <psapi.h>

#include "console.h"
#include "char.h"
#include "utils.h"
#include "address.h"

/**
    BOT NOTE:
    The bot is designed to work under Perfect World 1.6.x x64.

    It's basically a macros that presses buttons, according to mob target value.
    This version of bot heavily relies on TAB button to set targets

*/

#define BOT_FLAG_USE_HEALING_CONSUMABLES    FALSE
#define BOT_FLAG_USE_MANA_CONSUMABLES       FALSE

/* Essential variables */

// PW window handle
HWND pw_hwnd;

// Window name is used to search the game window,
// and then we use it to get the game process
LPCSTR windowName = "PWGlobal.org ¡X Perfect World 1.3.6";


//LPCSTR pw_executable_filename = "ElementClient.exe";//"elementclient_32.exe";
DWORD pid;
int i;
BOOL is_addr_inititalized = FALSE;

/* MobList stuff */
int moblist_filesize;
const char* moblist_filename = "moblist.dat";
int target_list[255];
int target_num;

// PW main module name, aka "elementclient.exe"
//TCHAR szModName[MAX_PATH];

unsigned int stat_total_mobs_killed = 0;
unsigned int stat_total_money_earned = 0;


/*
    Get main module base address of provided process

    This one is usually fixed to 0x00400000,
    but we receive it via WinApi every time just in case

    TODO: this function probably useless, because .exe is loaded
    first every time, and the module base address will ALWAYS be 0x0040000
*/
/*
DWORD GetBaseAddress(const HANDLE hProcess) {
	if (hProcess == NULL)
		return NULL;

	HMODULE lphModule[1024];
	DWORD lpcbNeeded;

	if (!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
		return NULL;


	if (!GetModuleFileNameEx(hProcess, lphModule[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		return NULL;

	return (DWORD)lphModule[0];
}
*/

DWORD_PTR GetBaseAddress(const HANDLE hProcess) {
	if(hProcess == NULL)
		return (DWORD_PTR)NULL;

	HMODULE lphModule[1024];
	DWORD lpcbNeeded;

	if(!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
		return (DWORD_PTR)NULL;

	return (DWORD_PTR)lphModule[0]; // assuming that address of elementclient.exe is always the first
}

void addr_init() {
    /*
       Find Perfect World's window
    */

    // TODO: also add finding by process name, like "elementclient.ext"
    // If window is not found, then search by process name
    pw_hwnd = FindWindow(NULL, windowName);

    if(!pw_hwnd) {
        console_error("Unable to find Perfect World window! FindWindow() error.");
        exit(0);
    }

    /*
       Get permissions to read data (On Vista, 7 and newer requires administrator rights)
    */
    GetWindowThreadProcessId(pw_hwnd, &pid);
    pw_pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if(!pw_pHandle) {
        console_error("OpenProcess() error. Please start as Administrator. ");
        exit(0);
    }
    console_log_1num("PW process id", pid);

    // elementclient.exe + 0xAC2D58 ~= 0x7FF777C72D58; // [[0x7FF67BDB2D58] + 0x68] + and apply offsets like 0x70C  ..............
    DWORD_PTR base_address = GetBaseAddress(pw_pHandle) + offset_base;
    console_log_1num_hex("PW base address", base_address);

    // Read pointer from the base address (static address)
    ReadProcessMemory(pw_pHandle, (LPCVOID)base_address, &address_target, sizeof(address_target), 0);
    //console_log_1num_hex("Read value is ", address_target);
    if(address_target == NULL) {
        console_error("Error reading base address!");
        exit(0);
    }

    // Don't be confused, address_target is also used here to store intermediate value
    address_character_struct = address_target + offset_to_character_struct;
    ReadProcessMemory(pw_pHandle,(void*)address_character_struct, &address_character_struct, sizeof(address_target), 0);

    address_target  = address_character_struct + offset_target;
    address_hp      = address_character_struct + offset_hp;
    address_maxhp   = address_character_struct + offset_maxhp;
    address_mp      = address_character_struct + offset_mp;
    address_maxmp   = address_character_struct + offset_maxmp;
    address_exp     = address_character_struct + offset_exp;
    address_maxexp  = address_character_struct + offset_maxexp;
    address_money   = address_character_struct + offset_money;
    address_lvl     = address_character_struct + offset_lvl;
    address_pos_x   = address_character_struct + offset_pos_x;
    address_pos_y   = address_character_struct + offset_pos_y;
    address_pos_z   = address_character_struct + offset_pos_z;


    // TODO: print some stuff to prove that addresses are correct
    /*
    int target = 0xFFFFFFFF;

    for(i = 0; i < 10; i++) {
        ReadProcessMemory(pw_pHandle,(void*)address_target, &target, sizeof(target), 0);
        printf("%d\n", i);
        console_log_1num_hex("Read target", target);
        console_confirm("press any key to read more...");
    }
    console_confirm("finished.");
    */

    // Applying offsets
    /*
    addr_hp     = addr_character_struc_actual + offset_hp;
    addr_mp     = addr_character_struc_actual + offset_mp;
    addr_maxhp  = addr_character_struc_actual + offset_maxhp;
    addr_maxmp  = addr_character_struc_actual + offset_maxmp;
    addr_target = addr_character_struc_actual + offset_target;
    addr_exp    = addr_character_struc_actual + offset_exp;
    addr_maxexp = addr_character_struc_actual + offset_maxexp;
    */



    is_addr_inititalized = TRUE;
}

void moblist_init() {

    FILE* moblist = fopen(moblist_filename, "a+b");
    if(!moblist) {
        console_error("Error opening moblist!");
        exit(0);
    }

    moblist_filesize = get_filesize(moblist);

    console_log_1str("Moblist filename", moblist_filename);
    console_log_1num("Moblist initial filesize", moblist_filesize);

    // Check whether moblist is empty or not
    if(moblist_filesize > 0) {
        // Moblist is not empty!
        // Check file integrity
        if((moblist_filesize % sizeof(int)) != 0) {
            console_error("Moblist file integrity check failed.");
            exit(0);
        }

        // Each target is 4 byte
        target_num = moblist_filesize / sizeof(int);

        console_log_1num("Mobs in file", target_num);

        for(i = 0; i < target_num; i++) {
            fread(&target_list[i], 4, 1, moblist);
            console_log_1num_hex("Read mob", target_list[i]);
        }

        console_log("Moblist read success");
    }
    else if(moblist_filesize == 0) {
        // Moblist is empty. Let's fill it up
        int prev_target = 0x0;

        // Let's store 5 mobs for now
        for(target_num = 0; target_num < 5; target_num++) {

            console_log_1num("Awaiting target", target_num + 1);

            // Don't let empty targets (0x0) go through
            while(char_get_target() == 0x0) {
                Sleep(500);
            }

            // Don't accept the same target again
            // Skip for the first time
            if(target_num != 0x0) {
                while(char_get_target() == prev_target) {
                    Sleep(500);
                }
            }

            target_list[target_num] = char_get_target();
            prev_target = target_list[target_num];

            console_log_1num_hex("Added target", target_list[target_num]);
        }

        console_log_1str("Done! Now writing targets into file", moblist_filename);
        // (Optional step) Write to file
        fwrite(target_list, sizeof(int), target_num, moblist);

    }
    else {
        console_error("Strange moblist filesize! Ny nnnaher...");
        exit(0);
    }
    fclose(moblist);
}


void bot_start() {
    console_confirm("Bot is ready to start!");

    is_bot_active = TRUE;

    while(is_bot_active) {
        Sleep(100);

        // Attempting to set target
        if(char_get_target() == 0x0)
            char_set_target(0xFFFFFFFF);

        // If target is still 0x0, then mob is out of range
        // or there's nothing to select
        if(char_get_target() == 0x0) {
            console_log("Target set failed. No mobs nearby?..");
            continue;
        }

        // Kill the target, while it's alive (i.e. is not null after being selected)
        while(char_get_target() != 0x0) {

            console_log_1num_hex("I attack", char_get_target());
            char_attack();

            Sleep(COMMON_DELAY);
        }

        stat_total_mobs_killed++;
        console_log_1num("Mob killed. Total count", stat_total_mobs_killed);
        char_collect_loot();
        console_print_char_stats();
    }
}

void misc_start() {
    asm volatile (" movl %eax,%eax ");
    asm volatile (" xor %eax,%eax ");
    /*
    asm volatile (" movl %rax,%rax ");
    asm volatile (" xor %rax,%rax ");
    */
    console_log("Misc done");

}

void misc_character_info() {
    while(TRUE) {
        console_print_char_stats();
        Sleep(1000);
    }
}


int main() {

    addr_init();

    //moblist_init();

    if(is_addr_inititalized == FALSE)
        console_log("WARNING: addressed are NOT initialized, some functions will NOT work.");

    console_log("Select which one to start");
    console_log("1. Bot");
    console_log("2. Misc functions");
    console_log("3. Print character info");
    console_log("9. Exit");

    int choice = 0;
    while(choice != 9) {
        choice = console_get_input();
        switch(choice) {
            case 1:
                bot_start();
                printf("Total kills: %d\n", stat_total_mobs_killed);
                console_confirm("Bot finished!");
                break;
            case 2:
                misc_start();
                break;
            case 3:
                misc_character_info();
                break;

            default:
                break;
        }
    }

    return 0;
}
