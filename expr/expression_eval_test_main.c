#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "string_util.h"

#define MAX_INPUT_SIZE 100

typedef struct {
  int buffer[50];
  int top;
} pstack_t;

void push(pstack_t*, int);
int pop(pstack_t*);

void parse_expression(char* buf);
int parse_number(char* buf, int* num_len);
int reverse_number(int num);

int precedence(char);
int is_operator(char);
char* infix_to_postfix(char*);

int main() {
  char* s = "524 ";
  int v = str_to_int(s);
  printf("v=%d\n", v);

  // pstack_t s = {0};
  // push(&s, 5);
  // push(&s, 2);
  // push(&s, pop(&s) + pop(&s));

  // char user_input[MAX_INPUT_SIZE] = {0};
  // pprintf("%s\n", postfix);
  // parse_expression(postfix);
  // free(postfix);

  char* test_postfix = "5020 1 4 + -";
  printf("%s\n", test_postfix);
  parse_expression(test_postfix);
}

void push(pstack_t* s, int value) {
  // printf("Pushed %d\n", value);
  s->buffer[s->top++] = value;
}

int peek(pstack_t* s) {
  if (s->top - 1 < 0) {
    // printf("Popping from empty stack not allowed\n");
    exit(1);
  }
  return s->buffer[s->top - 1];
}

int pop(pstack_t* s) {
  int x = peek(s);
  // printf("Popped %d\n", x);
  s->top--;
  return x;
}

void parse_expression(char* buf) {
  pstack_t s = {0};
  while (*buf) {
    if (*buf == ' ') {
      buf++;
    }
    if (isdigit(*buf)) {
      int length = 0;
      int number = parse_number(buf, &length);
      buf += length;
      push(&s, number);
      // printf("%d", peek(&s));
    }
    switch (*buf) {
    case '+': {
      int v1 = pop(&s);
      int v2 = pop(&s);
      // printf("Adding %d and %d\n", v1, v2);
      push(&s, v1 + v2);
      break;
    }
    case '-': { 
      int v1 = pop(&s);
      int v2 = pop(&s);
      push(&s, v2 - v1);
      break;
    }
    case '*': {
      int v1 = pop(&s);
      int v2 = pop(&s);
      push(&s, v1 * v2); 
      break;
    }
    case '/': {
      int v1 = pop(&s);
      int v2 = pop(&s);
      push(&s, v2 / v1); 
      break;
    }
    }
    buf++;
  }
  printf("Result: %d\n", peek(&s));
}

int parse_number(char* buf, int* number_len) {
  int value = 0, digits = 0;
  char* cursor = buf;
  while (isdigit(*cursor) != 0) {
    value += ((*cursor) - '0') * pow(10, digits);
    digits++;
    cursor++;
    printf("%c ", *cursor);
  }
  *number_len = digits;
  return reverse_number(value);
}

int reverse_number(int num) {
  int rev = 0;
  while (num > 0) {
    rev = rev * 10 + num % 10;
    num /= 10;
  }
  return rev;
}

int precedence(char c) {
  switch (c) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    default:
      return -1;
  }
}

int is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/';
}

char* infix_to_postfix(char* infix) {
  int infix_length = strlen(infix);
  char* postfix = malloc(infix_length + 2);
  char stack[100];
  int top = -1;

  int i, j;
  for (i = 0, j = 0; i < infix_length; i++) {
    // skip whitespace
    if (infix[i] == ' ' || infix[i] == '\t')
      continue;
    while (isalnum(infix[i])) {
      postfix[j++] = infix[i];
      i++;
    }
    postfix[j++] = ' ';
    if (infix[i] == '(') {
      stack[++top] = infix[i];
    }
    else if (infix[i] == ')') {
      while (top > -1 && stack[top] != '(') {
        postfix[j++] = stack[top--];
        postfix[j++] = ' ';
      }
      if (top > -1 && stack[top] != '(') {
        return "Invalid expression";
      }
      else {
        top--;
      }
    }
    else if (is_operator(infix[i])) {
      while (top > -1 && precedence(stack[top]) >= precedence(infix[i])) {
        postfix[j++] = stack[top--];
        postfix[j++] = ' ';
      } 
      stack[++top] = infix[i];
    }
  }
 
  while (top > -1) {
    if (stack[top] == '(') {
      return "Invalid expression";
    }
    postfix[j++] = stack[top--];
  }
  postfix[j] = '\0';
  return postfix;
}
