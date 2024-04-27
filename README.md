System Software Project

Course link: [13SS113SS](https://www.etf.bg.ac.rs/en/fis/karton_predmeta/13S113SS-2013)

# Table of contents

- [Table of contents](#table-of-contents)
- [Tasks](#tasks)
  - [Assembler](#assembler)
  - [Usage:](#usage)
    - [Options:](#options)
  - [Source code syntax](#source-code-syntax)
    - [General information](#general-information)
    - [Assembler directives](#assembler-directives)
    - [Assembler instructions](#assembler-instructions)
    - [Registers and operands](#registers-and-operands)
  - [Linker](#linker)
  - [Emulator](#emulator)
- [Process](#process)


# Tasks
The project consists of three tasks:
  1) The assembler
  2) The linker
  3) The emulator

The use of non-standard libraries (if they are not closely linked to the core of the project) is allowed
## Assembler
## Usage:
`asembler [options] <input_file>`
### Options:
- `-o <output_file>` - specifies output name and destination
## Source code syntax
### General information
- One line of source code shall contain at most one instruction or directive
- Comments are completely ignored during the assembly process
- One line comments, located at the end of a line, begin with the `#` character
- A label, which ends with `:`, must be located at the very beginning of a line of source code
- A label may exist 'as its own line', without a following assembler instruction or directive, in this case it is equivalent as being at the beginning of the next line of source code which contains an instruction/directive
### Assembler directives
- `.global <symbol_list>` - Exports all symbols within `<symbol_list>`. The list may contain one, or more comma separated symbols
- `.extern <symbol_list>` - Imports all symbols within `<symbol_list>`. The list may contain one, or more comma separated symbols
- `.section <section_name>` - Starts a new assembler section, which implicitly marks the end of the previous section, with the desired name
- `.word <symbol_or_literal_list>` - Allocates a fixed amount of space in 4 byte increments for each initializer (symbol or literal) within the supplied, comma separated list. The directive initializes the space with the value of the initializers
- `.skip <literal>` - Allocates space whose size is equal to the literal supplied. The directive initializes the space with zeroes
- `.ascii <string>` - Allocates a fixed amount of space 1 byte increments for each character of the supplied string. The directive initializes the space with the value of the supplied string
- `.equ <new_symbol>, <expression>` - Defines a new symbol whose value is equal to the supplied expression
- `.end` - Ends the process of assembling the input file. The contents of the file following this directive are discarded and are not assembled
### Assembler instructions
- `halt` - Ceases the execution of instructions
- `int` - Causes a software interrupt
- `iret` - `pop pc; pop status;`
- `call operand` - `push pc; pc <= operand`
- `ret` - `pop pc;`
- `jmp operand` - `pc <= operand`
- `beq %gpr1, %gpr2, operand` - `if (gpr1 == gpr2) pc <= operand`
- `bne %gpr1, %gpr2, operand ` - `if (gpr1 != gpr2) pc <= operand`
- `bgt %gpr1, %gpr2, operand ` - `if (gpr1 signed > gpr2) pc <= operand`
- `push %gpr ` - `sp <= sp -4; mem32[sp] <= gpr`
- `pop %gpr ` - `gpr <= mem32[sp]; sp <= sp + 4`
- `xchg %gprS, %gprD ` - `temp <= gprD; gprD <= gprS; gprS <= temp`
- `add %gprS, %gprD ` - `gprD <= gprD + gprS`
- `sub %gprS, %gprD ` - `gprD <= gprD - gprS`
- `mul %gprS, %gprD ` - `gprD <= gprD * gprS`
- `div %gprS, %gprD ` - `gprD <= gprD / gprS`
- `not %gpr ` - `gpr <= ~gpr`
- `and %gprS, %gprD ` - `gprD <= gprD & gprS`
- `or %gprS, %gprD ` - `gprD <= gprD | gprS`
- `xor %gprS, %gprD` - `gprD <= gprD ^ gprS`
- `shl %gprS, %gprD ` - `gprD <= gprD << gprS`
- `shr %gprS, %gprD ` - `gprD <= gprD >> gprS`
- `ld operand, %gpr ` - `gpr <= operand`
- `st %gpr, operand ` - `operand <= gpr`
- `csrrd %csr, %gpr ` - `gpr <= csrAcsrwr %gpr, %csrcsr <= gpr`
### Registers and operands
Above, the label `gprX` represents one of the programmatically available general purpose registers, which are: `r0`, `r1`, `r2`, `r3`, `r4`, `r5`, `r6`, `r7`, `r8`, `r9`, `r10`, `r11`, `r12`, `r13`, `r14/sp`, `r15/pc`
The label `csrX` represents one of the programmatically available control and status registers, which are: `status`, `handler`, `cause`
The label `operand` represents all syntax notations for labeling operands. Syntax notations differ between jump instructions or instructions which deal with data.
Data notations:
- `$<literal>` - the value `<literal>`
- `$<symbol>` - the value of `<symbol>`
- `<literal>` - the memory value at `<literal>`
- `<symbol>` - the memory value at `<symbol>`
- `%<reg>` - the value inside the register `<reg>`
- `[%<reg>]` - the memory value at address `<reg>`
- `[%<reg> + <literal>]` - the memory value at address `<reg> + <literal>`
- `[%<reg> + <symbol>]` - the memory value at address `<reg> + <symbol>`
Jump and subroutine notations:
- `<literal>` - the value of `<literal>`
- `<symbol>` - the value of `<symbol>`

## Linker


## Emulator


# Process