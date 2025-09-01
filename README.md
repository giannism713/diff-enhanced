# diff-enhanced

## Overview

diff-enhanced is a C-based automated testing tool designed to help students and instructors efficiently test C programs against a suite of input/output test cases. Unlike the standard `diff` utility in Linux, which simply compares two files and reports line-by-line differences, diff-enhanced provides a more user-friendly and educational output, highlighting exactly where and what the mistakes are in the program's output compared to the expected output.

## Features

- **Automated Compilation and Testing:**
	- Compiles the target C program automatically.
	- Runs the compiled program against multiple test cases located in a structured folder.
- **Detailed Output Comparison:**
	- Compares the program's output to the expected output file for each test case.
	- Reports the exact line and character position of the first mistake in each failed test.
	- Shows both the expected and actual output lines, with a caret (^) pointing to the first difference.
	- Counts and reports the number of passed/failed tests.
- **User-Friendly Feedback:**
	- Uses colored terminal output to clearly indicate pass/fail status.
	- Explains if the number of output lines differs from the expected output.
- **Batch Testing:**
	- Supports running all tests in a given folder automatically, making it ideal for coursework and automated grading.

## How It Works

1. **Test Folder Structure:**
	 - Each program to be tested should have its own subfolder inside the `tests/` directory (e.g., `tests/program/`).
	 - Each test consists of an input file (`testN.in`), an expected output file (`testN.out`), and a file where the student's output will be written (`testN-STUDENT.out`).

2. **Running the Tester:**
	 - Compile and run `test.c` (the tester program) with the following command:
		 ```bash
		 gcc -o tester test.c
		 ./tester program.c tests
		 ```
		 - `program.c` is the C file you want to test.
		 - `tests` is the folder containing the test cases.
	 - Alternatively, if run without arguments, the tester will prompt for the program and test folder names interactively.

3. **What Happens:**
	 - The tester compiles `program.c`.
	 - For each test case, it runs the compiled program with the corresponding input file and captures the output.
	 - It compares the output to the expected output, reporting detailed differences if any.
	 - At the end, it summarizes how many tests passed/failed.

## Example Output

```
Number of tests in tests/program: 3
./program < tests/program/test1.in > tests/program/test1-STUDENT.out
 -Test1: PASS

./program < tests/program/test2.in > tests/program/test2-STUDENT.out
Line 2:
Expected Output:42(END)
  Actual Output:41(END)
				 ^
 -Test2: FAIL

PASSED 2/3 TESTS
```

## How diff-enhanced Differs from Linux `diff`

- **Educational Feedback:**
	- Shows the exact location of the first mistake in each line, not just that a line differs.
	- Prints both the expected and actual output side by side, with a visual pointer to the error.
	- Reports if the number of output lines is incorrect.
- **Batch Automation:**
	- Automatically runs all test cases in a folder, rather than requiring manual invocation for each pair of files.
- **Integrated Compilation:**
	- Compiles the student's code as part of the process, reporting compilation errors clearly.
- **Summary Reporting:**
	- Provides a summary of how many tests passed/failed, with colored output for clarity.
- **No Need for Manual File Comparison:**
	- Handles input/output redirection and file management automatically.

## Use Cases

- **Students:**
	- Quickly check if your program meets the requirements for all test cases before submission.
- **Instructors/Graders:**
	- Automate grading of student submissions with clear, actionable feedback.

## Requirements

- GCC (for compiling C programs)
- Linux or Unix-like environment

## License

This project is provided for educational purposes. See LICENSE for details.