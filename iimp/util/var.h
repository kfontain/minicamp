typedef struct variable {
    char* id;
    int val;
    struct variable* next;
} var;

int getValue(char* id);
void addVar(char* id, int val);
