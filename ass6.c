#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define INPUT_BUFFER_CAPACITY 1024
#define DEFAULT_PROMPT_TEXT ">>> "
#define DEFAULT_ERROR_TEXT "Invalid input. Please try again."

typedef struct {
    char* data;
    size_t capacity;
} StringBuffer;

typedef struct {
    int64_t min;
    int64_t max;
} IntRange;

#define STRING_BUFFER(array) (&(StringBuffer){ .data = (array), .capacity = sizeof(array) })
#define INT_RANGE(min_val, max_val) (&(IntRange){ .min = (int64_t)(min_val), .max = (int64_t)(max_val) })

typedef bool (*ParseFunction)(const char* input, void* out_data, const void* range_context, bool allow_empty);

typedef struct {
    const char* prompt;
    const char* error_msg;
    ParseFunction parser;
    void* out_data;
    const void* range;
    bool allow_empty;
} PromptOptions;

void prompt_until_valid(PromptOptions options);

#define prompt_input(dest_ptr, ...) prompt_until_valid((PromptOptions){ \
    .prompt = DEFAULT_PROMPT_TEXT, \
    .error_msg = DEFAULT_ERROR_TEXT, \
    .out_data = (dest_ptr), \
    .parser = _Generic((dest_ptr), \
        int32_t*: parse_int32, \
        int64_t*: parse_int64, \
        StringBuffer*: parse_string \
    ), \
    __VA_ARGS__ \
})

bool parse_int32(const char* input, void* out_data, const void* context, bool allow_empty);
bool parse_int64(const char* input, void* out_data, const void* context, bool allow_empty);
bool parse_string(const char* input, void* out_data, const void* context, bool allow_empty);

typedef struct TermNode {
    int64_t coeff;
    int64_t exp;
    struct TermNode* next;
} TermNode;

static TermNode* create_term_node(int64_t coeff, int64_t exp) {
    TermNode* new_node = (TermNode*)malloc(sizeof(TermNode));
    if (!new_node) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    new_node->coeff = coeff;
    new_node->exp = exp;
    new_node->next = new_node;
    return new_node;
}

static void insert_term(TermNode** head, int64_t coeff, int64_t exp) {
    if (coeff == 0) return;

    if (*head == NULL) {
        *head = create_term_node(coeff, exp);
        return;
    }

    TermNode* curr = *head;
    TermNode* prev = NULL;

    do {
        if (curr->exp == exp) {
            curr->coeff += coeff;
            if (curr->coeff == 0) {
                if (curr->next == curr) {
                    free(curr);
                    *head = NULL;
                    return;
                }

                TermNode* tail = *head;
                while (tail->next != *head) {
                    tail = tail->next;
                }

                if (curr == *head) {
                    *head = curr->next;
                    tail->next = *head;
                }
                else {
                    prev->next = curr->next;
                }
                free(curr);
            }
            return;
        }

        if (exp > curr->exp) {
            TermNode* new_node = create_term_node(coeff, exp);
            if (!new_node) return;

            if (curr == *head) {
                TermNode* tail = *head;
                while (tail->next != *head) {
                    tail = tail->next;
                }
                new_node->next = *head;
                tail->next = new_node;
                *head = new_node;
            }
            else {
                new_node->next = curr;
                prev->next = new_node;
            }
            return;
        }

        prev = curr;
        curr = curr->next;
    } while (curr != *head);

    TermNode* new_node = create_term_node(coeff, exp);
    if (!new_node) return;

    new_node->next = *head;
    prev->next = new_node;
}

static void display_polynomial(TermNode* head) {
    if (head == NULL) {
        printf("0\n");
        return;
    }

    TermNode* curr = head;
    bool is_first = true;

    do {
        int64_t c = curr->coeff;
        int64_t e = curr->exp;

        if (c > 0 && !is_first) {
            printf(" + ");
        }
        else if (c < 0) {
            if (is_first) {
                printf("-");
            }
            else {
                printf(" - ");
            }
            c = -c;
        }

        if (e == 0) {
            printf("%ld", (long)c);
        }
        else if (e == 1) {
            if (c == 1) {
                printf("x");
            }
            else {
                printf("%ldx", (long)c);
            }
        }
        else {
            if (c == 1) {
                printf("x^%ld", (long)e);
            }
            else {
                printf("%ldx^%ld", (long)c, (long)e);
            }
        }

        is_first = false;
        curr = curr->next;
    } while (curr != head);

    printf("\n");
}

static TermNode* add_polynomials(TermNode* p1, TermNode* p2) {
    TermNode* result = NULL;

    if (p1 == NULL && p2 == NULL) return NULL;

    if (p1 != NULL) {
        TermNode* curr1 = p1;
        do {
            insert_term(&result, curr1->coeff, curr1->exp);
            curr1 = curr1->next;
        } while (curr1 != p1);
    }

    if (p2 != NULL) {
        TermNode* curr2 = p2;
        do {
            insert_term(&result, curr2->coeff, curr2->exp);
            curr2 = curr2->next;
        } while (curr2 != p2);
    }

    return result;
}

static void free_polynomial(TermNode** head) {
    if (*head == NULL) return;

    TermNode* curr = *head;
    TermNode* tail = *head;

    while (tail->next != *head) {
        tail = tail->next;
    }
    tail->next = NULL;

    while (curr != NULL) {
        TermNode* next_node = curr->next;
        free(curr);
        curr = next_node;
    }
    *head = NULL;
}

int main(void) {
    TermNode* poly1 = NULL;
    TermNode* poly2 = NULL;
    TermNode* poly_sum = NULL;
    int32_t choice = 0;

    while (choice != 6) {
        printf("\n=============================================================\n");
        printf("         CIRCULAR LINKED LIST POLYNOMIAL ARITHMETIC          \n");
        printf("=============================================================\n");
        printf(" 1. Create / Insert terms into Polynomial 1\n");
        printf(" 2. Create / Insert terms into Polynomial 2\n");
        printf(" 3. Display Polynomials\n");
        printf(" 4. Add Polynomial 1 and Polynomial 2\n");
        printf(" 5. Clear Polynomials\n");
        printf(" 6. Exit\n");
        printf("=============================================================\n");

        prompt_input(&choice,
            .prompt = "Enter your choice (1-6): ",
            .range = INT_RANGE(1, 6)
        );

        switch (choice) {
        case 1: {
            int32_t num_terms = 0;
            prompt_input(&num_terms,
                .prompt = "Enter number of terms to insert for Poly 1: ",
                .range = INT_RANGE(0, 100)
            );

            for (int32_t i = 0; i < num_terms; ++i) {
                int64_t coeff = 0;
                int64_t exp = 0;
                printf("\nTerm %d of %d:\n", i + 1, num_terms);
                prompt_input(&coeff, .prompt = "Enter coefficient: ", .range = INT_RANGE(INT64_MIN, INT64_MAX));
                prompt_input(&exp, .prompt = "Enter non-negative exponent: ", .range = INT_RANGE(0, INT64_MAX));
                insert_term(&poly1, coeff, exp);
            }

            printf("\nUpdated Polynomial 1: ");
            display_polynomial(poly1);
            break;
        }
        case 2: {
            int32_t num_terms = 0;
            prompt_input(&num_terms,
                .prompt = "Enter number of terms to insert for Poly 2: ",
                .range = INT_RANGE(0, 100)
            );

            for (int32_t i = 0; i < num_terms; ++i) {
                int64_t coeff = 0;
                int64_t exp = 0;
                printf("\nTerm %d of %d:\n", i + 1, num_terms);
                prompt_input(&coeff, .prompt = "Enter coefficient: ", .range = INT_RANGE(INT64_MIN, INT64_MAX));
                prompt_input(&exp, .prompt = "Enter non-negative exponent: ", .range = INT_RANGE(0, INT64_MAX));
                insert_term(&poly2, coeff, exp);
            }

            printf("\nUpdated Polynomial 2: ");
            display_polynomial(poly2);
            break;
        }
        case 3: {
            printf("\nPolynomial 1: ");
            display_polynomial(poly1);
            printf("Polynomial 2: ");
            display_polynomial(poly2);
            break;
        }
        case 4: {
            free_polynomial(&poly_sum);
            poly_sum = add_polynomials(poly1, poly2);

            printf("\n-------------------------------------------------------------\n");
            printf("Polynomial 1: ");
            display_polynomial(poly1);
            printf("Polynomial 2: ");
            display_polynomial(poly2);
            printf("Sum Result  : ");
            display_polynomial(poly_sum);
            printf("-------------------------------------------------------------\n");
            break;
        }
        case 5: {
            free_polynomial(&poly1);
            free_polynomial(&poly2);
            free_polynomial(&poly_sum);
            printf("All polynomials cleared successfully.\n");
            break;
        }
        case 6: {
            printf("Exiting application.\n");
            break;
        }
        }
    }

    free_polynomial(&poly1);
    free_polynomial(&poly2);
    free_polynomial(&poly_sum);
    return 0;
}

static void clear_input_stream(FILE* stream) {
    int c = 0;
    while ((c = fgetc(stream)) != EOF && c != '\n') {
    }
}

static bool string_to_int64(const char* str, int64_t* out_val) {
    if (!str || !out_val || *str == '\0') return false;
    char* end_ptr = NULL;
    errno = 0;
    int64_t val = strtoll(str, &end_ptr, 10);
    if (errno == ERANGE || end_ptr == str) return false;
    while (*end_ptr) {
        if (!isspace((unsigned char)*end_ptr)) return false;
        end_ptr++;
    }
    *out_val = val;
    return true;
}

bool parse_int32(const char* input, void* out_data, const void* context, bool allow_empty) {
    (void)allow_empty;
    if (!input || !out_data) return false;
    int64_t val = 0;
    if (!string_to_int64(input, &val)) return false;
    int64_t min_limit = INT32_MIN;
    int64_t max_limit = INT32_MAX;
    if (context) {
        const IntRange* range = (const IntRange*)context;
        if (range->min > range->max) {
            fprintf(stderr, "\n[FATAL] Range error: min > max.\n");
            exit(EXIT_FAILURE);
        }
        if (range->min > min_limit) min_limit = range->min;
        if (range->max < max_limit) max_limit = range->max;
    }
    if (val < min_limit || val > max_limit) return false;
    *(int32_t*)out_data = (int32_t)val;
    return true;
}

bool parse_int64(const char* input, void* out_data, const void* context, bool allow_empty) {
    (void)allow_empty;
    if (!input || !out_data) return false;
    int64_t val = 0;
    if (!string_to_int64(input, &val)) return false;
    if (context) {
        const IntRange* range = (const IntRange*)context;
        if (range->min > range->max) {
            fprintf(stderr, "\n[FATAL] Range error: min > max.\n");
            exit(EXIT_FAILURE);
        }
        if (val < range->min || val > range->max) return false;
    }
    *(int64_t*)out_data = val;
    return true;
}

bool parse_string(const char* input, void* out_data, const void* context, bool allow_empty) {
    (void)context;
    if (!out_data || !input) return false;
    StringBuffer* dest = (StringBuffer*)out_data;
    if (!dest->data || dest->capacity == 0) return false;
    size_t input_len = strlen(input);
    if (input_len == 0 && !allow_empty) return false;
    if (input_len + 1 > dest->capacity) return false;
    memcpy(dest->data, input, input_len);
    dest->data[input_len] = '\0';
    return true;
}

void prompt_until_valid(PromptOptions options) {
    if (!options.parser || !options.out_data) {
        fprintf(stderr, "\n[FATAL] Missing parser function or destination pointer.\n");
        exit(EXIT_FAILURE);
    }

    const char* prompt_str = options.prompt ? options.prompt : DEFAULT_PROMPT_TEXT;
    const char* error_str = options.error_msg ? options.error_msg : DEFAULT_ERROR_TEXT;
    char raw_buffer[INPUT_BUFFER_CAPACITY] = { 0 };

    while (true) {
        printf("%s", prompt_str);
        fflush(stdout);

        if (!fgets(raw_buffer, sizeof(raw_buffer), stdin)) {
            if (feof(stdin)) {
                fprintf(stderr, "\n[EOF] Input stream closed.\n");
            }
            else {
                fprintf(stderr, "\n[ERROR] Failed to read input stream.\n");
            }
            exit(EXIT_FAILURE);
        }

        size_t raw_len = strlen(raw_buffer);
        if (raw_len == 0) continue;

        if (raw_buffer[raw_len - 1] != '\n' && !feof(stdin)) {
            clear_input_stream(stdin);
            printf("%s\n", error_str);
            continue;
        }

        if (raw_buffer[raw_len - 1] == '\n') {
            raw_buffer[raw_len - 1] = '\0';
            raw_len--;
        }
        if (raw_len > 0 && raw_buffer[raw_len - 1] == '\r') {
            raw_buffer[raw_len - 1] = '\0';
            raw_len--;
        }

        char* start = raw_buffer;
        while (*start && isspace((unsigned char)*start)) start++;

        char* end = start + strlen(start);
        while (end > start && isspace((unsigned char)*(end - 1))) {
            end--;
            *end = '\0';
        }

        if (*start == '\0') {
            if (options.allow_empty && options.parser(start, options.out_data, options.range, true)) {
                break;
            }
            printf("%s\n", error_str);
            continue;
        }

        if (options.parser(start, options.out_data, options.range, options.allow_empty)) {
            break;
        }

        printf("%s\n", error_str);
    }
}