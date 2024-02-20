#ifndef MINESWEEPER_H
#define MINESWEEPER_H

typedef struct {
    int value;
    int is_mine;
    int is_boombed;
    int is_visited;
    int is_flagged;
} Node;

void ajouter_matrice(Node **tab, int l, int c);
void add_the_values_adjacent(Node **tab, int l, int c);
void Show_all(Node **tab, int l, int c);
void change_the_visited_state_of_the_node(Node **tab, int l, int c, int x, int y);
int Visit_Node(Node **tab, int l, int c, int x, int y);
void AddFlag(Node **tab, int x, int y);
void destroyFlag(Node **tab, int x, int y);
void play_game(Node **tab, int l, int c);

#endif /* MINESWEEPER_H */
