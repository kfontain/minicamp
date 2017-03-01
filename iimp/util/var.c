#include <string.h>
#include <var.h>

var* head = NULL;
head = malloc(sizeof(var));
head->val = -1;
head->id = NULL;
head->next = NULL;

void addVar(char* id, int val) {
    var* current = head;

    while (current != NULL) {
        if (strcmp(id, current->id)) {
            current->val = val;
            return;
        }
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    current->next->id = strdup(id);
    current->next->val = val;
    current->next->next = NULL;
}

int getValue(char* id) {
    var* current = head;

    while (current != NULL) {
        if (strcmp(id, current->id)) {
            return current->val;
        }
    }
}
