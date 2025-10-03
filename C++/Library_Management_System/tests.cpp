#include "library.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdio>

using namespace std;

void runTests() {
    cout << "[debug] tests: start" << endl;

    const string testFile = "test_library.csv";
    // remove old test file
    if (remove(testFile.c_str()) == 0) {
        cout << "[debug] tests: removed old test file '" << testFile << "'" << endl;
    } else {
        cout << "[debug] tests: no old test file to remove (or remove failed)" << endl;
    }

    Library lib(testFile);
    cout << "[debug] tests: library created for '" << testFile << "'" << endl;

    cout << "[debug] tests: adding books" << endl;
    lib.addBook("C++ Primer", "Collins Mahigi", 2020);
    lib.addBook("Effective C++", "James Kipsoi", 2021);

    cout << "[debug] tests: searching by title 'C++'" << endl;
    auto titleSearch = lib.searchByTitle("C++");
    if (titleSearch.empty()) throw runtime_error("Test failed: expected C++ title");

    cout << "[debug] tests: searching by author 'James'" << endl;
    auto authorSearch = lib.searchByAuthor("James");
    if (authorSearch.size() != 1) throw runtime_error("Test failed: expected 1 book by author James");

    // find ID of "Effective C++"
    int effectiveId = -1;
    auto all = lib.listBooks();
    for (const auto &b : all) {
        if (b.title == "Effective C++") {
            effectiveId = b.id;
            break;
        }
    }
    if (effectiveId == -1) throw runtime_error("Test failed: could not find 'Effective C++' id");

    cout << "[debug] tests: borrowing id=" << effectiveId << endl;
    lib.borrowBook(effectiveId);
    // reload to ensure persistence works
    Library lib2(testFile);
    bool foundBorrowed = false;
    for (auto &b : lib2.listBooks()) {
        if (b.id == effectiveId && b.isBorrowed) foundBorrowed = true;
    }
    if (!foundBorrowed) throw runtime_error("Test failed: book should be marked borrowed after borrowBook");

    cout << "[debug] tests: returning id=" << effectiveId << endl;
    lib.returnBook(effectiveId);
    Library lib3(testFile);
    bool foundReturned = false;
    for (auto &b : lib3.listBooks()) {
        if (b.id == effectiveId && !b.isBorrowed) foundReturned = true;
    }
    if (!foundReturned) throw runtime_error("Test failed: book should be marked returned after returnBook");

    cout << "[debug] tests: removing one book" << endl;
    // remove the first book in current list
    auto remaining = lib.listBooks();
    if (remaining.empty()) throw runtime_error("Test failed: expected at least 1 book");
    int removeId = remaining.front().id;
    lib.removeBook(removeId);

    auto finalList = lib.listBooks();
    cout << "[debug] tests: final book count = " << finalList.size() << endl;

    cout << "[debug] tests: all checks passed" << endl;
    cout << "All tests passed successfully!" << endl;
}

int main() {
    try {
        runTests();
    } catch (const exception &e) {
        cerr << "[error] tests: " << e.what() << endl;
        return 1;
    }
    return 0;
}

