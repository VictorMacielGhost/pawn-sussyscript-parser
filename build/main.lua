--- This file is responsable to modifiying the lexical analyzer and parser to work with the SussyScript language.
--- It changes the keywords and some functions to match the SussyScript syntax and semantics.

--- Functions

--- print: Prints a message from Lua to the parser's console.
    --- Parameters: string msg - The message to print.
    --- Returns: None
--- function print(msg) end;

--- ReplaceMapping: Replaces a mapping in the parser's mapping table.
    --- Parameters: integer memAddress - The memory address of the mapping to replace.
    ---             string arg1 - The new Pawn representation.
    ---             string arg2 - The new SussyScript representation.
    --- Returns: string - A message indicating the result of the operation.
--- function ReplaceMapping(memAddress, arg1, arg2) end;

--- GetMappingCount: Retrieves the total number of mappings in the parser's mapping table.
    --- Returns: integer - The total count of mappings.
--- function GetMappingCount() end;

--- GetMapping: Retrieves a specific mapping from the parser's mapping table.
    --- Parameters: integer memAddress - The memory address of the mapping to retrieve.
    --- Returns: table - A table containing the Pawn and SussyScript representations of the mapping.
--- function GetMapping(memAddress) end;

--- GetAllMappings: Retrieves all mappings from the parser's mapping table.
    --- Returns: table - A table containing all mappings with their Pawn and SussyScript representations.
--- function GetAllMappings() end;
--- WriteMappingsToFile: Writes all mappings to a specified file.
    --- Parameters: string filePath - The path to the file where mappings will be written.
    --- Returns: boolean - True if the operation was successful, false otherwise.
--- function WriteMappingsToFile(filePath) end;

--- InjectMapping: Injects a new mapping into the parser's mapping table.
    --- Parameters: integer address - The memory address for the new mapping.
    ---             string pawnRep - The Pawn representation of the new mapping.
    ---             string SussyRep - The SussyScript representation of the new mapping.
    --- Returns: None
--- function InjectMapping(address, pawnRep, SussyRep) end;

--- ClearMapping: Clear all mapping from the address.
    --- Parameters:
    --- Returns: None
--- function ClearMappings(address) end;

--- ResetMappings: Resets all mappings to NULL values.
    --- Parameters: None
    --- Returns: None
--- function ResetMappings() end;

--- Mappings
    --- memAddress: The memory address of the mapping.
    --- 0x0 - 0x8 - Pawn Keywords
    --- 0x9 - 0x1A - Pawn/Samp Events
    --- 0x1B - 0x34 - Pawn/Samp Functions

-- Main script initialization
print("SussyScript Parser initialized with Lua bindings")

-- Display all mappings to log
local mappings = GetAllMappings()
local count = 0
for _ in pairs(mappings) do
    count = count + 1
end
print("Total mappings loaded: " .. count)