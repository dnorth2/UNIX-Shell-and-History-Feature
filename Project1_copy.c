//Deondre North
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LINE 80 /* The maximum length command */
#define MAX_HISTORY_SIZE 10

int historyCount = -1; // use command count
char *commandLineHistory[MAX_HISTORY_SIZE];

void saveToHistory(char *instruction)
{

    if (historyCount != MAX_HISTORY_SIZE - 1)
    {
        // printf("in here\n");

        historyCount = historyCount + 1;
        // printf("%d\n", historyCount);
        commandLineHistory[historyCount] = instruction;
    }
    else if (historyCount == MAX_HISTORY_SIZE - 1)
    {
        int x;
        for (x = 1; x < MAX_HISTORY_SIZE; x++)
            commandLineHistory[x - 1] = commandLineHistory[x];
        commandLineHistory[MAX_HISTORY_SIZE - 1] = instruction;
    }
}

int main(void)
{

    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char copyOfCommands[100][15];
    int should_run = 1; /* flag to determine when to exit program */
    int argc;
    char commandLineArgs[MAX_LINE];
    bool andpersand = false;
    int i = 0;
    int k;

    bool dontsave = false;
    // char commands[100][];
    char *input;
    char *tempstring;
    while (should_run)
    {
        i = 0;
        int j = 0;
        printf("North PID>"); // use last name_process id
        fflush(stdout);
        fgets(commandLineArgs, MAX_LINE, stdin);
        size_t len = strlen(commandLineArgs);
        k = 0;
        if (len > 0 && commandLineArgs[len - 1] == '\n') // gets rid of new line
        {
            commandLineArgs[--len] = '\0';
        }

        j++;
        if (strchr(commandLineArgs, '&') != NULL)
        {
            andpersand = true;
        }
        const char space[2] = " ";
        args[i] = strtok(commandLineArgs, space);
        // for (int k = i; args[k] != NULL; k++)
        // {
        //     args[k] = strtok(NULL, " ");
        //     j++;
        // }
        while (args[i] != NULL)
        {
            i++;
            k++;
            args[i] = strtok(NULL, space);
        }

        input = commandLineArgs;
        tempstring = malloc((strlen(input)) * sizeof(char));
        strcpy(tempstring, input);
        if (!strcmp(commandLineArgs, "exit"))
        {
            should_run = 0;
            return 1;
        }

        if (strchr(commandLineArgs, '&') != NULL)
        {
            andpersand = true;
        }
        if (!strcmp(commandLineArgs, "history"))
        {
            dontsave = true;
            if (historyCount != -1)
            {
                for (int i = 0; i < historyCount + 1; i++)
                {
                    printf("%d: %s \n", i + 1, commandLineHistory[i]);
                }
            }
            else
            {
                printf("No Commands in History\n");
            }
        }
        if (!strcmp(commandLineArgs, "!!"))
        {
            dontsave = true;
            if (historyCount + 1> 0)
            {
                printf("%s\n", commandLineHistory[historyCount]);
            }
            else
            {
                printf("No Commands in History\n");
            }
        }
        else if(strchr(commandLineArgs, '!') != NULL)
        {
            dontsave = true;
            char num;
            int myIndex;
            for(int i = 0; i < strlen(commandLineArgs); i++)
            {
                if(commandLineArgs[i] == '!')
                {
                    num = commandLineArgs[i+1];
                }
            }
            myIndex = atoi(&num);
            if(myIndex > historyCount + 1)
            {
                printf("No such command in history.\n");
            }
            else if (historyCount + 1> 0)
            {
                printf("%s\n", commandLineHistory[myIndex-1]);
            }

        }
        if(dontsave == false)
        {
            saveToHistory(tempstring);
        }

        // printf("%d\n", andpersand);
        if (andpersand == true)
        {
            int rc = fork();
            if (rc < 0)
            {
                printf("Fork Failed\n");
                return -1;
            }
            else if (rc == 0)
            {
                args[k - 1] = NULL;
                // saveToHistory(args[0]);
                execvp(args[0], args);
                // memset(args, 0, sizeof(args));
                //   save to the history
            }
            else
            {
                printf("\n");
            }
        }
        else
        {
            int rc = fork();
            if (rc < 0)
            {
                printf("Fork Failed\n");
                return -1;
            }
            else if (rc == 0)
            {
                execvp(args[0], args);
                // printf("hello");
                memset(args, 0, sizeof(args));
                // saving history
            }
            else
            {
                wait(NULL);
            }
        }
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will not invoke wait()
         */
        andpersand = false;
        dontsave = false;

    }
    free(input);
    free(tempstring);

    return 0;
}
