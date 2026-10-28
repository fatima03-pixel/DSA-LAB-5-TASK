
#include <iostream>
#include <string>
#include <utility>
using namespace std;

struct DeliveryStop
{
    int orderID;
    string customer;
    string address;
    string food;
    double amount;
    DeliveryStop* next;
};

class LinkedList
{
private:
    DeliveryStop* head;
    int count;

    DeliveryStop* GetNodeAt(int index)
    {
        DeliveryStop* current = head;
        for (int i = 0; i < index && current != nullptr; i++)
            current = current->next;
        return current;
    }

    void CopyStopData(DeliveryStop* a, DeliveryStop* b)
    {
        swap(a->orderID, b->orderID);
        swap(a->customer, b->customer);
        swap(a->address, b->address);
        swap(a->food, b->food);
        swap(a->amount, b->amount);
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    void Insert(DeliveryStop* stop)
    {
        if (stop == nullptr) return;
        stop->next = head;
        head = stop;
        count++;
    }

    DeliveryStop* Remove()
    {
        if (IsEmpty()) return nullptr;
        DeliveryStop* stop = head;
        head = head->next;
        stop->next = nullptr;
        count--;
        return stop;
    }

    bool IsEmpty() { return head == nullptr; }
    bool IsNotEmpty() { return head != nullptr; }

    void InsertAt(DeliveryStop* stop, int index)
    {
        if (stop == nullptr) return;
        if (index < 0) index = 0;
        if (index > count) index = count;
        if (index == 0)
        {
            Insert(stop);
            return;
        }
        DeliveryStop* previous = GetNodeAt(index - 1);
        stop->next = previous->next;
        previous->next = stop;
        count++;
    }

    DeliveryStop* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        if (index < 0) index = 0;
        if (index >= count) index = count - 1;
        if (index == 0) return Remove();
        DeliveryStop* previous = GetNodeAt(index - 1);
        DeliveryStop* stop = previous->next;
        previous->next = stop->next;
        stop->next = nullptr;
        count--;
        return stop;
    }

    void Swap(int index1, int index2)
    {
        if (index1 < 0 || index1 >= count ||
            index2 < 0 || index2 >= count || index1 == index2)
            return;
        CopyStopData(GetNodeAt(index1), GetNodeAt(index2));
    }

    void Sort()   // ascending by order ID
    {
        for (DeliveryStop* i = head; i != nullptr; i = i->next)
            for (DeliveryStop* j = i->next; j != nullptr; j = j->next)
                if (i->orderID > j->orderID)
                    CopyStopData(i, j);
    }

    void Reverse()
    {
        DeliveryStop* previous = nullptr;
        DeliveryStop* current = head;
        DeliveryStop* next = nullptr;
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
        DeliveryStop* current = head;
        int stopNo = 0;
        double total = 0;
        cout << "\nStop\tOrder\tCustomer\tAddress\t\tFood\t\t\tAmount\n";
        cout << "----------------------------------------------------------------------------\n";
        while (current != nullptr)
        {
            cout << stopNo << "\t" << current->orderID << "\t"
                 << current->customer << "\t\t"
                 << current->address << "\t"
                 << current->food << "\t"
                 << current->amount << endl;
            total += current->amount;
            current = current->next;
            stopNo++;
        }
        cout << "Total stops: " << count << "  |  Cash to collect: " << total << endl;
    }

    LinkedList(const LinkedList& other) : head(nullptr), count(0)
    {
        for (DeliveryStop* c = other.head; c != nullptr; c = c->next)
            InsertAt(new DeliveryStop{ c->orderID, c->customer, c->address,
                                       c->food, c->amount, nullptr }, count);
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (this == &other) return *this;
        while (!IsEmpty()) delete Remove();
        for (DeliveryStop* c = other.head; c != nullptr; c = c->next)
            InsertAt(new DeliveryStop{ c->orderID, c->customer, c->address,
                                       c->food, c->amount, nullptr }, count);
        return *this;
    }

    ~LinkedList()
    {
        while (!IsEmpty()) delete Remove();
    }
};

DeliveryStop* CreateStop(int id, string customer, string address,
                         string food, double amount)
{
    DeliveryStop* stop = new DeliveryStop;
    stop->orderID = id;
    stop->customer = customer;
    stop->address = address;
    stop->food = food;
    stop->amount = amount;
    stop->next = nullptr;
    return stop;
}

int main()
{
    LinkedList route;

    route.InsertAt(CreateStop(203, "Ali", "Street 5", "Chicken Biryani", 650), 0);
    route.InsertAt(CreateStop(201, "Sara", "Block B", "Zinger Burger", 520), 1);
    route.InsertAt(CreateStop(205, "Usman", "Phase 2", "Pizza Large", 1400), 2);
    route.InsertAt(CreateStop(202, "Hina", "Main Road", "Karahi", 1800), 3);
    route.InsertAt(CreateStop(204, "Bilal", "Lane 9", "Shawarma", 300), 4);
    cout << "INITIAL DELIVERY ROUTE";
    route.Print();

    route.InsertAt(CreateStop(206, "Zara", "Park View", "Pasta", 750), 2);
    cout << "\nAFTER A NEW ORDER IS INSERTED AT STOP 2";
    route.Print();

    DeliveryStop* removed = route.RemoveAt(1);
    if (removed != nullptr)
    {
        cout << "\nCancelled order: " << removed->orderID
             << " (" << removed->customer << ")" << endl;
        delete removed;
    }
    cout << "\nAFTER CANCELLING STOP 1";
    route.Print();

    route.Swap(0, 2);
    cout << "\nAFTER SWAPPING STOP 0 AND STOP 2";
    route.Print();

    route.Sort();
    cout << "\nAFTER SORTING BY ORDER ID";
    route.Print();

    route.Reverse();
    cout << "\nAFTER REVERSING THE ROUTE";
    route.Print();

    route.Insert(CreateStop(207, "Noor", "Gate 1", "Fries", 250));
    cout << "\nAFTER AN URGENT ORDER IS ADDED AS THE FIRST STOP";
    route.Print();

    removed = route.Remove();
    if (removed != nullptr)
    {
        cout << "\nDelivered first stop: order " << removed->orderID << endl;
        delete removed;
    }

    cout << "\nMore stops remaining? " << (route.IsNotEmpty() ? "Yes" : "No") << endl;
    cout << "\nFINAL DELIVERY ROUTE";
    route.Print();

    return 0;
}