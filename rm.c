#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 100
#define MAX_NAME 50
#define FILENAME "railway_data.txt"
#define ADMIN_PASSWORD "admin123" // Admin password

typedef struct {
    int seat_number;
    char passenger_name[MAX_NAME];
    int reserved; // 0 = available, 1 = reserved
} Seat;

void initializeSeats(Seat seats[], int total_seats);
void displayAvailableSeats(Seat seats[], int total_seats);
void reserveSeat(Seat seats[], int total_seats);
void adminView(Seat seats[], int total_seats);
void loadSeatsFromFile(Seat seats[], int total_seats);
void saveSeatsToFile(Seat seats[], int total_seats);
int adminLogin();

int main() {
    int choice;
    Seat seats[MAX_SEATS];

    // Load seat data from the file (only when admin accesses)
    loadSeatsFromFile(seats, MAX_SEATS);

    while (1) {
        printf("\n=== Indian Railway Management System ===\n");
        printf("1. View Available Seats\n");
        printf("2. Reserve a Seat\n");
        printf("3. Admin Login (Manage Reservations)\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAvailableSeats(seats, MAX_SEATS);
                break;
            case 2:
                reserveSeat(seats, MAX_SEATS);
                saveSeatsToFile(seats, MAX_SEATS); // Save after each reservation
                break;
            case 3:
                if (adminLogin()) {
                    adminView(seats, MAX_SEATS);
                } else {
                    printf("Invalid password. Access denied.\n");
                }
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

// Function to authenticate admin
int adminLogin() {
    char password[20];
    printf("Enter admin password: ");
    scanf("%s", password);
    if (strcmp(password, ADMIN_PASSWORD) == 0) {
        return 1; // Success
    }
    return 0; // Failure
}

// Initialize seats
void initializeSeats(Seat seats[], int total_seats) {
    for (int i = 0; i < total_seats; i++) {
        seats[i].seat_number = i + 1;
        strcpy(seats[i].passenger_name, "Empty");
        seats[i].reserved = 0;
    }
}

// Display available seats
void displayAvailableSeats(Seat seats[], int total_seats) {
    printf("\nAvailable Seats:\n");
    for (int i = 0; i < total_seats; i++) {
        if (seats[i].reserved == 0) {
            printf("Seat %d\n", seats[i].seat_number);
        }
    }
}

// Reserve a seat
void reserveSeat(Seat seats[], int total_seats) {
    int seat_number;
    char name[MAX_NAME];

    printf("Enter seat number to reserve: ");
    scanf("%d", &seat_number);

    if (seat_number < 1 || seat_number > total_seats) {
        printf("Invalid seat number!\n");
        return;
    }

    if (seats[seat_number - 1].reserved == 1) {
        printf("Seat already reserved!\n");
        return;
    }

    printf("Enter your name: ");
    scanf("%s", name);

    seats[seat_number - 1].reserved = 1;
    strcpy(seats[seat_number - 1].passenger_name, name);

    printf("Seat %d reserved successfully for %s.\n", seat_number, name);
}

// Admin can view all reservations
void adminView(Seat seats[], int total_seats) {
    printf("\nAll Reservations (Admin Only):\n");
    for (int i = 0; i < total_seats; i++) {
        printf("Seat %d - %s [%s]\n", seats[i].seat_number, seats[i].passenger_name, seats[i].reserved ? "Reserved" : "Available");
    }
}

// Load seat data from file
void loadSeatsFromFile(Seat seats[], int total_seats) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No data found, initializing new seat data...\n");
        initializeSeats(seats, total_seats);
        saveSeatsToFile(seats, total_seats);
        return;
    }

    for (int i = 0; i < total_seats; i++) {
        fscanf(file, "%d %s %d", &seats[i].seat_number, seats[i].passenger_name, &seats[i].reserved);
    }

    fclose(file);
}

// Save seat data to file
void saveSeatsToFile(Seat seats[], int total_seats) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    for (int i = 0; i < total_seats; i++) {
        fprintf(file, "%d %s %d\n", seats[i].seat_number, seats[i].passenger_name, seats[i].reserved);
    }

    fclose(file);
}
