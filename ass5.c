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
    char *data;
    size_t capacity;
} StringBuffer;

typedef struct {
    int64_t min;
    int64_t max;
} IntRange;

#define STRING_BUFFER(array) (&(StringBuffer){ .data = (array), .capacity = sizeof(array) })
#define INT_RANGE(min_val, max_val) (&(IntRange){ .min = (int64_t)(min_val), .max = (int64_t)(max_val) })

typedef bool (*ParseFunction)(const char *input, void *out_data, const void *range_context, bool allow_empty);

typedef struct {
    const char *prompt;
    const char *error_msg;
    ParseFunction parser;
    void *out_data;
    const void *range;
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

bool parse_int32(const char *input, void *out_data, const void *context, bool allow_empty);
bool parse_int64(const char *input, void *out_data, const void *context, bool allow_empty);
bool parse_string(const char *input, void *out_data, const void *context, bool allow_empty);

typedef struct MemberNode {
    int64_t prn;
    char name[64];
    struct MemberNode *next;
} MemberNode;

static MemberNode *create_member_node(int64_t prn, const char *name) {
    MemberNode *new_node = (MemberNode *)malloc(sizeof(MemberNode));
    if (!new_node) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    new_node->prn = prn;
    strncpy(new_node->name, name, sizeof(new_node->name) - 1);
    new_node->name[sizeof(new_node->name) - 1] = '\0';
    new_node->next = NULL;
    return new_node;
}

static bool is_prn_duplicate(MemberNode *head, int64_t prn) {
    MemberNode *curr = head;
    while (curr) {
        if (curr->prn == prn) return true;
        curr = curr->next;
    }
    return false;
}

static void add_president(MemberNode **head, int64_t prn, const char *name) {
    if (is_prn_duplicate(*head, prn)) {
        printf("Error: Member with PRN %ld already exists in the club.\n", (long)prn);
        return;
    }
    MemberNode *new_node = create_member_node(prn, name);
    if (!new_node) return;
    new_node->next = *head;
    *head = new_node;
    printf("President added successfully.\n");
}

static void add_secretary(MemberNode **head, int64_t prn, const char *name) {
    if (is_prn_duplicate(*head, prn)) {
        printf("Error: Member with PRN %ld already exists in the club.\n", (long)prn);
        return;
    }
    MemberNode *new_node = create_member_node(prn, name);
    if (!new_node) return;

    if (*head == NULL) {
        *head = new_node;
    } else {
        MemberNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    printf("Secretary added successfully.\n");
}

static void add_regular_member(MemberNode **head, int64_t prn, const char *name) {
    if (is_prn_duplicate(*head, prn)) {
        printf("Error: Member with PRN %ld already exists in the club.\n", (long)prn);
        return;
    }
    MemberNode *new_node = create_member_node(prn, name);
    if (!new_node) return;

    if (*head == NULL) {
        *head = new_node;
    } else if ((*head)->next == NULL) {
        (*head)->next = new_node;
    } else {
        MemberNode *temp = *head;
        while (temp->next->next != NULL) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
    printf("Regular member added successfully.\n");
}

static void delete_member(MemberNode **head, int64_t prn) {
    if (*head == NULL) {
        printf("No students registered.\n");
        return;
    }

    MemberNode *curr = *head;
    MemberNode *prev = NULL;

    while (curr != NULL && curr->prn != prn) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Student not found.\n");
        return;
    }

    if (prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }

    free(curr);
    printf("Member with PRN %ld deleted successfully.\n", (long)prn);
}

static int count_members(MemberNode *head) {
    int count = 0;
    MemberNode *temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

static void display_members(MemberNode *head) {
    if (head == NULL) {
        printf("No students registered.\n");
        return;
    }

    printf("\n-------------------------------------------------------------\n");
    printf("%-15s %-25s %-15s\n", "PRN", "Name", "Role");
    printf("-------------------------------------------------------------\n");

    MemberNode *temp = head;
    while (temp != NULL) {
        const char *role = "Member";
        if (temp == head) {
            role = "President";
        } else if (temp->next == NULL) {
            role = "Secretary";
        }
        printf("%-15ld %-25s %-15s\n", (long)temp->prn, temp->name, role);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
    printf("Total Members: %d\n", count_members(head));
    printf("-------------------------------------------------------------\n");
}

static void reverse_list(MemberNode **head) {
    if (*head == NULL) {
        printf("No students registered.\n");
        return;
    }

    MemberNode *prev = NULL;
    MemberNode *curr = *head;
    MemberNode *next_node = NULL;

    while (curr != NULL) {
        next_node = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next_node;
    }
    *head = prev;
    printf("Linked list reversed successfully.\n");
}

static void sort_list_by_prn(MemberNode **head) {
    if (*head == NULL) {
        printf("No students registered.\n");
        return;
    }
    if ((*head)->next == NULL) {
        printf("List sorted successfully.\n");
        return;
    }

    bool swapped = false;
    MemberNode *ptr1;
    MemberNode *lptr = NULL;

    do {
        swapped = false;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (ptr1->prn > ptr1->next->prn) {
                int64_t temp_prn = ptr1->prn;
                char temp_name[64];
                strcpy(temp_name, ptr1->name);

                ptr1->prn = ptr1->next->prn;
                strcpy(ptr1->name, ptr1->next->name);

                ptr1->next->prn = temp_prn;
                strcpy(ptr1->next->name, temp_name);

                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("List sorted by PRN in ascending order.\n");
}

static MemberNode *merge_lists(MemberNode *list1, MemberNode *list2) {
    if (!list1 && !list2) return NULL;

    MemberNode dummy;
    dummy.next = NULL;
    MemberNode *tail = &dummy;

    MemberNode *p1 = list1;
    MemberNode *p2 = list2;

    while (p1 && p2) {
        if (p1->prn <= p2->prn) {
            tail->next = create_member_node(p1->prn, p1->name);
            p1 = p1->next;
        } else {
            tail->next = create_member_node(p2->prn, p2->name);
            p2 = p2->next;
        }
        tail = tail->next;
    }

    while (p1) {
        tail->next = create_member_node(p1->prn, p1->name);
        p1 = p1->next;
        tail = tail->next;
    }

    while (p2) {
        tail->next = create_member_node(p2->prn, p2->name);
        p2 = p2->next;
        tail = tail->next;
    }

    return dummy.next;
}

static void delete_entire_list(MemberNode **head) {
    MemberNode *curr = *head;
    while (curr != NULL) {
        MemberNode *next_node = curr->next;
        free(curr);
        curr = next_node;
    }
    *head = NULL;
}

int main(void) {
    MemberNode *club_list = NULL;
    MemberNode *auxiliary_list = NULL;
    MemberNode *merged_list = NULL;
    int32_t choice = 0;

    while (choice != 11) {
        printf("\n=============================================================\n");
        printf("            PINNACLE CLUB MEMBERSHIP MANAGEMENT              \n");
        printf("=============================================================\n");
        printf(" 1. Add President (Beginning of Club)\n");
        printf(" 2. Add Secretary (End of Club)\n");
        printf(" 3. Add Regular Member\n");
        printf(" 4. Delete Member by PRN (Includes President/Secretary)\n");
        printf(" 5. Count Total Members\n");
        printf(" 6. Display Club Members\n");
        printf(" 7. Reverse Member List (Using 3 Pointers)\n");
        printf(" 8. Sort Club List by PRN\n");
        printf(" 9. Populate Second Division List & Merge\n");
        printf("10. Clear All Members\n");
        printf("11. Exit\n");
        printf("=============================================================\n");

        prompt_input(&choice,
            .prompt = "Enter your choice (1-11): ",
            .range = INT_RANGE(1, 11)
        );

        switch (choice) {
            case 1: {
                int64_t prn = 0;
                char name[64] = {0};
                prompt_input(&prn, .prompt = "Enter President PRN: ", .range = INT_RANGE(1, INT64_MAX));
                prompt_input(STRING_BUFFER(name), .prompt = "Enter President Name: ");
                add_president(&club_list, prn, name);
                break;
            }
            case 2: {
                int64_t prn = 0;
                char name[64] = {0};
                prompt_input(&prn, .prompt = "Enter Secretary PRN: ", .range = INT_RANGE(1, INT64_MAX));
                prompt_input(STRING_BUFFER(name), .prompt = "Enter Secretary Name: ");
                add_secretary(&club_list, prn, name);
                break;
            }
            case 3: {
                int64_t prn = 0;
                char name[64] = {0};
                prompt_input(&prn, .prompt = "Enter Member PRN: ", .range = INT_RANGE(1, INT64_MAX));
                prompt_input(STRING_BUFFER(name), .prompt = "Enter Member Name: ");
                add_regular_member(&club_list, prn, name);
                break;
            }
            case 4: {
                if (club_list == NULL) {
                    printf("No students registered.\n");
                    break;
                }
                int64_t prn = 0;
                prompt_input(&prn, .prompt = "Enter PRN to delete: ", .range = INT_RANGE(1, INT64_MAX));
                delete_member(&club_list, prn);
                break;
            }
            case 5: {
                printf("Total number of members: %d\n", count_members(club_list));
                break;
            }
            case 6: {
                display_members(club_list);
                break;
            }
            case 7: {
                reverse_list(&club_list);
                display_members(club_list);
                break;
            }
            case 8: {
                sort_list_by_prn(&club_list);
                display_members(club_list);
                break;
            }
            case 9: {
                printf("\n--- Setting up Division B List ---\n");
                delete_entire_list(&auxiliary_list);
                int32_t num_members = 0;
                prompt_input(&num_members,
                    .prompt = "Enter number of members in Division B to add: ",
                    .range = INT_RANGE(1, 20)
                );

                for (int32_t i = 0; i < num_members; ++i) {
                    int64_t b_prn = 0;
                    char b_name[64] = {0};
                    printf("\nMember %d of %d:\n", i + 1, num_members);
                    prompt_input(&b_prn, .prompt = "Enter PRN: ", .range = INT_RANGE(1, INT64_MAX));
                    prompt_input(STRING_BUFFER(b_name), .prompt = "Enter Name: ");
                    add_secretary(&auxiliary_list, b_prn, b_name);
                }

                sort_list_by_prn(&club_list);
                sort_list_by_prn(&auxiliary_list);

                delete_entire_list(&merged_list);
                merged_list = merge_lists(club_list, auxiliary_list);

                printf("\n--- Merged Pinnacle Club List ---\n");
                display_members(merged_list);
                break;
            }
            case 10: {
                delete_entire_list(&club_list);
                printf("All members removed successfully.\n");
                break;
            }
            case 11: {
                printf("Exiting application.\n");
                break;
            }
        }
    }

    delete_entire_list(&club_list);
    delete_entire_list(&auxiliary_list);
    delete_entire_list(&merged_list);
    return 0;
}

static void clear_input_stream(FILE *stream) {
    int c = 0;
    while ((c = fgetc(stream)) != EOF && c != '\n') {
    }
}

static bool string_to_int64(const char *str, int64_t *out_val) {
    if (!str || !out_val || *str == '\0') return false;
    char *end_ptr = NULL;
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

bool parse_int32(const char *input, void *out_data, const void *context, bool allow_empty) {
    (void)allow_empty;
    if (!input || !out_data) return false;
    int64_t val = 0;
    if (!string_to_int64(input, &val)) return false;
    int64_t min_limit = INT32_MIN;
    int64_t max_limit = INT32_MAX;
    if (context) {
        const IntRange *range = (const IntRange *)context;
        if (range->min > range->max) {
            fprintf(stderr, "\n[FATAL] Range error: min > max.\n");
            exit(EXIT_FAILURE);
        }
        if (range->min > min_limit) min_limit = range->min;
        if (range->max < max_limit) max_limit = range->max;
    }
    if (val < min_limit || val > max_limit) return false;
    *(int32_t *)out_data = (int32_t)val;
    return true;
}

bool parse_int64(const char *input, void *out_data, const void *context, bool allow_empty) {
    (void)allow_empty;
    if (!input || !out_data) return false;
    int64_t val = 0;
    if (!string_to_int64(input, &val)) return false;
    if (context) {
        const IntRange *range = (const IntRange *)context;
        if (range->min > range->max) {
            fprintf(stderr, "\n[FATAL] Range error: min > max.\n");
            exit(EXIT_FAILURE);
        }
        if (val < range->min || val > range->max) return false;
    }
    *(int64_t *)out_data = val;
    return true;
}

bool parse_string(const char *input, void *out_data, const void *context, bool allow_empty) {
    (void)context;
    if (!out_data || !input) return false;
    StringBuffer *dest = (StringBuffer *)out_data;
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

    const char *prompt_str = options.prompt ? options.prompt : DEFAULT_PROMPT_TEXT;
    const char *error_str = options.error_msg ? options.error_msg : DEFAULT_ERROR_TEXT;
    char raw_buffer[INPUT_BUFFER_CAPACITY] = {0};

    while (true) {
        printf("%s", prompt_str);
        fflush(stdout);

        if (!fgets(raw_buffer, sizeof(raw_buffer), stdin)) {
            if (feof(stdin)) {
                fprintf(stderr, "\n[EOF] Input stream closed.\n");
            } else {
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

        char *start = raw_buffer;
        while (*start && isspace((unsigned char)*start)) start++;

        char *end = start + strlen(start);
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