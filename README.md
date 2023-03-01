# README

----

# VC23 Cart Language Spec

## Source

- write cart source in plain text utf-8 `.vcc` files.
- empty lines will be ignored
- comments will be ignored

## Compiling and Decompiling

- compile `.vcc` files using `vcc32`.
- decompile `.cart` files using `vcd32`

```
vcc23 sourcefile.vcc outputfile.cart
```

Note that the output cart file is a binary file and cannot be read with a text editor.

```
vcd23 my.cart outputsourcefile.vcc
```

Note that comments and whitespace will not be present in the decompiled source file.

## Comments

```
; this is a comment and will be stripped by the compiler
```

## Numbers

- base ten (decimal) values use a `d` prefix like `d5` is five.
- base 16 (hexadecimal) values use a `&` prefix like `&a` is ten.
- memory addresses are always written in decimal, are zero-indexed and do not require the `d` prefix and always have a
  prefix of `@` like `@16` means the address of the 17th byte of selected memory.
- device numbers are always written in decimal, are wrapped with `{` and `}` like `{4}` means the fourth hardware
  device.

## Data

Any fixed data that your cart depends on needs to be declared in the cart rom. You are technically not limited in the
size of the rom space, however, more than 32MB should be avoided.

- data declaration values are always written in hexadecimal, and do not need to have the "&" prefix.
- data is declared in lines of 8 bytes. if you do not need a full 8 bytes, you must provide filler bytes with the value
  of `00`.
- you can write any amount of whitespace between your values, the whitespace will be ignored.

- data declaration lines begin with a capital `D` followed by 8 hex values.
- for example: `D 00 0a cd 00 23 4c 7a ff`

## Memory

There are two types of memory available to your cart programs.

RAM is read-write and volatile (cleared on hardware reset) and is limited to 8MB.

ROM is read-only and fixed state (stored in the cart binary) and is determined by the data declarations you have made in
your cart.

- there are three "named" special memory locations:
    - memory at location `0` is named `x`
    - memory at location `1` is named `y`
    - memory at location `2` is named `z`
- `@0` is the same as `@x` and can be written either way
- `@1` is the same as `@y` and can be written either way
- `@2` is the same as `@z` and can be written either way

## Instruction Set:

### Wasting Time

```
; no-op, consume a single cycle of execution time
~
```

### Assignment

```
; literal assignment: x = 22`
.&16@0

; referential assignment: y = x
.@0@1
```

### Addition

```
; literal addition: y = y + 8
+d8@1

; referential addition: y = y + y
+@1@1
```

### Subtraction

```
; literal subtraction: x = x - 10
-&a@0

; referential subtraction: y = y - x
-@0@1
```

### Multiplication

```
; literal multiplication: x = x * 2
*d2@0

; referential multiplication: x = x * y
*@1@0
```

### Division

```
; literal division: x = x / 128
/&80@0

; referential division: x = x / y
/@1@0
```

### Negation

```
; negation: x = -x
_@0
```

### Bitwise XOR

```
; literal xor: x = x XOR 2
^d2@0

; referential xor: x = x XOR x
^@0@0
```

### Bitwise AND

```
; literal bitwise and: x = x AND x
&&1@0

; referential bitwise and: x = x AND x
&@0@0
```

### Bitwise OR

```
; literal bitwise or: x = x OR x
|d16@0

; referential bitwise or: x = x OR x
|@0@0
```

### Shift Bits Left

```
; literal bit shift left: x = x << 2
<d2@0
```

### Shift Bits Right

```
; literal bit shift right: x = x >> 2
>&2@0
```

### Comparison

```
; compare the memory at address 0 (named x)
; against the literal value 0
; if the two are equal, set comparison result to 0
; if the literal is less, set comparison result to -1
; if the literal is more, set comparison result to 1
=d0@0

; compare the memory at address 0 (named x)
; against the memory at address 1 (named y)
; if the two are equal, set comparison result to 0
; if the literal is less, set comparison result to -1
; if the literal is more, set comparison result to 1
=@0@1

; compare the memory at address 1 (named y)
; against the implicit literal value 0
; if the two are equal, set comparison result to 0
; if the literal is less, set comparison result to -1
; if the literal is more, set comparison result to 1
z@1
```

### Execution Path Manipulation

> Note: attempting to move before `0` or after the last code byte will cause the operation to clamp to either the start or the end of the code respectively. This side-effect can be used to your advantage by using a negative relative move with a large offset can restart a program from the start, and using a position relative move with an offset larger than the code can cause the program to end immediately.

All move operations depend on the last comparison result to be zero (the comparison was equal). If the last comparison
result is not zero, the move operation will be skipped over.

```
; goto absolute code offset 23
g|d23

; goto absolute code offset 255
g|&ff

; read value in memory address 6 as A
; goto absolute code offset A
g|@6

; goto relative code offset +10
g>&a

; read value in memory address 6 as A
; goto relative code offset +A
g>@6

; goto relative code offset -4
g<d4

; read value in memory address 6 as A
; goto relative code offset -A
g<@6
```

### Memory Select

RAM memory selection operations will have all memory addresses that are used after the selection operation point at the
RAM memory.

```
; set memory pointer to ram at default offset 0
`R

; set memory pointer to ram at offset 0
; same as `R
`R[&0]

; set memory pointer to ram at offset 32
`R[d32]
```

> Note: If the offset exceeds the RAM limit 8192, then the offset will be set to zero.

ROM memory selection operations will have all memory addresses that are used after the selection operation point at the
ROM memory.

> Note: If the cart has zero bytes of ROM (data), then ROM memory selection operations will cause the program to exit with the non-zero hex error code `&f6` (246 decimal)

> Note: If there is at least one byte of ROM (data), and the offset exceeds the ROM limit, then the offset will be set to zero.

```
; set memory pointer to rom at default offset 0
; write operations are automatically no-opped
`D

; set memory pointer to rom at offset 0
; same as `D
`D[&0]

; set memory pointer to rom at offset 64
`D[&40]
```

### Input

The hardware has at least one input device, `{0}` which is the standard input device.

```
; read from input device #0 and place result into memory at address 2
i{0}@2

; read 4 characters from device #0
; write the characters sequentially into memory at addresses
; 3+0, 3+1, 3+2, 3+3
rd4{0}@3

; read value from memory address 8 as n
; read characters from device #0
; write the characters sequentially into memory at addresses
; 3+0, 3+.., 3+n-1
r@8{0}@3
```

### Output

The hardware has at least one output device, `{0}` which is the standard output device.

```
; output literal value 0 to output device #0
od0{0}

; output value from memory address 3 to output device #0
o@3{0}
```

#### Printing Strings

A utility for printing out a string of characters of data to the standard output device is provided for convenience.

```
; print 4 characters from memory addresses
; 3+0, 3+1, 3+2, 3+3 to output device #0
pd4@3

; read value of from memory address 3 as n
; print n characters from memory addresses
; 4+0, 4+.., 4+n-1 to output device #0
p@3@4
```

### Ending Program Execution

```
; end program immediately with hex error code
q&fc

; end program immediately with success
qd0
; or just by itself
q
```

---

## License

MIT License

© 2023 SYS 64738 CONSULTING SERVICES LLC, Richard Marks

----

# vc23 compiler `vcc23`

```
input.vcc → [R] → [T] → [P] → [G] → [W] → output.cart
```

- R: Read the cart source file and pre-process
- T: Tokenize the cart source into a stream of lexemes
- P: Parse the lexeme stream into an abstract syntax tree model
- G: Traverse the AST and generate the cart binary code stream
- W: Write the cart binary file from code stream

### Reader

- Reads the .vcc input file contents
- Strips comments and empty lines
- Separates data declarations from instructions

### Syntax Lexer

- Depends on reader
- Tokenizes each instruction line from the reader into a list of lexemes
- Reduces all lexeme lists into a single stream

### Syntax Parser

- Depends on lexer
- Validates correct syntax
- Builds abstract syntax tree from lexer lexeme stream

### Code Generator

- Depends on parser and reader
- Compiles ROM table from reader data declarations
- Traverses parser AST nodes and reduces to binary code stream

##### ROM Table

The ROM table consists of all the data declaration bytes padded to have an even number of groups of eight 8-bit unsigned integers.

##### Code Stream

The code stream is written out in the following manner

- for each instruction
    - `unsigned 16-bit integer: INSTRUCTION_OPCODE`
    - `unsigned 8-bit integer: OPERAND_COUNT`
        - for each operand
            - `unsigned 8-bit integer: OPERAND_BYTES`
            - if value <= **255**:
                - `unsigned 8-bit integer: VALUE`
            - if value > **255** and <= **65535**:
                - `unsigned 16-bit integer: VALUE`
            - if value > **65535** and <= **4294967295**:
                - `unsigned 32-bit integer: VALUE`
            - if value > **4294967295**:
                - `unsigned 64-bit integer: VALUE`

### Writer

- Depends on code generator
- Calculates cart header from code stream and ROM table
- Writes cart binary output file

### `.cart` file format

```
0x00: char[4] signature bytes
0x04: char[33] game name
0x25: unsigned long[16] reserved
0xA5: unsigned long data_length
0xAD: unsigned long code_length
0xB5: begin ROM data block
0xB5 + data_length: begin code block
```

The `.cart` file format is a binary little-endian ordered format.

The first four bytes are the file signature bytes:

`0x76`, `0x63`, `0x32`, `0x33` which identifies the file.

After the file signature, the name of the game, a fixed-length 33-byte null-terminated string.

128 bytes are then reserved for future use.

The next 8 bytes specify the size of the ROM data (aka data block).

The next 8 bytes specify the size of the code block.

After the header, the data block and code block follow.


----

## vc23 Instruction Set

The following table lists all of the operand type specifiers that will be used in the instruction table. These
specifiers will be used throughout the vc23 programmer's reference.

| Specifier | Type                                      |
|-----------|-------------------------------------------|
| **DL**    | Decimal Literal Number _eg, 10_           |
| **HL**    | Hexadecimal Literal Number _eg, f6_       |
| **RA**    | Reference Address Number (always decimal) |
| **ID**    | Input Device Number (always decimal)      |
| **OD**    | Output Device Number (always decimal)     |

The following table lists all of the instructions and their opcode values and operands.

| Instruction                | Opcode | Operands | Operand Types |
|----------------------------|--------|----------|---------------|
| ✅ Nop                      | 1      | 0        |               |
| ✅ AssignDecLitRef          | 2      | 2        | DL, RA        |
| ✅ AssignHexLitRef          | 3      | 2        | HL, RA        |
| ✅ AssignRefRef             | 4      | 2        | RA,RA         |
| ✅ AddDecLitRef             | 5      | 2        | DL, RA        |
| ✅ AddHexLitRef             | 6      | 2        | HL, RA        |
| ✅ AddRefRef                | 7      | 2        | RA, RA        |
| ✅ SubDecLitRef             | 8      | 2        | DL, RA        |
| ✅ SubHexLitRef             | 9      | 2        | HL, RA        |
| ✅ SubRefRef                | 10     | 2        | RA, RA        |
| ✅ MulDecLitRef             | 11     | 2        | DL, RA        |
| ✅ MulHexLitRef             | 12     | 2        | HL, RA        |
| ✅ MulRefRef                | 13     | 2        | RA, RA        |
| ✅ DivDecLitRef             | 14     | 2        | DL, RA        |
| ✅ DivHexLitRef             | 15     | 2        | HL, RA        |
| ✅ DivRefRef                | 16     | 2        | RA, RA        |
| ✅ NegRef                   | 17     | 1        | RA            |
| ✅ XorDecLitRef             | 18     | 2        | DL, RA        |
| ✅ XorHexLitRef             | 19     | 2        | HL, RA        |
| ✅ XorRefRef                | 20     | 2        | RA, RA        |
| ✅ AndDecLitRef             | 21     | 2        | DL, RA        |
| ✅ AndHexLitRef             | 22     | 2        | HL, RA        |
| ✅ AndRefRef                | 23     | 2        | RA, RA        |
| ✅ OrDecLitRef              | 24     | 2        | DL, RA        |
| ✅ OrHexLitRef              | 25     | 2        | HL, RA        |
| ✅ OrRefRef                 | 26     | 2        | RA, RA        |
| ✅ LeftShiftDecLitRef       | 27     | 2        | DL, RA        |
| ✅ LeftShiftHexLitRef       | 28     | 2        | HL, RA        |
| ✅ RightShiftDecLitRef      | 29     | 2        | DL, RA        |
| ✅ RightShiftHexLitRef      | 30     | 2        | HL, RA        |
| ✅ CmpDecLitRef             | 31     | 2        | DL, RA        |
| ✅ CmpHexLitRef             | 32     | 2        | HL, RA        |
| ✅ CmpRefRef                | 33     | 2        | RA, RA        |
| ✅ CmpRef                   | 34     | 1        | RA            |
| ✅ AbsJumpDecLit            | 35     | 1        | DL            |
| ✅ AbsJumpHexLit            | 36     | 1        | HL            |
| ✅ AbsJumpRef               | 37     | 1        | RA            |
| ✅ RelJumpBackDecLit        | 38     | 1        | DL            |
| ✅ RelJumpBackHexLit        | 39     | 1        | HL            |
| ✅ RelJumpBackRef           | 40     | 1        | RA            |
| ✅ RelJumpForwardDecLit     | 41     | 1        | DL            |
| ✅ RelJumpForwardHexLit     | 42     | 1        | HL            |
| ✅ RelJumpForwardRef        | 43     | 1        | RA            |
| SelectRamDefault           | 44     | 0        |               |
| SelectRamDecLit            | 45     | 1        | DL            |
| SelectRamHexLit            | 46     | 1        | HL            |
| SelectRomDefault           | 47     | 0        |               |
| SelectRomDecLit            | 48     | 1        | DL            |
| SelectRomHexLit            | 49     | 1        | HL            |
| ✅ ReadInputDeviceRef       | 50     | 2        | ID, RA        |
| ✅ ReadInputDeviceDecLitRef | 51     | 3        | DL, ID, RA    |
| ✅ ReadInputDeviceHexLitRef | 52     | 3        | HL, ID, RA    |
| ✅ ReadInputDeviceRefRef    | 53     | 3        | RA, ID, RA    |
| ✅ WriteOutputDeviceDecLit  | 54     | 2        | DL, OD        |
| ✅ WriteOutputDeviceHexLit  | 55     | 2        | HL, OD        |
| ✅ WriteOutputDeviceRef     | 56     | 2        | RA, OD        |
| ✅ PrintDecLitRef           | 57     | 2        | DL, RA        |
| ✅ PrintHexLitRef           | 58     | 2        | HL, RA        |
| ✅ PrintRefRef              | 59     | 2        | RA, RA        |
| ✅ EndWithDecLitExitCode    | 60     | 1        | DL            |
| ✅ EndWithHexLitExitCode    | 61     | 1        | HL            |
| ✅ EndWithSuccess           | 62     | 0        |               |

----

## ROADMAP

New comparison instructions

- CmpLTDecLitRef `?<d10@5`
- CmpLTHexLitRef `?<&a@5`
- CmpLTRefRef `?<@5@6`

- CmpGTDecLitRef `?>d10@5`
- CmpGTHexLitRef `?>&a@5`
- CmpGTRefRef `?>@5@6`

- CmpLEDecLitRef `=<d10@5`
- CmpLEHexLitRef `=<&a@5`
- CmpLERefRef `=<@5@6`

- CmpGEDecLitRef `=>d10@5`
- CmpGEHexLitRef `=>&a@5`
- CmpGERefRef `=>@5@6`

Division with remainder:

`/d2@8@100` -> `@8 = quotient, @100 = remainder`

Variables:

`$a $b $c $delta $thisIsAlsoAValidVariable`

Labels to make branching easier:

```asm
; declare counter variable
v counter d10 ; initialize counter to 10
o&ff{2} ; enable number to string printing
start:
p$counter   ; print the counter as a string
o&a{0}      ; output a newline to stdout
-d1$counter ; decrement counter by one
z$counter   ; if counter is zero
g|exit      ;   jump to exit
g|start     ; else jump to start
exit:
o&0{2}      ; disable number to string printing
q           ; exit with success code 0
```
