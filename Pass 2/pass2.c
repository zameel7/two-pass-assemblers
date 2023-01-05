#include<stdio.h>
#include<string.h>
#include<ctype.h>

void main()
{
    FILE *finp,*foptab,*fsymtab,*foutput;
    int i,j=0,len;
    char end_addr[10],addr[5],symb_addr[5],opcode[5];
    char start_addr[10],st_label[50],mnemo[10],inp_oper[10];
    char symbol[10],op_mnemo[10],st_endloc[10],st_len[5];

    // Open the files
    printf("\n*** PASS 2 Assembler ****\n\n");
    finp=fopen("input.txt","r");
    foptab=fopen("optab.txt","r");
    fsymtab=fopen("symtab.txt","r");
    foutput=fopen("output.txt","w+");

    // take the first input line
    fscanf(finp,"%s%s%s",st_label,mnemo,inp_oper);

    // initialize locctr to starting address if opcode=start
    if(strcmp(mnemo,"START")==0)
    {
        strcpy(start_addr,inp_oper);
    }

    // write the first header record and start the text record
    printf("H^%s^%s^\nT^00^%s^",st_label,start_addr,start_addr);
    fprintf(foutput,"H^%s^%s^\nT^00^%s^",st_label,start_addr,start_addr);

    // take the next input line
    fscanf(finp,"%s%s%s%s",addr,st_label,mnemo,inp_oper);
    // loop through the intermediate file till the end
    while(strcmp(mnemo,"END")!=0)
    {
        // loop to check if the opcode is in the optab
        fscanf(foptab,"%s%s",op_mnemo,opcode);
        while(!feof(foptab))
        {
            // if opcode found, check if the operand is in the symtab
            if(strcmp(mnemo,op_mnemo)==0)
            {
                fscanf(fsymtab,"%s%s",symb_addr,symbol);
                while(!feof(fsymtab))
                {
                    // if symbol found, write the operand and the address of the symbol into text record
                    if(strcmp(inp_oper,symbol)==0)
                    {
                        printf("%s%s^",opcode,symb_addr);
                        fprintf(foutput,"%s%s^",opcode,symb_addr);
                        break;
                    }
                    else
                        fscanf(fsymtab,"%s%s",symb_addr,symbol);
                }
             break;
            }
            else
                fscanf(foptab,"%s%s",op_mnemo,opcode);
        }
        // if opcode not in optab, check if it is either word or byte
        if((strcmp(mnemo,"BYTE")==0)||(strcmp(mnemo,"WORD")==0))
        {
            // if opcode=word, write it into text record
            if(strcmp(mnemo,"WORD")==0)
            {
                printf("0000%s^",inp_oper);
                fprintf(foutput,"0000%s^",inp_oper);
            }
            // else if opcode=byte, convert to object code and write to text record
            else
            {
                len=strlen(inp_oper);
                for(i=2;i<len;i++)
                {
                    printf("%d",inp_oper[i]);
                    fprintf(foutput,"%d",inp_oper[i]);
                }
                printf("^");
                fprintf(foutput,"^");
            }
        }

        // take the next input line
        fscanf(finp,"%s%s%s%s",addr,st_label,mnemo,inp_oper);

        // rewind is used to take the pointer to the starting of the file - here optab and symtab
        rewind(foptab);
        rewind(fsymtab);

        // copy input address into end_addr
        strcpy(end_addr,addr);
    }

    // write the end record into output file
    printf("\nE^00%s",start_addr);
    fprintf(foutput,"\nE^00%s",start_addr);

    // find the len of the program
    len=atoi(end_addr)-atoi(start_addr);

    // close the files
    fclose(finp);
    fclose(foptab);
    fclose(fsymtab);

    // rewind the output file to reach at the start, scan the text that is already written in output file
    rewind(foutput);
    fscanf(foutput,"%s",st_label);

    // convert len of program into string and write it along with header record and close output file
    itoa(len, st_len, 10);
    rewind(foutput);
    fprintf(foutput,"%s",st_label);
    fprintf(foutput,"%X\n",len);
    fclose(foutput);

    // output file generated and end of code
    printf("\n\n Output File Generated......");
    getch();
}