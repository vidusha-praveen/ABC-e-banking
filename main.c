#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node *link;

struct node
{
    char name[50];
    char userName[20];
    char password[20];
    int id;
    float balance;
    int accNo;
    int year;
    int month;
    int day;
    int isVIP;

    link next;
};

typedef struct
{
    link head;
    link tail;
} list;

void initList(list *l)
{
    l -> head = NULL;
    l -> tail = NULL;
}

link newNode(int isVIP)
{
    char name[50];
    char userName[20];
    char password[20];
    int id;
    float balance = 0.00;
    int accNo;
    int year;
    int month;
    int day;

    printf("Enter the name: ");
    scanf("%s", name);

    printf("Enter user name: ");
    scanf("%s", userName);

    printf("Enter the password: ");
    scanf("%s", password);

    printf("Enter you id: ");
    scanf("%d", &id);

    printf("Enter your birth year: ");
    scanf("%d", &year);

    printf("Enter your birth month: ");
    scanf("%d", &month);

    printf("Enter your birth day: ");
    scanf("%d", &day);

    link temp;
    //temp = malloc(sizeof(link));
    temp = malloc(sizeof(struct node));

    if (!temp)
    {
        printf("\033[1;31m Account creation failed. \033[0m\n");
        exit(1);
    }

    accNo = generateAccNo(year);

    strcpy(temp->name, name);
    strcpy(temp->userName, userName);
    strcpy(temp->password, password);
    temp -> accNo = accNo;
    temp -> balance = balance;
    temp -> id = id;
    temp -> year = year;
    temp -> month = month;
    temp -> day = day;
    temp->isVIP = isVIP;
    temp -> next = NULL;

    if (isVIP) {
        printf("\033[1;36m VIP Account created successfully for %s (Acc No: %d) \033[0m\n", name, accNo);
    } else {
        printf("\033[1;32m Account created successfully for %s (Acc No: %d) \033[0m\n", name, accNo);
    }

    return temp;
}

// Account number generation

int generateAccNo(int year) {
    static int counter = 1000;
    return year * 10000 + counter++;
}

// Insert account rear

void insertRear(list *l)
{
    link n1 = newNode(0);

    if (l -> head == NULL)
    {
        l -> head = l -> tail = n1;
    }
    else
    {
        l -> tail -> next = n1;
        l -> tail = n1;
    }
}

// Insert account front

void insertFront(list *l)
{
    link n1 = newNode(1);

    if (l -> head == NULL)
    {
        l -> head = l -> tail = n1;
    }
    else
    {
        n1 -> next = l -> head;
        l -> head = n1;
    }
}

// Search account by account no

link searchAccount(list *l, int accNo)
{
    link temp;
    int found = 0;

    temp = l -> head;

    while (temp != NULL && (!found))
    {
        if (temp -> accNo == accNo)
        {
            found = 1;
            printf("\n \033[1;32m Account found! \033[0m\n");
            printf("Name: %s\n", temp->name);
            //printf("Username: %s\n", temp->userName);
            printf("Balance: %.2f\n", temp->balance);
            printf("ID: %d\n", temp->id);
            printf("Account No: %d\n", temp->accNo);

            break;
        }
        temp = temp -> next;
    }

    if (!found)
    {
        printf("\n \033[1;31m Account %d not found. \033[0m \n", accNo);
    }

    return temp;
}

// Deposit money

void depositMoney(list *l)
{
    int accNo;
    printf("Enter account number to deposit money: ");
    scanf("%d", &accNo);

    link acc = searchAccount(l, accNo);

    if (acc != NULL)
    {
        float depositAmount;
        printf("Enter the deposit amount LKR: ");
        scanf("%f", &depositAmount);

        acc->balance += depositAmount;
        printf("Deposit successful! \n Your new account balance is: LKR %.2f\n", acc->balance);
    }
}

// verify credentials

int verifyCredentials(list *l, int accNo){
    link acc = searchAccount(l, accNo);

    if (acc == NULL) {
        printf("Account not found.\n");
        return 0;
    }

    char userName[20];
    char password[20];

    printf("Enter account user name: \n");
    scanf("%s", userName);


    printf("Enter account password: \n");
    scanf("%s", password);

    if (strcmp(acc -> userName, userName) == 0 && strcmp(acc -> password, password) == 0){
        printf("Credentials verified");
        return 1;
    } else {
        printf("Invalid credentials. Please try again");
        return 0;
    }
}

// Withdraw money

void withdrawMoney(list *l)
{

    int accNo;
    printf("Enter account number to withdraw money: ");
    scanf("%d", &accNo);

    link acc = searchAccount(l, accNo);

    if (acc != NULL)
    {
       if (verifyCredentials(l, accNo))
        {
            float withdrawAmount;
            printf("Enter amount of withdraw");
            scanf("%lf", &withdrawAmount);

            if (acc -> balance - withdrawAmount >= 500.00)
            {
                acc -> balance = acc -> balance - withdrawAmount;

                printf("New balance: LKR %.2f\n", acc -> balance);
            }
            else
            {
                printf("Insufficient balance! Minimum LKR 500 must remain. Your current balance is: LKR %.2f\n", acc->balance);
            }

        }
    }
}

// Delete account

void deleteAccount(list *l)
{
    int accNo;

    printf("Enter the account number to delete the account: ");
    scanf("%d", &accNo);

    link acc = searchAccount(l, accNo);

    if (acc != NULL)
    {
        if (verifyCredentials(l, accNo))
        {
            printf("User name and Password is successful");

            if (acc -> balance <= 500)
            {
                link temp = l->head;
                link prev = NULL;

                if (temp != NULL && temp == acc)
                {
                    l->head = temp->next;
                    if (temp == l->tail)
                    {
                        l->tail = NULL;
                    }
                    free(temp);
                    printf(" Account %d deleted successfully.\n", accNo);
                    return;
                }

                while (temp != NULL && temp != acc)
                {
                    prev = temp;
                    temp = temp -> next;
                }

                prev->next = temp->next;

                if (temp == l->tail)
                {
                    l->tail = prev;
                }

                free(temp);
                printf(" Account %d deleted successfully.\n", accNo);
            }
            else
            {
                printf("Withdraw the account balance first and try again");
                withdrawMoney(l);
            }
        }
    }
    else
    {
        printf(" Cannot delete account.\n", accNo);
    }
}

// Money transfer

void transferMoney(list *l){
    int myAccNo;
    int payeeAccNo;

    float transferAmount;

    printf("Enter your account number");
    scanf("%d", &myAccNo);

    link acc = searchAccount(l, myAccNo);

    if (acc != NULL){

        if (verifyCredentials(l, myAccNo)){
            printf("User name and Password is correct.\nEnter the payee account no");
            scanf("%d", &payeeAccNo);

            link payeeAcc = searchAccount(l , payeeAccNo);

            if (payeeAcc != NULL){
                printf("Enter the amount to transfer. Minimum LKR 500 can transfer.\n LKR: ");
                scanf("%f", &transferAmount);

                if (acc -> balance - transferAmount - 30 >= 500){
                    if (transferAmount >= 500){
                    acc -> balance = acc -> balance - transferAmount - 30;
                    payeeAcc -> balance = payeeAcc -> balance + transferAmount;
                    printf("\033[1;32m Transaction is successful. \033[0m");
                    printf("\033[1;34m Your account balance is %f \n Transaction fee LKR 30 \033[0m", acc -> balance);
                    } else {
                        printf("\033[1;34m Minimum transfer amount: LKR 500.00 \033[0m");
                    }
                } else{
                    printf("Transaction failed. Insufficient account balance!!");
                }
            }

        }
    }
}

// Change Password and user name;

void changeCredintials(list *l){
    int accNo;
    char userName[20];
    char password[20];
    char newUserName[20];
    char newPassword[20];

    printf("Enter your account number");
    scanf("%d", &accNo);

    link acc = searchAccount(l , accNo);

    printf("Enter old user name: ");
    scanf("%s", userName);

    printf("Enter old password: ");
    scanf("%s", password);

    if (strcmp(acc -> userName, userName) == 0 && strcmp(acc -> password, password) == 0){
        printf("\033[1;32m User name and Password is correct. \033[0m");

        printf("Enter new user name");
        scanf("%s", newUserName);

        strcpy(acc->userName, newUserName);

        printf("Enter new password");
        scanf("%s", newPassword);

        strcpy(acc->password, newPassword);

        printf("\033[1;32m Account is safe now. User name and password updated successfully!! \033[0m");
    } else {
        printf("Credentials verification fail!!. Try aganin ");
    }
}

// Display all accounts

void displayAllAccounts(list *l) {
    link temp = l->head;

    if (temp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("\n--- All Accounts ---\n");
    while (temp != NULL) {
        printf("Name: %s | Acc No: %d | Balance: LKR %.2f\n", temp->name, temp->accNo, temp->balance);
        temp = temp->next;
    }
}

int main() {
    list l1;
    initList(&l1);

    int choice;
    while (1) {
        printf("\n===============================\n");
        printf("\033[1;33m|     ABC e-Banking System     |       \033[0m\n");
        printf("===============================\n");
        printf("| Create New VIP Account  \033[1;30;47m 1 \033[0m     |\n");
        printf("|________________________________ |");
        printf("\n| Create New Account      \033[1;30;47m 2 \033[0m     |\n");
        printf("|________________________________ |");

        printf("\n| Deposit Money           \033[1;30;47m 3 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Withdraw Money          \033[1;30;47m 4 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Transfer Money          \033[1;30;47m 5 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Delete Account          \033[1;30;47m 6 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Change Credentials      \033[1;30;47m 7 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Display All Accounts    \033[1;30;47m 8 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("| Exit                    \033[1;30;47m 9 \033[0m     |\n");
        printf("|________________________________ |\n");

        printf("  Enter your choice:      ");
        scanf("%d", &choice);
        printf("=================================\n");


        switch (choice) {
            case 1:
                insertFront(&l1);
                break;
            case 2:
                insertRear(&l1);  // or insertFront(&l1)
                break;
            case 3:
                depositMoney(&l1);
                break;
            case 4:
                withdrawMoney(&l1);
                break;
            case 5:
                transferMoney(&l1);
                break;
            case 6:
                deleteAccount(&l1);
                break;
            case 7:
                changeCredintials(&l1);
                break;
            case 8:
                displayAllAccounts(&l1);
                break;
            case 9:
                printf(" \033[1;35;47m Thank you for using the system! \033[0m \n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

