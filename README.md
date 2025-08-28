# FinTrack (Console Application)

FinTrack is a simple C console application for tracking personal finances. It allows users to manage accounts, record transactions, and view summaries, all from the command line.

## Features

- User authentication
- Add, view, and delete transactions
- Store data in CSV files
- Simple and fast command-line interface

## Folder Structure

- `main.c` - Entry point of the application
- `auth.c`, `auth.h` - User authentication logic
- `transaction.c`, `transaction.h` - Transaction management
- `data/` - Stores user and transaction CSV files
- `bin/` - Compiled binaries

## Usage

1. Build the project using a C compiler (e.g., GCC):

```bash
gcc main.c auth.c transaction.c -o bin/fin-track.exe
```

2. Run the application:

```bash
./bin/fin-track.exe
```

## Requirements

- GCC or compatible C compiler
- Windows or Linux

## License

N/A
