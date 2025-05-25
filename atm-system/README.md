# ATM Management System

## Overview

*A secure and feature-rich ATM (Automated Teller Machine) Management System implemented in C. This system provides a comprehensive solution for managing bank accounts, transactions, and user authentication with a focus on security and user experience.*

## Features

- 🔐 *Secure User Authentication*
- 👥 *User Account Management*
- 💰 *Multiple Account Support*
- 💳 *Transaction Processing*
  - *Deposits*
  - *Withdrawals*
  - *Balance Inquiry*
- 📝 *Account Creation and Deletion*
- 🔄 *Account Ownership Transfer*
- 🛡️ *Password Encryption (SHA-256)*

## Technical Specifications

- *Language: C*
- *Encryption: SHA-256*
- *Storage: File-based database system*
- *Interface: Command-line interface (CLI)*
- *Platform: Cross-platform compatible*

## System Requirements

- *GCC Compiler*
- *Standard C Libraries*
- *Minimum 512MB RAM*
- *1GB Storage Space*

## Installation

Clone the repository

```bash
git clone https://github.com/yourusername/atm-management-system.git
```

Navigate to the project directory

```bash
cd atm-management-system
```

*Run the program:*

- *I provided optional ways to run the program:*
  - *Using the provided Makefile:*

```bash
make run
```

- *Using the starting script:*

```sh
./setup.sh
```

- *Using the traditional way:*

Compile the source code:

```bash
gcc -o atm src/*.c -lcrypto
```

Execute the compiled program:

```bash
./atm
```

- *If at all the Makefile commands escape your memory, no need to stress get updated by executing the script:*

```bash
./commands.sh
```

## Usage

1. **Login/Register**
   - *New users can register with a username and password*
   - *Existing users can login with their credentials*

2. **Main Menu Options**
   - *Create New Account*
   - *View Account Details*
   - *Make Transaction*
   - *Remove Account*
   - *Transfer Account Ownership*
   - *Exit*

3. **Transaction Types**
   - *Deposit: Add funds to account*
   - *Withdraw: Remove funds from account*

4. **Security Features**
   - *Password hashing*
   - *Session management*
   - *Input validation*
   - *Error handling*

## File Structure

```sh
atm-system/
├── data/
│   ├── notifications.txt
│   ├── records.txt
│   ├── temp_notif.txt
│   └── users.txt
├── docs/
│   ├── CODE_OF_CONDUCT.md
│   ├── CONTRIBUTING.md
│   ├── LICENSE
│   └── SECURITY.md
├── src/
│   ├── auth.c
│   ├── header.h
│   ├── main.c
│   └── system.c
│
├── .gitignore
├── commands.sh
├── Makefile
├── README.md
└── setup.sh
```

## Data Storage

- `users.txt`: Stores user credentials and authentication data
- `records.txt`: Stores account information and transaction records

## Security Measures

- *Passwords are hashed using SHA-256*
- *Input validation to prevent buffer overflows*
- *File access controls*
- *Session management*
- *Error handling and logging*

## Contributing

- *Do not forget to check the security of the system before starting on any necessary action changes. [SECURITY](docs/SECURITY.md)*
- *Checkout the contributing guidelines in the [CONTRIBUTING.md](docs/CONTRIBUTING.md) and [CODE_OF_CONDUCT.md](docs/CODE_OF_CONDUCT.md) files*

## Author

- [**Raymond**](https://martinraymondogwel.netlify.app)
  - Initial work and main development

## License

This project is licensed under the MIT License - see the [LICENSE](docs/LICENSE.md) file for details

## Acknowledgments

- OpenSSL for SHA-256 implementation
- C Standard Library contributors
- All contributors and testers

## Support

For support, please open an issue in the repository or contact the author directly.

---
