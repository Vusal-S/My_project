#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

int lines[8][3][2] = {
    // 3 строки
    {{0, 0}, {0, 1}, {0, 2}},
    {{1, 0}, {1, 1}, {1, 2}},
    {{2, 0}, {2, 1}, {2, 2}},
    // 3 столбца
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 2}, {2, 2}},
    // 2 диагонали
    {{0, 0}, {1, 1}, {2, 2}},
    {{0, 2}, {1, 1}, {2, 0}}};

int digit_segments[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

void draw_digit(SDL_Renderer *renderer, int digit, int x, int y, int size)
{
    int t = size / 5; // толщина сегмента

    int *seg = digit_segments[digit];

    // a: верхняя горизонтальная
    if (seg[0])
    {
        SDL_Rect r = {x, y, size, t};
        SDL_RenderFillRect(renderer, &r);
    }
    // b: правая верхняя вертикальная
    if (seg[1])
    {
        SDL_Rect r = {x + size - t, y, t, size};
        SDL_RenderFillRect(renderer, &r);
    }
    // c: правая нижняя вертикальная
    if (seg[2])
    {
        SDL_Rect r = {x + size - t, y + size, t, size};
        SDL_RenderFillRect(renderer, &r);
    }
    // d: нижняя горизонтальная
    if (seg[3])
    {
        SDL_Rect r = {x, y + 2 * size - t, size, t};
        SDL_RenderFillRect(renderer, &r);
    }
    // e: левая нижняя вертикальная
    if (seg[4])
    {
        SDL_Rect r = {x, y + size, t, size};
        SDL_RenderFillRect(renderer, &r);
    }
    // f: левая верхняя вертикальная
    if (seg[5])
    {
        SDL_Rect r = {x, y, t, size};
        SDL_RenderFillRect(renderer, &r);
    }
    // g: средняя горизонтальная
    if (seg[6])
    {
        SDL_Rect r = {x, y + size - t / 2, size, t};
        SDL_RenderFillRect(renderer, &r);
    }
}

int check_winner(int board[][3])
{
    int check[3] = {0};

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int row = lines[i][j][0];
            int col = lines[i][j][1];
            check[j] = board[row][col];
        }

        if (check[0] == check[1] && check[1] == check[2] && check[0] != 0)
            return i;
    }

    return 10;
}

int is_board_full(int board[][3])
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (board[row][col] == 0)
                return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("XO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    int running = 1;
    SDL_Event event;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    int cell_w = w / 3;
    int cell_h = cell_w;
    int offset_x = (w - cell_w * 3) / 2;
    int offset_y = (h - cell_h * 3) / 2;

    int board[3][3] = {{0}};
    int current_player = 1;
    int current_win = 10;
    int is_draw = 0;
    int score_x = 0;
    int score_o = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (current_win != 10 || is_draw)
                {
                    current_win = 10;
                    is_draw = 0;
                    current_player = 1;
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < 3; j++)
                            board[i][j] = 0;
                }
                else
                {
                    int mx = event.button.x - offset_x;
                    int my = event.button.y - offset_y;
                    int col = mx / cell_w;
                    int row = my / cell_h;

                    if (row >= 0 && row < 3 && col >= 0 && col < 3)
                    {
                        if (current_win == 10 && !is_draw)
                        {
                            if (current_player == 1 && board[row][col] == 0)
                            {
                                board[row][col] = 1;
                                current_player = 2;
                                if (current_win == 10 && is_board_full(board))
                                    is_draw = 1;
                                int prev_win = current_win;
                                current_win = check_winner(board);
                                if (prev_win == 10 && current_win != 10)
                                {
                                    int winner = board[lines[current_win][0][0]][lines[current_win][0][1]];
                                    if (winner == 1)
                                        score_x++;
                                    else
                                        score_o++;
                                }
                            }
                            else if (current_player == 2 && board[row][col] == 0)
                            {
                                board[row][col] = 2;
                                current_player = 1;
                                if (current_win == 10 && is_board_full(board))
                                    is_draw = 1;
                                int prev_win = current_win;
                                current_win = check_winner(board);
                                if (prev_win == 10 && current_win != 10)
                                {
                                    int winner = board[lines[current_win][0][0]][lines[current_win][0][1]];
                                    if (winner == 1)
                                        score_x++;
                                    else
                                        score_o++;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (is_draw)
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0); // серый — ничья
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // белый — обычная игра
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        for (int i = 1; i < 3; i++)
        {
            SDL_Rect rect1 = {offset_x + cell_w * i - 3, offset_y, 6, cell_h * 3};
            SDL_RenderFillRect(renderer, &rect1);

            SDL_Rect rect2 = {offset_x, offset_y + cell_h * i - 3, cell_w * 3, 6};
            SDL_RenderFillRect(renderer, &rect2);
        }
        for (int row = 0; row < 3; row++)
            for (int col = 0; col < 3; col++)
            {
                if (board[row][col] == 1)
                {
                    for (int i = -3; i <= 3; i++)
                    {
                        SDL_RenderDrawLine(renderer,
                                           offset_x + 27 + i + col * cell_w, offset_y + 30 + row * cell_h,
                                           offset_x - 27 + i + (col + 1) * cell_w, offset_y - 30 + (row + 1) * cell_h);
                        SDL_RenderDrawLine(renderer,
                                           offset_x - 27 + i + (col + 1) * cell_w, offset_y + 30 + row * cell_h,
                                           offset_x + 27 + i + col * cell_w, offset_y - 30 + (row + 1) * cell_h);
                    }
                }
                else if (board[row][col] == 2)
                {
                    double center_x = offset_x + col * cell_w + cell_w / 2.0;
                    double center_y = offset_y + row * cell_h + cell_h / 2.0;
                    double base_radius = cell_w / 3.0;
                    for (int i = -2; i <= 2; i++)
                    {
                        double radius = base_radius + i;
                        for (double a = 0; a < 2 * M_PI; a += 0.001)
                        {
                            double x1 = center_x + radius * cos(a);
                            double y1 = center_y + radius * sin(a);
                            double x2 = center_x + radius * cos(a + 0.1);
                            double y2 = center_y + radius * sin(a + 0.1);
                            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                        }
                    }
                }
            }
        if (current_win != 10)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            int row1 = lines[current_win][0][0];
            int col1 = lines[current_win][0][1];
            int row2 = lines[current_win][2][0];
            int col2 = lines[current_win][2][1];

            double x1 = offset_x + col1 * cell_w + cell_w / 2.0;
            double y1 = offset_y + row1 * cell_h + cell_h / 2.0;
            double x2 = offset_x + col2 * cell_w + cell_w / 2.0;
            double y2 = offset_y + row2 * cell_h + cell_h / 2.0;

            double dx = x2 - x1;
            double dy = y2 - y1;
            double length = sqrt(dx * dx + dy * dy);
            dx /= length;
            dy /= length;

            double extend = cell_w / 2.0;
            double ex1 = x1 - dx * extend;
            double ey1 = y1 - dy * extend;
            double ex2 = x2 + dx * extend;
            double ey2 = y2 + dy * extend;

            for (int i = -4; i <= 4; i++)
            {
                SDL_RenderDrawLine(renderer, ex1 + i, ey1, ex2 + i, ey2);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        draw_digit(renderer, score_x, offset_y / 4 + 200, offset_y / 5 + 16, 33);
        draw_digit(renderer, score_o, offset_y + 335, offset_y / 5 + 16, 33);

        for (int i = -3; i <= 3; i++)
        {
            SDL_RenderDrawLine(renderer, offset_y / 4 + i, offset_y / 4 - 10, 3 * offset_y / 8 + i, offset_y / 3 + 20);
            SDL_RenderDrawLine(renderer, 3 * offset_y / 8 + i, offset_y / 4 - 10, offset_y / 4 + i, offset_y / 3 + 20);
        }

        SDL_Rect rect1 = {3 * offset_y / 8 + 40, offset_y / 4 + 20, 50, 5};
        SDL_RenderFillRect(renderer, &rect1);

        double center_x = 1.3 * offset_y;
        double center_y = 7 * offset_y / 24;
        double base_radius = offset_y / 15;

        for (int i = -2; i <= 2; i++)
        {
            double radius = base_radius + i;
            for (double a = 0; a < 2 * M_PI; a += 0.001)
            {
                double x1 = center_x + radius * cos(a);
                double y1 = center_y + radius * sin(a);
                double x2 = center_x + radius * cos(a + 0.1);
                double y2 = center_y + radius * sin(a + 0.1);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }

        SDL_Rect rect2 = {offset_y + 240, offset_y / 4 + 20, 50, 5};
        SDL_RenderFillRect(renderer, &rect2);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
