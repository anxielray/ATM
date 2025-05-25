#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
extern const char *RECORDS;
extern const char *USERS;

#define MAX_NAME_LENGTH 50
#define MAX_PASS_LENGTH 65
#define MAX_PHONE_LENGTH 13


// Structure to represent each account record
struct Record
{
    int accountNbr;
    int id;
    char name[MAX_NAME_LENGTH];
    int userId;
    char date[11];
    char country[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    double amount;
    char accountType[10];
};

// Structure to represent a user
struct User
{
    int id;
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASS_LENGTH];
};

void loginMenu(char username[MAX_NAME_LENGTH], char password[MAX_PASS_LENGTH]);
int registerMenu();
const char *getPassword(struct User u);

// Account management function prototypes
void createNewAcc(struct User u);
void checkAllAccounts(struct User u);
void updateAccountInfo(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferAccountOwnership(struct User u);
void getCurrentDate(struct Record *r);
int countEmptyLines(const char *filename);
void initMenu(struct User *u);
int countLinesInFile();
void mainMenu(struct User u);
void login(struct User u);
int getUserIdByName(struct User u);
void selectAccountType(char *accountType);
int getNextAccountNumber(const char *filename);
void checkNotifications(char *username);
char* hashPassword(const char* password, char *hashedPassword);
int isValidUsername(const char *username);

#endif
