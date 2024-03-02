#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Include the header file containing the Node structure definition

typedef struct {
    int value;
    int is_mine;
    int is_boombed;
    int is_visited;
    int is_flagged;
} Node;

// Constants for window dimensions and button IDs
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define BUTTON_SIZE 40
#define ID_BUTTON_START 1000

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitializeGame(HWND hwnd);
void AddMatrix(HWND hwnd, int l, int c);
void AddAdjacentValues(Node **tab, int l, int c);
void ShowAll(HWND hwnd, HDC hdc);
void VisitNode(HWND hwnd, int x, int y);
void AddFlag(HWND hwnd, int x, int y);
void DestroyFlag(HWND hwnd, int x, int y);
void PlayGame(HWND hwnd);
void DrawGrid(HWND hwnd, HDC hdc);
void DrawCell(HWND hwnd, HDC hdc, int x, int y, Node *cell);
void add_matrix(Node **tab, int l, int c);

// Global variables
HWND g_hButtonMatrix[10][10]; // Buttons representing cells
Node **g_tab; // Matrix representing the game board
int g_l, g_c; // Dimensions of the matrix
int g_gameOver = 0; // Flag indicating whether the game is over

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class.
    const char CLASS_NAME[] = "MinesweeperClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Minesweeper",                  // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        InitializeGame(hwnd);
        break;
    case WM_COMMAND:
        if (!g_gameOver) {
            int buttonId = LOWORD(wParam);
            if (buttonId >= ID_BUTTON_START && buttonId < ID_BUTTON_START + g_l * g_c) {
                int x = (buttonId - ID_BUTTON_START) / g_c;
                int y = (buttonId - ID_BUTTON_START) % g_c;
                VisitNode(hwnd, x, y);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        ShowAll(hwnd, GetDC(hwnd));
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Function to add the values of adjacent cells
void AddAdjacentValues(Node **tab, int l, int c) {
    // Loop through each cell in the matrix
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // If the current cell contains a mine, increment the value of adjacent cells
            if (tab[i][j].is_mine == 1) {
                for (int k = i - 1; k <= i + 1; k++) {
                    for (int z = j - 1; z <= j + 1; z++) {
                        // Check if the adjacent cell is within the bounds of the matrix
                        if (k >= 0 && k < l && z >= 0 && z < c && !(k == i && z == j)) {
                            // Increment the value of the adjacent cell
                            tab[k][z].value++;
                        }
                    }
                }
            }
        }
    }
}

void InitializeGame(HWND hwnd) {
    g_l = g_c = 10;
    g_tab = (Node**)malloc(g_l * sizeof(Node*));
    for (int i = 0; i < g_l; ++i) {
        g_tab[i] = (Node*)malloc(g_c * sizeof(Node));
    }
    add_matrix(g_tab, g_l, g_c); // Call the add_matrix function to initialize the game board
    AddMatrix(hwnd, g_l, g_c);
    AddAdjacentValues(g_tab, g_l, g_c);
    PlayGame(hwnd);
}

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

void AddMatrix(HWND hwnd, int l, int c) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int xPos = 10, yPos = 10;
    for (int i = 0; i < l; ++i) {
        for (int j = 0; j < c; ++j) {
            g_hButtonMatrix[i][j] = CreateWindow(
                "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                xPos, yPos, BUTTON_SIZE, BUTTON_SIZE, hwnd, (HMENU)(ID_BUTTON_START + i * c + j),
                hInstance, NULL);
            xPos += BUTTON_SIZE + 5;
        }
        xPos = 10;
        yPos += BUTTON_SIZE + 5;
    }
}

void ShowAll(HWND hwnd, HDC hdc) {
    DrawGrid(hwnd, hdc);
    // Implement this function to draw the current state of the game
    // This function will draw the cells with appropriate labels based on their state
    for (int i = 0; i < g_l; i++) {
        for (int j = 0; j < g_c; j++) {
            DrawCell(hwnd, hdc, i, j, &g_tab[i][j]);
        }
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

void VisitNode(HWND hwnd, int x, int y) {
    // Implement this function to handle visiting a cell
    // This function will reveal the content of the cell and recursively explore adjacent cells if needed
    int is_lose = visit_node(g_tab, g_l, g_c, x, y);
    if (is_lose == 0) {
        g_gameOver = 1;
        MessageBox(hwnd, "You lose!", "Game Over", MB_OK);
    }
    InvalidateRect(hwnd, NULL, TRUE);
}

void AddFlag(HWND hwnd, int x, int y) {
    // Implement this function to add a flag to a cell
    g_tab[x][y].is_flagged = 1;
    InvalidateRect(hwnd, NULL, TRUE);
}

void DestroyFlag(HWND hwnd, int x, int y) {
    // Implement this function to remove a flag from a cell
    g_tab[x][y].is_flagged = 0;
    InvalidateRect(hwnd, NULL, TRUE);
}

void PlayGame(HWND hwnd) {
    // Implement this function to handle the game loop
    // This function will handle player actions and update the game state
    // It will continue until the game is over
    g_gameOver = 0;
}

void DrawGrid(HWND hwnd, HDC hdc) {
    // Implement this function to draw the grid
    // This function will draw the grid lines to separate the cells
    for (int i = 0; i <= g_l; ++i) {
        MoveToEx(hdc, 10, 10 + i * (BUTTON_SIZE + 5), NULL);
        LineTo(hdc, 10 + g_l * (BUTTON_SIZE + 5), 10 + i * (BUTTON_SIZE + 5));
    }
    for (int j = 0; j <= g_c; ++j) {
        MoveToEx(hdc, 10 + j * (BUTTON_SIZE + 5), 10, NULL);
        LineTo(hdc, 10 + j * (BUTTON_SIZE + 5), 10 + g_c * (BUTTON_SIZE + 5));
    }
}

void DrawCell(HWND hwnd, HDC hdc, int x, int y, Node *cell) {
    // Implement this function to draw the content of a cell
    // This function will draw the appropriate label based on the cell's state
    RECT rect;
    rect.left = 10 + y * (BUTTON_SIZE + 5);
    rect.top = 10 + x * (BUTTON_SIZE + 5);
    rect.right = rect.left + BUTTON_SIZE;
    rect.bottom = rect.top + BUTTON_SIZE;

    if (cell->is_boombed==1) {
        // Draw a bomb symbol
        DrawText(hdc, "*", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    } else if (cell->is_flagged==1) {
        // Draw a flag symbol
        DrawText(hdc, "F", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    } else if (cell->is_visited==1) {
        if (cell->is_mine==1) {
            // Draw an 'M' for mine
            DrawText(hdc, "M", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        } else if (cell->value > 0) {
            // Draw the value of the cell

            char value[2];
            sprintf(value, "%d", cell->value);
            DrawText(hdc, value, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        } else {
            // Draw an empty cell
            DrawText(hdc, "", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    } else {
        // Draw an empty cell
        DrawText(hdc, "", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
}
