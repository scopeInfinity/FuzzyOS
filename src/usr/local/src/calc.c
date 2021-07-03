// Simple Calculator
#include <stdio.h>
#include <stdlib.h>

#define BUILD_FOR_FUZZY
#ifndef BUILD_FOR_FUZZY
//#include<stdio.h>
void read_line(char *s) {
    scanf("%[^\n]",s);;
}
void print_line(char *s) {
    printf("%s", s);
}
void print_int(int x) {
    printf("%d", x);
}
#else
#include <lib/utils/input.h>
#include <lib/utils/output.h>
#include <lib/utils/time.h>
#include <lib/utils/string.h>
#endif

int err;
char tokens[2][20];
int a,b;
char op;
int t;
int tl;
int solve(char s[]) {
    t=0;
    tl=0;
    op = 0;
    for(int i=0;s[i]!='\0';i++) {
        if(s[i]>='0' && s[i]<='9') {
            if(t>=2) {
                err = 1;
                return 0;
            }
            tokens[t][tl]=s[i];
            tl++;
        } else {
            if(op!=0) {
                err = 2;
                return 0;
            }
            op=s[i];
            tokens[t][tl]='\0';
            tl=0;
            t++;
        }
    }
    tokens[t][tl]='\0';
    tl=0;
    t++;
    if(t!=2) {
        err = 3;
        return 0;
    }
    a = atoi(tokens[0]);
    b = atoi(tokens[1]);
    switch(op) {
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '*':
            return a*b;
        case '/':
            return a/b;
        default:
            err = 4;
    }
    return 0;
}

void show_usage() {
    print_line("  Usage: \n");
    print_line("    HELP - Show Usage\n");
    print_line("    EXIT - To quit Program\n");
    print_line("\n");
    print_line("    <num><op><num> to calculate expression.\n");
    print_line("    Where <op> can be one of +-*/.\n");
}
char expr[100];
int result;

int handle_expression(char str[]) {
    if(strcmpi(str, "HELP")==0) {
        show_usage();
        return 1;
    }
    if(strcmpi(str, "EXIT")==0) {
        return 0;
    }
    result = solve(str);
    if(err) {
        print_line("  Error[");
        print_int(err);
        print_line("]: Only '<num> [+-*/] <num>' syntax supported!\n");
        print_line("  Type 'HELP' for instructions!\n\n");
    } else {
        print_line("  Result: ");
        print_int(result);
        print_line("\n\n");
    }
    return 1;
}

void console_init() {
    set_color_bg(C_BLACK);
    set_color_fg(C_WHITE);
    print_rectangle(0, 0, TEXT_WINDOW_WIDTH-1, TEXT_WINDOW_HEIGHT-1);
    move_xy(0,0);
}

int main(int argc,char *argv[]) {
    console_init();
    print_line("Simple Calculator\n");
    print_line("-----------------\n");
    print_line("\n");
    show_usage();
    while(1) {
        err = 0;
        print_line("Expression: ");
        read_line(expr);
        if(!handle_expression(expr)) {
            break;
        }
    }
    return 0;
}