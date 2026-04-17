#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_LINE_LEN 2048
#define MAPPING_COUNT 2048 // ajuste conforme necessário

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
    {"SendDeathMessage", "ReportDeadBody"}
};

FILE *log_file = NULL;
bool debug_mode = false;
int replace_count = 0;
int max_debug_logs = 1000; // limite para evitar flood

// ================ Lua Hook functions to be called from Lua scripts ================

int lua_print(lua_State *L)
{
    const char *msg = luaL_checkstring(L, 1);
    printf("[LUA]: %s\n", msg);
    return 0; // número de valores retornados para o Lua
}

int lua_replace_mapping(lua_State *L)
{
    const size_t memAddress = luaL_checkinteger(L, 1);
    const char *arg1 = luaL_checkstring(L, 2);
    const char *arg2 = luaL_checkstring(L, 3);

    if(memAddress >= MAPPING_COUNT)
    {
        lua_pushstring(L, "Invalid mapping index.");
        return 1; // número de valores retornados para o Lua
    }

    if(mappings[memAddress].pawn == NULL || mappings[memAddress].sussy == NULL)
    {
        lua_pushstring(L, "Mapping index is empty. Use InjectMapping instead.");
        return 1; // número de valores retornados para o Lua
    }

    mappings[memAddress].pawn = arg1;
    mappings[memAddress].sussy = arg2;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Mapping at index %zu replaced: '%s' -> '%s'", memAddress, arg1, arg2);

    lua_pushstring(L, buffer); // empurrar a mensagem de volta para o Lua
    return 1; // número de valores retornados para o Lua
}

int lua_get_mapping_count(lua_State *L)
{
    lua_pushinteger(L, MAPPING_COUNT);
    return 1; // número de valores retornados para o Lua
}

int lua_get_mapping(lua_State *L)
{
    const size_t memAddress = luaL_checkinteger(L, 1);
    if (memAddress >= MAPPING_COUNT)
    {
        lua_pushnil(L);
        lua_pushstring(L, "Invalid mapping index.");
        return 2; // número de valores retornados para o Lua
    }

    lua_pushstring(L, mappings[memAddress].pawn);
    lua_pushstring(L, mappings[memAddress].sussy);
    return 2; // número de valores retornados para o Lua
}

int lua_get_all_mappings(lua_State *L)
{
    lua_newtable(L); // criar uma nova tabela

    for (int i = 0; i < MAPPING_COUNT; i++)
    {
        if(mappings[i].pawn == NULL || mappings[i].sussy == NULL) continue;
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "0x%X", i);

        lua_pushinteger(L, i + 1); // índice da tabela começa em 1 no Lua
        lua_newtable(L);           // criar uma sub-tabela para cada mapeamento

        lua_pushstring(L, "address");
        lua_pushstring(L, buffer);
        lua_settable(L, -3); // definir campo 'address' na sub-tabela

        lua_pushstring(L, "pawn");
        lua_pushstring(L, mappings[i].pawn);
        lua_settable(L, -3); // definir campo 'pawn' na sub-tabela

        lua_pushstring(L, "sussy");
        lua_pushstring(L, mappings[i].sussy);
        lua_settable(L, -3); // definir campo 'sussy' na sub-tabela

        lua_settable(L, -3); // definir a sub-tabela na tabela principal
    }

    return 1; // número de valores retornados para o Lua
}

int lua_write_mappings_to_file(lua_State *L)
{
    const char *filename = luaL_checkstring(L, 1);
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        lua_pushboolean(L, 0);
        return 1; // número de valores retornados para o Lua
    }

    for (int i = 0; i < MAPPING_COUNT; i++)
    {
        if(mappings[i].pawn == NULL || mappings[i].sussy == NULL) continue;
        fprintf(file, "[0x%X] - '%s' -> '%s'\n", i, mappings[i].pawn, mappings[i].sussy);
    }

    fclose(file);
    lua_pushboolean(L, 1);
    return 1; // número de valores retornados para o Lua
}

int lua_inject_mapping(lua_State *L)
{
    const size_t memAddress = luaL_checkinteger(L, 1);
    const char *pawn = luaL_checkstring(L, 2);
    const char *sussy = luaL_checkstring(L, 3);

    if(memAddress >= MAPPING_COUNT)
    {
        lua_pushstring(L, "Invalid mapping index.");
        return 1; // número de valores retornados para o Lua
    }

    if(mappings[memAddress].pawn != NULL || mappings[memAddress].sussy != NULL)
    {
        lua_pushstring(L, "Mapping index already in use.");
        return 1; // número de valores retornados para o Lua
    }

    mappings[memAddress].pawn = pawn;
    mappings[memAddress].sussy = sussy;
    lua_pushstring(L, "Mapping injected successfully.");
    return 1; // número de valores retornados para o Lua

}

int lua_clear_mapping(lua_State *L)
{
    const size_t memAddress = luaL_checkinteger(L, 1);

    if (memAddress >= MAPPING_COUNT)
    {
        lua_pushstring(L, "Invalid mapping index.");
        return 1; // número de valores retornados para o Lua
    }

    mappings[memAddress].pawn = NULL;
    mappings[memAddress].sussy = NULL;
    lua_pushstring(L, "Mapping cleared successfully.");
    return 1; // número de valores retornados para o Lua
}

int lua_reset_mappings(lua_State *L)
{
    for (int i = 0; i < MAPPING_COUNT; i++)
    {
        mappings[i].pawn = NULL;
        mappings[i].sussy = NULL;
    }
    lua_pushstring(L, "All mappings reset successfully.");
    return 1; // número de valores retornados para o Lua
}

int lua_pushline(lua_State *L)
{
    const char *line = luaL_checkstring(L, 1);
    printf("[LUA LINE]: %s\n", line);
    return 0; // número de valores retornados para o Lua
}

// ================ end Lua Hook functions ================

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

void StartLua()
{
    log_msg("[INFO] Iniciando ambiente Lua...");

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Registrar funções C para Lua
    
    lua_register(L, "print", lua_print);
    lua_register(L, "ReplaceMapping", lua_replace_mapping);
    lua_register(L, "GetMappingCount", lua_get_mapping_count);
    lua_register(L, "GetMapping", lua_get_mapping);
    lua_register(L, "GetAllMappings", lua_get_all_mappings);
    lua_register(L, "WriteMappingsToFile", lua_write_mappings_to_file);
    lua_register(L, "InjectMapping", lua_inject_mapping);
    lua_register(L, "ClearMapping", lua_clear_mapping);
    lua_register(L, "ResetMappings", lua_reset_mappings);
    lua_register(L, "pushline", lua_pushline);

    // Carregar e executar script Lua
    if(luaL_dofile(L, "main.lua"))
    {
        fprintf(stderr, "Erro ao carregar script Lua: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // remover mensagem de erro da pilha
    }

    lua_close(L);
}

int is_word_boundary(char c)
{
    return !isalnum(c) && c != '_';
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
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);

    while ((pos = strstr(line, old)) != NULL)
    {
        // Verificar word boundaries
        int before_ok = (pos == line) || is_word_boundary(*(pos - 1));
        int after_ok = (*(pos + old_len) == '\0') || is_word_boundary(*(pos + old_len));

        if (!before_ok || !after_ok)
        {
            // Não substitui, procura próxima ocorrência
            pos[old_len] = '\0';
            char *temp = (char *)malloc(MAX_LINE_LEN);
            strcpy(temp, line);
            strcat(temp, old);
            pos = strstr(line, old);
            if (pos)
                pos += old_len;
            else
                break;
            free(temp);
            continue;
        }

        if (++replace_count <= max_debug_logs)
            log_msg("[DEBUG] Replace #%d: '%s' -> '%s'", replace_count, old, new);

        size_t prefix_len = pos - line;

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

    StartLua();

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