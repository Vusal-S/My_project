#include "raylib.h"

#define CELL_SIZE 25

typedef struct
{
	int x;
	int y;
}body;

void Position(body *snake, body *apple, int size)
{
	int flag;
	
	do
	{
		flag = 1;
		
		for( int i = 0; i < size; i++)
		{
			if (snake[i].x == apple->x && snake[i].y == apple->y)
			{
				flag = 0;
				apple->x = GetRandomValue(0, 31); apple->y = GetRandomValue(0, 17);
				break;
			}
		}
	}
	while (flag == 0);
}

int main(void)
{
    InitWindow(820, 470, "Игра змейка от Вусала");
    
    body tail;
    body eye[2];
    body snake[600];
    snake[0].x = GetRandomValue(0, 21); snake[0].y = GetRandomValue(0, 11);
    
    body apple;
    apple.x = GetRandomValue(0, 31); apple.y = GetRandomValue(0, 17);
    
	int direction = 0, snakeLength = 1, gameOver = 0;
	double lastMoveTime = GetTime();
	
    Position(snake, &apple, snakeLength);
	
    while (!WindowShouldClose())
    {
		if (gameOver == 0)
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);
        
			DrawRectangle(10, 10, 800, 450, DARKGREEN);
        
			for (int i = 0; i < 32*18; i++)
			{
				int col = i % 32;
				int row = i / 32;
		
				DrawRectangleLines(10 + col * CELL_SIZE, 10 + row * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
			}
		
			if (IsKeyPressed(KEY_RIGHT) && direction != 1) direction = 0;
			else if (IsKeyPressed(KEY_LEFT) && direction != 0) direction = 1;
			else if (IsKeyPressed(KEY_DOWN) && direction != 3) direction = 2;
			else if (IsKeyPressed(KEY_UP) && direction != 2) direction = 3;
        
			if (GetTime() - lastMoveTime > 0.20)
			{
				tail.x = snake[snakeLength - 1].x;
				tail.y = snake[snakeLength - 1].y; 
				for (int i = snakeLength - 1; i > 0; i--)
				{
					snake[i] = snake[i-1];
				}
			
				switch (direction)
				{
					case 0:
						snake[0].x++;
						break;
					case 1:
						snake[0].x--;
						break;
					case 2:
						snake[0].y++;
						break;
					case 3:
						snake[0].y--;
						break;
				}
				lastMoveTime = GetTime();
			}
		
			if (snake[0].x < 0 || snake[0].y < 0 || snake[0].x > 31 || snake[0].y > 17)	gameOver = 1;
		
			DrawRectangleRounded((Rectangle){10 + apple.x * CELL_SIZE, 10 + apple.y * CELL_SIZE, CELL_SIZE, CELL_SIZE}, 0.4, 6, RED);
			DrawRectangleRounded((Rectangle){10 + snake[0].x * CELL_SIZE + 1, 10 + snake[0].y * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1}, 0.4, 6, BLUE);
			switch (direction)
			{
				case 0:
					eye[0].x = 10 + snake[0].x * CELL_SIZE + 19; eye[1].x = eye[0].x;
					eye[0].y = 10 + snake[0].y * CELL_SIZE + 7; eye[1].y = eye[0].y + 12;
					break;
				case 1:
					eye[0].x = 10 + snake[0].x * CELL_SIZE + 6; eye[1].x = eye[0].x;
					eye[0].y = 10 + snake[0].y * CELL_SIZE + 7; eye[1].y = eye[0].y + 12;
					break;
				case 2:
					eye[0].x = 10 + snake[0].x * CELL_SIZE + 7; eye[1].x = eye[0].x + 12;
					eye[0].y = 10 + snake[0].y * CELL_SIZE + 19; eye[1].y = eye[0].y;
					break;
				case 3:
					eye[0].x = 10 + snake[0].x * CELL_SIZE + 7; eye[1].x = eye[0].x + 12;
					eye[0].y = 10 + snake[0].y * CELL_SIZE + 6; eye[1].y = eye[0].y;
					break;
			}
			DrawCircle(eye[0].x, eye[0].y, 2, BLACK);
			DrawCircle(eye[1].x, eye[1].y, 2, BLACK);
			
				
			for (int i = 1; i < snakeLength; i++)
			{
				if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)	gameOver = 1;
				
				DrawRectangleRounded((Rectangle){10 + snake[i].x * CELL_SIZE + 1, 10 + snake[i].y * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1}, 0.4, 6, DARKBLUE);
			}
		
			if (snake[0].x == apple.x && snake[0].y == apple.y)
			{
				apple.x = GetRandomValue(0, 31);
				apple.y = GetRandomValue(0, 17);
				Position(snake, &apple, snakeLength);
				snakeLength++;
				snake[snakeLength - 1].x = tail.x;
				snake[snakeLength - 1].y = tail.y;
			}
		
			EndDrawing();
		}
		else
		{
			BeginDrawing();
			ClearBackground(RAYWHITE);
			
			DrawText("GAMEOVER", 210, 200, 60, BLACK);
			
			if (IsKeyPressed(KEY_ENTER))
			{
				gameOver = 0;
				apple.x = GetRandomValue(0, 31); apple.y = GetRandomValue(0, 17);
				snake[0].x = GetRandomValue(0, 21); snake[0].y = GetRandomValue(0, 11);
				direction = 0; snakeLength = 1;
				Position(snake, &apple, snakeLength);
				lastMoveTime = GetTime();
			}
			
			EndDrawing();
		}
	}

    CloseWindow();
    return 0;
}
