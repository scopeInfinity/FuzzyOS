// Simple Calculator
#define BUILD_FOR_FUZZY
#ifndef BUILD_FOR_FUZZY
#include<stdio.h>
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
#include <lib/syscalls/io.h>
#include <lib/syscalls/time.h>
#include <lib/util/string.h>
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
    a = parse_int(tokens[0]);
    b = parse_int(tokens[1]);
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
    if(strcmp(str, "HELP")==0) {
        show_usage();
        return 1;
    }
    if(strcmp(str, "EXIT")==0) {
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

int calc_main(int argc,char **argv) {
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