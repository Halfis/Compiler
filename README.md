# Compiler

## Language Essentials:
- The language contains at least 5 keywords, most of them more than 2 characters in length. These are strings of letters chosen by the student (depending on the selected language).
- It supports natural numbers, including multi-digit numbers.
- Variables can have any name, starting with a letter (without requiring a specific first character). Variable names can be followed by letters or numbers and must be at least 1 character long.
- The language includes a way to read a value into a variable from the user (e.g., `read`, `calculate`, `scan`) and a way to print the current value of a variable (e.g., `write`, `dump`).
- It supports basic arithmetic operators (`+`, `-`, `*`, `/`), parentheses, and an assignment operator. Operator precedence must be preserved in calculations. Optionally, it can also include logical operators (`and`, `or`, `not`) alongside relational operators (e.g., `<`, `>`, `<=`, `>=`, `==`).
- The compiler will evaluate simple mathematical expressions, including operator precedence and parentheses.
- Variables can be used in expressions (on the right-hand side of assignment statements), meaning the value of one variable can be used to calculate the value of another.
- A specific symbol for assignment (assigning the value of an expression to a variable) must be defined. Consider whether the variable needs to be declared before use.
- The language allows the use of blank characters (spaces, blank lines, or multiple statements on a single line). However, unnecessary blank characters should not be required (e.g., at least one space between keywords is recommended, but not between an operator and a number or before a semicolon).
- Commands can be nested, or at least mathematical expressions can be used as arguments to other commands.
- Conditional statements (e.g., `if-then-else`) are supported, but the use of loops (`for`, `while`, etc.) is discouraged due to their more complex semantics.
