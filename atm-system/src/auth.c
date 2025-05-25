#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "header.h"

/* Function to hash a password using SHA-256 (with EVP for OpenSSL 3.0 compatibility)
=============================================================================================*/
char* hashPassword(const char* password, char *hashedPassword)
{
    EVP_MD_CTX *mdctx;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    if ((mdctx = EVP_MD_CTX_new()) == NULL)
    {
        perror("Error initializing message digest context");
        exit(1);
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1)
    {
        perror("Error initializing digest");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }

    if (EVP_DigestUpdate(mdctx, password, strlen(password)) != 1)
    {
        perror("Error updating digest");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }

    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1)
    {
        perror("Error finalizing digest");
        EVP_MD_CTX_free(mdctx);
        exit(1);
    }

    for (unsigned int i = 0; i < hash_len; i++)
    {
        sprintf(hashedPassword + (i * 2), "%02x", hash[i]);
    }
    hashedPassword[hash_len * 2] = '\0';

    EVP_MD_CTX_free(mdctx);
}

/* Function to securely handle user login input
============================================================================*/
void loginMenu(char username[MAX_NAME_LENGTH], char password[MAX_PASS_LENGTH])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
    printf("\n\t\t\t\t\t\t║             ===== Bank Management System =====           ║");
    printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n");
    printf("\n\n\t\t\t\t\t User Login: ");
    scanf("%s", username);

    // ======== Disable terminal echo for password entry ========
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    printf("\n\n\t\t\t\t\t Enter the password to login: ");
    scanf("%s", password);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
}

/* Function to fetch a hashed password based on the username
==============================================================================*/
const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen(USERS, "r")) == NULL)
    {  
        system("clear");
        printf("\n\n\t\t\t\t\t\t╔══════════════════════════════════════════════════════════╗");
        printf("\n\t\t\t\t\t\t║                     ===== ERROR! =====                   ║");
        printf("\n\t\t\t\t\t\t╚══════════════════════════════════════════════════════════╝\n");
        printf("\n\t\t\t\tError! The users file could not be opened.\n");
        exit(1);
    }

    while (fscanf(fp, "%d %49s %64s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0) {
            printf("\n\t%s\n", userChecker.name);
            char *buff = userChecker.password;
            return buff;
            fclose(fp);
        } else {
            printf("\n\t%s\n", userChecker.name);
            memset(userChecker.name, 0, sizeof(userChecker.name));
            memset(userChecker.password, 0, sizeof(userChecker.password));
            continue;
        }
    }

    fclose(fp);
    return "no user found";
}

/* Function to attempt a login with error handling for incorrect attempts
==================================================================================*/
void login(struct User u) {
    char hashedInputPassword[65];
    const char *storedPassword;
    char inputPassword[65];
    int attempts = 3;

    while (attempts > 0) {
        loginMenu(u.name, inputPassword);

        hashPassword(inputPassword, hashedInputPassword);
        storedPassword = getPassword(u);

        if (strcmp(storedPassword, "no user found") == 0) {
            initMenu(&u);
            printf("\n\n\t\t\t\tError: User not found. Try registering\n");
            break;
        } else if (strcmp(hashedInputPassword, storedPassword) == 0) {
            mainMenu(u);
            return;
        } else {
            printf("\n\n\\t\t\t\tError: Incorrect password. Try again, you have (%d) attempts left\n", attempts);
        }

        attempts--;
        printf("\t\t\t\tYou have %d attempt(s) remaining.\n", attempts);

        if (attempts == 0) {
            printf("\t\t\t\tToo many failed login attempts. Exiting...\n\n");
            initMenu(&u);
            break;
        }
    }
}