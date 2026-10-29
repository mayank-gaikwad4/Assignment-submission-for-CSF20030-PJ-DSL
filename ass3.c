#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#define MAX_STUDENT_COUNT 100
#define MAX_NAME_LENGTH 50

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    i32 roll_number;
    char full_name[MAX_NAME_LENGTH];
} Student;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

void PopulateDatabase(Student* database, i32* count);
void GenerateRandomDatabase(Student* database, i32* count);
void PrintDatabase(const Student* database, i32 count);
void SwapStudents(Student* a, Student* b);
i32 LinearSearch(const Student* database, i32 count, i32 target_roll);
i32 BinarySearch(const Student* database, i32 count, i32 target_roll);
void InsertionSort(Student* database, i32 count);
void SelectionSort(Student* database, i32 count);
void ShellSort(Student* database, i32 count);

void PopulateDatabase(Student* database, i32* count) {
    printf("Enter total number of students (1-%d): ", MAX_STUDENT_COUNT);
    *count = GetIntInRange(1, MAX_STUDENT_COUNT);
    for (i32 i = 0; i < count; i++) {
        printf("\nStudent %d Roll Number: ", i + 1);
        database[i].roll_number = GetIntInRange(1, 99999);
        printf("Student %d Name: ", i + 1);
        char temp_name = GetString();
        strncpy(database[i].full_name, temp_name, MAX_NAME_LENGTH - 1);
        database[i].full_name[MAX_NAME_LENGTH - 1] = '\0';
        free(temp_name);
    }
}

void GenerateRandomDatabase(Student* database, i32* count) {
    printf("Enter total number of random records to generate (1-%d): ", MAX_STUDENT_COUNT);
    count = GetIntInRange(1, MAX_STUDENT_COUNT);
    const char first_names[] = {"Alex", "Sam", "Jordan", "Taylor", "Morgan", "Chris", "Pat", "Riley", "Dakota", "Avery", "Liam", "Emma", "Noah", "Olivia"};
    const char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez", "Hernandez", "Lopez"};
    u32 first_name_count = sizeof(first_names) / sizeof(first_names[0]);
    u32 last_name_count = sizeof(last_names) / sizeof(last_names[0]);
    b32 used_rolls[1000] = {0};
    code Code

    for (i32 i = 0; i < *count; i++) {
        i32 roll;
        do {
            roll = (rand() % 900) + 100;
        } while (used_rolls[roll]);
        used_rolls[roll] = true;
        database[i].roll_number = roll;
        snprintf(database[i].full_name, MAX_NAME_LENGTH, "%s %s", first_names[rand() % first_name_count], last_names[rand() % last_name_count]);
    }
    printf("\nSuccessfully generated %d random student records!\n", *count);

}

void PrintDatabase(const Student* database, i32 count) {
    printf("\n%-15s %-30s\n", "Roll Number", "Name");
    printf("----------------------------------------------\n");
    for (i32 i = 0; i < count; i++) {
        printf("%-15d %-30s\n", database[i].roll_number, database[i].full_name);
    }
}

void SwapStudents(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

i32 LinearSearch(const Student* database, i32 count, i32 target_roll) {
    for (i32 i = 0; i < count; i++) {
        if (database[i].roll_number == target_roll) {
            return i;
        }
    }
    return -1;
}

i32 BinarySearch(const Student* database, i32 count, i32 target_roll) {
    i32 left = 0;
    i32 right = count - 1;
    while (left <= right) {
        i32 mid = left + (right - left) / 2;
        if (database[mid].roll_number == target_roll) {
            return mid;
        }
        if (database[mid].roll_number < target_roll) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void InsertionSort(Student* database, i32 count) {
    for (i32 i = 1; i < count; i++) {
        Student key = database[i];
        i32 j = i - 1;
        while (j >= 0 && database[j].roll_number > key.roll_number) {
            database[j + 1] = database[j];
            j--;
        }
        database[j + 1] = key;
    }
}

void SelectionSort(Student* database, i32 count) {
    for (i32 i = 0; i < count - 1; i++) {
        i32 min_index = i;
        for (i32 j = i + 1; j < count; j++) {
            if (database[j].roll_number < database[min_index].roll_number) {
                min_index = j;
            }
        }
        if (min_index != i) {
            SwapStudents(&database[min_index], &database[i]);
        }
    }
}

void ShellSort(Student* database, i32 count) {
    for (i32 gap = count / 2; gap > 0; gap /= 2) {
        for (i32 i = gap; i < count; i++) {
            Student temp = database[i];
            i32 j;
            for (j = i; j >= gap && database[j - gap].roll_number > temp.roll_number; j -= gap) {
                database[j] = database[j - gap];
            }
            database[j] = temp;
        }
    }
}

i32 main(void) {
    srand((u32)time(NULL));
    Student database[MAX_STUDENT_COUNT];
    i32 student_count = 0;
    b32 is_loaded = false;
    b32 is_sorted = false;
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n1. Input Records\n2. Randomize Records\n3. Display Records\n4. Linear Search\n5. Binary Search\n6. Insertion Sort\n7. Selection Sort\n8. Shell Sort\n9. Exit\nSelection: ");
        i32 selection = GetIntInRange(1, 9);

        switch (selection) {
            case 1:
                PopulateDatabase(database, &student_count);
                is_loaded = true;
                is_sorted = false;
                break;
            case 2:
                GenerateRandomDatabase(database, &student_count);
                is_loaded = true;
                is_sorted = false;
                break;
            case 3:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else {
                    PrintDatabase(database, student_count);
                }
                break;
            case 4:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else {
                    printf("Enter Roll Number to search: ");
                    i32 target = GetInt();
                    i32 found_index = LinearSearch(database, student_count, target);
                    if (found_index != -1) {
                        printf("Record Found: %s (Roll No: %d) at index %d\n", database[found_index].full_name, database[found_index].roll_number, found_index);
                    } else {
                        printf("Record with Roll Number %d not found.\n", target);
                    }
                }
                break;
            case 5:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else if (!is_sorted) {
                    printf("Error: Binary search requires sorted data. Please sort the database first.\n");
                } else {
                    printf("Enter Roll Number to search: ");
                    i32 target = GetInt();
                    i32 found_index = BinarySearch(database, student_count, target);
                    if (found_index != -1) {
                        printf("Record Found: %s (Roll No: %d) at index %d\n", database[found_index].full_name, database[found_index].roll_number, found_index);
                    } else {
                        printf("Record with Roll Number %d not found.\n", target);
                    }
                }
                break;
            case 6:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else {
                    InsertionSort(database, student_count);
                    is_sorted = true;
                    printf("Database sorted using Insertion Sort.\n");
                    PrintDatabase(database, student_count);
                }
                break;
            case 7:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else {
                    SelectionSort(database, student_count);
                    is_sorted = true;
                    printf("Database sorted using Selection Sort.\n");
                    PrintDatabase(database, student_count);
                }
                break;
            case 8:
                if (!is_loaded) {
                    printf("Database is empty.\n");
                } else {
                    ShellSort(database, student_count);
                    is_sorted = true;
                    printf("Database sorted using Shell Sort.\n");
                    PrintDatabase(database, student_count);
                }
                break;
            case 9:
                is_running = false;
                break;
        }
    }
    return 0;

}

// ignore input sanitization
i32 GetInt(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        if (input_buffer[0] == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        char* end_pointer = NULL;
        errno = 0;
        i64 parsed_value = strtol(input_buffer, &end_pointer, 10);
        if (end_pointer == input_buffer) {
            printf("Error: Invalid input.\nRetry: ");
            continue;
        }
        if (errno == ERANGE || parsed_value < INT32_MIN || parsed_value > INT32_MAX) {
            printf("Error: Out of range.\nRetry: ");
            continue;
        }
        while (*end_pointer != '\0' && isspace((u8)*end_pointer)) {
            end_pointer++;
        }
        if (*end_pointer != '\0') {
            printf("Error: Trailing characters.\nRetry: ");
            continue;
        }
        return (i32)parsed_value;
    }
}

f64 GetDouble(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        if (input_buffer[0] == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        char* end_pointer = NULL;
        errno = 0;
        f64 parsed_value = strtod(input_buffer, &end_pointer);
        if (end_pointer == input_buffer) {
            printf("Error: Invalid input.\nRetry: ");
            continue;
        }
        if (errno == ERANGE) {
            printf("Error: Out of range.\nRetry: ");
            continue;
        }
        while (*end_pointer != '\0' && isspace((u8)*end_pointer)) {
            end_pointer++;
        }
        if (*end_pointer != '\0') {
            printf("Error: Trailing characters.\nRetry: ");
            continue;
        }
        return parsed_value;
    }
}

char* GetString(void) {
    char input_buffer[1024];
    while (true) {
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            printf("\nError: End of input.\n");
            exit(1);
        }
        u64 length = strlen(input_buffer);
        if (length > 0 && input_buffer[length - 1] == '\n') {
            input_buffer[length - 1] = '\0';
        }
        const char* checker = input_buffer;
        while (*checker != '\0' && isspace((u8)*checker)) {
            checker++;
        }
        if (*checker == '\0') {
            printf("Error: Empty input.\nRetry: ");
            continue;
        }
        return strdup(input_buffer);
    }
}

i32 GetIntInRange(i32 min_value, i32 max_value) {
    while (true) {
        i32 value = GetInt();
        if (value >= min_value && value <= max_value) {
            return value;
        }
        printf("Error: Out of bounds (%d to %d).\nRetry: ", min_value, max_value);
    }
}
