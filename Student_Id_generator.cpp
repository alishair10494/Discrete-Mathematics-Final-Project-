#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
using namespace std;

// ==================== GLOBAL ARRAYS ====================
string studentNames[100];
string studentRolls[100];
long long studentSignatures[100];
int studentCount = 0;

// Activity log arrays
string activityLog[1000];
int activityCount = 0;

const long long MOD = 1000000007;
const int BASE = 31;

// ==================== FUNCTION PROTOTYPES ====================
void setColor(int k);
void printHeader();
void pause();
void readingDataFromFile();
void writingDataInFile();
int extractNumericFromRoll(string roll);
long long generateSignature(string name, string roll);
int searchStudent(string name, string roll);
void generateStudentSignature();
void verifyStudentData();
void viewAllRecords();
void collisionAnalysis();
void viewAuditLog();
void printMenu();
void addToActivityLog(string activity);
void readActivityLogFromFile();
void writeActivityLogToFile();

// ==================== UTILITY FUNCTIONS ====================
void setColor(int k)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k);
}

void printHeader()
{
    setColor(14);
    cout << "========================================================================" << endl;
    cout << "          STUDENT ID MATHEMATICAL SIGNATURE SYSTEM                      " << endl;
    cout << "========================================================================" << endl;
    setColor(7);
}

void pause()
{
    setColor(8);
    cout << "Press any key to continue..." << endl;
    setColor(7);
    getch();
}

void addToActivityLog(string activity)
{
    if (activityCount < 1000)
    {
        activityLog[activityCount] = activity;
        activityCount++;
        writeActivityLogToFile();
    }
}

// ==================== FILE HANDLING ====================
void readingDataFromFile()
{
    fstream file;
    file.open("StudentData.txt", ios::in);
    if (!file)
        return;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        string name = "", roll = "", sig = "";
        int commaCount = 0;

        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == ',')
            {
                commaCount++;
            }
            else
            {
                if (commaCount == 0)
                    name += line[i];
                else if (commaCount == 1)
                    roll += line[i];
                else if (commaCount == 2)
                    sig += line[i];
            }
        }

        if (name != "" && roll != "" && sig != "")
        {
            studentNames[studentCount] = name;
            studentRolls[studentCount] = roll;
            studentSignatures[studentCount] = stoll(sig);
            studentCount++;
        }
    }
    file.close();
}

void writingDataInFile()
{
    fstream file;
    file.open("StudentData.txt", ios::out);
    for (int i = 0; i < studentCount; i++)
    {
        file << studentNames[i] << "," << studentRolls[i] << "," << studentSignatures[i] << ",\n";
    }
    file.close();
}

void readActivityLogFromFile()
{
    fstream file;
    file.open("ActivityLog.txt", ios::in);
    if (!file)
        return;

    string line;
    while (getline(file, line))
    {
        if (!line.empty() && activityCount < 1000)
        {
            activityLog[activityCount] = line;
            activityCount++;
        }
    }
    file.close();
}

void writeActivityLogToFile()
{
    fstream file;
    file.open("ActivityLog.txt", ios::out);
    for (int i = 0; i < activityCount; i++)
    {
        file << activityLog[i] << endl;
    }
    file.close();
}

// ==================== SIGNATURE GENERATION ====================
int extractNumericFromRoll(string roll)
{
    int numeric = 0;
    for (int i = 0; i < roll.length(); i++)
    {
        if (roll[i] >= '0' && roll[i] <= '9')
        {
            numeric = numeric * 10 + (roll[i] - '0');
        }
    }
    return numeric;
}

long long generateSignature(string name, string roll)
{
    long long signature = 0;
    long long power = 1;

    for (int i = 0; i < name.length(); i++)
    {
        long long charValue = (long long)name[i];
        signature = (signature + (charValue * power) % MOD) % MOD;
        power = (power * BASE) % MOD;
    }

    int numericRoll = extractNumericFromRoll(roll);
    signature = (signature + numericRoll) % MOD;

    return signature;
}

// ==================== SEARCH FUNCTION ====================
int searchStudent(string name, string roll)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (studentNames[i] == name && studentRolls[i] == roll)
        {
            return i;
        }
    }
    return -1;
}

int searchStudentByRoll(string roll)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (studentRolls[i] == roll)
        {
            return i;
        }
    }
    return -1;
}

// ==================== MENU FUNCTIONS ====================
void generateStudentSignature()
{
    system("cls");
    printHeader();
    setColor(14);
    cout << "Generate Student Signature" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    if (studentCount >= 100)
    {
        setColor(4);
        cout << "Database full! Cannot add more students." << endl;
        setColor(7);
        pause();
        return;
    }

    string name, roll;
    cout << "Enter Student Name: ";
    getline(cin >> ws, name);
    cout << "Enter Roll Number (Format: 2025-CS-107): ";
    getline(cin >> ws, roll);

    if (name.length() == 0 || roll.length() == 0)
    {
        setColor(4);
        cout << "Invalid input! Name and Roll cannot be empty." << endl;
        setColor(7);
        pause();
        return;
    }

    // Check if roll number already exists
    int rollIdx = searchStudentByRoll(roll);
    if (rollIdx != -1)
    {
        setColor(4);
        cout << "Student with this roll number already exists in database!" << endl;
        cout << "Existing Student: " << studentNames[rollIdx] << " (" << studentRolls[rollIdx] << ")" << endl;
        setColor(7);
        pause();
        return;
    }

    long long signature = generateSignature(name, roll);

    studentNames[studentCount] = name;
    studentRolls[studentCount] = roll;
    studentSignatures[studentCount] = signature;
    studentCount++; 

    writingDataInFile();

    // Add to activity log
    string logEntry = "----> GENERATE | Name: " + name + " | Roll: " + roll + " | Signature: " + to_string(signature);
    addToActivityLog(logEntry);

    setColor(10);
    cout << "\n------------------------------------------------------------------------" << endl;
    cout << "Signature Generated Successfully!" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Name: " << name << endl;
    cout << "Roll: " << roll << endl;
    cout << "Mathematical Signature: " << signature << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    pause();
}

void verifyStudentData()
{
    system("cls");
    printHeader();
    setColor(14);
    cout << "Verify Data Integrity" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    string name, roll;
    cout << "Enter Student Name: ";
    getline(cin >> ws, name);
    cout << "Enter Roll Number: ";
    getline(cin >> ws, roll);

    if (name.length() == 0 || roll.length() == 0)
    {
        setColor(4);
        cout << "Invalid input!" << endl;
        setColor(7);
        pause();
        return;
    }

    int idx = searchStudent(name, roll);
    if (idx == -1)
    {
        setColor(4);
        cout << "\n------------------------------------------------------------------------" << endl;
        cout << "Student not found in database!" << endl;
        cout << "------------------------------------------------------------------------" << endl;
        setColor(7);
        pause();
        return;
    }

    long long storedSignature = studentSignatures[idx];
    long long recomputedSignature = generateSignature(name, roll);

    cout << "\n------------------------------------------------------------------------" << endl;
    cout << "Verification Results:" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Student Name: " << name << endl;
    cout << "Roll Number: " << roll << endl;
    cout << "Stored Signature: " << storedSignature << endl;
    cout << "Recomputed Signature: " << recomputedSignature << endl;
    cout << "------------------------------------------------------------------------" << endl;

    string status;
    if (storedSignature == recomputedSignature)
    {
        setColor(10);
        cout << "STATUS: VERIFIED" << endl;
        cout << "Data integrity maintained. No tampering detected." << endl;
        setColor(7);
        status = "VERIFIED";
    }
    else
    {
        setColor(4);
        cout << "STATUS: TAMPERED" << endl;
        cout << "Data has been modified! Signatures do not match." << endl;
        setColor(7);
        status = "TAMPERED";
    }
    cout << "------------------------------------------------------------------------" << endl;

    // Add to activity log
    string logEntry = "----> VERIFY | Name: " + name + " | Roll: " + roll + " | STATUS: " + status;
    addToActivityLog(logEntry);

    pause();
}

void viewAllRecords()
{
    system("cls");
    printHeader();
    setColor(14);
    cout << "View All Student Records" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    if (studentCount == 0)
    {
        setColor(4);
        cout << "No records found in database!" << endl;
        setColor(7);
        pause();
        return;
    }

    for (int i = 0; i < studentCount; i++)
    {
        cout << "\nRecord " << (i + 1) << ":" << endl;
        cout << "  Name: " << studentNames[i] << endl;
        cout << "  Roll: " << studentRolls[i] << endl;
        cout << "  Signature: " << studentSignatures[i] << endl;
        cout << "------------------------------------------------------------------------" << endl;
    }

    pause();
}

void collisionAnalysis()
{
    system("cls");
    printHeader();
    setColor(14);
    cout << "Collision Analysis" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    if (studentCount < 2)
    {
        setColor(4);
        cout << "Need at least 2 students for collision analysis!" << endl;
        setColor(7);
        pause();
        return;
    }

    cout << "Scanning " << studentCount << " students for collisions..." << endl;
    cout << "------------------------------------------------------------------------" << endl;

    int collisionCount = 0;

    for (int i = 0; i < studentCount; i++)
    {
        for (int j = i + 1; j < studentCount; j++)
        {
            if (studentSignatures[i] == studentSignatures[j])
            {
                collisionCount++;
                setColor(4);
                cout << "\nCOLLISION DETECTED!" << endl;
                setColor(7);
                cout << "Student 1: " << studentNames[i] << " (" << studentRolls[i] << ")" << endl;
                cout << "Student 2: " << studentNames[j] << " (" << studentRolls[j] << ")" << endl;
                cout << "Common Signature: " << studentSignatures[i] << endl;
                cout << "------------------------------------------------------------------------" << endl;
            }
        }
    }

    if (collisionCount == 0)
    {
        setColor(10);
        cout << "\nNo collisions detected!" << endl;
        cout << "All signatures are unique." << endl;
        setColor(7);
    }
    else
    {
        setColor(4);
        cout << "\nTotal Collisions Found: " << collisionCount << " pair(s)" << endl;
        setColor(7);
    }
    cout << "------------------------------------------------------------------------" << endl;

    pause();
}

void viewAuditLog()
{
    system("cls");
    printHeader();
    setColor(14);
    cout << "Activity Audit Log" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    setColor(7);

    if (activityCount == 0)
    {
        setColor(4);
        cout << "No activity recorded yet!" << endl;
        setColor(7);
        pause();
        return;
    }

    setColor(10); // Sage green color
    for (int i = 0; i < activityCount; i++)
    {
        cout << activityLog[i] << endl;
        cout << endl;
    }
    setColor(7);

    cout << "------------------------------------------------------------------------" << endl;
    cout << "Total Activities Logged: " << activityCount << endl;
    cout << "------------------------------------------------------------------------" << endl;

    pause();
}

void printMenu()
{
    setColor(14);
    cout << "1. Generate Student Signature" << endl;
    cout << "2. Verify Data Integrity" << endl;
    cout << "3. View All Records" << endl;
    cout << "4. Collision Analysis" << endl;
    cout << "5. View Audit Log" << endl;
    cout << "6. Exit" << endl;
    setColor(7);
    cout << "Enter your choice: ";
}

// ==================== MAIN ====================
int main()
{
    system("color 0E");

    readingDataFromFile();
    readActivityLogFromFile();

    string choice;
    while (true)
    {
        system("cls");
        printHeader();
        printMenu();
        getline(cin >> ws, choice);

        if (choice == "1")
        {
            generateStudentSignature();
            
        }
        else if (choice == "2")
        {
            verifyStudentData();
        }
        else if (choice == "3")
        {
            viewAllRecords();
        }
        else if (choice == "4")
        {
            collisionAnalysis();
        }
        else if (choice == "5")
        {
            viewAuditLog();
        }
        else if (choice == "6")
        {
            writingDataInFile();
            setColor(10);
            cout << "\nData saved successfully. Goodbye!" << endl;
            setColor(7);
            break;
        }
        else
        {
            setColor(4);
            cout << "Invalid choice! Please try again." << endl;
            setColor(7);
            pause();
        }
    }

    return 0;
}