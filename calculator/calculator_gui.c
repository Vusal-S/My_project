#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_TOKENS 100

int top = -1;
char stack[MAX_TOKENS];
int topd = -1;
double stackd[MAX_TOKENS];

typedef enum { NUMBER, OPERATOR } CalcTokenType;

typedef struct
{
    CalcTokenType type;
    double value;   // используется, если type == NUMBER
    char op;        // используется, если type == OPERATOR
} Token;

int find(const char *m, const char a)
{
	for (int i=0; m[i] != '\0'; i++)
	{
		if (m[i] == a) return 1;
	}
	return 0;
}

int fdigit(const char a)
{
	return a >= '0' && a <= '9';
}

void push(char x)
{
	if (top >= MAX_TOKENS - 1)	printf("Ошибка стек полный\n");
	else
	{
		top++;
		stack[top] = x;
	}
}

char pop()
{
	if(top < 0)
	{
		printf("Ошибка стек пуст\n");
		return -1;
	}
	
	top--;
	return stack[top + 1];
}

char peek()
{
	if(top < 0)
	{
		printf("Ошибка стек пуст\n");
		return -1;
	}
	
	return stack[top];
}

void pushd(double x)
{
	if (topd >= MAX_TOKENS - 1)	printf("Ошибка стек полный\n");
	else
	{
		topd++;
		stackd[topd] = x;
	}
}

double popd()
{
	if(topd < 0)
	{
		printf("Ошибка стек пуст\n");
		return -1;
	}
	
	topd--;
	return stackd[topd + 1];
}

double peekd()
{
	if(topd < 0)
	{
		printf("Ошибка стек пуст\n");
		return -1;
	}
	
	return stackd[topd];
}

int priority(char x)
{
	if (x == '+' || x == '-') return 1;
	if (x == '*' || x == '/') return 2;
	if (x == '$') return 3;
	return 0;
}

double evaluate(char *input)
{
    Token tokens[MAX_TOKENS];
    int kol = 0;

    input[strcspn(input, "\n")] = '\0';
    
    if (input[0] == '\0')
    {
		return 0;
	}
    
    char *p = input;

    while (*p != '\0')
    {
        if (*p == ' ')
        {
            p++;
		}
        else if (fdigit(*p))
        {
            char *end;
            double num = strtod(p, &end);
            tokens[kol].type = NUMBER;
            tokens[kol].value = num;
            kol++;
            p = end;
        }
        else if (find("+-*/()", *p))
        {
			if ((kol == 0  || (tokens[kol - 1].type == OPERATOR && tokens[kol - 1].op != ')')) && *p == '-')
			{
				tokens[kol].type = OPERATOR;
				tokens[kol].op = '$';
				kol++;
				p++;
			}
			else
			{
				tokens[kol].type = OPERATOR;
				tokens[kol].op = *p;
				kol++;
				p++;
			}
        }
        else
        {
            p++;
        }
    }
    
    Token output[200];
    int k = 0;
    
    for (int i = 0; i < kol; i++)
    {
		if (tokens[i].type == NUMBER)
		{
			output[k].value = tokens[i].value;
			output[k++].type = NUMBER;
		}
		else if (tokens[i].op == '(')
		{
			push(tokens[i].op);
		}
		else if (tokens[i].op == ')')
		{
			while (peek() != '(' && top >= 0)
			{
				output[k].op = pop();
				output[k++].type = OPERATOR;
			}
			pop();
		}
		else
		{
			while (top >= 0 && priority(peek()) >= priority(tokens[i].op))
			{
				output[k].op = pop();
				output[k++].type = OPERATOR;
			}
			
			push(tokens[i].op);
		}
	}		
    
    while (top >= 0)
    {
		output[k].op = pop();
		output[k++].type = OPERATOR;
	}
	
	int error = 0;
	
    for (int i = 0; i < k; i++)
    {
        if (output[i].type == NUMBER)	pushd(output[i].value);
        else
        {
			if (output[i].op == '$')
			{
				double c = -popd();
				pushd(c);
				continue;
			}
			
			double b = popd();
			double a = popd();
			
			switch (output[i].op)
			{
				case '+':
					pushd(a+b);
					break;
					
				case '-':
					pushd(a-b);
					break;
					
				case '*':
					pushd(a*b);
					break;
					
				case '/':
					if (b == 0)
					{
						error = 1;
						break;
					}
					pushd(a/b);
					break;
			}
		}
		if (error == 1) break;
    }
    
    if (error != 1)
    {
		double a = popd();
		return a;
	}
	else
	{
		return NAN;
	}
		
    top = -1; topd = -1;
}



HWND hDisplay;

char operators[] = {'=', 'A', 'C','(', ')', '+', '-', '/', '*', '.'};
int haserorr = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char input[100];
	char output[100];
	
	
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_COMMAND:
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (haserorr == 1)
				{
					SetWindowText(hDisplay, "");
					haserorr = 0;
				}
				
				GetWindowText(hDisplay, input, 100);
			
				if (99 < LOWORD(wParam) && LOWORD(wParam) < 110)
				{
					sprintf(output, "%d", LOWORD(wParam)-100);
					strcat(input, output);
					SetWindowText(hDisplay, input);
				}
				else if (202 < LOWORD(wParam) && LOWORD(wParam) < 210)
				{
					sprintf(output, "%c", operators[LOWORD(wParam)-200]);
					strcat(input, output);
					SetWindowText(hDisplay, input);
				}
				else if (LOWORD(wParam) == 200)
				{
					double a = evaluate(input);
					
					if (isnan(a))
					{
						SetWindowText(hDisplay, "Erorr division by 0!");
						haserorr = 1;
					}
					else
					{
						sprintf(output, "%g", a);
						SetWindowText(hDisplay, output);
						haserorr = 0;
					}
				}
				else if (LOWORD(wParam) == 201)
				{
					SetWindowText(hDisplay, "");
				}
				else if (LOWORD(wParam) == 202)
				{
					if (strlen(input) == 0) return 0;
					
					input[strlen(input) - 1] = '\0';
					SetWindowText(hDisplay,input);
				}
			}
			return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char CLASS_NAME[] = "CalculatorWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "My calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
        NULL, NULL, hInstance, NULL
    );
	
	hDisplay = CreateWindowEx(
		0, "EDIT", "",
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
		20, 20, 240, 30,
		hwnd, NULL, hInstance, NULL
	);
	
	HWND hDigitButtons[10];
	
	int col;
	int row;

	for (int i = 0; i < 10; i++)
	{
		col = (i-1) % 3;
		row = (i-1) / 3 + 1;
		
		if (i == 0)
		{
			col = 1;
			row = 4;
		}

		char label[2];
		label[0] = '0' + i;
		label[1] = '\0';

		hDigitButtons[i] = CreateWindowEx(
			0, "BUTTON", label,
			WS_VISIBLE | WS_CHILD,
			61 * col + 20, 52 * row + 75, 61, 52,
			hwnd, (HMENU)(i + 100), hInstance, NULL
		);
	}
	
	HWND hOpButtons[10];
	
	col = 0;
	row = 4;
	
	for (int i = 0; i < 10; i++)
	{
		char label[3];
		if (i == 1)
		{
			strcpy(label, "AC");
		}
		else
		{
			label[0] = operators[i];
			label[1] = '\0';
		}
		
		hOpButtons[i] = CreateWindowEx(
			0, "BUTTON", label,
			WS_VISIBLE | WS_CHILD,
			61 * col + 20, 52 * row + 75, 61, 52,
			hwnd, (HMENU)(i + 200), hInstance, NULL
		);
		
		if (i >= 1 && i <= 3) col++;
		else if (i == 8) col--;
		else if (i == 0) row = 0;
		else row++;
	}
	
    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
