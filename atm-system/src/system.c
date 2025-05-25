#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <ctype.h>
#include <time.h>



const char *RECORDS = "./data/records.txt";
const char *USERS = "./data/users.txt";
const char *NOTIFICATIONS = "./data/notifications.txt";
const char *TEMP_NOTIFICATIONS = "./data/temp_notif.txt";

int* AvailableAccounts(struct User *u, struct Record *r, int *accessibleAccounts) {
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        perror("Error opening RECORDS file");
        return NULL;
    }

    char buffer[256];
    int current_size = 0;

    printf("\n\n\t\t====== Available Accounts ======\n\n");

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

                    printf("\n\t=== Account #%d Details ===\n", accountNbr);
                    printf("\tAccount Number: %d\n", accountNbr);
                    printf("\tAccount Holder: %s\n", name);
                    printf("\tAccount Type: %s\n", accountType);
                    printf("\tAccount Creation Date: %s\n", date);
                    printf("\tCountry: %s\n", country);
                    printf("\tPhone Number: %s\n", phone);
                    printf("\tCurrent Balance: $%.2f\n", amount);
                    printf("\t===========================\n");

                    current_size++;
                } else {
                    printf("\n\t\t✖ Maximum number of accounts reached.\n");
                    break;
                }
            }
        }
    }

    fclose(pf);

    if (current_size == 0) {
        printf("\n\t\t✖ No accounts found for user %s.\n", u->name);
        printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf("\n\t\tEnter your choice: ");
        
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


/* Helper function to save account details to the file
=============================================================================*/
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s, %d %s %s %s %lf %s\n",
            r.accountNbr,
            r.userId,
            u.name,
            u.id,
            r.date,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

/* Helper function to retrieve an account from the records file
================================================================================================*/
int getAccountFromFile(FILE *ptr, char name[MAX_NAME_LENGTH], struct Record *r)
{
    return fscanf(ptr, "%d %d %49s %d %9s %s %s %lf %s",
                  &r->accountNbr,
                  &r->id,
                  r->name,
                  &r->userId,
                  r->date,
                  r->country,
                  r->phone,
                  &r->amount,
                  r->accountType) != EOF;
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
    printf("\n\n\t\t====== Update Account Information ======\n");
    printf("\nYour available accounts:\n");
    AvailableAccounts(&u, &r, arr);
    
    printf("\n\nPlease enter the account number you wish to update: ");
    scanf("%d", &accountNumber);

    int count = 0;
    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, "\n\t\t✖ Error opening records file.\n");
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
        printf("Hello: %d\n", i);
        if ((records[i].accountNbr == accountNumber)&& strcmp(u.name, records[i].name) == 0)
        {
            found = 1;
            system("clear");
            printf("\n\t\t====== Current Account Details ======\n");
            printf("\n\t\tAccount Number: %d", records[i].accountNbr);
            printf("\n\t\tName: %s", records[i].name);
            printf("\n\t\tCountry: %s", records[i].country);
            printf("\n\t\tPhone: %s", records[i].phone);
            printf("\n\t\tAccount Type: %s", records[i].accountType);
            printf("\n\t\tBalance: $%.2f", records[i].amount);
            printf("\n\t\tCreation Date: %s", records[i].date);

            double interestRate = 0.0;
            int tenureYears = 0;
            if (strcmp(records[i].accountType, "savings") == 0) {
                interestRate = 0.07;
                tenureYears = 1; // For calculation purposes
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
                    printf("\n\n\t\tYou will get $%.2f as interest on day %s of every month", monthlyInterest, day);
                } else {
                    int day, month, year;
                    sscanf(records[i].date, "%d/%d/%d", &day, &month, &year);
                    year += tenureYears;
                    printf("\n\n\t\tYou will get $%.2f as interest on %02d/%02d/%04d", interest, day, month, year);
                }
            } else if (strcmp(records[i].accountType, "current") == 0) {
                printf("\n\n\t\tYou will not get interests because the account is of type current");
            } else {
                printf("\n\n\t\tUnknown account type. Interest information not available.");
            }
            
            printf("\n\t\tWhat would you like to update?\n");
            printf("\n\t\t[1] Country");
            printf("\n\t\t[2] Phone Number");
            printf("\n\t\t[3] Both");
            printf("\n\t\t[4] Cancel\n");
            printf("\n\t\t[5] Exit\n");
            printf("\n\t\tEnter your choice (1-4): ");
            
            int choice;
            scanf("%d", &choice);
            getchar();

            char newCountry[30], newPhone[15];
            switch(choice) {
                case 1:
                    printf("\n\t\tEnter new country: ");
                    fgets(newCountry, sizeof(newCountry), stdin);
                    newCountry[strcspn(newCountry, "\n")] = 0;
                    strncpy(records[i].country, newCountry, sizeof(records[i].country) - 1);
                    break;
                    
                case 2:
                    printf("\n\t\tEnter new phone number: ");
                    fgets(newPhone, sizeof(newPhone), stdin);
                    newPhone[strcspn(newPhone, "\n")] = 0;
                    strncpy(records[i].phone, newPhone, sizeof(records[i].phone) - 1);
                    break;
                    
                case 3:
                    printf("\n\t\tEnter new country: ");
                    fgets(newCountry, sizeof(newCountry), stdin);
                    newCountry[strcspn(newCountry, "\n")] = 0;
                    strncpy(records[i].country, newCountry, sizeof(records[i].country) - 1);
                    
                    printf("\t\tEnter new phone number: ");
                    fgets(newPhone, sizeof(newPhone), stdin);
                    newPhone[strcspn(newPhone, "\n")] = 0;
                    strncpy(records[i].phone, newPhone, sizeof(records[i].phone) - 1);
                    break;
                    
                case 4:
                    printf("\n\t\t✖ Update cancelled.\n");
                    updateAccountInfo(u);
                    break;
                    
                case 5:
                    mainMenu(u);
                    break;
                
                default:
                    printf("\n\t\t✖ Invalid choice.\n");
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
        printf("\n\t\t✖ Account number %d not found or does not belong to you.\n", accountNumber);
        printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf("\n\t\tEnter your choice: ");
        
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
        fprintf(stderr, "\n\t\t✖ Error opening records file for writing.\n");
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
    printf("\n\n\t\t✔ Account information updated successfully!\n");
    printf("\n\t\t[1] Return to main menu");
    printf("\n\t\t[2] Exit\n");
    printf("\n\t\tEnter your choice (1-2): ");
    
    int returnChoice;
    scanf("%d", &returnChoice);

    if (returnChoice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf("\n\t\tThank you for using our services, %s!\n\n", u.name);
        exit(0);
    }
}


// =========function to check user accounts =======
void checkAccountDetails(struct User u)
{
    struct Record r;
    int found = 0;
    
    system("clear");
    printf("\n\n\t\t====== Detailed Account Information ======\n\n");

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, "\n\t\t✖ Error opening records file!\n");
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
            printf("\n\t=== Account #%d Details ===\n", r.accountNbr);
            printf("\tAccount Number: %d\n", r.accountNbr);
            printf("\tAccount Holder: %s\n", r.name);
            printf("\tAccount Type: %s\n", r.accountType);
            printf("\tAccount Creation Date: %s\n", r.date);
            printf("\tCountry: %s\n", r.country);
            printf("\tPhone Number: %s\n", r.phone);
            printf("\tCurrent Balance: $%.2f\n", r.amount);
            printf("\t===========================\n\n");
            found = 1;
        }
    }

    fclose(pf);

    if (!found)
    {
        printf("\n\t\t✖ No accounts found for user %s.\n", u.name);
        printf("\n\n\t\t[1] Return to main menu\t\t[2] Exit\n");
        printf("\n\t\tEnter your choice: ");
        
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
    printf("\n\t\tEnter your choice: ");
    
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
    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Make Transaction                       ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n");

    fp = fopen(RECORDS, "r+");
    if (fp == NULL)
    {
        printf("\n\t\t✖ Error opening records file.\n");
        exit(1);
    }

    printf("\n\t\tEnter your account number: ");
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
        
        if (result == 9 && record.accountNbr == accountNumber && strcmp(record.name, u.name) == 0 || record.accountType == "current" || record.accountType == "savings")
        {
            found = 1;
            break;
        }
        else if (record.accountType != "current" && record.accountType != "savings" && record.accountType != "account")
        {
            printf("\n\t\t✖ Transactions are not allowed on fixed type of accounts.\n");
            fclose(fp);
            printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
            printf("\n\n\t\tEnter your choice: ");
            
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
        printf("\n\t\t✖ Account not found or does not belong to you.\n");
        fclose(fp);
        
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
        printf("\n\n\t\tEnter your choice: ");
        
        int choice;
        scanf("%d", &choice);
        if (choice == 1) mainMenu(u);
        else exit(0);
        return;
    }

    system("clear");
    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Account Details                        ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
    printf("\n\t\tAccount Number: %d", record.accountNbr);
    printf("\n\t\tAccount Holder: %s", record.name);
    printf("\n\t\tCurrent Balance: $%.2f\n", record.amount);

    printf("\n\t\tChoose transaction type:\n");
    printf("\n\t\t[1] Deposit");
    printf("\n\t\t[2] Withdraw\n");
    printf("\n\t\t[3] Cancel\n");
    printf("\n\t\t[4] Exit\n");
    printf("\n\t\tEnter your choice: ");
    scanf("%d", &transactionType);

    if (transactionType == 1)
    {
        printf("\n\t\tEnter amount to deposit: $");
        scanf("%lf", &amount);
        record.amount += amount;
        system("clear");
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                   Transaction Complete                   ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\t✔ Deposited: $%.2f", amount);
        printf("\n\t\t✔ New balance: $%.2f\n", record.amount);
    }
    else if (transactionType == 2)
    {
        printf("\n\t\tEnter amount to withdraw: $");
        scanf("%lf", &amount);
        if (amount <= record.amount)
        {
            record.amount -= amount;
            system("clear");
            printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║                   Transaction Complete                   ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
            printf("\n\t\t✔ Withdrew: $%.2f", amount);
            printf("\n\t\t✔ New balance: $%.2f\n", record.amount);
        }
        else
        {
            printf("\n\t\t✖ Insufficient balance for withdrawal.\n");
        }
    }
    else if (transactionType == 3)
    {
        system("clear");
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                   Transaction Cancelled                   ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\tTransaction cancelled.\n");
        makeTransaction(u);
    }
    else if (transactionType == 4)
    {
       mainMenu(u);
    }
    else
    {
        printf("\n\t\t✖ Invalid choice.\n");
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

    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf("\n\t\tThank you for using our services, %s!\n\n", u.name);
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
    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                   Remove Account                         ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n");

    fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("\n\t\t✖ Error opening records file.\n");
        exit(1);
    }

    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("\n\t\t✖ Error creating temporary file.\n");
        fclose(fp);
        exit(1);
    }

    printf("\n\t\tEnter the account number to remove: ");
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
            printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
            printf("\n\t\t║                   Account Removed                        ║");
            printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
            printf("\n\t\t✔ Account %d has been successfully removed.\n", accountNumber);
        }
        else
        {
            printf("\n\t\t✖ Error updating records file.\n");
        }
    }
    else
    {
        remove("temp.txt");
        printf("\n\t\t✖ Account not found or does not belong to you.\n");
    }

    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1)
    {
        mainMenu(u);
    }
    else
    {
        printf("\n\t\tThank you for using our services, %s!\n\n", u.name);
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
    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║                Transfer Account Ownership                ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n");

    fp = fopen(RECORDS, "r");
    tempFile = fopen("temp.txt", "w");
    userFile = fopen(USERS, "r");

    if (fp == NULL || tempFile == NULL || userFile == NULL) {
        printf("\n\t\t✖ Error accessing files.\n");
        if (fp) fclose(fp);
        if (tempFile) fclose(tempFile);
        if (userFile) fclose(userFile);
        return;
    }

    printf("\n\t\tYour accounts:\n");
    printf("\n\t\t%-10s %-15s %-10s\n", "Account #", "Type", "Balance");
    printf("\t\t----------------------------------------\n");
    
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

    printf("\n\t\tEnter the account number to transfer: ");
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
                    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
                    printf("\n\t\t║                      WARNING                             ║");
                    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
                    printf("\n\t\t✖ This account (#%d) doesn't belong to you!\n", accountNumber);
                    printf("\n\t\tPress Enter to continue...");
                    getchar();
                    fclose(fp);
                    fclose(tempFile);
                    fclose(userFile);
                    remove("temp.txt");
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
        printf("\n\t\t✖ Account not found!\n");
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("temp.txt");
        printf("\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    printf("\n\t\tEnter the new owner's username: ");
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
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                      WARNING                             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\t✖ User '%s' is not registered in the system!\n", newOwner.name);
        printf("\n\t\t✖ Transfer can only be made to registered users.\n");
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("temp.txt");
        
        printf("\n\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    printf("\n\t\tPlease ask %s to enter their password to confirm the transfer: ", newOwner.name);
    scanf("%49s", inputPassword);
    getchar();

    hashPassword(inputPassword, hashedInputPassword);

    if (strcmp(hashedInputPassword, existingUser.password) != 0) {
        system("clear");
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║                      WARNING                             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\t✖ Incorrect password! Transfer cancelled.\n");
        printf("\n\t\t✖ The new owner must consent to the transfer.\n");
        fclose(fp);
        fclose(tempFile);
        fclose(userFile);
        remove("temp.txt");
        
        printf("\n\n\t\tPress Enter to continue...");
        getchar();
        mainMenu(u);
        return;
    }

    rewind(fp);
    tempFile = freopen("temp.txt", "w", tempFile);
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
        printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t║              Transfer Completed Successfully             ║");
        printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\t✔ Account #%d has been transferred to %s.\n", accountNumber, newOwner.name);
        
        time_t now = time(NULL);                    
        struct tm *t = localtime(&now);             

        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

        FILE *notifFile = fopen(NOTIFICATIONS, "a");
        if (notifFile) {
            fprintf(notifFile, "[%s] %s: You have received account #%d from %s.\n",
                    timestamp, newOwner.name, accountNumber, u.name);
            fclose(notifFile);
        }


    } else {
        printf("\n\t\t✖ Error updating records.\n");
    }

    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        mainMenu(u);
    } else {
        printf("\n\t\tThank you for using our services, %s!\n\n", u.name);
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
        perror("Error opening file");
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

/* Function to handle the registration of a new user
============================================================================*/
int registerMenu()
{
    struct User newUser;

    FILE *userFile = fopen(USERS, "a+");
    if (userFile == NULL)
    {
        fprintf(stderr, "Error opening users file.\n");
        return 0;
    }

    while (1)
    {
        printf("\n\t\tEnter your username (One Name, alphanumeric): ");
        scanf("%49s", newUser.name);

        if (!isValidUsername(newUser.name))
        {
            printf("\n\t\t\t\tInvalid username. Only alphanumeric characters are allowed.\n");
            continue;
        }

        struct User existingUser;
        int usernameTaken = 0;

        rewind(userFile);
        while (fscanf(userFile, "%d %49s %64s", &existingUser.id, existingUser.name, existingUser.password) == 3)
        {
            if (strcmp(existingUser.name, newUser.name) == 0)
            {
                printf("\n\t\t✖ Username already exists! Please choose a different name.\n\n");
                usernameTaken = 1;
                break;
            }
        }

        if (usernameTaken)
        {
            continue;
        }

        printf("\n\t\tEnter your password: ");
        scanf("%64s", newUser.password);

        char hashedPassword[65];
        hashPassword(newUser.password, hashedPassword);

        int usersCount = countLinesInFile();
        if (usersCount < 0)
        {
            fprintf(stderr, "Error counting users.\n");
            fclose(userFile);
            return 0;
        }
        const char *filename = USERS;
        newUser.id = getNextAccountNumber(filename) ;
        fprintf(userFile, "%d %s %s\n", newUser.id, newUser.name, hashedPassword);
        fclose(userFile);

        mainMenu(newUser);
        break;
    }

    return 1;
}

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
        perror("\n\n\t\t\t\tError: Unable to open file\n");
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
======================================================================================*/
void checkAllAccounts(struct User u)
{
    struct Record r;
    int found = 0;
    
    system("clear");
    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║              All Account Information                     ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝\n\n");

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL)
    {
        fprintf(stderr, "\n\t\t✖ Error opening records file!\n");
        return;
    }

    printf("\t╔═════════╦═══════════════════╦═══════════════╦═══════════════╦═══════════════╦═══════════╗\n");
    printf("\t║ Acc No. ║      Name         ║    Country    ║     Phone     ║    Balance    ║   Type    ║\n");
    printf("\t╠═════════╬═══════════════════╬═══════════════╬═══════════════╬═══════════════╬═══════════╣\n");

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
            printf("\t║ %-7d ║ %-17s ║ %-13s ║ %-13s ║ $%-12.2f ║ %-9s ║\n",
                   r.accountNbr, r.name, r.country, r.phone, r.amount, r.accountType);
            found = 1;
        }
    }

    if (found)
    {
        printf("\t╚═════════╩═══════════════════╩═══════════════╩═══════════════╩═══════════════╩═══════════╝\n");
    }
    else
    {
        printf("\t╚═════════╩═══════════════════╩═══════════════╩═══════════════╩═══════════════╩═══════════╝\n");
        printf("\n\t\t✖ No accounts found for user %s.\n", u.name);
    }

    printf("\n\n\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t║  [1] Return to main menu          [2] Exit               ║");
    printf("\n\t\t╚══════════════════════════════════════════════════════════╝");
    printf("\n\n\t\tEnter your choice: ");
    
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
==============================================================*/
void checkNotifications(char *username) {
    FILE *notifFile = fopen(NOTIFICATIONS, "r");
    FILE *tempFile = fopen(TEMP_NOTIFICATIONS, "w");
    char line[256];

    if (!notifFile || !tempFile) return;

    printf("\n\t\t🔔 Notifications for %s:\n", username);
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
        printf("\t\t(No new notifications)\n");
}

