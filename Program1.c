#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structures for NFA
typedef struct NFAState {
    int id;
    struct NFAState* next[256];
    int is_accepting;
} NFAState;

typedef struct NFA {
    NFAState* start;
    NFAState* accept;
} NFA;

// Create a new NFA state
NFAState* createState(int id) {
    NFAState* state = (NFAState*)malloc(sizeof(NFAState));
    state->id = id;

    for (int i = 0; i < 256; ++i) {
        state->next[i] = NULL;
    }

    state->is_accepting = 0;
    return state;
}

// Create a new NFA
NFA* createNFA() {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->start = createState(0);
    nfa->accept = createState(1);
    nfa->accept->is_accepting = 1;
    return nfa;
}

// Connect two states with a transition
void addTransition(NFAState* from, NFAState* to, char symbol) {
    from->next[(unsigned char)symbol] = to;
}

// Print NFA states and transitions
void printNFA(NFA* nfa, NFAState* state, int visited[], int* count) {
    if (visited[state->id])
        return;

    visited[state->id] = 1;

    printf("State %d%s\n", state->id,
           state->is_accepting ? " (accepting)" : "");

    for (int i = 0; i < 256; ++i) {
        if (state->next[i]) {
            printf("  %c -> State %d\n", i, state->next[i]->id);
            printNFA(nfa, state->next[i], visited, count);
        }
    }
}

// Basic parsing of regular expression
NFA* parseRegExp(const char* regexp) {
    NFA* nfa = createNFA();
    NFAState* current = nfa->start;
    NFAState* prev = NULL;
    NFAState* start = nfa->start;
    int id_counter = 2;

    for (int i = 0; regexp[i] != '\0'; ++i) {
        char c = regexp[i];

        switch (c) {

            case '*': {
                // Handle Kleene star
                NFAState* new_accept = createState(id_counter++);
                addTransition(current, new_accept, '\0');
                addTransition(new_accept, start, '\0');
                start->is_accepting = 1;
                current = new_accept;
                break;
            }

            case '|': {
                // Handle union
                NFA* left_nfa = createNFA();
                NFA* right_nfa = createNFA();

                NFAState* new_accept = createState(id_counter++);

                addTransition(left_nfa->start, new_accept, '\0');
                addTransition(right_nfa->start, new_accept, '\0');

                left_nfa->accept->is_accepting = 1;
                right_nfa->accept->is_accepting = 1;

                current = new_accept;
                break;
            }

            default: {
                // Handle literals
                NFAState* new_state = createState(id_counter++);
                addTransition(current, new_state, c);
                current = new_state;
                break;
            }
        }
    }

    current->is_accepting = 1;
    nfa->accept = current;

    return nfa;
}

int main() {
    const char* regexp = "a*b|c";
    NFA* nfa = parseRegExp(regexp);

    int visited[256] = {0};
    int count = 0;

    printf("NFA states and transitions:\n");
    printNFA(nfa, nfa->start, visited, &count);

    // Memory free omitted for brevity

    return 0;
}
