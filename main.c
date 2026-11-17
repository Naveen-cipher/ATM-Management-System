#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 500
#define NAME_LEN 50
#define MIN_BALANCE 500.0
#define DAILY_WITHDRAWAL_LIMIT 20000.0

/* Structure for Bank Account */
typedef struct {
    int accountNumber;
    char name[NAME_LEN];
    int pin;
    double balance;
    double dailyWithdrawn; /* Session tracked withdrawal amount */
} Account;

/* Structure for Transaction Record */
typedef struct {
    int transactionId;
    int accountNumber;
    char type[15]; /* "Deposit" or "Withdrawal" */
    double amount;
    char timestamp[25];
} Transaction;

/* Global Storage */
static Account accounts[MAX_ACCOUNTS];
static int accountCount = 0;

static Transaction transactions[MAX_TRANSACTIONS];
static int transactionCount = 0;
static int nextTransactionId = 1001;

/* Helper: Clear standard input buffer */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* Helper: Get current system date and time formatted string */
void getCurrentDateTime(char *buffer, size_t size) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (timeinfo != NULL) {
        strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
    } else {
        snprintf(buffer, size, "Unknown");
    }
}

/* Helper: Safe string input */
void readSafeString(char *dest, size_t size) {
    if (fgets(dest, (int)size, stdin) != NULL) {
        size_t len = strlen(dest);
        if (len > 0 && dest[len - 1] == '\n') {
            dest[len - 1] = '\0';
        }
    } else {
        dest[0] = '\0';
    }
}

/* DSA IMPLEMENTATION: Linear Search for Account by Account Number */
/* Returns array index if found, or -1 if not found */
int searchAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i; /* Found */
        }
    }
    return -1; /* Not found */
}

/* MODULE 4 Helper: Add a recorded transaction */
void addTransaction(int accountNumber, const char *type, double amount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("\n[Warning] Transaction log capacity reached. History cannot be stored.\n");
        return;
    }

    Transaction *tx = &transactions[transactionCount];
    tx->transactionId = nextTransactionId++;
    tx->accountNumber = accountNumber;
    strncpy(tx->type, type, sizeof(tx->type) - 1);
    tx->type[sizeof(tx->type) - 1] = '\0';
    tx->amount = amount;
    getCurrentDateTime(tx->timestamp, sizeof(tx->timestamp));

    transactionCount++;
}

/* MODULE 1: Create New Account */
void addAccount(void) {
    printf("\n--- Create New Account ---\n");
    if (accountCount >= MAX_ACCOUNTS) {
        printf("[Error] Account storage limit reached (%d accounts max).\n", MAX_ACCOUNTS);
        return;
    }

    int accNo;
    printf("Enter unique Account Number (positive integer): ");
    if (scanf("%d", &accNo) != 1 || accNo <= 0) {
        printf("[Error] Invalid account number format.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Duplicate check via Linear Search */
    if (searchAccount(accNo) != -1) {
        printf("[Error] Account Number %d already exists! Duplicate account numbers are not allowed.\n", accNo);
        return;
    }

    char holderName[NAME_LEN];
    printf("Enter Account Holder Name: ");
    readSafeString(holderName, sizeof(holderName));
    if (strlen(holderName) == 0) {
        printf("[Error] Account holder name cannot be empty.\n");
        return;
    }

    int pin;
    printf("Set a 4-Digit Security PIN (1000 - 9999): ");
    if (scanf("%d", &pin) != 1 || pin < 1000 || pin > 9999) {
        printf("[Error] Invalid PIN! PIN must be a 4-digit number between 1000 and 9999.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    double initialDeposit;
    printf("Enter Initial Deposit (Minimum Rs. %.2f): Rs. ", MIN_BALANCE);
    if (scanf("%lf", &initialDeposit) != 1 || initialDeposit < MIN_BALANCE) {
        printf("[Error] Initial deposit must be at least Rs. %.2f.\n", MIN_BALANCE);
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Store the account */
    accounts[accountCount].accountNumber = accNo;
    strncpy(accounts[accountCount].name, holderName, NAME_LEN - 1);
    accounts[accountCount].name[NAME_LEN - 1] = '\0';
    accounts[accountCount].pin = pin;
    accounts[accountCount].balance = initialDeposit;
    accounts[accountCount].dailyWithdrawn = 0.0;
    accountCount++;

    /* Record initial deposit transaction */
    addTransaction(accNo, "Deposit", initialDeposit);

    printf("\n[Success] Account #%d successfully registered for %s with balance Rs. %.2f!\n",
           accNo, holderName, initialDeposit);
}

/* MODULE 1: View Account Details (Requires PIN Authentication) */
void displayAccounts(void) {
    printf("\n--- View Account Details ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    printf("\n========================================\n");
    printf("            ACCOUNT DETAILS             \n");
    printf("========================================\n");
    printf(" Account Number     : %d\n", accounts[idx].accountNumber);
    printf(" Account Holder Name: %s\n", accounts[idx].name);
    printf(" Current Balance    : Rs. %.2f\n", accounts[idx].balance);
    printf(" Session Withdrawn  : Rs. %.2f (Daily Limit: Rs. %.2f)\n",
           accounts[idx].dailyWithdrawn, DAILY_WITHDRAWAL_LIMIT);
    printf(" Security PIN       : [Protected]\n");
    printf("========================================\n");
}

/* MODULE 1: Update Account Holder Name and PIN */
void updateAccount(void) {
    printf("\n--- Update Account Details ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter Current 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    printf("\n1. Update Account Holder Name\n");
    printf("2. Update Security PIN\n");
    printf("3. Update Both Name and PIN\n");
    printf("Enter choice (1-3): ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("[Error] Invalid choice.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 1 || choice == 3) {
        char newName[NAME_LEN];
        printf("Enter New Account Holder Name: ");
        readSafeString(newName, sizeof(newName));
        if (strlen(newName) > 0) {
            strncpy(accounts[idx].name, newName, NAME_LEN - 1);
            accounts[idx].name[NAME_LEN - 1] = '\0';
            printf("[Success] Account holder name updated successfully.\n");
        } else {
            printf("[Warning] Name cannot be blank. Skipped name update.\n");
        }
    }

    if (choice == 2 || choice == 3) {
        int newPin;
        printf("Enter New 4-Digit PIN (1000 - 9999): ");
        if (scanf("%d", &newPin) == 1 && newPin >= 1000 && newPin <= 9999) {
            accounts[idx].pin = newPin;
            printf("[Success] Security PIN updated successfully.\n");
        } else {
            printf("[Error] Invalid PIN format. PIN was not modified.\n");
        }
        clearInputBuffer();
    }
}

/* MODULE 2: Balance Inquiry */
void checkBalance(void) {
    printf("\n--- Balance Inquiry ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    printf("\n========================================\n");
    printf("Account Holder: %s\n", accounts[idx].name);
    printf("Account Number: %d\n", accounts[idx].accountNumber);
    printf("Available Balance: Rs. %.2f\n", accounts[idx].balance);
    printf("Minimum Balance Requirement: Rs. %.2f\n", MIN_BALANCE);
    printf("========================================\n");
}

/* MODULE 2: Cash Deposit */
void depositMoney(void) {
    printf("\n--- Cash Deposit ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    double amount;
    printf("Enter deposit amount: Rs. ");
    if (scanf("%lf", &amount) != 1 || amount <= 0.0) {
        printf("[Error] Invalid deposit amount. Amount must be strictly greater than 0.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Perform Deposit */
    accounts[idx].balance += amount;
    addTransaction(accNo, "Deposit", amount);

    printf("\n[Success] Deposit of Rs. %.2f completed successfully!\n", amount);
    printf("Updated Balance: Rs. %.2f\n", accounts[idx].balance);
}

/* MODULE 3: Cash Withdrawal and Balance Validation */
void withdrawMoney(void) {
    printf("\n--- Cash Withdrawal ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    double amount;
    printf("Enter withdrawal amount: Rs. ");
    if (scanf("%lf", &amount) != 1 || amount <= 0.0) {
        printf("[Error] Invalid withdrawal amount. Amount must be strictly greater than 0.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Rule 1: Check daily session withdrawal limit */
    if (accounts[idx].dailyWithdrawn + amount > DAILY_WITHDRAWAL_LIMIT) {
        printf("\n[Transaction Failed] Daily withdrawal limit exceeded!\n");
        printf("Daily Limit: Rs. %.2f\n", DAILY_WITHDRAWAL_LIMIT);
        printf("Already Withdrawn (This Session): Rs. %.2f\n", accounts[idx].dailyWithdrawn);
        printf("Maximum Available to Withdraw Today: Rs. %.2f\n",
               DAILY_WITHDRAWAL_LIMIT - accounts[idx].dailyWithdrawn);
        return;
    }

    /* Rule 2: Check minimum balance requirement */
    if (accounts[idx].balance - amount < MIN_BALANCE) {
        printf("\n[Transaction Failed] Insufficient balance!\n");
        printf("Current Balance: Rs. %.2f\n", accounts[idx].balance);
        printf("Minimum Balance Required: Rs. %.2f\n", MIN_BALANCE);
        printf("Maximum allowed withdrawal at this time: Rs. %.2f\n",
               (accounts[idx].balance > MIN_BALANCE) ? (accounts[idx].balance - MIN_BALANCE) : 0.0);
        return;
    }

    /* Perform Withdrawal */
    accounts[idx].balance -= amount;
    accounts[idx].dailyWithdrawn += amount;
    addTransaction(accNo, "Withdrawal", amount);

    printf("\n[Success] Please collect your cash: Rs. %.2f\n", amount);
    printf("Remaining Balance: Rs. %.2f\n", accounts[idx].balance);
    printf("Remaining Daily Limit: Rs. %.2f\n", DAILY_WITHDRAWAL_LIMIT - accounts[idx].dailyWithdrawn);
}

/* Helper to print a single transaction row */
void printTransactionRow(const Transaction *t) {
    printf("%-8d | %-12d | %-12s | Rs. %10.2f | %-19s\n",
           t->transactionId, t->accountNumber, t->type, t->amount, t->timestamp);
}

/* MODULE 4: Display Transaction History for Logged-In Account */
void displayTransactions(void) {
    printf("\n--- Transaction History ---\n");
    int accNo;
    printf("Enter Account Number: ");
    if (scanf("%d", &accNo) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int idx = searchAccount(accNo);
    if (idx == -1) {
        printf("[Error] Account #%d not found.\n", accNo);
        return;
    }

    int enteredPin;
    printf("Enter 4-Digit PIN: ");
    if (scanf("%d", &enteredPin) != 1) {
        printf("[Error] Invalid input.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (accounts[idx].pin != enteredPin) {
        printf("[Error] Authentication failed: Incorrect PIN.\n");
        return;
    }

    printf("\n========================================================================\n");
    printf("                   TRANSACTION HISTORY FOR ACCOUNT #%d                  \n", accNo);
    printf("========================================================================\n");
    printf("%-8s | %-12s | %-12s | %-14s | %-19s\n",
           "TX ID", "Account No", "Type", "Amount", "Date & Time");
    printf("------------------------------------------------------------------------\n");

    int foundCount = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accNo) {
            printTransactionRow(&transactions[i]);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No transactions recorded for this account.\n");
    }
    printf("========================================================================\n");
    printf("Total Transactions Displayed: %d\n", foundCount);
}

/* DSA IMPLEMENTATION: Linear Search for Transaction by ID or Type */
void searchTransaction(void) {
    printf("\n--- Search Transactions ---\n");
    printf("1. Search by Transaction ID (Linear Search)\n");
    printf("2. Search by Transaction Type (Linear Search)\n");
    printf("Enter search option (1-2): ");
    int opt;
    if (scanf("%d", &opt) != 1 || (opt != 1 && opt != 2)) {
        printf("[Error] Invalid choice.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (opt == 1) {
        int searchId;
        printf("Enter Transaction ID to search: ");
        if (scanf("%d", &searchId) != 1) {
            printf("[Error] Invalid ID.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        /* Linear Search by Transaction ID */
        int foundIndex = -1;
        for (int i = 0; i < transactionCount; i++) {
            if (transactions[i].transactionId == searchId) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            printf("\n[Linear Search Result] Transaction Record Found:\n");
            printf("========================================================================\n");
            printf("%-8s | %-12s | %-12s | %-14s | %-19s\n",
                   "TX ID", "Account No", "Type", "Amount", "Date & Time");
            printf("------------------------------------------------------------------------\n");
            printTransactionRow(&transactions[foundIndex]);
            printf("========================================================================\n");
        } else {
            printf("\n[Result] Transaction ID #%d not found in the records.\n", searchId);
        }
    } else {
        printf("Enter Transaction Type (1 for Deposit, 2 for Withdrawal): ");
        int typeChoice;
        if (scanf("%d", &typeChoice) != 1 || (typeChoice != 1 && typeChoice != 2)) {
            printf("[Error] Invalid type selection.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        const char *targetType = (typeChoice == 1) ? "Deposit" : "Withdrawal";
        printf("\n[Linear Search Result] Transactions of type '%s':\n", targetType);
        printf("========================================================================\n");
        printf("%-8s | %-12s | %-12s | %-14s | %-19s\n",
               "TX ID", "Account No", "Type", "Amount", "Date & Time");
        printf("------------------------------------------------------------------------\n");

        int count = 0;
        for (int i = 0; i < transactionCount; i++) {
            if (strcmp(transactions[i].type, targetType) == 0) {
                printTransactionRow(&transactions[i]);
                count++;
            }
        }

        if (count == 0) {
            printf("No transactions matching type '%s' found.\n", targetType);
        }
        printf("========================================================================\n");
        printf("Total Matching Transactions: %d\n", count);
    }
}

/* DSA IMPLEMENTATION: Bubble Sort by Transaction Amount */
void bubbleSortTransactions(void) {
    printf("\n--- Sort Transactions by Amount (Bubble Sort) ---\n");
    if (transactionCount <= 1) {
        printf("Not enough transactions to sort (count = %d).\n", transactionCount);
        return;
    }

    printf("1. Ascending Order (Lowest to Highest Amount)\n");
    printf("2. Descending Order (Highest to Lowest Amount)\n");
    printf("Enter sorting order (1-2): ");
    int order;
    if (scanf("%d", &order) != 1 || (order != 1 && order != 2)) {
        printf("[Error] Invalid choice.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Make a copy of transactions to preserve original chronological order */
    Transaction sortedList[MAX_TRANSACTIONS];
    for (int i = 0; i < transactionCount; i++) {
        sortedList[i] = transactions[i];
    }

    /* Standard Bubble Sort Algorithm */
    for (int i = 0; i < transactionCount - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < transactionCount - i - 1; j++) {
            int condition = (order == 1) 
                ? (sortedList[j].amount > sortedList[j + 1].amount)    /* Ascending */
                : (sortedList[j].amount < sortedList[j + 1].amount);   /* Descending */

            if (condition) {
                /* Swap elements */
                Transaction temp = sortedList[j];
                sortedList[j] = sortedList[j + 1];
                sortedList[j + 1] = temp;
                swapped = 1;
            }
        }
        /* Optimization: if no two elements were swapped by inner loop, then break */
        if (!swapped) {
            break;
        }
    }

    printf("\n========================================================================\n");
    printf("     SORTED TRANSACTIONS (%s)      \n", (order == 1) ? "ASCENDING" : "DESCENDING");
    printf("========================================================================\n");
    printf("%-8s | %-12s | %-12s | %-14s | %-19s\n",
           "TX ID", "Account No", "Type", "Amount", "Date & Time");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < transactionCount; i++) {
        printTransactionRow(&sortedList[i]);
    }
    printf("========================================================================\n");
    printf("Total Transactions Displayed: %d\n", transactionCount);
}

int main(void) {
    int choice;

    while (1) {
        printf("\n========================================\n");
        printf("          ATM MANAGEMENT SYSTEM         \n");
        printf("========================================\n");
        printf("1. Create New Account\n");
        printf("2. View Account Details\n");
        printf("3. Update Account Details\n");
        printf("4. Balance Inquiry\n");
        printf("5. Cash Deposit\n");
        printf("6. Cash Withdrawal\n");
        printf("7. Transaction History\n");
        printf("8. Search Transaction\n");
        printf("9. Sort Transactions\n");
        printf("10. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n[Error] Invalid input! Please enter a number between 1 and 10.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addAccount();
                break;
            case 2:
                displayAccounts();
                break;
            case 3:
                updateAccount();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                depositMoney();
                break;
            case 6:
                withdrawMoney();
                break;
            case 7:
                displayTransactions();
                break;
            case 8:
                searchTransaction();
                break;
            case 9:
                bubbleSortTransactions();
                break;
            case 10:
                printf("\nThank you for using ATM Management System. Goodbye!\n");
                exit(0);
            default:
                printf("\n[Error] Invalid choice (%d). Please select between 1 and 10.\n", choice);
                break;
        }
    }

    return 0;
}
