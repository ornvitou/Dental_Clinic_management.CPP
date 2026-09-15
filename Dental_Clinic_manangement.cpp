#include <iostream>
#include <string>
using namespace std;

struct Service {
    string name;
    double price;
};

Service services[5] = {
    {"Teeth Cleaning",   30.00},
    {"Cavity Filling",   80.00},
    {"Root Canal",       250.00},
    {"Tooth Extraction", 100.00},
    {"Teeth Whitening",  150.00}
};

struct Dentist {
    int    id;
    string name;
    string phone;
    int    appointmentPatientId[100];
    int    patientCount;
};

Dentist dentists[5] = {
    {1, "Dr. Sok Dara",    "012111222", {}, 0},
    {2, "Dr. Chan Sophea", "012333444", {}, 0},
    {3, "Dr. Lim Vanna",   "012555666", {}, 0},
    {4, "Dr. Ry Pisach",   "012777888", {}, 0},
    {5, "Dr. Heng Kunthea","012999000", {}, 0}
};

struct Patient {
    int     patientId;
    string  name;
    int     age;
    string  email;
    string  phone;
    string  startDate;
    int     serviceIndex;
    int     dentistId;
    Patient* next;
};

Patient* head = nullptr;

int dentistIndexFromId(int id){
    for (int i = 0; i < 5; i++) {
        if (dentists[i].id == id) return i;
    }
    return -1;
}

bool patientIdExists(int id) {
    Patient* temp = head;
    while (temp != nullptr) {
        if (temp->patientId == id) return true;
        temp = temp->next;
    }
    return false;
}

void insertPatientNode(Patient* newPatient){
    newPatient->next = nullptr;
    if (head == nullptr){
        head = newPatient;
        return;
    }

    Patient* temp = head;
    while (temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = newPatient;
}

void insertPatients(){
    int count;

    cout << "How many patients do you want to insert? "; cin >> count;

    int nextId = 1; 

    for (int i = 0; i < count; i++) {
        cout << "\n--- Patient #" <<(i + 1)<< " ---\n";

        Patient* p = new Patient();

        while (patientIdExists(nextId)) {
            nextId++;
        }

        p->patientId = nextId;
        nextId++;

        cout << "Assigned Patient ID: " << p->patientId << "\n";

        cout << "Enter Patient Name: ";cin.ignore();getline(cin, p->name);

        cout << "Enter Patient Age: ";cin >> p->age;cin.ignore();

        cout << "Enter Patient Email (press Enter to skip): ";getline(cin, p->email);

        cout << "Enter Patient Phone Number: ";getline(cin, p->phone);

        cout << "Enter Start Date (e.g. 2026-09-15): ";getline(cin, p->startDate);

        cout << "\nAvailable Services:\n";
        for (int j = 0; j < 5; j++) {
            cout << "  " << (j + 1) << ". " << services[j].name << " - $" << services[j].price << "\n";
        }

        int choice;
        
        while (true) {
            cout << "Choose a service (1-5): ";cin >> choice;
            if (choice >= 1 && choice <= 5) break;
            cout << "Invalid choice, try again.\n";
        }
        p->serviceIndex = choice - 1;

        p->dentistId = dentists[choice-1].id;
        dentists[choice-1].appointmentPatientId[dentists[choice-1].patientCount] = p->patientId;
        dentists[choice-1].patientCount++;
        insertPatientNode(p);

        cout << "\nPatient added successfully! Assigned to " << dentists[choice-1].name
             << " for " << services[p->serviceIndex].name << ".\n";
    }
}

void showAllPatients() {
    if (head == nullptr) {
        cout << "No patients in the system yet.\n";
        return;
    }

    Patient* temp = head;
    cout << "\n===================== ALL PATIENTS =====================\n";
    while (temp != nullptr) {
        int dIdx = dentistIndexFromId(temp->dentistId);
        cout << "Patient ID   : " << temp->patientId << "\n";
        cout << "Name         : " << temp->name << "\n";
        cout << "Age          : " << temp->age << "\n";
        cout << "Email        : " << (temp->email.empty() ? "N/A" : temp->email) << "\n";
        cout << "Phone        : " << temp->phone << "\n";
        cout << "Start Date   : " << temp->startDate << "\n";
        cout << "Service      : " << services[temp->serviceIndex].name << " ($" << services[temp->serviceIndex].price << ")\n";
        cout << "Dentist      : " << (dIdx != -1 ? dentists[dIdx].name : "N/A") << "\n";
        cout << "----------------------------------------------------------\n";
        temp = temp->next;
    }
}

void showAppointments(){
    if (head == nullptr){
        cout << "No appointments to show.\n";
        return;
    }

    cout << "\n================ DENTIST - PATIENT APPOINTMENTS ================\n";

    Patient* temp = head;
    while (temp != nullptr){
        int dIdx = dentistIndexFromId(temp->dentistId);
        cout << (dIdx != -1 ? dentists[dIdx].name : "N/A") << " | Patient_id: " << temp->patientId <<
         " | Service: " << services[temp->serviceIndex].name << "\n";
        temp = temp->next;
    }
}

void dischargePatient() {

    if (head == nullptr){
        cout << "No patients to discharge.\n";
        return;
    }

    int id;

    cout << "Enter Patient ID to discharge (after checkup): ";cin >> id;

    Patient* current = head;
    Patient* prev = nullptr;

    while (current != nullptr && current->patientId != id) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Patient ID not found.\n";
        return;
    }

    cout << "\nPatient: " << current->name << "\n";
    cout << "Service received: " << services[current->serviceIndex].name << "\n";
    cout << "Total Service Price: $" << services[current->serviceIndex].price << "\n";

    int dIdx = dentistIndexFromId(current->dentistId);

    if (dIdx != -1) {
        Dentist &d = dentists[dIdx];
        for (int i = 0; i < d.patientCount; i++){
            if (d.appointmentPatientId[i] == id){
                for (int j = i; j < d.patientCount - 1; j++) {
                    d.appointmentPatientId[j] = d.appointmentPatientId[j + 1];
                }
                d.patientCount--;
                break;
            }
        }
    }

    if (prev == nullptr) {
        head = current->next;
    } else {
        prev->next = current->next;
    }
    delete current;

    cout << "Patient has been discharged and removed from the system.\n";
}

void countBusyDentists() {
    int busyCount = 0;
    cout << "\n================ DENTIST WORKLOAD ================\n";
    for (int i = 0; i < 5; i++) {
        cout << dentists[i].name << " -> " << dentists[i].patientCount << " patient(s)\n";
        if (dentists[i].patientCount > 0) {
            busyCount++;
        }
    }
    cout << "----------------------------------------------------\n";
    cout << "Total dentists currently with patients to check up: " << busyCount << " / 5\n";
}

void freeAllPatients() {
    Patient* temp = head;
    while (temp != nullptr) {
        Patient* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
    head = nullptr;
}

void printMenu() {
    cout << "\n=====================================================\n";
    cout << "            DENTAL CLINIC MANAGEMENT SYSTEM\n";
    cout << "=====================================================\n";
    cout << "1. Insert Patient(s)\n";
    cout << "2. Show All Patient Data\n";
    cout << "3. Show Appointment (Dentist - Patient - Service)\n";
    cout << "4. Discharge Patient (Show Total Service Price)\n";
    cout << "5. Show How Many Dentists Have Patients to Check Up\n";
    cout << "X. Exit\n";
    cout << "=====================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    srand((unsigned int)time(0));

    string choice;
    bool running = true;

    while (running) {
        printMenu();
        cin >> choice;

        if (choice == "1") {
            insertPatients();
        } else if (choice == "2") {
            showAllPatients();
        } else if (choice == "3") {
            showAppointments();
        } else if (choice == "4") {
            dischargePatient();
        } else if (choice == "5") {
            countBusyDentists();
        } else if (choice == "x" || choice == "X") {
            cout << "Exiting the Dental Clinic Management System. Goodbye!\n";
            running = false;
        } else {
            cout << "Invalid choice, please try again.\n";
        }
    }
    freeAllPatients();
}