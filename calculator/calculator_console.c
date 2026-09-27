#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKENS 100

int top = -1;
char stack[MAX_TOKENS];
int topd = -1;
double stackd[MAX_TOKENS];

typedef enum { NUMBER, OPERATOR } TokenType;

typedef struct
{
    TokenType type;
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


int main(void)
{
	double history[100];
	int history_count = 0;
	
	while(1)
	{
    char input[200];
    Token tokens[MAX_TOKENS];
    int kol = 0;

    printf("Введите выражение: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "exit") == 0)	return 0;
    else if (strcmp(input, "history") == 0)
    {
		if (history_count == 0)
		{
			printf("История пустая\n");
			continue;
		}
		for (int i = 0; i < history_count; i++)
		{
			printf("%d)%g; ", i, history[i]);
		}
		printf("\n");
		continue;
	}
	
    else if (input[0] == '\0')
    {
		continue;
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
            printf("Неизвестный символ: %c\n", *p);
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
						printf("Ошибка деление на ноль\n");
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
		if (history_count >= 100)	printf("История переполнена\n");
		else   history[history_count++] = peekd();
		
		printf("Результат: %g\n\n", popd());
	}
    
    top = -1; topd = -1;
	}
}
