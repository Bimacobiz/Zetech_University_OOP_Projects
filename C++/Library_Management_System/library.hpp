#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <string>
#include <vector>
#include <map>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int year;
    bool isBorrowed;

    Book() = default;
    Book(int id, const string &title, const string &author, int year, bool borrowed = false);
};

class Library {
private:
    map<int, Book> books;
    int nextId;
    string dataFile;

    void saveToFile() const;
    void loadFromFile();

public:
    // default file in project root (avoid platform-specific directories)
    Library(const string &filename = "library.csv");

    void addBook(const string &title, const string &author, int year);
    void removeBook(int id);
    vector<Book> listBooks() const;

    vector<Book> searchByTitle(const string &title) const;
    vector<Book> searchByAuthor(const string &author) const;
    vector<Book> searchByYear(int year) const;

    void borrowBook(int id);
    void returnBook(int id);
};

#endif // LIBRARY_HPP

