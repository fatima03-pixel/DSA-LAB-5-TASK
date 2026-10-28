#include <iostream>
#include <string>
#include <limits>
#include <utility>
using namespace std;

struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};

class LinkedList
{
private:
    BookNode* head;
    int count;

    BookNode* GetNodeAt(int index)
    {
        BookNode* current = head;
        for (int i = 0; i < index && current != nullptr; i++)
            current = current->next;
        return current;
    }

    void CopyBookData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    int GetCount() const { return count; }

    void Insert(BookNode* book)
    {
        if (book == nullptr) return;
        book->next = head;
        head = book;
        count++;
    }

    BookNode* Remove()
    {
        if (IsEmpty()) return nullptr;
        BookNode* book = head;
        head = head->next;
        book->next = nullptr;
        count--;
        return book;
    }

    bool IsEmpty() { return head == nullptr; }
    bool IsNotEmpty() { return head != nullptr; }

    void InsertAt(BookNode* book, int index)
    {
        if (book == nullptr) return;
        if (index < 0) index = 0;
        if (index > count) index = count;
        if (index == 0)
        {
            Insert(book);
            return;
        }
        BookNode* previous = GetNodeAt(index - 1);
        book->next = previous->next;
        previous->next = book;
        count++;
    }

    BookNode* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        if (index < 0) index = 0;
        if (index >= count) index = count - 1;
        if (index == 0) return Remove();
        BookNode* previous = GetNodeAt(index - 1);
        BookNode* book = previous->next;
        previous->next = book->next;
        book->next = nullptr;
        count--;
        return book;
    }

    void Swap(int index1, int index2)
    {
        if (index1 < 0 || index1 >= count ||
            index2 < 0 || index2 >= count)
            return;
        if (index1 == index2) return;
        CopyBookData(GetNodeAt(index1), GetNodeAt(index2));
    }

    void Sort()
    {
        for (BookNode* i = head; i != nullptr; i = i->next)
            for (BookNode* j = i->next; j != nullptr; j = j->next)
                if (i->bookID > j->bookID)
                    CopyBookData(i, j);
    }

    void Reverse()
    {
        BookNode* previous = nullptr;
        BookNode* current = head;
        BookNode* next = nullptr;
        while (current != nullptr)
        {
            next = current->next;
            current->next = previous;
            previous = current;
            current = next;
        }
        head = previous;
    }

    void Print()
    {
        BookNode* current = head;
        int index = 0;
        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "-----------------------------------\n";
        while (current != nullptr)
        {
            cout << index << "\t" << current->bookID << "\t"
                 << current->title << endl;
            current = current->next;
            index++;
        }
        cout << "Total books: " << count << endl;
    }

    LinkedList(const LinkedList& other) : head(nullptr), count(0)
    {
        BookNode* current = other.head;
        while (current != nullptr)
        {
            InsertAt(new BookNode{ current->bookID, current->title, nullptr }, count);
            current = current->next;
        }
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (this == &other) return *this;
        while (!IsEmpty()) delete Remove();
        BookNode* current = other.head;
        while (current != nullptr)
        {
            InsertAt(new BookNode{ current->bookID, current->title, nullptr }, count);
            current = current->next;
        }
        return *this;
    }

    ~LinkedList()
    {
        while (!IsEmpty()) delete Remove();
    }
};

BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;
    book->bookID = id;
    book->title = title;
    book->next = nullptr;
    return book;
}

// Read an integer safely; re-prompt until valid
int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

BookNode* ReadBook()
{
    int id = ReadInt("Enter Book ID: ");
    string title;
    cout << "Enter Book Title: ";
    getline(cin, title);
    return CreateBook(id, title);
}

void ShowMenu()
{
    cout << "\n========================================\n"
         << "     LIBRARY BOOK COLLECTION SYSTEM\n"
         << "========================================\n"
         << "1. Display All Books\n"
         << "2. Insert Book at Head\n"
         << "3. Remove Book from Head\n"
         << "4. Insert Book at Index\n"
         << "5. Remove Book at Index\n"
         << "6. Swap Books\n"
         << "7. Sort Books by ID\n"
         << "8. Reverse Book Collection\n"
         << "9. Check Collection Status\n"
         << "10. Exit\n";
}

// ========================================
// Main
// ========================================
int main()
{
    LinkedList library;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            library.Print();
            break;

        case 2:
            library.Insert(ReadBook());
            cout << "Book inserted at head.\n";
            break;

        case 3:
        {
            BookNode* removed = library.Remove();
            if (removed != nullptr)
            {
                cout << "Removed from head: " << removed->title << endl;
                delete removed;
            }
            else
                cout << "The collection is empty.\n";
            break;
        }

        case 4:
        {
            int index = ReadInt("Enter index (0 to " + to_string(library.GetCount()) + "): ");
            if (index < 0 || index > library.GetCount())
            {
                cout << "Invalid index.\n";
                break;
            }
            library.InsertAt(ReadBook(), index);
            cout << "Book inserted at index " << index << ".\n";
            break;
        }

        case 5:
        {
            if (library.IsEmpty())
            {
                cout << "The collection is empty.\n";
                break;
            }
            int index = ReadInt("Enter index (0 to " + to_string(library.GetCount() - 1) + "): ");
            if (index < 0 || index >= library.GetCount())
            {
                cout << "Invalid index.\n";
                break;
            }
            BookNode* removed = library.RemoveAt(index);
            cout << "Removed book: " << removed->title << endl;
            delete removed;
            break;
        }

        case 6:
        {
            if (library.GetCount() < 2)
            {
                cout << "At least two books are needed to swap.\n";
                break;
            }
            int a = ReadInt("Enter first index: ");
            int b = ReadInt("Enter second index: ");
            if (a < 0 || a >= library.GetCount() || b < 0 || b >= library.GetCount())
                cout << "Invalid index.\n";
            else
            {
                library.Swap(a, b);
                cout << "Books swapped.\n";
            }
            break;
        }

        case 7:
            library.Sort();
            cout << "Books sorted by ID.\n";
            break;

        case 8:
            library.Reverse();
            cout << "Collection reversed.\n";
            break;

        case 9:
            if (library.IsEmpty())
                cout << "The collection is EMPTY.\n";
            else
                cout << "The collection is NOT empty. Total books: "
                     << library.GetCount() << endl;
            break;

        case 10:
            cout << "Exiting. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please enter 1-10.\n";
        }
    } while (choice != 10);

    return 0;
}