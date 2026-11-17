# ATM Management System (C11)

A console-based **ATM Management System** built strictly in C (C11 Standard) demonstrating real-world Data Structures and Algorithms (DSA). The project implements arrays of structures, modular procedural programming, **Linear Search** for account and transaction lookups, and **Bubble Sort** for transaction analysis.

---

## 📌 Project Overview

This application simulates the core operations of an automated teller machine (ATM). It is designed as an educational and academic demonstration of algorithmic principles, memory safety, input validation, and structured programming without reliance on external databases or third-party libraries.

---

## 🚀 Key Features & Modules

### Module 1: Account Creation & Management
- **Registration**: Create a new account with account number, holder name, 4-digit PIN (1000–9999), and initial deposit.
- **Duplicate Prevention**: Linear Search enforces strictly unique account numbers.
- **Security & Privacy**: PINs are never displayed in public listings. PIN authentication is mandatory for viewing and modifying records.
- **Account Updates**: Securely change account holder name and/or PIN.

### Module 2: Balance Inquiry & Deposit
- **Authentication**: Strict Account Number + PIN verification.
- **Balance Inquiry**: View current balance alongside minimum balance guidelines.
- **Cash Deposit**: Deposit funds with real-time balance updates and input validation (rejects zero/negative values).
- **Audit Logging**: Successful deposits automatically log a transaction record with timestamp.

### Module 3: Cash Withdrawal & Limit Validation
- **Withdrawal Checks**:
  - Rejects zero or negative amounts.
  - **Minimum Balance**: Maintains a mandatory minimum balance of **Rs. 500.00**.
  - **Daily Session Withdrawal Limit**: Enforces a configurable cap of **Rs. 20,000.00** per session.
  - **Atomic Failure Handling**: Balances are never modified if limits or checks fail; failed transactions are never recorded as successes.

### Module 4: Transaction History, Searching & Sorting
- **Transaction Log**: Stores Transaction ID, Account Number, Type (`Deposit`/`Withdrawal`), Amount, and Date/Time timestamp (via `<time.h>`).
- **Account-Specific History**: View chronological statements filtered to the authenticated account.
- **Linear Search**:
  - Search transaction by unique Transaction ID.
  - Filter transactions by Type (`Deposit` or `Withdrawal`).
- **Bubble Sort**:
  - Sort transactions by amount in **Ascending** or **Descending** order.
  - Preserves original transaction history sequence while displaying formatted reports.

---

## 🧠 Data Structures and Algorithms (DSA) Implemented

1. **Structures (`struct`)**:
   - `Account`: Stores account number, holder name, PIN, current balance, and session-withdrawn total.
   - `Transaction`: Stores transaction ID, linked account number, transaction type string, amount, and timestamp.
2. **Fixed-Size Contiguous Arrays**:
   - `accounts[MAX_ACCOUNTS]`: Array of structures for O(1) direct indexing.
   - `transactions[MAX_TRANSACTIONS]`: Array of structures storing transaction records.
3. **Linear Search ($O(N)$)**:
   - `searchAccount(int accountNumber)`: Linearly traverses `accounts` to prevent duplicates and authenticate users.
   - `searchTransaction()`: Linearly searches transactions by ID or filters by transaction type.
4. **Bubble Sort ($O(N^2)$ with early exit flag)**:
   - `bubbleSortTransactions()`: Compares adjacent transaction amounts and swaps them according to user-selected order (Ascending/Descending). Includes an early-break optimization if already sorted.

---

## 📁 Project Structure

```text
ATM-Management-System/
├── main.c           # Complete C11 source code with all 4 modules and DSA
├── README.md        # Project documentation and specifications
└── .gitignore       # Git ignore rules for compiled binaries and objects
```

---

## 🛠️ Compilation Instructions

Ensure you have a C11-compliant compiler such as GCC installed.

Compile with all standard warnings enabled:
```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o atm_system
```

On Windows (MinGW / MSYS2 / Clang):
```powershell
gcc -std=c11 -Wall -Wextra -pedantic main.c -o atm_system.exe
```

---

## 💻 Execution Instructions

Run the compiled executable from your terminal:

**Windows**:
```powershell
.\atm_system.exe
```

**Linux / macOS**:
```bash
./atm_system
```

---

## 🖥️ Sample Console Interaction

```text
========================================
          ATM MANAGEMENT SYSTEM         
========================================
1. Create New Account
2. View Account Details
3. Update Account Details
4. Balance Inquiry
5. Cash Deposit
6. Cash Withdrawal
7. Transaction History
8. Search Transaction
9. Sort Transactions
10. Exit
========================================
Enter your choice: 1

--- Create New Account ---
Enter unique Account Number (positive integer): 101
Enter Account Holder Name: Naveen Sharma
Set a 4-Digit Security PIN (1000 - 9999): 1234
Enter Initial Deposit (Minimum Rs. 500.00): Rs. 2000

[Success] Account #101 successfully registered for Naveen Sharma with balance Rs. 2000.00!

========================================
Enter your choice: 9

--- Sort Transactions by Amount (Bubble Sort) ---
1. Ascending Order (Lowest to Highest Amount)
2. Descending Order (Highest to Lowest Amount)
Enter sorting order (1-2): 2

========================================================================
     SORTED TRANSACTIONS (DESCENDING)      
========================================================================
TX ID    | Account No   | Type         | Amount         | Date & Time        
------------------------------------------------------------------------
1001     | 101          | Deposit      | Rs.    2000.00 | 2026-11-17 22:18:52
1002     | 101          | Deposit      | Rs.    1500.00 | 2026-11-17 22:19:10
1003     | 101          | Withdrawal   | Rs.     500.00 | 2026-11-17 22:19:35
========================================================================
Total Transactions Displayed: 3
```

---

## ⚠️ Limitations & Academic Disclaimer

- **Academic Simulation**: This software is strictly an educational tool to demonstrate data structures, algorithms, and modular C programming. It is **not** a production-grade banking system.
- **In-Memory Storage**: Account data and transaction history reside in volatile memory for the lifetime of the program session.
- **PIN Storage**: PINs are kept in memory as plain integers for basic control-flow authentication; in production banking applications, cryptographic salting and hashing are mandatory.
