#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define STACK_SIZE 100

// Function prototypes
double evaluateRPN(char *expression);
int isOperator(char token);
int getPrecedence(char operator);
void shuntingYard(char *infix, char *output);

// Stack structure
typedef struct {
    double data[STACK_SIZE];
    int top;
} Stack;

void initializeStack(Stack *stack) {
    stack->top = -1;
}

void push(Stack *stack, double value) {
    if (stack->top < STACK_SIZE - 1) {
        stack->data[++stack->top] = value;
    } else {
        printf("Error: Stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

double pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top--];
    } else {
        printf("Error: Stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

double peek(Stack *stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top];
    } else {
        printf("Error: Stack is empty\n");
        exit(EXIT_FAILURE);
    }
}

int isEmpty(Stack *stack) {
    return stack->top == -1;
}

void shuntingYard(char *infix, char *output)
{
    Stack operatorStack;
    initializeStack(&operatorStack);

    int outputIndex = 0;

    for (int i = 0; infix[i] != '\0'; ++i)
    {
        char token = infix[i];

        if (isdigit(token) || token == '.')
        {
            // If the token is a number, add it to the output
            while (isdigit(token) || token == '.')
            {
                output[outputIndex++] = token;
                token = infix[++i];
            }
            output[outputIndex++] = ' '; // Add space to separate numbers
            --i;                         // Move back one step to reprocess the current non-number token
        }
        else if (isOperator(token))
        {
            // If the token is an operator, pop operators from the stack to the output until the stack is empty or the top operator has lower precedence
            while (!isEmpty(&operatorStack) && isOperator(peek(&operatorStack)) &&
                   getPrecedence(peek(&operatorStack)) >= getPrecedence(token))
            {
                output[outputIndex++] = pop(&operatorStack);
                output[outputIndex++] = ' ';
            }
            push(&operatorStack, token);
        }
        else if (token == '(')
        {
            // If the token is '(', push it onto the stack
            push(&operatorStack, token);
        }
        else if (token == ')')
        {
            // If the token is ')', pop operators from the stack to the output until a '(' is encountered
            while (!isEmpty(&operatorStack) && peek(&operatorStack) != '(')
            {
                output[outputIndex++] = pop(&operatorStack);
                output[outputIndex++] = ' ';
            }
            if (!isEmpty(&operatorStack) && peek(&operatorStack) == '(')
            {
                pop(&operatorStack); // Pop '(' from the stack, but do not add it to the output
            }
            else
            {
                printf("Error: Mismatched parentheses\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Pop any remaining operators from the stack to the output
    while (!isEmpty(&operatorStack))
    {
        if (peek(&operatorStack) == '(')
        {
            printf("Error: Mismatched parentheses\n");
            exit(EXIT_FAILURE);
        }
        output[outputIndex++] = pop(&operatorStack);
        output[outputIndex++] = ' ';
    }

    // Null-terminate the output string
    output[outputIndex] = '\0';
}
double evaluateRPN(char *expression) {
    Stack operandStack;
    initializeStack(&operandStack);

    char *token = strtok(expression, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // If the token is a number, push it onto the operand stack
            push(&operandStack, atof(token));
        } else if (isOperator(token[0])) {
            // If the token is an operator, pop the necessary operands from the stack, perform the operation, and push the result back onto the stack
            double operand2 = pop(&operandStack);
            double operand1 = pop(&operandStack);

            switch (token[0]) {
                case '+':
                    push(&operandStack, operand1 + operand2);
                    break;
                case '-':
                    push(&operandStack, operand1 - operand2);
                    break;
                case '*':
                    push(&operandStack, operand1 * operand2);
                    break;
                case '/':
                    if (operand2 != 0) {
                        push(&operandStack, operand1 / operand2);
                    } else {
                        printf("Error: Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case '^':
                    push(&operandStack, pow(operand1, operand2));
                    break;
                default:
                    printf("Error: Invalid operator\n");
                    exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " ");
    }

    // The final result should be at the top of the operand stack
    if (!isEmpty(&operandStack)) {
        return pop(&operandStack);
    } else {
        printf("Error: Empty operand stack\n");
        exit(EXIT_FAILURE);
    }
}

int isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '^';
}

int getPrecedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

int main(int argc, char* argv[]) {
    char infix[100];
    char postfix[100];
    Stack stack;

    // Input
    printf("Enter an infix expression: ");
    scanf("%s", infix);

    // Shunting Yard algorithm to convert infix to postfix
    shuntingYard(infix, postfix);

    // Output RPN expression
    printf("Postfix (RPN) expression: %s\n", postfix);

    // Evaluate RPN expression
    double result = evaluateRPN(postfix);
    printf("Result: %lf\n", result);

    return 0;
}
