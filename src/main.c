#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define MAX_LINE_LEN 2048
#define MAPPING_COUNT 64 // ajuste conforme necessário

typedef struct
{
    const char *pawn;
    const char *sussy;
} Mapping;

Mapping mappings[MAPPING_COUNT] = {
    {"new", "crewmate"},
    {"static", "impostor"},
    {"if", "suscheck"},
    {"else", "not_sus"},
    {"while", "vent_loop"},
    {"for", "scan_loop"},
    {"printf", "emergency_meeting"},
    {"return", "report_body"},
    {"public", "TASK"},

    // Eventos
    {"OnGameModeInit", "OnShipModeInit"},
    {"OnGameModeExit", "OnShipModeExit"},
    {"OnPlayerConnect", "OnCrewmateConnect"},
    {"OnPlayerDisconnect", "OnCrewmateDisconnect"},
    {"OnPlayerSpawn", "OnCrewmateSpawn"},
    {"OnPlayerDeath", "OnCrewmateDeath"},
    {"OnPlayerRequestClass", "OnCrewmateRequestClass"},
    {"OnPlayerTakeDamage", "OnCrewmateTakeDamage"},
    {"OnPlayerUpdate", "OnCrewmateUpdate"},
    {"OnPlayerCommandText", "OnCrewmateCommandText"},
    {"OnPlayerText", "OnCrewmateText"},
    {"OnPlayerKeyStateChange", "OnCrewmateKeyStateChange"},
    {"OnPlayerEnterVehicle", "OnCrewmateEnterVehicle"},
    {"OnPlayerExitVehicle", "OnCrewmateExitVehicle"},
    {"OnPlayerStateChange", "OnCrewmateStateChange"},
    {"OnPlayerPickUpPickup", "OnCrewmatePickUpPickup"},
    {"OnPlayerClickPlayer", "OnCrewmateClickPlayer"},
    {"OnPlayerClickTextDraw", "OnCrewmateClickTextDraw"},

    // Macros de função
    {"SetPlayerPos", "SetCrewmatePos"},
    {"GetPlayerHealth", "GetCrewmateHealth"},
    {"SetPlayerHealth", "SetCrewmateHealth"},
    {"SetPlayerFacingAngle", "SetCrewmateAngle"},
    {"SetPlayerWorldBounds", "SetCrewmateBounds"},
    {"SetCameraBehindPlayer", "FollowCrewmateWithCamera"},
    {"SetPlayerCameraPos", "SetCrewmateCameraPos"},
    {"SetPlayerCameraLookAt", "SetCrewmateCameraTarget"},
    {"PlayerSpectatePlayer", "MakeCrewmateSpectate"},
    {"TogglePlayerSpectating", "ToggleCrewmateSpectating"},
    {"GivePlayerWeapon", "GiveCrewmateWeapon"},
    {"ResetPlayerWeapons", "ResetCrewmateWeapons"},
    {"GetPlayerWeapon", "GetCrewmateWeapon"},
    {"SetPlayerArmedWeapon", "SetCrewmateArmed"},
    {"GetPlayerKeys", "GetCrewmateKeys"},
    {"GetPlayerScore", "GetCrewmateScore"},
    {"SetPlayerScore", "SetCrewmateScore"},
    {"SetPlayerColor", "SetCrewmateColor"},
    {"IsPlayerConnected", "IsCrewmateConnected"},
    {"SpawnPlayer", "SpawnCrewmate"},
    {"GetPlayerName", "GetCrewmateName"},
    {"SendClientMessageToAll", "SendMessageToAllCrewmates"},
    {"SendClientMessage", "SendMessageToCrewmate"},
    {"GameTextForPlayer", "ShowTextToCrewmate"},
    {"GameTextForAll", "ShowTextToAllCrewmates"},
    {"SendDeathMessage", "ReportDeadBody"}};

FILE *log_file = NULL;
bool debug_mode = false;
int replace_count = 0;
int max_debug_logs = 1000; // limite para evitar flood

void log_msg(const char *fmt, ...)
{
    if (!debug_mode || !log_file)
        return;

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
    fflush(log_file);
    va_end(args);
}

void replace_all(char *line, const char *old, const char *new)
{
    if (!line || !old || !new)
    {
        log_msg("[ERROR] Invalid input to replace_all");
        return;
    }

    char buffer[MAX_LINE_LEN];
    char *pos;
    while ((pos = strstr(line, old)) != NULL)
    {
        if (++replace_count <= max_debug_logs)
            log_msg("[DEBUG] Replace #%d: '%s' -> '%s'", replace_count, old, new);

        size_t prefix_len = pos - line;
        size_t old_len = strlen(old);
        size_t new_len = strlen(new);

        if (prefix_len > MAX_LINE_LEN - 1)
            prefix_len = MAX_LINE_LEN - 1;

        // Copia prefixo
        memcpy(buffer, line, prefix_len);

        // Copia nova string
        if (prefix_len + new_len > MAX_LINE_LEN - 1)
        {
            log_msg("[ERROR] Replacement too long, aborting.");
            exit(1);
        }
        memcpy(buffer + prefix_len, new, new_len);

        // Copia sufixo
        size_t suffix_len = strlen(pos + old_len);
        if (prefix_len + new_len + suffix_len > MAX_LINE_LEN - 1)
            suffix_len = MAX_LINE_LEN - 1 - prefix_len - new_len;

        memcpy(buffer + prefix_len + new_len, pos + old_len, suffix_len);
        buffer[prefix_len + new_len + suffix_len] = '\0';

        strcpy(line, buffer);
    }
}

int main(int argc, char *argv[])
{
    const char *input_filename = "script.pwn";
    const char *output_filename = "script.sus";

    // Processar argumentos
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--in") == 0 && i + 1 < argc)
        {
            input_filename = argv[++i];
        }
        else if (strcmp(argv[i], "--out") == 0 && i + 1 < argc)
        {
            output_filename = argv[++i];
        }
        else if (strcmp(argv[i], "--debug") == 0)
        {
            debug_mode = true;
        }
    }

    if (debug_mode)
    {
        log_file = fopen("log.txt", "w");
        if (!log_file)
        {
            fprintf(stderr, "Erro ao abrir arquivo de log.\n");
            return 1;
        }
        log_msg("[INFO] Modo debug ativado.");
        log_msg("[INFO] Arquivo de entrada: %s", input_filename);
        log_msg("[INFO] Arquivo de saída: %s", output_filename);
    }

    FILE *input = fopen(input_filename, "r");
    if (!input)
    {
        fprintf(stderr, "Erro ao abrir arquivo de entrada: %s\n", input_filename);
        if (log_file)
            fclose(log_file);
        return 1;
    }
    FILE *output = fopen(output_filename, "w");
    if (!output)
    {
        fprintf(stderr, "Erro ao abrir arquivo de saída: %s\n", output_filename);
        fclose(input);
        if (log_file)
            fclose(log_file);
        return 1;
    }

    char line[MAX_LINE_LEN];
    int line_num = 0;
    while (fgets(line, sizeof(line), input))
    {
        line_num++;
        if (debug_mode)
            log_msg("[DEBUG] Processando linha %d: %s", line_num, line);

        for (int i = 0; i < MAPPING_COUNT; i++)
        {
            replace_all(line, mappings[i].pawn, mappings[i].sussy);
        }

        fputs(line, output);
    }

    fclose(input);
    fclose(output);

    if (debug_mode)
    {
        log_msg("[INFO] Conversão concluída. Arquivo salvo como '%s'", output_filename);
        fclose(log_file);
    }
    else
    {
        printf("Conversão concluída. Arquivo salvo como '%s'\n", output_filename);
    }

    return 0;
}
