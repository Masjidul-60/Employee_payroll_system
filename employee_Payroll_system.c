#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define PASSWORD "admin123"

typedef struct {
    int id;
    char name[50];
    char position[50];
    float basic_salary;
    float allowance;
    float deduction;
    float net_salary;
} Employee;

Employee employees[MAX];
int employee_count = 0;

void saveToFile() {
    FILE *f = fopen("employees.dat", "wb");
    if (f) {
        fwrite(&employee_count, sizeof(int), 1, f);
        fwrite(employees, sizeof(Employee), employee_count, f);
        fclose(f);
    }
}

void loadFromFile() {
    FILE *f = fopen("employees.dat", "rb");
    if (f) {
        fread(&employee_count, sizeof(int), 1, f);
        fread(employees, sizeof(Employee), employee_count, f);
        fclose(f);
    }
}

void saveToCSV() {
    FILE *f = fopen("employees.csv", "w");
    if (!f) {
        perror("Error opening employees.csv");
        return;
    }

    fprintf(f, "ID,Name,Position,Basic Salary,Allowance,Deduction,Net Salary\n");
    for (int i = 0; i < employee_count; i++) {
        // Escape commas or newlines in name and position
        char name[100], position[100];
        strncpy(name, employees[i].name, sizeof(name));
        strncpy(position, employees[i].position, sizeof(position));
        for (int j = 0; j < strlen(name); j++) {
            if (name[j] == ',') name[j] = ';';
        }
        for (int j = 0; j < strlen(position); j++) {
            if (position[j] == ',') position[j] = ';';
        }

        fprintf(f, "%d,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                employees[i].id, name, position,
                employees[i].basic_salary, employees[i].allowance,
                employees[i].deduction, employees[i].net_salary);
    }

    fclose(f);

#ifdef _WIN32
    system("start employees.csv");  // Only auto-open on Windows
#else
    printf("CSV saved as 'employees.csv'.\n");
#endif
}


void generateSalarySlip(Employee e) {
    char filename[100];
    sprintf(filename, "SalarySlip_%d.txt", e.id);
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "=========== Salary Slip ===========\n");
        fprintf(f, "Employee ID: %d\n", e.id);
        fprintf(f, "Name       : %s\n", e.name);
        fprintf(f, "Position   : %s\n", e.position);
        fprintf(f, "-----------------------------------\n");
        fprintf(f, "Basic Salary : %.2f\n", e.basic_salary);
        fprintf(f, "Allowance    : %.2f\n", e.allowance);
        fprintf(f, "Deduction    : %.2f\n", e.deduction);
        fprintf(f, "-----------------------------------\n");
        fprintf(f, "Net Salary   : %.2f\n", e.net_salary);
        fprintf(f, "===================================\n");
        fclose(f);
    }
}

int findEmployee(int id) {
    for (int i = 0; i < employee_count; i++) {
        if (employees[i].id == id) return i;
    }
    return -1;
}

void addEmployee() {
    if (employee_count >= MAX) {
        printf("Employee list is full!\n");
        return;
    }
    Employee e;
    printf("Enter Employee ID: ");
    scanf("%d", &e.id);
    getchar();
    printf("Enter Name: ");
    fgets(e.name, 50, stdin);
    e.name[strcspn(e.name, "\n")] = 0;
    printf("Enter Position: ");
    fgets(e.position, 50, stdin);
    e.position[strcspn(e.position, "\n")] = 0;
    printf("Enter Basic Salary: ");
    scanf("%f", &e.basic_salary);
    printf("Enter Allowance: ");
    scanf("%f", &e.allowance);
    printf("Enter Deduction: ");
    scanf("%f", &e.deduction);
    e.net_salary = e.basic_salary + e.allowance - e.deduction;
    employees[employee_count++] = e;
    saveToFile();
    saveToCSV();
    generateSalarySlip(e);
    printf("Employee added successfully!\n");
}

void displayEmployees() {
    if (employee_count == 0) {
        printf("No employees to display!\n");
        return;
    }
    printf("\nEmployee Records:\n");
    printf("ID\tName\t\tPosition\t\tNet Salary\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < employee_count; i++) {
        printf("%d\t%s\t\t%s\t\t%.2f\n", employees[i].id,
               employees[i].name, employees[i].position, employees[i].net_salary);
    }
}
void searchEmployee() {
    int id;
    printf("Enter ID to search: ");
    scanf("%d", &id);
    int index = findEmployee(id);
    if (index != -1) {
        Employee e = employees[index];
        printf("Found: %s (%s) - Net Salary: %.2f\n", e.name, e.position, e.net_salary);
    } else {
        printf("Employee not found.\n");
    }
}

void updateEmployee() {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    int index = findEmployee(id);
    if (index != -1) {
        printf("Updating employee %s\n", employees[index].name);
        addEmployee();
        for (int i = index; i < employee_count - 1; i++) {
            employees[i] = employees[i + 1];
        }
        employee_count--;
        saveToFile();
        saveToCSV();
    } else {
        printf("Employee not found.\n");
    }
}

void deleteEmployee() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    int index = findEmployee(id);
    if (index != -1) {
        for (int i = index; i < employee_count - 1; i++) {
            employees[i] = employees[i + 1];
        }
        employee_count--;
        saveToFile();
        saveToCSV();
        printf("Employee deleted.\n");
    } else {
        printf("Employee not found.\n");
    }
}

int login() {
    char pass[20];
    printf("Enter password to access system: ");
    scanf("%s", pass);
    if (strcmp(pass, PASSWORD) == 0) return 1;
    else {
        printf("Wrong password!\n");
        return 0;
    }
}

int main() {

    if (!login()) return 0;
    loadFromFile();
    int choice;
    do {
        printf("\n===== Employee Payroll System =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Sort by Salary\n");
        printf("7. Sort by Name\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addEmployee();system("cls"); break;
            case 2: system("cls");displayEmployees(); break;
            case 3: system("cls");searchEmployee(); break;
            case 4: updateEmployee();system("cls"); break;
            case 5: system("cls");deleteEmployee(); break;
            case 6: system("cls");sortBySalary(); break;
            case 7: system("cls");sortByName();break;
            case 8: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);
    return 0;
}
