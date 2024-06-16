#include <stdio.h>

int main() {
    const char *SCRIPTED_EFFECT_FILE = "/home/dm/.var/app/com.valvesoftware.Steam/.local/share/Paradox Interactive/Imperator/mod/TotalWarBattle/events/00000_battle_test.txt";
    const char *SCRIPTED_EFFECT = "namespace = battle_test\nbattle_test.2 = { type = country_event hidden = yes immediate = { add_treasury = 999 } }";

    FILE *file = fopen(SCRIPTED_EFFECT_FILE, "w");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    fprintf(file, "%s", SCRIPTED_EFFECT);

    fclose(file);
    return 0;
}
