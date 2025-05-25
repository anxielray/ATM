#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <ctype.h>
#include <time.h>
#include "colors.h"

const char *RECORDS = "./data/records.txt";
const char *USERS = "./data/users.txt";
const char *NOTIFICATIONS = "./data/notifications.txt";
const char *TEMP_NOTIFICATIONS = "./data/temp_notif.txt";

/*Function will check for the available accounts based on the user's name
======================================================================================*/
int* AvailableAccounts(struct User *u, struct Record *r, int *accessibleAccounts) {
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        perror(RED"Error opening RECORDS file"RESET);
        return NULL;
    }

    char buffer[256];
    int current_size = 0;

    printf(BG_WHITE BYELLOW"\n\n\t\t====== Available Accounts ======\n\n"RESET);

    while (fgets(buffer, sizeof(buffer), pf) != NULL) {
        int accountNbr, recordUserId, usersID;
        char name[MAX_NAME_LENGTH];
        char date[20], country[30], phone[15], accountType[10];
        double amount;

        if (sscanf(buffer, "%d %d %49s %d %19s %29s %14s %lf %9s",
                   &accountNbr, &recordUserId, name, &usersID, date, country, phone, &amount, accountType) == 9) {
            if (strcmp(name, u->name) == 0) {
                if (current_size < 10) {
                    accessibleAccounts[current_size] = accountNbr;

                    printf(BG_YELLOW BLUE"\n\t=== Account #%d Details ===\n", accountNbr);
                    printf(BLUE"\tAccount Number: %d\n", accountNbr);
                    printf("\tAccount Holder: %s\n", name);
                    printf("\tAccount Type: %s\n", accountType);
                    printf("\tAccount Creation Date: %s\n", date);
                    printf("\tCountry: %s\n", country);
                    printf("\tPhone Number: %s\n", phone);
                    printf("\tCurrent Balance: $%.2f\n", amount);
                    printf(BG_YELLOW BLUE"\t===========================\n"RESET);

                    current_size++;
                } else {
                    printf(RED"\n\t\t✖ Maximum number of accounts reached.\n"RESET);
                    break;
                }
            }
        }
    }

    fclose(pf);

    if (current_size == 0) {
        printf(RED"\n\t\t✖ No accounts found for user %s.\n", u->name);
        printf(RESET"\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf(BG_YELLOW BLUE"\n\t\tEnter your choice: "RESET);
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 1)
        {
            mainMenu(*u);
        }
        else
        {
            exit(1);
        }
    }

    return accessibleAccounts;
}

/* Update account information: (country or phone number)
===================================================================================*/
void updateAccountInfo(struct User u)
{
    int accountNumber;
    struct Record r;
    int arr[10];
    struct Record records[100];

    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t====== Update Account Information ======\n");
    printf("\nYour available accounts:\n"RESET);
    AvailableAccounts(&u, &r, arr);
    
    printf(BG_YELLOW BLUE"\n\nPlease enter the account number you wish to update: "RESET);
    scanf("%d", &accountNumber);

    int count = 0;
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, RED"\n\t\t✖ Error opening records file.\n"RESET);
        return;
    }

    while (fscanf(pf, "%d %d %s %d %s %s %s %lf %s",
                  &r.accountNbr, &r.id, r.name, &r.userId,
                  r.date, r.country, r.phone, &r.amount,
                  r.accountType) == 9)
    {
        records[count++] = r;
    }
    fclose(pf);

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if ((records[i].accountNbr == accountNumber)&& strcmp(u.name, records[i].name) == 0)
        {
            found = 1;
            system("clear");
            printf(BG_YELLOW BLUE"\n\t\t====== Current Account Details ======\n");
            printf(BBLUE"\n\t\tAccount Number: %d"YELLOW, records[i].accountNbr);
            printf(BBLUE"\n\t\tName: %s"YELLOW, records[i].name);
            printf(BBLUE"\n\t\tCountry: %s"YELLOW, records[i].country);
            printf(BBLUE"\n\t\tPhone: %s"YELLOW, records[i].phone);
            printf(BBLUE"\n\t\tAccount Type: %s"YELLOW, records[i].accountType);
            printf(BBLUE"\n\t\tBalance: $%.2f"YELLOW, records[i].amount);
            printf(BBLUE"\n\t\tCreation Date: %s"YELLOW, records[i].date);

            double interestRate = 0.0;
            int tenureYears = 0;
            if (strcmp(records[i].accountType, "savings") == 0) {
                interestRate = 0.07;
                tenureYears = 1; 
            } else if (strcmp(records[i].accountType, "fixed01") == 0) {
                interestRate = 0.04;
                tenureYears = 1;
            } else if (strcmp(records[i].accountType, "fixed02") == 0) {
                interestRate = 0.05;
                tenureYears = 2;
            } else if (strcmp(records[i].accountType, "fixed03") == 0) {
                interestRate = 0.08;
                tenureYears = 3;
            }

            if (interestRate > 0.0) {
                double interest = records[i].amount * interestRate * tenureYears;
                if (strcmp(records[i].accountType, "savings") == 0) {
                    double monthlyInterest = interest / 12.0;
                    char day[3];
                    strncpy(day, records[i].date, 2);
                    day[2] = '\0';
                    printf(BG_WHITE BLUE"\n\n\t\tYou will get $%.2f as interest on day %s of every month", monthlyInterest, day);
                } else {
                    int day, month, year;
                    sscanf(records[i].date, "%d/%d/%d", &day, &month, &year);
                    year += tenureYears;
                    printf(BG_WHITE BLUE"\n\n\t\tYou will get $%.2f as interest on %02d/%02d/%04d"RESET, interest, day, month, year);
                }
            } else if (strcmp(records[i].accountType, "current") == 0) {
                printf(BG_WHITE BLUE"\n\n\t\tYou will not get interests because the account is of type current");
            } else {
                printf(RED"\n\n\t\tUnknown account type. Interest information not available."RESET);
            }
            
            printf(BG_YELLOW BLUE"\n\t\tWhat would you like to update?\n"RESET);
            printf(BBLUE"\n\t\t[1] Country");
            printf(BBLUE"\n\t\t[2] Phone Number");
            printf(BBLUE"\n\t\t[3] Both");
            printf(BBLUE"\n\t\t[4] Cancel\n");
            printf(BBLUE"\n\t\t[5] Exit\n");
            printf(BBLUE"\n\t\tEnter your choice (1-4): "RESET);
            
            int choice;
            scanf("%d", &choice);
            getchar();

            char newCountry[30], newPhone[15];
            switch(choice) {
                case 1:
                    printf(BG_YELLOW BLUE"\n\t\tEnter new country: "RESET);
                    fgets(newCountry, sizeof(newCountry), stdin);
                    newCountry[strcspn(newCountry, "\n")] = 0;
                    strncpy(records[i].country, newCountry, sizeof(records[i].country) - 1);
                    break;
                    
                case 2:
                    printf(BG_YELLOW BLUE"\n\t\tEnter new phone number: "RESET);
                    fgets(newPhone, sizeof(newPhone), stdin);
                    newPhone[strcspn(newPhone, "\n")] = 0;
                    strncpy(records[i].phone, newPhone, sizeof(records[i].phone) - 1);
                    break;
                    
                case 3:
                    printf(BG_YELLOW BLUE"\n\t\tEnter new country: "RESET);
                    fgets(newCountry, sizeof(newCountry), stdin);
                    newCountry[strcspn(newCountry, "\n")] = 0;
                    strncpy(records[i].country, newCountry, sizeof(records[i].country) - 1);
                    
                    printf(BG_YELLOW BLUE"\t\tEnter new phone number: "RESET);
                    fgets(newPhone, sizeof(newPhone), stdin);
                    newPhone[strcspn(newPhone, "\n")] = 0;
                    strncpy(records[i].phone, newPhone, sizeof(records[i].phone) - 1);
                    break;
                    
                case 4:
                    printf(BG_YELLOW BLUE"\n\t\t✖ Update cancelled.\n"RESET);
                    updateAccountInfo(u);
                    break;
                    
                case 5:
                    mainMenu(u);
                    break;
                
                default:
                    printf(RED"\n\t\t✖ Invalid choice.\n"RESET);
                    updateAccountInfo(u);
                    break;
            }
            break;
        }else {
            continue;
        }
    }

    if (!found)
    {
        printf(RED"\n\t\t✖ Account number %d not found or does not belong to you.\n"RESET, accountNumber);
        printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf(BG_YELLOW BLUE"\n\t\tEnter your choice: "RESET);
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 1)
        {
            mainMenu(u);
        }
        else
        {
            exit(1);
        }
    }

    pf = fopen(RECORDS, "w");
    if (pf == NULL)
    {
        fprintf(stderr, RED"\n\t\t✖ Error opening records file for writing.\n"RESET);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        if (i == 0) {
            fprintf(pf, "%d %d %s %d %s %s %s %.2f %s",
                    records[i].accountNbr, records[i].userId, records[i].name,
                    records[i].id, records[i].date, records[i].country,
                    records[i].phone, records[i].amount, records[i].accountType);
        } else {
            fprintf(pf, "\n\n%d %d %s %d %s %s %s %.2f %s",
                    records[i].accountNbr, records[i].userId, records[i].name,
                    records[i].id, records[i].date, records[i].country,
                    records[i].phone, records[i].amount, records[i].accountType);
        }
    }

    fclose(pf);

    system("clear");
    printf(BG_WHITE BGREEN"\n\n\t\t✔ Account information updated successfully!\n"RESET);
    printf("\n\t\t[1] Return to main menu");
    printf("\n\t\t[2] Exit\n");
    printf(BG_YELLOW BLUE"\n\t\tEnter your choice (1-2): "RESET);
    
    int returnChoice;
    scanf("%d", &returnChoice);

    if (returnChoice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf(BG_WHITE BCYAN"\n\t\tThank you for using our services, %s!\n\n"RESET, u.name);
        exit(0);
    }
}


/*function to check user accounts
==================================================================================*/
void checkAccountDetails(struct User u)
{
    struct Record r;
    int found = 0;
    
    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t====== Detailed Account Information ======\n\n"RESET);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, RED"\n\t\t✖ Error opening records file!\n"RESET);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), pf))
    {
        if (line[0] == '\n') continue;
        
        int result = sscanf(line, "%d %d %s %d %s %s %s %lf %s",
                    &r.accountNbr, &r.userId, r.name, &r.id,
                    r.date, r.country, r.phone, &r.amount,
                    r.accountType);
        
        if (result == 9 && strcmp(r.name, u.name) == 0)
        {
            printf(BG_YELLOW BLUE"\n\t=== Account #%d Details ===\n"RESET, r.accountNbr);
            printf(BBLUE"\tAccount Number: %d\n"YELLOW, r.accountNbr);
            printf(BBLUE"\tAccount Holder: %s\n"YELLOW, r.name);
            printf(BBLUE"\tAccount Type: %s\n"YELLOW, r.accountType);
            printf(BBLUE"\tAccount Creation Date: %s\n"YELLOW, r.date);
            printf(BBLUE"\tCountry: %s\n"YELLOW, r.country);
            printf(BBLUE"\tPhone Number: %s\n"YELLOW, r.phone);
            printf(BBLUE"\tCurrent Balance: $%.2f\n"YELLOW, r.amount);
            printf(BBLUE"\t===========================\n\n"RESET);
            found = 1;
        }
    }

    fclose(pf);

    if (!found)
    {
        printf(RED"\n\t\t✖ No accounts found for user %s.\n"RESET, u.name);
        printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf(BG_YELLOW BLUE"\n\t\tEnter your choice: "RESET);
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 1)
        {
            mainMenu(u);
        }
        else
        {
            exit(1);
        }
    }

    printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
    printf(BG_YELLOW BLUE"\n\t\tEnter your choice: "RESET);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        exit(1);
    }
}

/* Function to handle user transactions (deposits/withdrawals)
=======================================================================================*/
void makeTransaction(struct User u)
{
    FILE *fp;
    struct Record record;
    int accountNumber, transactionType;
    double amount;

    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Make Transaction                       ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n"RESET);

    fp = fopen(RECORDS, "r+");
    if (fp == NULL)
    {
        printf(RED"\n\t\t✖ Error opening records file.\n"RESET);
        exit(1);
    }

    printf(BG_YELLOW BLUE"\n\t\tEnter your account number: "RESET);
    scanf("%d", &accountNumber);

    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n') continue;
        
        int result = sscanf(line, "%d %d %s %d %s %s %s %lf %s",
                    &record.accountNbr, &record.userId, record.name, &record.id,
                    record.date, record.country, record.phone, &record.amount,
                    record.accountType);
        
        if (result == 9 && record.accountNbr == accountNumber && strcmp(record.name, u.name) == 0 || strcmp(record.accountType, "current") == 0 || strcmp(record.accountType, "savings") == 0)
        {
            found = 1;
            break;
        }
        else if (record.accountType != "current" && record.accountType != "savings" && record.accountType != "account")
        {
            printf(RED"\n\t\t✖ Transactions are not allowed on fixed type of accounts.\n"RESET);
            fclose(fp);
            printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
            printf("\n\n\t\tEnter your choice: "RESET);
            
            int choice;
            scanf("%d", &choice);
            if (choice == 1) mainMenu(u);
            else exit(0);
            return;
            return;
        }
    }

    if (!found)
    {
        printf(RED"\n\t\t✖ Account not found or does not belong to you.\n"RESET);
        fclose(fp);
        
        printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
        printf("\n\n\t\tEnter your choice: "RESET);
        
        int choice;
        scanf("%d", &choice);
        if (choice == 1) mainMenu(u);
        else exit(0);
        return;
    }

    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Account Details                        ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
    printf(BBLUE"\n\t\tAccount Number: %d"YELLOW, record.accountNbr);
    printf(BBLUE"\n\t\tAccount Holder: %s"YELLOW, record.name);
    printf(BBLUE"\n\t\tCurrent Balance: $%.2f\n"RESET, record.amount);

    printf(BG_YELLOW BLUE"\n\t\tChoose transaction type:\n"RESET);
    printf(BBLUE"\n\t\t[1] Deposit");
    printf(BBLUE"\n\t\t[2] Withdraw\n");
    printf(BWHITE"\n\t\t[3] Cancel\n");
    printf(BRED"\n\t\t[4] Exit\n");
    printf(BG_YELLOW BLUE"\n\t\tEnter your choice: "RESET);
    scanf("%d", &transactionType);

    if (transactionType == 1)
    {
        printf(BG_YELLOW BLUE"\n\t\tEnter amount to deposit: $"RESET);
        scanf("%lf", &amount);
        record.amount += amount;
        system("clear");
        printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                   Transaction Complete                   ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
        printf(BG_WHITE BGREEN"\n\t\t✔ Deposited: $%.2f"YELLOW, amount);
        printf(BG_WHITE BGREEN"\n\t\t✔ New balance: $%.2f\n"RESET, record.amount);
    }
    else if (transactionType == 2)
    {
        printf(BG_YELLOW BLUE"\n\t\tEnter amount to withdraw: $"RESET);
        scanf("%lf", &amount);
        if (amount <= record.amount)
        {
            record.amount -= amount;
            system("clear");
            printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║                   Transaction Complete                   ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
            printf(BG_WHITE BGREEN"\n\t\t✔ Withdrew: $%.2f"YELLOW, amount);
            printf(BG_WHITE BGREEN"\n\t\t✔ New balance: $%.2f\n"RESET, record.amount);
        }
        else
        {
            printf(RED"\n\t\t✖ Insufficient balance for withdrawal.\n"RESET);
        }
    }
    else if (transactionType == 3)
    {
        system("clear");
        printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                   Transaction Cancelled                   ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
        printf(YELLOW"\n\t\tTransaction cancelled.\n"RESET);
        makeTransaction(u);
    }
    else if (transactionType == 4)
    {
       mainMenu(u);
    }
    else
    {
        printf(RED"\n\t\t✖ Invalid choice.\n"RESET);
        fclose(fp);
        return;
    }


    rewind(fp);
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n') continue;
        
        int temp_accountNbr;
        sscanf(line, "%d", &temp_accountNbr);
        
        if (temp_accountNbr == accountNumber)
        {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "%d %d %s %d %s %s %s %.2f %s\n",
                    record.accountNbr, record.userId, record.name, record.id,
                    record.date, record.country, record.phone, record.amount,
                    record.accountType);
            break;
        }
    }

    fclose(fp);

    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: "RESET);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf(BG_WHITE BCYAN"\n\t\tThank you for using our services, %s!\n\n", u.name);
        exit(0);
    }
}

/* Function to remove an existing account
============================================================================*/
void removeAccount(struct User u)
{
    FILE *fp, *tempFile;
    struct Record record;
    int accountNumber;
    int found = 0;

    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Remove Account                         ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n"RESET);

    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf(RED"\n\t\t✖ Error opening records file.\n"RESET);
        exit(1);
    }

    tempFile = fopen("data/temp.txt", "w");
    if (tempFile == NULL)
    {
        printf(RED"\n\t\t✖ Error creating temporary file.\n"RESET);
        fclose(fp);
        exit(1);
    }

    printf(BG_YELLOW BLUE"\n\t\tEnter the account number to remove: "RESET);
    scanf("%d", &accountNumber);

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n') continue;
        
        int result = sscanf(line, "%d %d %s %d %s %s %s %lf %s",
                    &record.accountNbr, &record.userId, record.name, &record.id,
                    record.date, record.country, record.phone, &record.amount,
                    record.accountType);
        
        if (result == 9)
        {
            if (record.accountNbr != accountNumber || strcmp(record.name, u.name) != 0)
            {
                fprintf(tempFile, "%s", line);
            }
            else
            {
                found = 1;
            }
        }
    }

    fclose(fp);
    fclose(tempFile);

    if (found)
    {
        if (remove(RECORDS) == 0 && rename("temp.txt", RECORDS) == 0)
        {
            system("clear");
            printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║                   Account Removed                        ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
            printf(BG_WHITE BGREEN"\n\t\t✔ Account %d has been successfully removed.\n", accountNumber);
        }
        else
        {
            printf(RED"\n\t\t✖ Error updating records file.\n"RESET);
        }
    }
    else
    {
        remove("data/temp.txt");
        printf(RED"\n\t\t✖ Account not found or does not belong to you.\n"RESET);
    }

    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: "RESET);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf(BG_WHITE BCYAN"\n\t\tThank you for using our services, %s!\n\n"RESET, u.name);
        exit(0);
    }
}

/* Function to transfer account ownership
=================================================================================*/
void transferAccountOwnership(struct User u)
{
    FILE *fp, *tempFile, *userFile;
    struct Record record;
    struct User newOwner;
    char hashedInputPassword[50];
    int accountNumber;
    int newOwnerId = -1;
    int found = 0;

    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                Transfer Account Ownership                ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n"RESET);

    fp = fopen(RECORDS, "r");
    tempFile = fopen("data/temp.txt", "w");
    userFile = fopen(USERS, "r");

    if (fp == NULL || tempFile == NULL || userFile == NULL) {
        printf(RED"\n\t\t✖ Error accessing files.\n"RESET);
        if (fp) fclose(fp);
        if (tempFile) fclose(tempFile);
        if (userFile) fclose(userFile);
        return;
    }

    printf(BG_YELLOW BLUE"\n\t\tYour accounts:\n");
    printf("\n\t\t%-10s %-15s %-10s\n", "Account #", "Type", "Balance");
    printf("\t\t----------------------------------------\n"RESET);
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;
        
        struct Record temp;
        if (sscanf(line, "%d %d %s %d %s %s %s %lf %s",
            &temp.accountNbr, &temp.userId, temp.name, &temp.id,
            temp.date, temp.country, temp.phone, &temp.amount,
            temp.accountType) == 9) {
            
            if (strcmp(temp.name, u.name) == 0) {
                printf("\t\t%-10d %-15s $%.2f\n", 
                       temp.accountNbr, temp.accountType, temp.amount);
            }
        }
    }

    printf(BG_YELLOW BLUE"\n\t\tEnter the account number to transfer: "RESET);
    scanf("%d", &accountNumber);
    getchar();

    rewind(fp);
    found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') continue;
        
        if (sscanf(line, "%d %d %s %d %s %s %s %lf %s",
            &record.accountNbr, &record.userId, record.name, &record.id,
            record.date, record.country, record.phone, &record.amount,
            record.accountType) == 9) {
            
            if (record.accountNbr == accountNumber) {
                if (strcmp(record.name, u.name) != 0) {
                    system("clear");
                    printf(BG_YELLOW RED"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
                    printf("\n\t\t║                      WARNING                             ║");
                    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
                    printf(RED"\n\t\t✖ This account (#%d) doesn't belong to you!\n"RESET, accountNumber);
                    printf("\n\t\tPress Enter to continue...");
                    getchar();
                    fclose(fp);
                    fclose(tempFile);
                    fclose(userFile);
                    remove("data/temp.txt");
                    mainMenu(u);
                    return;
                }
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        system("clear");
        printf(RED"\n\t\t✖ Account not found!\n"RESET);
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("data/temp.txt");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    printf(BG_YELLOW BLUE"\n\t\tEnter the new owner's username: "RESET);
    scanf("%49s", newOwner.name);
    getchar();
    
    struct User existingUser;
    int newOwnerFound = 0;
    char inputPassword[50];
    
    rewind(userFile);
    while (fscanf(userFile, "%d %s %s", &existingUser.id, existingUser.name, existingUser.password) != EOF) {
        if (strcmp(existingUser.name, newOwner.name) == 0) {
            newOwnerId = existingUser.id;
            newOwnerFound = 1;
            strcpy(newOwner.password, existingUser.password);
            break;
        }
    }

    if (!newOwnerFound) {
        system("clear");
        printf(BG_YELLOW RED"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                      WARNING                             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
        printf(RED"\n\t\t✖ User '%s' is not registered in the system!\n", newOwner.name);
        printf("\n\t\t✖ Transfer can only be made to registered users.\n"RESET);
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("data/temp.txt");
        
        printf("\n\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    printf(BG_YELLOW BLUE"\n\t\tPlease ask %s to enter their password to confirm the transfer: "BLACK, newOwner.name);
    scanf("%49s", inputPassword);
    getchar();

    hashPassword(inputPassword, hashedInputPassword);

    if (strcmp(hashedInputPassword, existingUser.password) != 0) {
        system("clear");
        printf(BG_YELLOW RED"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                      WARNING                             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
        printf(RED"\n\t\t✖ Incorrect password! Transfer cancelled.\n");
        printf("\n\t\t✖ The new owner must consent to the transfer.\n"RESET);
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("data/temp.txt");
        
        printf("\n\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    rewind(fp);
    tempFile = freopen("data/temp.txt", "w", tempFile);
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n') {
            fprintf(tempFile, "\n");
            continue;
        }
        
        struct Record currentRecord;
        if (sscanf(line, "%d %d %s %d %s %s %s %lf %s",
            &currentRecord.accountNbr, &currentRecord.userId, currentRecord.name, 
            &currentRecord.id, currentRecord.date, currentRecord.country, 
            currentRecord.phone, &currentRecord.amount, currentRecord.accountType) == 9) {
            
            if (currentRecord.accountNbr == accountNumber) {
                fprintf(tempFile, "%d %d %s %d %s %s %s %.2f %s\n",
                        currentRecord.accountNbr,
                        newOwnerId,
                        newOwner.name,
                        newOwnerId,
                        currentRecord.date,
                        currentRecord.country,
                        currentRecord.phone,
                        currentRecord.amount,
                        currentRecord.accountType);
            } else {
                fprintf(tempFile, "%s", line);
            }
        }
    }

    fclose(fp);
    fclose(tempFile);
    fclose(userFile);

    if (remove(RECORDS) == 0 && rename("temp.txt", RECORDS) == 0) {
        system("clear");
        printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║              Transfer Completed Successfully             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n"RESET);
        printf(BG_WHITE BGREEN"\n\t\t✔ Account #%d has been transferred to %s.\n"RESET, accountNumber, newOwner.name);
        
        time_t now = time(NULL);                    
        struct tm *t = localtime(&now);             

        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

        FILE *notifFile = fopen(NOTIFICATIONS, "a");
        if (notifFile) {
            fprintf(notifFile, BG_WHITE BCYAN"[%s] %s: You have received account #%d from %s.\n"RESET,
                    timestamp, newOwner.name, accountNumber, u.name);
            fclose(notifFile);
        }


    } else {
        printf(RED"\n\t\t✖ Error updating records.\n"RESET);
    }

    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: "RESET);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        mainMenu(u);
    } else {
        printf(BG_WHITE BCYAN"\n\t\tThank you for using our services, %s!\n\n"RESET, u.name);
        exit(0);
    }
}

/* Function to determine the next account number
===================================================================*/
int countLinesInFile()
{
    FILE *fp;
    int lineCount = 0;
    char ch;

    fp = fopen(USERS, "r");
    if (fp == NULL)
    {
        perror(RED"Error opening file"RESET);
        return -1;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            lineCount++;
        }
    }

    if (lineCount > 0 || (lineCount == 0 && ch != EOF))
    {
        lineCount++;
    }

    return lineCount;
}

/* Function to validate the input of a user if is a valid user input
=================================================================================*/
int isValidUsername(const char *username)
{

    if (username[0] == '\0')
        return 0;
    for (int i = 0; username[i] != '\0'; i++)
    {
        if (!isalnum(username[i]))
        {
            return 0;
        }
    }
    return 1;
}

/*Function fetches the user ID based on the username
=================================================================================*/
int getUserIdByName(struct User u)
{
    FILE *file;
    int id;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASS_LENGTH];
    int lastId = -1;

    file = fopen(USERS, "r");
    if (file == NULL)
    {
        perror(RED"\n\n\t\t\t\tError: Unable to open file\n"RESET);
        return -1;
    }

    while (fscanf(file, "%d %49s %64s", &id, name, password) == 3)
    {
        lastId = id;
        if (strcmp(name, u.name) == 0)
        {
            fclose(file);
            return id;
        }
    }

    fclose(file);

    return lastId + 1;
}

/*
Function to retrieve account numbers that a particular user in the u.name should be able to access
===================================================================================================*/
void checkAllAccounts(struct User u)
{
    struct Record r;
    int found = 0;
    
    system("clear");
    printf(BG_YELLOW BLUE"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║              All Account Information                     ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n"RESET);

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, RED"\n\t\t✖ Error opening records file!\n"RESET);
        return;
    }

    printf(BG_YELLOW BLUE"\t╔═════════╦═══════════════════╦═══════════════╦═══════════════╦═══════════════╦═══════════╗\n");
    printf("\t║ Acc No. ║      Name         ║    Country    ║     Phone     ║    Balance    ║   Type    ║\n");
    printf("\t╠═════════╬═══════════════════╬═══════════════╬═══════════════╬═══════════════╬═══════════╣\n"RESET);

    char line[256];
    while (fgets(line, sizeof(line), pf))
    {
        if (line[0] == '\n') continue;
        
        int result = sscanf(line, "%d %d %s %d %s %s %s %lf %s",
                    &r.accountNbr, &r.userId, r.name, &r.id,
                    r.date, r.country, r.phone, &r.amount,
                    r.accountType);
        
        if (result == 9 && strcmp(r.name, u.name) == 0)
        {
            printf(BG_YELLOW BLUE"\t║ %-7d ║ %-17s ║ %-13s ║ %-13s ║ $%-12.2f ║ %-9s ║\n"RESET,
                   r.accountNbr, r.name, r.country, r.phone, r.amount, r.accountType);
            found = 1;
        }
    }

    if (found)
    {
        printf(BG_YELLOW BLUE"\t╚═════════╩═══════════════════╩═══════════════╩═══════════════╩═══════════════╩═══════════╝\n"RESET);
    }
    else
    {
        printf(BG_YELLOW BLUE"\t╚═════════╩═══════════════════╩═══════════════╩═══════════════╩═══════════════╩═══════════╝\n"RESET);
        printf(RED"\n\t\t✖ No accounts found for user %s.\n"RESET, u.name);
    }

    printf(BG_YELLOW RED"\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝"RESET);
    printf(BG_YELLOW BLUE"\n\n\t\tEnter your choice: "RESET);
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf("\n\t\tThank you for using our services %s!\n\n", u.name);
        exit(0);
    }
}

/*Function to send notifications to the new owners account
=======================================================================================*/
void checkNotifications(char *username) {
    FILE *notifFile = fopen(NOTIFICATIONS, "r");
    FILE *tempFile = fopen(TEMP_NOTIFICATIONS, "w");
    char line[256];

    if (!notifFile || !tempFile) return;

    printf(BG_BLACK BYELLOW"\n\t\t🔔 Notifications for %s:\n", username);
    int hasNotification = 0;

    while (fgets(line, sizeof(line), notifFile)) {
        if (strstr(line, username)) {
            printf("\t\t- %s", line);
            hasNotification = 1;
        } else {
            fputs(line, tempFile);
        }
    }

    fclose(notifFile);
    fclose(tempFile);
    remove(NOTIFICATIONS);
    rename(TEMP_NOTIFICATIONS, NOTIFICATIONS);

    if (!hasNotification)
        printf(YELLOW"\t\t(No new notifications)\n"RESET);
}

