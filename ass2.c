#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

#define MAX_ELEMENTS 101

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t u8;
typedef double f64;
typedef bool b32;

typedef struct {
    u32 row_index;
    u32 col_index;
    i32 value;
} SparseElement;

typedef struct {
    SparseElement elements[MAX_ELEMENTS];
} SparseMatrix;

i32 GetInt(void);
f64 GetDouble(void);
char* GetString(void);
i32 GetIntInRange(i32 min_value, i32 max_value);

b32 IsValidCoordinate(u32 row, u32 col, u32 max_row, u32 max_col);
void PopulateSparseMatrix(SparseMatrix* matrix);
void PrintSparseMatrix(const SparseMatrix* matrix);
void SimpleTranspose(const SparseMatrix* source, SparseMatrix* target);
void FastTranspose(const SparseMatrix* source, SparseMatrix* target);

b32 IsValidCoordinate(u32 row, u32 col, u32 max_row, u32 max_col) {
    return (row < max_row) && (col < max_col);
}

void PopulateSparseMatrix(SparseMatrix* matrix) {
    printf("Total Rows: ");
    matrix->elements[0].row_index = (u32)GetIntInRange(1, MAX_ELEMENTS - 1);
    printf("Total Columns: ");
    matrix->elements[0].col_index = (u32)GetIntInRange(1, MAX_ELEMENTS - 1);
    printf("Number of Non-zero Elements: ");
    matrix->elements[0].value = GetIntInRange(0, MAX_ELEMENTS - 1);
    code Code

    for (u32 i = 1; i <= (u32)matrix->elements[0].value; i++) {
        printf("Enter Row, Column, and Value for element %u (one per line):\n", i);
        while (true) {
            u32 row = (u32)GetInt();
            u32 col = (u32)GetInt();
            i32 val = GetInt();

            if (IsValidCoordinate(row, col, matrix->elements[0].row_index, matrix->elements[0].col_index)) {
                matrix->elements[i].row_index = row;
                matrix->elements[i].col_index = col;
                matrix->elements[i].value = val;
                break;
            }
            printf("Coordinates out of matrix bounds.\nRetry Row, Column, and Value (one per line):\n");
        }
    }

}

void PrintSparseMatrix(const SparseMatrix* matrix) {
    printf("Row\tCol\tValue\n");
    for (u32 i = 0; i <= (u32)matrix->elements[0].value; i++) {
        printf("%u\t%u\t%d\n", matrix->elements[i].row_index, matrix->elements[i].col_index, matrix->elements[i].value);
    }
}

void SimpleTranspose(const SparseMatrix* source, SparseMatrix* target) {
    target->elements[0].row_index = source->elements[0].col_index;
    target->elements[0].col_index = source->elements[0].row_index;
    target->elements[0].value = source->elements[0].value;
    code Code

    if (source->elements[0].value > 0) {
        u32 target_index = 1;
        for (u32 col = 0; col < source->elements[0].col_index; col++) {
            for (u32 source_index = 1; source_index <= (u32)source->elements[0].value; source_index++) {
                if (source->elements[source_index].col_index == col) {
                    target->elements[target_index].row_index = source->elements[source_index].col_index;
                    target->elements[target_index].col_index = source->elements[source_index].row_index;
                    target->elements[target_index].value = source->elements[source_index].value;
                    target_index++;
                }
            }
        }
    }

}

void FastTranspose(const SparseMatrix* source, SparseMatrix* target) {
    u32 column_counts[MAX_ELEMENTS] = {0};
    u32 target_positions[MAX_ELEMENTS] = {0};
    code Code

    target->elements[0].row_index = source->elements[0].col_index;
    target->elements[0].col_index = source->elements[0].row_index;
    target->elements[0].value = source->elements[0].value;

    if (source->elements[0].value > 0) {
        for (u32 i = 1; i <= (u32)source->elements[0].value; i++) {
            column_counts[source->elements[i].col_index]++;
        }
        target_positions[0] = 1;
        for (u32 i = 1; i < source->elements[0].col_index; i++) {
            target_positions[i] = target_positions[i - 1] + column_counts[i - 1];
        }
        for (u32 i = 1; i <= (u32)source->elements[0].value; i++) {
            u32 target_index = target_positions[source->elements[i].col_index]++;
            target->elements[target_index].row_index = source->elements[i].col_index;
            target->elements[target_index].col_index = source->elements[i].row_index;
            target->elements[target_index].value = source->elements[i].value;
        }
    }

}

i32 main(void) {
    SparseMatrix active_matrix = {0};
    SparseMatrix transposed_matrix = {0};
    b32 is_loaded = false;
    b32 is_running = true;
    code Code

    while (is_running) {
        printf("\n1. Input Matrix\n2. Display Matrix\n3. Simple Transpose\n4. Fast Transpose\n5. Exit\nSelection: ");
        i32 selection = GetIntInRange(1, 5);

        switch (selection) {
            case 1:
                PopulateSparseMatrix(&active_matrix);
                is_loaded = true;
                break;
            case 2:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    PrintSparseMatrix(&active_matrix);
                }
                break;
            case 3:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    SimpleTranspose(&active_matrix, &transposed_matrix);
                    PrintSparseMatrix(&transposed_matrix);
                }
                break;
            case 4:
                if (!is_loaded) {
                    printf("Matrix is currently empty.\n");
                } else {
                    FastTranspose(&active_matrix, &transposed_matrix);
                    PrintSparseMatrix(&transposed_matrix);
                }
                break;
            case 5:
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
