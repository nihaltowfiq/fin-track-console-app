# FinTrack (Console Application)

FinTrack is a simple C console application for tracking personal finances. It allows users to manage accounts, record transactions, and view summaries, all from the command line.

## Features

- User signup and signin (with CSV-based credential storage)
- Add, view, and delete financial transactions
- Track income and expenses per user
- Simple command-line interface

## Folder Structure

- `main.c` — Application entry point, handles main menu and user flow
- `auth.c`, `auth.h` — User authentication (signup, signin)
- `transaction.c`, `transaction.h` — Transaction management (add, view, delete)
- `config.h` — Configuration constants (e.g., file paths, limits)
- `data/` — Stores CSV files for users and transactions
- `bin/Debug/fin-track.exe` — Compiled executable (output)
- `obj/Debug/` — Object files from compilation

## How to Build and Run

1. Open a terminal in the project directory.
2. Build the project (example for GCC):

```bash
gcc main.c auth.c transaction.c -o bin/fin-track.exe
```

3. Run the application:

```bash
./bin/fin-track.exe
```

## Requirements

- GCC or compatible C compiler
- Windows or Linux

## License

N/A
