int test () {
    int z=1;
    return z *2;
}
extern int _low_print();
void entry_stage2() {
    int status = _low_print();
}