#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "minesweeper.h" // Include the header file containing the Node structure definition

// Function to initialize the matrix with nodes of the game
void add_matrix(Node **tab, int l, int c) {
    // Fill the matrix with null values
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // Initialize each node with default values
            tab[i][j].value = 0;
            tab[i][j].is_mine = 0;
            tab[i][j].is_visited = 0;
            tab[i][j].is_flagged = 0;
            tab[i][j].is_boombed = 0;
        }
    }
    // Fill randomly selected cells with mines
    int ind1, ind2;
    srand(time(NULL));
    for (int i = 0; i < l + 10; i++) {
        ind1 = rand() % l;
        ind2 = rand() % c;
        tab[ind1][ind2].value = -1; // Indicate that there is a mine in this cell
        tab[ind1][ind2].is_mine = 1;
    }
}

// Function to add the values of adjacent cells
void add_adjacent_values(Node **tab, int l, int c) {
    // Loop through each cell in the matrix
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // If the current cell contains a mine, increment the value of adjacent cells
            if (tab[i][j].is_mine == 1) {
                for (int k = i - 1; k <= i + 1; k++) {
                    for (int z = j - 1; z <= j + 1; z++) {
                        // Check if the adjacent cell is within the bounds of the matrix
                        if (k >= 0 && k < l && z >= 0 && z < c && k != z) {
                            // Increment the value of the adjacent cell
                            tab[k][z].value++;
                        }
                    }
                }
            }
        }
    }
}

// Function to display the current state of the game
void Show_all(Node **tab, int l, int c) {
    // Print the current state of each cell in the matrix
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            if (tab[i][j].is_boombed == 1)
                printf("[ * ] "); // Print a '*' if the cell is bombed
            else if (tab[i][j].is_flagged == 1)
                printf("[ f ] "); // Print an 'f' if the cell is flagged
            else if (tab[i][j].is_visited == 1) {
                if (tab[i][j].is_mine == 1)
                    printf("[ m ] "); // Print an 'm' if the cell contains a mine
                else
                    printf("[ %d ] ", tab[i][j].value); // Print the value of the cell
            } else
                printf("[%d:%d] ", i, j); // Print the coordinates of the cell
        }
        printf("\n");
    }
}

// Function to recursively change the visited state of adjacent nodes
void change_visited_state(Node **tab, int l, int c, int x, int y) {
    // Mark the current node as visited
    tab[x][y].is_visited = 1;
    // If the value of the current node is 0, recursively change the visited state of adjacent nodes
    if (tab[x][y].value == 0) {
        for (int k = x - 1; k <= x + 1; k++) {
            for (int z = y - 1; z <= y + 1; z++) {
                if (k >= 0 && k < l && z >= 0 && z < c) {
                    if (tab[k][z].is_mine != 1 && tab[k][z].is_visited == 0) {
                        change_visited_state(tab, l, c, k, z);
                    }
                }
            }
        }
    }
}

// Function to handle visiting a node
int visit_node(Node **tab, int l, int c, int x, int y) {
    // If the visited node contains a mine, the game is lost
    if (tab[x][y].is_mine == 1) {
        printf("You lose :( \n");
        tab[x][y].is_boombed = 1;
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < c; j++)
                tab[i][j].is_visited = 1;
        }
        return 0;
    } else if (tab[x][y].is_visited == 1) {
        printf("You have been here before. Choose another one!\n");
    } else {
        printf("Nice work :) \n");
        // Change the visited state of adjacent nodes recursively
        change_visited_state(tab, l, c, x, y);
    }
    return 1;
}

// Function to add a flag to a node
void AddFlag(Node **tab, int x, int y) {
    tab[x][y].is_flagged = 1;
}

// Function to remove a flag from a node
void destroyFlag(Node **tab, int x, int y) {
    tab[x][y].is_flagged = 0;
}

// Function to play the game
void play_game(Node **tab, int l, int c) {
    int x, y, is_lose, choose;
    do {
        printf("\n");
        printf("*************************Mini Menu *************************\n");
        printf("1 - Add flag \n");
        printf("2 - Visit the case \n");
        printf("3 - Delete flag \n");
        printf("*************************End of Mini Menu *************************\n");
        printf("Enter your request (1/2): ");
        scanf("%d", &choose);
        printf("\nEnter the x and y coordinates of your request: ");
        scanf("%d %d", &x, &y);

        switch (choose) {
            case 1:
                AddFlag(tab, x, y);
                break;
            case 2:
                is_lose = visit_node(tab, l, l, x, y);
                break;
            case 3:
                destroyFlag(tab, x, y);
                break;
            default:
                printf("Try again with 1 or 2 :) \n");
                break;
        }

        Show_all(tab, l, l);
    } while (is_lose != 0);
}

int main() {
    printf("Hi! Welcome to the game of Minesweeper :) \n");
    int l;
    do {
        printf("Choose the level of difficulty:\n");
        printf("1 - Easy \n");
        printf("2 - Medium \n");
        printf("3 - Hard \n");
        printf("Choose between 1, 2, or 3:  \n");
        scanf("%d", &l);
    } while (l > 3 && l < 1);
    l = l * 10;

    // Create the matrix using pointers and malloc
    Node **tab = malloc(l * sizeof(Node *));
    for (int i = 0; i < l; i++) {
        tab[i] = malloc(l * sizeof(Node));
    }

    add_matrix(tab, l, l);
    add_adjacent_values(tab, l, l);
    Show_all(tab, l, l);
    play_game(tab, l, l);

    // Free dynamically allocated memory
    for (int i = 0; i < l; i++) {
        free(tab[i]);
    }
    free(tab);
    return 0;
}
