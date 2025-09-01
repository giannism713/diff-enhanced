#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int i, j, k, count, line, position, FailedTestFlag, mistakeColumn, faultFlag, passedTests, FirstMistakePosition, faultyRows[1000], faultyColumns[1000];

char ProgramNameC[20], ProgramName[20], TestFolderName[20], AllTestFolder[200];

int UserInputValidation(char ProgramNameC[], char TestFolderName[]);
void processFiles(char ProgramName[], char TestFolderName[], const char extension[]);
int Check(char ProgramNameC[], char TestFolderName[]);

int main(int argc, char *argv[])
{
    passedTests = 0;
    faultFlag = 0;
    if (argc > 1)
    {
        strcpy(ProgramNameC, argv[1]);
        strcpy(TestFolderName, argv[2]);
    }
    else
    {
        printf("Which file do you want to test ? \n");
        scanf(" %s", ProgramNameC);

        printf("What's the name of the tests folder ? \n");
        scanf(" %s", TestFolderName);
        printf("\n");
    }

    if (Check(ProgramNameC, TestFolderName))
    {
        if (passedTests == count)
        {
            printf("\033[1;32mPASSED ALL TESTS\033[1;0m\n");
        }
        else
        {
            printf("\033[1;31mPASSED %d/%d TESTS\033[1;0m\n", passedTests, count);
        }
    }
    else
    {
        printf("\033[1;31mFail\033[1;0m\n");
    }

    return 0;
}

int UserInputValidation(char ProgramNameC[], char TestFolderName[])
{
    char CompilationFlags[20] = "gcc -o ";

    char AllCompilationCommand[200];
    strcpy(AllCompilationCommand, CompilationFlags);
    i = 0;
    while (ProgramNameC[i] != '.' && ProgramNameC[i] != '\0')
    {
        ProgramName[i] = ProgramNameC[i];
        i++;
    }
    ProgramName[i] = '\0';
    strcat(AllCompilationCommand, ProgramName);
    strcat(AllCompilationCommand, " ");
    strcat(AllCompilationCommand, ProgramName);
    strcat(AllCompilationCommand, ".c");

    // printf("%s\n", AllCompilationCommand);
    if (system(AllCompilationCommand) == 0)
    {
        printf("\033[1;32mCompilation Done Successfully (no errors in your code) !\n\033[1;0m\n");
        return 1;
    }
    else
    {
        printf("There are errors in your code or the file is non-existent. \n");
        return 0;
    }
}

void PrintLines(FILE *fPtr1, FILE *fPtr2)
{
    fseek(fPtr1, -position, SEEK_CUR);
    fseek(fPtr2, -position, SEEK_CUR);

    char p_chEXOUTPUT = fgetc(fPtr2);
    printf("Line %d:\n", line);
    printf("Expected Output:");
    while (p_chEXOUTPUT != '\n' && p_chEXOUTPUT != EOF)
    {
        printf("%c", p_chEXOUTPUT);

        p_chEXOUTPUT = fgetc(fPtr2);
    }
    printf("(END)");
    // fseek(fPtr2, -position + 1, SEEK_CUR);

    printf("\n");

    char p_chOUTPUT = fgetc(fPtr1);
    printf("  Actual Output:");
    while (p_chOUTPUT != '\n' && p_chOUTPUT != EOF)
    {
        printf("%c", p_chOUTPUT);

        p_chOUTPUT = fgetc(fPtr1);
    }
    printf("(END)");
    // fseek(fPtr1, -position + 1, SEEK_CUR);

    printf("\n");

    for (k = 0; k < 15 + FirstMistakePosition; k++)
    {
        printf(" ");
    }

    printf("^");

    printf("\n");
}

int compareFiles(FILE *fPtr1, FILE *fPtr2)
{
    int FaultFlagLine = 0;

    FirstMistakePosition = 0;

    int CountStepBacks1 = 0;
    int CountStepBacks2 = 0;

    int MistakeLine = 1;
    int OldMistakeLine = -1;

    int OutputLines = 0;
    int ExpectedOutputLines = 0;
    FailedTestFlag = 0;
    line = 1;
    position = 1;

    int printed = 0;

    /* count how many lines does output have */
    char chOUTPUT = fgetc(fPtr1);
    while (chOUTPUT != EOF)
    {
        if (chOUTPUT == '\n')
        {
            OutputLines++;
        }
        chOUTPUT = fgetc(fPtr1);
        if (chOUTPUT == EOF)
        {
            OutputLines++;
        }
    }

    // Reset the file pointer to the beginning of the file
    rewind(fPtr1);

    char chEXOUTPUT = fgetc(fPtr2);

    while (chEXOUTPUT != EOF)
    {
        if (chEXOUTPUT == '\n')
        {
            ExpectedOutputLines++;
        }
        chEXOUTPUT = fgetc(fPtr2);
        if (chEXOUTPUT == EOF)
        {
            ExpectedOutputLines++;
        }
    }

    // Reset the file pointer to the beginning of the file
    rewind(fPtr2);

    if (OutputLines != ExpectedOutputLines)
    {

        printf("Your output has %d lines, but it should have %d lines\n", OutputLines, ExpectedOutputLines);
        FailedTestFlag = 1;
        return 0;
    }

    while (1)
    {

        chOUTPUT = fgetc(fPtr1);
        chEXOUTPUT = fgetc(fPtr2);

        // If both files reach the end simultaneously, break the loop
        if (chOUTPUT == EOF && chEXOUTPUT == EOF)
        {
            if (!printed && FaultFlagLine)
            {
                position = position - 1;
                // printf("printaa\n");
                PrintLines(fPtr1, fPtr2);
            }
            break;
        }

        // Check for differences in characters
        if (chOUTPUT != chEXOUTPUT)
        {
            /*maybe position + 1*/
            FailedTestFlag = 1;
            // printf("%d", line);
            if (MistakeLine != OldMistakeLine || line == 1)
            {
                FaultFlagLine = 1;
                OldMistakeLine = MistakeLine;
                FirstMistakePosition = position;
            }
            // if(chOUTPUT != '\n' && chEXOUTPUT == '\n')
            // {
            //     printf("Difference at line %d, position %d, (No NEWLINE on your Output)\n", line, position);
            // }
            // if(chOUTPUT == '\n' && chEXOUTPUT != '\n')
            // {
            //     printf("Difference at line %d, position %d, your Output printed 'NEWLINE' but expected '%c' \n", line, position, chEXOUTPUT);
            // }
            // if(chOUTPUT == '\n' || chEXOUTPUT != '\n')
            // {
            //     printf("Difference at line %d, position %d, your Output printed 'NEWLINE' but Expected '%c'\n", line, position, chEXOUTPUT);
            // }
            // if(chOUTPUT != '\n' || chEXOUTPUT == '\n')
            // {
            //     printf("Difference at line %d, position %d, your Output printed '%c' but Expected 'NEWLINE'\n", line, position, chOUTPUT);
            // }
            // else if(chEXOUTPUT != EOF && chOUTPUT == EOF)
            // {
            //     printf("Difference at line %d, position %d, your Output ended but Expected '%c'\n", line, position, chEXOUTPUT);
            // }
            // else
            // {_
            //     printf("Difference at line %d, position %d, Expected '%c', your Output '%c'\n", line, position, chEXOUTPUT, chOUTPUT);
            // }
        }

        // Move to the next line and reset position if newline is encountered
        if (chEXOUTPUT == '\n' || chEXOUTPUT == EOF)
        {
            if (FaultFlagLine == 1)
            {
                /* for the break section */
                printed = 1;
                // printf("print\n");
                PrintLines(fPtr1, fPtr2);
                FaultFlagLine = 0;
            }
            line++;
            MistakeLine = line;
            position = 1;
        }
        else
        {
            position++;
        }
    }

    return FailedTestFlag;
}

void processFiles(char ProgramName[], char TestFolderName[], const char extension[])
{
    char AllTestFolder[200];

    // Initialize the AllTestFolder array
    strcpy(AllTestFolder, TestFolderName);
    strcat(AllTestFolder, "/");
    strcat(AllTestFolder, ProgramName);

    DIR *dir;
    struct dirent *entry;
    count = 0;

    // Open the test directory
    dir = opendir(AllTestFolder);

    if (dir == NULL)
    {
        printf("Error opening test directory (There is no such directory) make sure executable name is the same as the test subfolder !\n");
        faultFlag = 1;
        return; // You might want to handle the error and return accordingly
    }

    // Iterate through files in the directory
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        { // Check if it is a regular file
            // Check if the file has the desired extension
            if (strstr(entry->d_name, extension) != NULL && strcmp(entry->d_name + strlen(entry->d_name) - strlen(extension), extension) == 0)
            {
                count++;
            }
        }
    }

    closedir(dir);

    printf("Number of tests in %s: %d\n", AllTestFolder, count);

    // Loop over the test files
    for (i = 1; i <= count; i++)
    {
        int returning;
        char InputFile[200];
        char OutputFile[200];
        char TestingCommand[200];

        // Initialize InputFile before using it
        strcpy(InputFile, AllTestFolder);

        // Your existing code for forming InputFile and OutputFile...
        returning = snprintf(InputFile, sizeof(InputFile), "%s/test%d%s", AllTestFolder, i, extension);
        if (returning < 0)
        {
            abort();
        }

        returning = snprintf(OutputFile, sizeof(OutputFile), "%s/test%d-STUDENT.out", AllTestFolder, i);
        if (returning < 0)
        {
            abort();
        }

        // Your existing code for forming TestingCommand...
        returning = snprintf(TestingCommand, sizeof(TestingCommand), "./%s < %s > %s", ProgramName, InputFile, OutputFile);
        if (returning < 0)
        {
            abort();
        }

        printf("%s\n", TestingCommand); // Print the command (for debugging)

        // Execute the command
        char plainOutputFile[200];
        returning = snprintf(plainOutputFile, sizeof(InputFile), "%s/test%d.out", AllTestFolder, i);
        if (returning < 0)
        {
            abort();
        }

        if (system(TestingCommand) == 0)
        {
            FILE *fPtr1;
            fPtr1 = fopen(OutputFile, "rb+");
            FILE *fPtr2;
            fPtr2 = fopen(plainOutputFile, "rb+");
            if (fPtr1 == NULL || fPtr2 == NULL)
            {
                /* Unable to open file hence exit */
                printf("\nUnable to open file.\n");
                printf("Please check whether file exists and you have read privilege.\n");
                faultFlag = 1;

                exit(EXIT_FAILURE);
            }
            else
            {
                compareFiles(fPtr1, fPtr2);
                if (FailedTestFlag == 0)
                {
                    passedTests++;
                    printf(" -Test%d: \033[1;32mPASS\033[1;0m\n\n", i);
                }
                else if (FailedTestFlag == 1)
                {
                    printf(" -Test%d: \033[1;31mFAIL\033[1;0m\n\n", i);
                }
            }
        }
    }
}

int Check(char ProgramNameC[], char TestFolderName[])
{
    if (UserInputValidation(ProgramNameC, TestFolderName) == 1)
    {
        // processFiles(ProgramName, TestFolderName, ".out");
        processFiles(ProgramName, TestFolderName, ".in");
        if (faultFlag == 1)
        {
            return 0;
        }
        return 1; // Return 1 for successful execution
    }
    else
    {
        printf("Compilation failed\n");
        return 0; // Return 0 for compilation errors
    }
}
