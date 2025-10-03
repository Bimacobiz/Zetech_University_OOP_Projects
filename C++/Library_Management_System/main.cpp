#include "library.hpp"
#include <iostream>
#include <limits>

using namespace std;

void displayMenu() {
    cout << "\nLibrary Management System\n";
    cout << "1. Add Book\n";
    cout << "2. Remove Book\n";
    cout << "3. List All Books\n";
    cout << "4. Search by Title\n";
    cout << "5. Search by Author\n";
    cout << "6. Search by Year\n";
    cout << "7. Borrow Book\n";
    cout << "8. Return Book\n";
    cout << "0. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    cout << "[debug] main: program started" << endl;

    // construct library and print debug
    cout << "[debug] main: constructing Library with 'library.csv'" << endl;
    Library library("library.csv");
    cout << "[debug] main: Library constructed" << endl;

    int choice;
    while (true) {
        displayMenu();

        if (!(cin >> choice)) {
            cerr << "[error] main: invalid input for menu choice" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // clear newline before any getline usage
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "[debug] main: selected choice=" << choice << endl;

        try {
            if (choice == 1) {
                cout << "[debug] main: handling Add Book" << endl;
                string title, author;
                int year;
                cout << "Enter title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter year: ";
                if (!(cin >> year)) {
                    cerr << "[error] main: invalid year input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                library.addBook(title, author, year);
            }
            else if (choice == 2) {
                cout << "[debug] main: handling Remove Book" << endl;
                int id;
                cout << "Enter Book ID to remove: ";
                if (!(cin >> id)) {
                    cerr << "[error] main: invalid ID input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                library.removeBook(id);
            }
            else if (choice == 3) {
                cout << "[debug] main: handling List All Books" << endl;
                auto books = library.listBooks();
                if (books.empty()) {
                    cout << "No books available in the library." << endl;
                } else {
                    cout << "\nAll Books:\n";
                    for (const auto &b : books) {
                        cout << b.id << ". " << b.title << " by " << b.author
                             << " (" << b.year << ") " << (b.isBorrowed ? "[Borrowed]" : "[Available]") << endl;
                    }
                }
            }
            else if (choice == 4) {
                cout << "[debug] main: handling Search by Title" << endl;
                string title;
                cout << "Enter title to search: ";
                getline(cin, title);
                auto results = library.searchByTitle(title);
                if (results.empty()) {
                    cout << "No books found. Try again later when available." << endl;
                } else {
                    for (const auto &b : results) {
                        cout << b.id << ". " << b.title << " by " << b.author
                             << " (" << b.year << ") " << (b.isBorrowed ? "[Borrowed]" : "[Available]") << endl;
                    }
                }
            }
            else if (choice == 5) {
                cout << "[debug] main: handling Search by Author" << endl;
                string author;
                cout << "Enter author to search: ";
                getline(cin, author);
                auto results = library.searchByAuthor(author);
                if (results.empty()) {
                    cout << "No books found. Try again later when available." << endl;
                } else {
                    for (const auto &b : results) {
                        cout << b.id << ". " << b.title << " by " << b.author
                             << " (" << b.year << ") " << (b.isBorrowed ? "[Borrowed]" : "[Available]") << endl;
                    }
                }
            }
            else if (choice == 6) {
                cout << "[debug] main: handling Search by Year" << endl;
                int year;
                cout << "Enter year to search: ";
                if (!(cin >> year)) {
                    cerr << "[error] main: invalid year input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                auto results = library.searchByYear(year);
                if (results.empty()) {
                    cout << "No books found. Try again later when available." << endl;
                } else {
                    for (const auto &b : results) {
                        cout << b.id << ". " << b.title << " by " << b.author
                             << " (" << b.year << ") " << (b.isBorrowed ? "[Borrowed]" : "[Available]") << endl;
                    }
                }
            }
            else if (choice == 7) {
                cout << "[debug] main: handling Borrow Book" << endl;
                int id;
                cout << "Enter Book ID to borrow: ";
                if (!(cin >> id)) {
                    cerr << "[error] main: invalid ID input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                library.borrowBook(id);
                cout << "Borrowed if ID existed and was available." << endl;
            }
            else if (choice == 8) {
                cout << "[debug] main: handling Return Book" << endl;
                int id;
                cout << "Enter Book ID to return: ";
                if (!(cin >> id)) {
                    cerr << "[error] main: invalid ID input" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                library.returnBook(id);
                cout << "Returned if ID existed and was borrowed." << endl;
            }
            else if (choice == 0) {
                cout << "[debug] main: exiting" << endl;
                cout << "Exiting and saving data... Goodbye!" << endl;
                break;
            }
            else {
                cerr << "[warn] main: invalid option. Try again." << endl;
            }
        } catch (const exception &e) {
            cerr << "[exception] main: " << e.what() << endl;
        }
    }

    return 0;
}

