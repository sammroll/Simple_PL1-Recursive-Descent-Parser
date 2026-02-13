# Simple_PL1 Recursive Descent Parser

A recursive descent parser implementation in C for the Simple_PL1 programming language, built as part of CS 435 Compiler Construction coursework.

## Overview

This project implements a complete lexical analyzer (scanner) and recursive descent parser for Simple_PL1, a simple imperative programming language. The parser validates source code syntax and reports errors with meaningful messages.

## Features

- **Lexical Analysis**: Tokenizes Simple_PL1 source code
- **Syntax Analysis**: Validates program structure using recursive descent parsing
- **Error Reporting**: Provides clear error messages for syntax and lexical errors
- **Command-line Interface**: Accepts source files as command-line arguments

## Language Specification

### Supported Tokens

| Token | Pattern | Examples |
|-------|---------|----------|
| ID | `(letter\|_)(letter\|digit\|_)*` | `max`, `a_1`, `_a1` |
| NUMBER | `digit+` | `1`, `21`, `1024` |
| READ | `read` | Reserved keyword |
| WRITE | `write` | Reserved keyword |
| ASSIGN | `:=` | Assignment operator |
| PLUS | `+` | Addition |
| MINUS | `-` | Subtraction |
| TIMES | `*` | Multiplication |
| DIV | `/` | Division |
| SEMICOLON | `;` | Statement terminator |
| COMMA | `,` | List separator |
| LPAREN | `(` | Left parenthesis |
| RPAREN | `)` | Right parenthesis |

### Grammar

```
program → stmt_list $
stmt_list → stmt stmt_list | ε
stmt → id := expr; 
     | read( id_list ); 
     | write( expr_list );
expr → term ((+|-) term)*
term → factor ((*|/) factor)*
factor → ( expr ) | id | number
```

### Example Programs

**Valid Program:**
```
x := 2;
y := 3;
read(a, b);
write(a, b, a+b*(2*x/y));
```

**Output:** `Parsing complete. No errors.`

## Building the Project

### Prerequisites

- Visual Studio 2022
- C Compiler (MSVC)
- Windows OS (or compatible environment)

### Compilation

1. Open the project in Visual Studio 2022
2. Build the solution (Ctrl+Shift+B)
3. The executable will be generated in the build directory

## Usage

Run the parser from the command line with a source file:

```bash
name.exe source_file.txt
```

### Example

```bash
parser.exe test1.txt
```

## Error Handling

The parser detects and reports:

- **Lexical Errors**: Unrecognized characters
- **Syntax Errors**: Missing or unexpected tokens

### Error Message Examples

```
Expected symbol: RPAREN
Expected symbol: SEMICOLON
Error in expression: Expected ID, NUMBER, or '('.
Unexpected symbol: DIV
```

## Project Structure

```
.
├── scanner.c          # Lexical analyzer implementation
├── scanner.h          # Scanner header file
├── parser.c           # Recursive descent parser
├── parser.h           # Parser header file
├── main.c             # Entry point
├── tokens.h           # Token definitions
└── test_cases/        # Sample test files
    ├── valid/         # Valid programs
    └── invalid/       # Programs with errors
```

## Implementation Details

### Key Functions

- `scan()` - Returns the next token from input
- `match(token)` - Matches expected token or reports error
- `parse_program()` - Entry point for parsing
- `parse_stmt()` - Parses statements
- `parse_expr()` - Parses expressions
- `parse_term()` - Parses terms
- `parse_factor()` - Parses factors

### Parsing Strategy

The parser uses **predictive parsing** with **first sets** to:
- Determine which production to apply
- Detect syntax errors early
- Provide meaningful error messages

## Testing

Test files should cover:
- Valid programs with all language features
- Lexical errors (invalid characters)
- Syntax errors (missing/unexpected tokens)
- Edge cases (empty statements, nested expressions)

### Sample Test Cases

Located in `test_cases/` directory:
- `test1.txt` - Basic assignment and I/O
- `test2.txt` - Complex expressions
- `test3.txt` - Error cases
