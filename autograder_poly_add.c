#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int coef;
    int exp;
    struct node* next;
} pnode;

pnode* new_poly(void)
{
    pnode* header = (pnode*)malloc(sizeof(pnode));
    if (header == NULL) {
        return NULL;
    }
    header->coef = 0;
    header->exp = -1;
    header->next = header;
    return header;
}

void insert_term(pnode* head, int coef, int exp)
{
    if (head == NULL || coef == 0) {
        return;
    }

    pnode* previous_node = head;
    pnode* current_node = head->next;

    while (current_node != head && current_node->exp > exp) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != head && current_node->exp == exp) {
        current_node->coef += coef;
        if (current_node->coef == 0) {
            previous_node->next = current_node->next;
            free(current_node);
        }
    }
    else {
        pnode* new_term = (pnode*)malloc(sizeof(pnode));
        if (new_term == NULL) {
            return;
        }
        new_term->coef = coef;
        new_term->exp = exp;
        new_term->next = current_node;
        previous_node->next = new_term;
    }
}

pnode* read_poly(void)
{
    pnode* poly = new_poly();
    int term_count = 0;

    if (scanf("%d", &term_count) != 1) {
        return poly;
    }

    for (int i = 0; i < term_count; i++) {
        int coefficient = 0;
        int exponent = 0;
        if (scanf("%d %d", &coefficient, &exponent) == 2) {
            insert_term(poly, coefficient, exponent);
        }
    }

    return poly;
}

pnode* poly_add(pnode* h1, pnode* h2)
{
    pnode* result_poly = new_poly();

    if (h1 != NULL) {
        pnode* ptr1 = h1->next;
        while (ptr1 != h1) {
            insert_term(result_poly, ptr1->coef, ptr1->exp);
            ptr1 = ptr1->next;
        }
    }

    if (h2 != NULL) {
        pnode* ptr2 = h2->next;
        while (ptr2 != h2) {
            insert_term(result_poly, ptr2->coef, ptr2->exp);
            ptr2 = ptr2->next;
        }
    }

    return result_poly;
}

void free_poly(pnode* head)
{
    if (head == NULL) {
        return;
    }

    pnode* current_node = head->next;
    while (current_node != head) {
        pnode* node_to_delete = current_node;
        current_node = current_node->next;
        free(node_to_delete);
    }

    free(head);
}

void print_poly(pnode* head)
{
    pnode* p = head->next;

    if (p == head) {
        printf("0\n");
        return;
    }
    while (p != head) {
        printf("%+dx^%d", p->coef, p->exp);
        p = p->next;
        if (p != head)
            printf(" ");
    }
    printf("\n");
}

int main(void)
{
    pnode* p1 = read_poly();
    pnode* p2 = read_poly();
    pnode* sum = poly_add(p1, p2);

    printf("P1 = ");  print_poly(p1);
    printf("P2 = ");  print_poly(p2);
    printf("SUM = "); print_poly(sum);

    free_poly(p1);
    free_poly(p2);
    free_poly(sum);
    return 0;
}