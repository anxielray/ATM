#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "header.h"
#include "colors.h"


#define MAX_TYPE_LENGTH 10

/*The main body of the system menu comprising of 9 options
=============================================================================*/
void mainMenu(struct User u)
{
    struct User new_usr;
    int option;
    system("clear");
    checkNotifications(u.name);
    printf(BYELLOW"\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t\t\t\t\t║                           ATM                            ║");
    printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n");
    printf(RESET"\n\t\t\t\t\t-->> Feel free to choose one of the options below <<--\n");
    printf(BLUE"\n\t\t\t\t\t\t-->> Welcome %s <<--\n", u.name);
    printf(RESET"\n\n\n\t\t\t\t\t[1]- Create a new account\n");
    printf("\n\t\t\t\t\t[2]- Update account information\n");
    printf("\n\t\t\t\t\t[3]- Check accounts\n");
    printf("\n\t\t\t\t\t[4]- Check list of owned accounts\n");
    printf("\n\t\t\t\t\t[5]- Make Transaction\n");
    printf("\n\t\t\t\t\t[6]- Remove existing account\n");
    printf("\n\t\t\t\t\t[7]- Transfer ownership\n");
    printf("\n\t\t\t\t\t[8]- Logout\n");
    printf(BRED"\n\t\t\t\t\t[9]- Exit\n\n\n\t\t\t\t\t"RESET);

    if (scanf("%d", &option) != 1)
    {
        printf(RED"Invalid input. Please enter a valid option.\n"RESET);
        return;
    }

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccountInfo(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferAccountOwnership(u);
        break;
    case 8:
        printf(BCYAN"\n\t\t\tIt was nice having you around %s!\n\n"RESET, u.name);
        initMenu(&new_usr);
        break;
    case 9:
        printf(BCYAN"\n\t\t\tIt was nice having you around %s!\n\n"RESET, u.name);
        exit(0);
        break;
    default:
        printf(RED"Invalid operation!\n"RESET);
        break;
    }
}

/*initializing menu for a login or registration into the system
=============================================================================*/
void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf(BYELLOW "\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t\t\t\t\t║                   Welcome to ATM                         ║");
    printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
    printf(BLUE"\n\t\t\t\t\t-->> Feel free to login / register  <<--\n"RESET);
    printf("\n\n\n\t\t\t\t\t[1]- login\n");
    printf("\n\t\t\t\t\t[2]- register\n");
    printf("\n\t\t\t\t\t[3]- exit\n\t\t");

    while (!r)
    {
        if (scanf("%d", &option) != 1)
        {
            printf(RED"Invalid input. Please enter a valid option.\n\n\t"RESET);
            while (getchar() != '\n');
            continue;
        }

        switch (option)
        {
        case 1:
            login(*u);
            break;
        case 2:
            registerMenu();
            r = 1;
            break;
        case 3:
            printf(BCYAN"\n\t\t\tIt was nice having you around!\n\n"RESET);
            exit(0);
            break;
        default:
            printf(RED"\n\t\t\t\tInsert a valid operation!\n\n"RESET);
            continue;
        }
    }
}

/*Starting point of the program
=============================================================================*/
int main()
{
    struct User u;

    initMenu(&u);

    return 0;
}

/*Function to extract the current data
//=============================================================================*/
void getCurrentDate(struct Record *r)
{
    time_t t;
    struct tm *tm;

    if (time(&t) == -1) {
        fprintf(stderr, RED"Error getting current time\n"RESET);
        strcpy(r->date, "00/00/0200");
        return;
    }

    tm = localtime(&t);
    if (tm == NULL) {
        fprintf(stderr, RED"Error converting time to local time\n"RESET);
        strcpy(r->date, "00/00/0700");
        return;
    }

    if (strftime(r->date, sizeof(r->date), "%d/%m/%Y", tm) == 0) {
        fprintf(stderr, RED"Error formatting date string\n"RESET);
        strcpy(r->date, "00/00/0800");
    }
}

/* Function to handle acount creation
=============================================================================*/
void createNewAcc(struct User u)
{
    FILE *fp;
    struct Record newRecord;
    char currentDate[11];
    char Country[MAX_NAME_LENGTH];

    fp = fopen(RECORDS, "a+");
    if (fp == NULL)
    {
        printf(RED"Error opening records file.\n"RESET);
        exit(1);
    }

    printf(BRED"\t! State only one name for each field in this section.\n\n");

    printf(BG_YELLOW BLUE"\tEnter phone number: "RESET);
    scanf("%s", newRecord.phone);


    strcpy(newRecord.name, u.name);
    printf(BG_YELLOW BLUE"\tEnter country: "RESET);
    scanf("%49s", newRecord.country);

    selectAccountType(newRecord.accountType);

    printf(BG_YELLOW BLUE"\tEnter amount to deposit in (Dollars): "RESET);
    scanf("%lf", &newRecord.amount);

    newRecord.userId = getUserIdByName(u);
    newRecord.id = rand() % 1000;
    const char *filename = RECORDS;
    newRecord.accountNbr = getNextAccountNumber(filename);

    getCurrentDate(&newRecord);

    fprintf(fp, "\n%d %d %s %d %s %s %s %.2lf %s\n",
            newRecord.accountNbr,
            newRecord.userId,
            newRecord.name,
            newRecord.id,
            newRecord.date,
            newRecord.country,
            newRecord.phone,
            newRecord.amount,
            newRecord.accountType);

    fclose(fp);

    printf(BG_WHITE BGREEN"\n\n\n\t\t\t\t✔ Congratulations! Your account was created successfully!\n\n\n\t\t\t\tYour account number is %d", newRecord.accountNbr);
    printf(RESET"\n\n\n\t\t\t\t[1] Return back to main menu.\t\t[2] Exit\n");
    int choice;
    scanf("%d", &choice);

    while (1)
    {
        if (choice == 1)
        {
            mainMenu(u);
            break;
        }
        else if (choice == 2)
        {
            printf(BG_WHITE CYAN"\n\t\t\t\tIt was nice having you around %s!\n\n"RESET, u.name);
            exit(0);
            break;
        }
        else
        {
            while (getchar() != '\n')
            {
            }
            printf(RED"\t\t\t\tInvalid input! Please enter a number (1 or 2).\n"RESET);
            continue;
        }
    }
}

/*Function to handle the selection of an account type during an account creation
========================================================================================*/
void selectAccountType(char *accountType) {
    int choice;
    const char *accountTypes[] = {"savings", "current", "fixed01", "fixed02", "fixed03"};
    int numTypes = sizeof(accountTypes) / sizeof(accountTypes[0]);

    printf(BG_YELLOW BLUE"\tSelect account type by number:\n"RESET);
    for (int i = 0; i < numTypes; i++) {
        printf(BG_WHITE MAGENTA"\t%d. %s\n"RESET, i + 1, accountTypes[i]);
    }
    printf(BG_YELLOW BLUE"\n\tEnter your choice (1-%d): "RESET, numTypes);

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numTypes) {
        printf(RED"\n\t\t✖ Invalid choice. Please try again.\n"RESET);
        while (getchar() != '\n');
        selectAccountType(accountType);
    } else {
        strncpy(accountType, accountTypes[choice - 1], MAX_TYPE_LENGTH - 1);
        accountType[MAX_TYPE_LENGTH - 1] = '\0';
    }
}


/*Function to calculate the account number
=============================================================================*/
int countEmptyLines(const char *filename)
{
    FILE *fp;
    int emptyLineCount = 0;
    char line[1024];

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror(RED"Error opening file"RESET);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int isEmpty = 1;

        for (int i = 0; line[i] != '\0'; i++)
        {
            if (!isspace((unsigned char)line[i]))
            {
                isEmpty = 0;
                break;
            }
        }

        if (isEmpty)
        {
            emptyLineCount++;
        }
    }

    return emptyLineCount;
}

/* Function to get the next account number based on existing records
=============================================================================*/
int getNextAccountNumber(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1;
    }

    int maxAccountNumber = 0;
    char line[1024];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }

        int accountNumber;
        if (sscanf(line, "%d", &accountNumber) == 1) {
            if (accountNumber > maxAccountNumber) {
                maxAccountNumber = accountNumber;
            }
        }
    }

    fclose(fp);
    return maxAccountNumber + 1;
}
