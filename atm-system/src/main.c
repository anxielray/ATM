#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

#define MAX_TYPE_LENGTH 10

/*The main body of the system menu
=============================================================================*/
void mainMenu(struct User u)
{
    struct User new_usr;
    int option;
    system("clear");
    checkNotifications(u.name);
    printf("\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t\t\t\t\t║                           ATM                            ║");
    printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n");
    printf("\n\t\t\t\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t\t\t\t\t-->> Welcome %s <<--\n", u.name);
    printf("\n\n\n\t\t\t\t\t[1]- Create a new account\n");
    printf("\n\t\t\t\t\t[2]- Update account information\n");
    printf("\n\t\t\t\t\t[3]- Check accounts\n");
    printf("\n\t\t\t\t\t[4]- Check list of owned accounts\n");
    printf("\n\t\t\t\t\t[5]- Make Transaction\n");
    printf("\n\t\t\t\t\t[6]- Remove existing account\n");
    printf("\n\t\t\t\t\t[7]- Transfer ownership\n");
    printf("\n\t\t\t\t\t[8]- Logout\n");
    printf("\n\t\t\t\t\t[9]- Exit\n\n\n\t\t\t\t\t");

    if (scanf("%d", &option) != 1)
    {
        printf("Invalid input. Please enter a valid option.\n");
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
        printf("\n\t\t\tIt was nice having you around %s!\n\n", u.name);
        initMenu(&new_usr);
        break;
    case 9:
        printf("\n\t\t\tIt was nice having you around %s!\n\n", u.name);
        exit(0);
        break;
    default:
        printf("Invalid operation!\n");
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
    printf("\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t\t\t\t\t║                   Welcome to ATM                         ║");
    printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n");
    printf("\n\t\t\t\t\t-->> Feel free to login / register  <<--\n");
    printf("\n\n\n\t\t\t\t\t[1]- login\n");
    printf("\n\t\t\t\t\t[2]- register\n");
    printf("\n\t\t\t\t\t[3]- exit\n\t");

    while (!r)
    {
        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input. Please enter a valid option.\n\n\t");
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
            printf("\n\t\t\tIt was nice having you around!\n\n");
            exit(0);
            break;
        default:
            printf("\n\t\t\t\tInsert a valid operation!\n\n");
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
        fprintf(stderr, "Error getting current time\n");
        strcpy(r->date, "00/00/0200");
        return;
    }

    tm = localtime(&t);
    if (tm == NULL) {
        fprintf(stderr, "Error converting time to local time\n");
        strcpy(r->date, "00/00/0700");
        return;
    }

    if (strftime(r->date, sizeof(r->date), "%d/%m/%Y", tm) == 0) {
        fprintf(stderr, "Error formatting date string\n");
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
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("\t!State only one name for each field in this section.\n\n");

    printf("\tEnter phone number: ");
    scanf("%s", newRecord.phone);


    strcpy(newRecord.name, u.name);
    printf("\tEnter country: ");
    scanf("%49s", newRecord.country);

    selectAccountType(newRecord.accountType);

    printf("\tEnter amount to deposit in (Dollars): ");
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

    printf("\n\n\n\t\t\t\t✔ Congratulations! Your account was created successfully!\n\n\n\t\t\t\tYour account number is %d", newRecord.accountNbr);
    printf("\n\n\n\t\t\t\t[1] Return back to main menu.\t\t[2] Exit\n");
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
            printf("\n\t\t\t\tIt was nice having you around %s!\n\n", u.name);
            exit(0);
            break;
        }
        else
        {
            while (getchar() != '\n')
            {
            }
            printf("\t\t\t\tInvalid input! Please enter a number (1 or 2).\n");
            continue;
        }
    }
}


void selectAccountType(char *accountType) {
    int choice;
    const char *accountTypes[] = {"savings", "current", "fixed01", "fixed02", "fixed03"};
    int numTypes = sizeof(accountTypes) / sizeof(accountTypes[0]);

    printf("\tSelect account type by number:\n");
    for (int i = 0; i < numTypes; i++) {
        printf("\t%d. %s\n", i + 1, accountTypes[i]);
    }
    printf("\n\tEnter your choice (1-%d): ", numTypes);

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > numTypes) {
        printf("\n\t\t✖ Invalid choice. Please try again.\n");
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
        perror("Error opening file");
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
