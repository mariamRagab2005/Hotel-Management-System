#include <iostream>
#include <string>
#include <queue>
#include <stack>
using namespace std;

// Room class represents individual rooms in the hotel
class Room {
public:
    int roomNumber;       // Room number
    bool isAvailable;     // Availability status of the room
    string guestName;     // Name of the guest who booked the room

    Room(int number) {
        roomNumber = number;
        isAvailable = true;
        guestName = "";
    }

    void bookRoom(string name) {
        isAvailable = false;
        guestName = name;
    }

    void freeRoom() {
        isAvailable = true;
        guestName = "";
    }
};

// Node structure for the linked list (to manage rooms)
struct Node {
    Room* room;
    Node* next;

    Node(Room* r) {
        room = r;
        next = nullptr;
    }
};

// RoomList class to manage rooms using a linked list
class RoomList {
private:
    Node* head;

public:
    RoomList() {
        head = nullptr;
    }

    void addRoom(int roomNumber) {
        Room* newRoom = new Room(roomNumber);
        Node* newNode = new Node(newRoom);

        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    Room* findAvailableRoom() {
        Node* temp = head;
        while (temp) {
            if (temp->room->isAvailable) {
                return temp->room;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    Room* findRoom(int roomNumber) {
        Node* temp = head;
        while (temp) {
            if (temp->room->roomNumber == roomNumber) {
                return temp->room;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void displayAvailableRooms() {
        Node* temp = head;
        cout << "Available Rooms:\n";
        while (temp) {
            if (temp->room->isAvailable) {
                cout << "Room " << temp->room->roomNumber << " is available.\n";
            }
            temp = temp->next;
        }
    }
};

// Function to handle room booking
void bookRoom(RoomList& roomList, string guestName, queue<string>& waitingList, stack<pair<int, string>>& history) {
    Room* availableRoom = roomList.findAvailableRoom();

    if (availableRoom) {
        availableRoom->bookRoom(guestName);
        history.push({availableRoom->roomNumber, guestName}); // Add to history stack
        cout << "Room " << availableRoom->roomNumber << " booked successfully for " << guestName << ".\n";
    } else {
        cout << "No rooms available. Adding " << guestName << " to the waiting list.\n";
        waitingList.push(guestName); // Add to waiting list queue
    }
}

// Function to handle check-out
void checkoutRoom(RoomList& roomList, int roomNumber, queue<string>& waitingList) {
    Room* room = roomList.findRoom(roomNumber);
    if (room) {
        if (!room->isAvailable) {
            cout << "Room " << roomNumber << " checked out by " << room->guestName << ".\n";
            room->freeRoom();

            // Assign room to the next guest in the waiting list
            if (!waitingList.empty()) {
                string nextGuest = waitingList.front();
                waitingList.pop();
                room->bookRoom(nextGuest);
                cout << "Room " << roomNumber << " is now booked for " << nextGuest << " from the waiting list.\n";
            } else {
                cout << "Room " << roomNumber << " is now available.\n";
            }
        } else {
            cout << "Room " << roomNumber << " is already free.\n";
        }
    } else {
        cout << "Room " << roomNumber << " does not exist.\n";
    }
}

// Function to display the check-in history
void displayHistory(stack<pair<int, string>> history) {
    if (history.empty()) {
        cout << "No check-in history available.\n";
        return;
    }

    cout << "Check-in History:\n";
    while (!history.empty()) {
        auto entry = history.top();
        history.pop();
        cout << "Room " << entry.first << " was booked by " << entry.second << ".\n";
    }
}

int main() {
    RoomList roomList;

    // Add some rooms
    for (int i = 1; i <= 10; ++i) {
        roomList.addRoom(i);
    }

    // Create a queue for the waiting list and a stack for check-in history
    queue<string> waitingList;
    stack<pair<int, string>> history;

    int choice, roomNumber;
    string guestName;

    do {
        cout << "\nHotel Management System:\n";
        cout << "1. Display Available Rooms\n";
        cout << "2. Book a Room\n";
        cout << "3. Check-out a Room\n";
        cout << "4. Display Check-in History\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            roomList.displayAvailableRooms();
            break;
        case 2:
            cout << "Enter guest name: ";
            cin.ignore();
            getline(cin, guestName);
            bookRoom(roomList, guestName, waitingList, history);
            break;
        case 3:
            cout << "Enter room number to check out: ";
            cin >> roomNumber;
            checkoutRoom(roomList, roomNumber, waitingList);
            break;
        case 4:
            displayHistory(history);
            break;
        case 5:
            cout << "Exiting system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);


}