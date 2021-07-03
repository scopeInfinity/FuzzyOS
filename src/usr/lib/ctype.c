#include<ctype.h>

int tolower(int x) {
    if(x>='A' && x<='Z') {
        return x-'A'+'a';
    }
    return x;
}