# Pawn SussyScript Parser

This project is a simple command-line tool written in C that converts Pawn source code into its "SussyScript" variant by replacing predefined keywords and function names. It helps developers switch between normal Pawn syntax and the SussyScript syntax easily.

---

## Project Structure

```

C:.
├── .vscode
│   └── tasks.json           # VSCode build task configuration
├── build
│   └── pawn-sussy-parser.exe # Compiled executable of the parser
├── scripts
│   ├── pawn.pwn             # Example Pawn source file (input)
│   └── sussy.pwn            # Example output file in SussyScript syntax
└── src
└── main.c               # Source code of the parser tool

````

---

## What is SussyScript?

SussyScript is a playful macro-based variant of Pawn that replaces common keywords, function names, and event names with alternate "sussy" terms. For example:

- `new` → `crewmate`
- `static` → `impostor`
- `if` → `suscheck`
- `else` → `not_sus`
- Event `OnPlayerConnect` → `OnCrewmateConnect`

This parser automates the conversion between normal Pawn code and SussyScript syntax.

---

## How to Build

Use your preferred C compiler (e.g., `gcc` or `clang`) to compile the source:

```bash
gcc -o build/pawn-sussy-parser.exe src/main.c
````

Or use the provided VSCode task (`.vscode/tasks.json`) to build easily.

---

## Usage

Run the parser from the command line:

```bash
pawn-sussy-parser.exe --in path/to/input.pwn --out path/to/output.sus [--debug]
```

### Arguments:

* `--in`
  Specify the input Pawn source file. Default: `script.pwn`

* `--out`
  Specify the output SussyScript file. Default: `script.sus`

* `--debug`
  Enable debug mode, which creates a `log.txt` file recording all replacements and processing steps.

---

## Example

Convert `scripts/pawn.pwn` into SussyScript with debug log:

```bash
pawn-sussy-parser.exe --in scripts/pawn.pwn --out scripts/sussy.pwn --debug
```

The output will be saved as `scripts/sussy.pwn`, and detailed logs will be in `log.txt`.

---

## Notes

* The parser performs simple textual replacements and may not handle complex syntax nuances.
* Debug mode helps trace replacement operations and identify issues.
* Adjust the mappings in `src/main.c` to add or change keyword substitutions.

---

## License

This project is provided as-is for educational and experimental use.

---

If you have any questions or want to contribute, feel free to open issues or pull requests.

```
