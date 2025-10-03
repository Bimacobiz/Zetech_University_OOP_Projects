# Online Library Management System (C++ OOP)

## Overview
This project implements an **Online Library Management System** using **Object-Oriented Programming (OOP)** principles in **C++**. The system manages books and users, enabling key operations such as adding and removing books, searching by different criteria, and managing borrowing and returning. It demonstrates modular, maintainable software design with classes like `Book`, `User`, and `Library`. A built-in test suite ensures functionality through comprehensive positive and negative test cases.

---

## Features
- **Book Management:** Add, remove, and search books by title, author, or year.
- **User Management:** Register/deregister users and enforce borrowing limits.
- **Borrowing & Returning:** Borrow books if available, return them, and prevent duplicate borrowing.
- **Validation & Error Handling:** Gracefully handle invalid operations such as removing borrowed books or borrowing unavailable copies.
- **Comprehensive Testing:** Automated test suite validates all core functions and edge cases.

---

## Class Design

| Class | Description |
|-------|-------------|
| **Book** | Represents a book with title, author, year, and copy counts. Includes methods for borrowing and returning copies. |
| **User** | Represents a user with an ID, name, borrowing limit, and record of borrowed books. |
| **Library** | Manages collections of books and users, providing methods for all library operations and searches. |

The design follows OOP principles including encapsulation, abstraction, and modularity, ensuring maintainable and scalable code.

---

