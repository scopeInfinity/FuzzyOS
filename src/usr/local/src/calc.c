// Simple Calculator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int err;
char tokens[2][20];
int a,b;
char op;
int t;
int tl;

int _print_int(int x) {
    int is_negative = 0;
    int tailing_zero = 0;
    if(x<0) {
        is_negative = 1;
        x=-x;
    }
    int rev = 0;
    while(x && x%10==0) {
        x/=10;
        tailing_zero++;
    }
    if(x==0) {
        // If the initial number is zero, add one here.
        tailing_zero++;
    }
    while(x) {
        rev = rev*10 + x%10;
        x/=10;
    }
    if(is_negative) {
        putchar('-');
    }
    while(rev) {
        putchar((char)(rev%10+'0'));
        rev/=10;
    }
    while(tailing_zero) {
        putchar('0');
        tailing_zero--;
    }
}

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
    puts("  Usage: \n");
    puts("    HELP - Show Usage\n");
    puts("    EXIT - To quit Program\n");
    puts("\n");
    puts("    <num><op><num> to calculate expression.\n");
    puts("    Where <op> can be one of +-*/.\n");
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
        puts("  Error[");
        _print_int(err);
        puts("]: Only '<num> [+-*/] <num>' syntax supported!\n");
        puts("  Type 'HELP' for instructions!\n\n");
    } else {
        puts("  Result: ");
        _print_int(result);
        puts("\n\n");
    }
    return 1;
}


int main(int argc,char *argv[]) {
    puts("Simple Calculator\n");
    puts("-----------------\n");
    puts("\n");
    show_usage();
    while(1) {
        err = 0;
        puts("Expression: ");
        gets(expr);

        if(!handle_expression(expr)) {
            break;
        }
    }
    return 0;
}