#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <locale>

using namespace std;

// Mutex for synchronizing access to checkpoints
mutex checkpoint_mutex;

// Class PassengerData: stores passenger information
class PassengerData
{
private:
    string name;
    bool hasLuggage;
    bool hasTicket;
    bool isCashPayment;

public:
    PassengerData(string name, bool luggage, bool ticket, bool cashPayment)
        : name(name), hasLuggage(luggage), hasTicket(ticket), isCashPayment(cashPayment) {}

    // Methods to retrieve name and check conditions
    string getName() const { return name; }
    bool getHasLuggage() const { return hasLuggage; }
    bool getHasTicket() const { return hasTicket; }
    bool getIsCashPayment() const { return isCashPayment; }
};

// Class SecurityPost: inspects passenger's luggage
class SecurityPost
{
public:
    void inspectLuggage(const PassengerData &passenger)
    {
        lock_guard<mutex> lock(checkpoint_mutex);
        cout << passenger.getName() << " is undergoing luggage inspection.\n";
        this_thread::sleep_for(chrono::milliseconds(500)); // Simulating work
        cout << "Luggage inspection for " << passenger.getName() << " is complete.\n";
    }
};

// Class Turnstile: checks passenger access to the platform
class Turnstile
{
public:
    void checkAccess(const PassengerData &passenger)
    {
        lock_guard<mutex> lock(checkpoint_mutex);
        cout << passenger.getName() << " is passing through the turnstile.\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "The turnstile opened for " << passenger.getName() << ".\n";
    }
};

// Class TicketOffice: buying a ticket with cash
class TicketOffice
{
public:
    void buyTicket(const PassengerData &passenger)
    {
        lock_guard<mutex> lock(checkpoint_mutex);
        cout << passenger.getName() << " is buying a ticket with cash.\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Ticket for " << passenger.getName() << " has been purchased.\n";
    }
};

// Class Terminal: ticket payment via card
class Terminal
{
public:
    void processPayment(const PassengerData &passenger)
    {
        lock_guard<mutex> lock(checkpoint_mutex);
        cout << passenger.getName() << " is paying for the ticket via terminal.\n";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Payment for " << passenger.getName() << " is complete.\n";
    }
};

// Function to simulate a passenger passing through the station
void simulatePassenger(PassengerData passenger, SecurityPost &security, Turnstile &turnstile,
                       TicketOffice &ticketOffice, Terminal &terminal)
{
    if (passenger.getHasLuggage())
    {
        security.inspectLuggage(passenger);
    }
    turnstile.checkAccess(passenger);

    if (!passenger.getHasTicket())
    {
        if (passenger.getIsCashPayment())
        {
            ticketOffice.buyTicket(passenger);
        }
        else
        {
            terminal.processPayment(passenger);
        }
    }

    cout << passenger.getName() << " has entered the platform.\n\n";
}

int main()
{
    setlocale(LC_ALL, "English");

    // Create checkpoints
    SecurityPost security;
    Turnstile turnstile;
    TicketOffice ticketOffice;
    Terminal terminal;

    // List of passengers
    vector<PassengerData> passengers = {
        {"Anna", true, false, true},
        {"Boris", false, true, false},
        {"Victoria", true, false, false},
        {"Gregory", false, true, true},
        {"Daria", true, true, false}};

    // Create threads to simulate passengers
    vector<thread> threads;
    for (auto &passenger : passengers)
    {
        threads.emplace_back(simulatePassenger, passenger, ref(security), ref(turnstile),
                             ref(ticketOffice), ref(terminal));
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}