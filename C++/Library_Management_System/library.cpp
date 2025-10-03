#include "library.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Book::Book(int id, const string &title, const string &author, int year, bool borrowed)
    : id(id), title(title), author(author), year(year), isBorrowed(borrowed) {}

Library::Library(const string &filename) : nextId(1), dataFile(filename) {
    cout << "[debug] Library::Library - start, dataFile='" << dataFile << "'" << endl;
    loadFromFile();
    cout << "[debug] Library::Library - done (nextId=" << nextId << ", entries=" << books.size() << ")" << endl;
}

void Library::addBook(const string &title, const string &author, int year) {
    int id = nextId;
    books[id] = Book(id, title, author, year, false);
    nextId++;
    cout << "[debug] addBook: added id=" << id << " title='" << title << "'" << endl;

    // try to save but do not throw to caller; just report error if any
    try {
        saveToFile();
        cout << "[debug] addBook: saveToFile succeeded" << endl;
    } catch (const exception &e) {
        cerr << "[error] addBook: saveToFile failed: " << e.what() << endl;
    }
}

void Library::removeBook(int id) {
    cout << "[debug] removeBook: id=" << id << endl;
    auto it = books.find(id);
    if (it == books.end()) {
        throw runtime_error("Book ID not found.");
    }
    books.erase(it);
    try {
        saveToFile();
        cout << "[debug] removeBook: saveToFile succeeded" << endl;
    } catch (const exception &e) {
        cerr << "[error] removeBook: saveToFile failed: " << e.what() << endl;
    }
}

vector<Book> Library::listBooks() const {
    vector<Book> result;
    for (const auto &p : books) result.push_back(p.second);
    return result;
}

vector<Book> Library::searchByTitle(const string &title) const {
    vector<Book> results;
    for (const auto &p : books) {
        if (p.second.title.find(title) != string::npos) results.push_back(p.second);
    }
    return results;
}

vector<Book> Library::searchByAuthor(const string &author) const {
    vector<Book> results;
    for (const auto &p : books) {
        if (p.second.author.find(author) != string::npos) results.push_back(p.second);
    }
    return results;
}

vector<Book> Library::searchByYear(int year) const {
    vector<Book> results;
    for (const auto &p : books) {
        if (p.second.year == year) results.push_back(p.second);
    }
    return results;
}

void Library::borrowBook(int id) {
    cout << "[debug] borrowBook: id=" << id << endl;
    auto it = books.find(id);
    if (it == books.end()) throw runtime_error("Book ID not found.");
    if (it->second.isBorrowed) throw runtime_error("Book already borrowed.");
    it->second.isBorrowed = true;
    try {
        saveToFile();
        cout << "[debug] borrowBook: saveToFile succeeded" << endl;
    } catch (const exception &e) {
        cerr << "[error] borrowBook: saveToFile failed: " << e.what() << endl;
    }
}

void Library::returnBook(int id) {
    cout << "[debug] returnBook: id=" << id << endl;
    auto it = books.find(id);
    if (it == books.end()) throw runtime_error("Book ID not found.");
    if (!it->second.isBorrowed) throw runtime_error("Book was not borrowed.");
    it->second.isBorrowed = false;
    try {
        saveToFile();
        cout << "[debug] returnBook: saveToFile succeeded" << endl;
    } catch (const exception &e) {
        cerr << "[error] returnBook: saveToFile failed: " << e.what() << endl;
    }
}

void Library::saveToFile() const {
    cout << "[debug] saveToFile: opening '" << dataFile << "'" << endl;
    ofstream file(dataFile);
    if (!file.is_open()) {
        // Throw to indicate failure to caller (caller catches)
        throw runtime_error(string("Unable to open file for saving: ") + dataFile);
    }

    int count = 0;
    for (const auto &p : books) {
        const Book &b = p.second;
        // Replace any newline characters in fields to keep CSV consistent
        string safeTitle = b.title;
        string safeAuthor = b.author;
        replace(safeTitle.begin(), safeTitle.end(), '\n', ' ');
        replace(safeAuthor.begin(), safeAuthor.end(), '\n', ' ');

        file << b.id << "," << safeTitle << "," << safeAuthor << "," << b.year << "," << (b.isBorrowed ? "1" : "0") << "\n";
        ++count;
    }
    file.close();
    cout << "[debug] saveToFile: wrote " << count << " entries" << endl;
}

void Library::loadFromFile() {
    cout << "[debug] loadFromFile: attempting to open '" << dataFile << "'" << endl;
    ifstream file(dataFile);
    if (!file.is_open()) {
        cout << "[debug] loadFromFile: file not found or cannot open (that's OK on first run)" << endl;
        return;
    }
    cout << "[debug] loadFromFile: file opened" << endl;

    books.clear();
    string line;
    int maxId = 0;
    int lineNo = 0;
    while (getline(file, line)) {
        ++lineNo;
        if (line.empty()) {
            cout << "[debug] loadFromFile: skipping empty line " << lineNo << endl;
            continue;
        }

        // parse up to 5 fields, borrowed field optional for backward compatibility
        stringstream ss(line);
        string idStr, title, author, yearStr, borrowedStr;
        if (!getline(ss, idStr, ',')) {
            cerr << "[warn] loadFromFile: missing id at line " << lineNo << endl;
            continue;
        }
        if (!getline(ss, title, ',')) title = "";
        if (!getline(ss, author, ',')) author = "";
        if (!getline(ss, yearStr, ',')) yearStr = "";
        // borrowed may or may not be present
        if (!getline(ss, borrowedStr, ',')) borrowedStr = "";

        // Trim whitespace (simple)
        auto trim = [](string &s) {
            while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
            while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
        };
        trim(idStr);
        trim(title);
        trim(author);
        trim(yearStr);
        trim(borrowedStr);

        if (idStr.empty() || yearStr.empty()) {
            cerr << "[warn] loadFromFile: missing id or year at line " << lineNo << " -> skipping" << endl;
            continue;
        }

        try {
            int id = stoi(idStr);
            int year = stoi(yearStr);
            bool borrowed = false;
            if (!borrowedStr.empty()) {
                if (borrowedStr == "1" || borrowedStr == "true" || borrowedStr == "True") borrowed = true;
            }
            books[id] = Book(id, title, author, year, borrowed);
            if (id > maxId) maxId = id;
            cout << "[debug] loadFromFile: parsed line " << lineNo << " -> id=" << id
                 << " title='" << title << "' author='" << author << "' year=" << year
                 << " borrowed=" << borrowed << endl;
        } catch (const exception &e) {
            cerr << "[warn] loadFromFile: parse error on line " << lineNo << ": " << e.what() << endl;
            continue;
        }
    }

    nextId = maxId + 1;
    file.close();
    cout << "[debug] loadFromFile: finished, nextId=" << nextId << ", entries=" << books.size() << endl;
}

