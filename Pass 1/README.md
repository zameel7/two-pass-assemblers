```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {

    FILE *f1, *f2, *f3, *f4;
    char opcode[10], label[10], operand[10];
    char code[10], mnemonics[10];
    int start_address, locctr;

    // open files of input, output, symtab and optab
    f1 = fopen("input.txt", "r");
    f2 = fopen("optab.txt", "r");
    f3 = fopen("symtab.txt", "w");
    f4 = fopen("output.txt", "w");

    // accept the first input from input file
    fscanf(f1, "%s%s%s", label, opcode, operand);

    // check if the opcode is start, add its operandess
    // as start operandess
    if (strcmp("START", opcode))
    {
        start_address = atoi(operand);
        locctr = start_address;
        fprintf(f4, "\t%s\t%s\t%s", label, opcode, operand);
        fscanf(f1, "%s%s%s", label, opcode, operand);
    }
    else {
        locctr = 0;
    }

    // loop till the opcode reaches end
    while (strcmp(opcode, "END")!=0)
    {
        // print the first location counter
        fprintf(f4, "%d\t", locctr);
        // check if there is a symbol, if yes print it in the symtab else reject
        if (strcmp(label, "-")!=0)
        {
            fprintf(f3, "%s  %s", label, locctr);
        }
        // go through the optab
        fscanf(f2, "%s\t%s", code, mnemonics);
        while (strcmp(code, "END")!=0)
        {
            if(strcmp(opcode, code)==0) {
                locctr += 3;
                break;
            }
            fscanf(f2, "%s\t%s", code, mnemonics);
        }
        if (strcmp(opcode, "WORD")==0)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "RESW")==0)
        {
            locctr += (3*atoi(operand));
        }
        else if (strcmp(opcode, "RESB")==0)
        {
            locctr += atoi(operand);
        }
        else if (strcmp(opcode, "BYTE")==0) {
            locctr += strlen(operand)-2;
        }
        
        // print the details in output file and load next input
        fprintf(f4, "%s\t%s\t%s\t\n", label, opcode, operand);
        fscanf(f1, "%s\t%s\t%s", label, opcode, operand);
    }
    fprintf(f4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    printf("\nLength of the program: %d", locctr-start_address);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
}
```
