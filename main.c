//Gil ben hamo

#include "funcs.c"

int main(int argc, char **argv)
{
    char *inp_file, *out_file;
    int m,n,algo;

    inp_file = argv[INPUT_INDEX];
    out_file = argv[OUTPUT_INDEX];
    n = atoi(argv[N_INDEX]);
    m = atoi(argv[M_INDEX]);
    pf_algorithm = atoi(argv[ALGO_INDEX])==1 ? LRU : SCFIFO;
    
    if(pf_algorithm==0)
        printf("~ Runnig Memory simulation with SCFIFO Page fault algorithm ~\n");
    else
        printf("~ Runnig Memory simulation with LRU Page fault algorithm ~\n");

    HD_SIZE=m;
    HD = BuildSecondaryMemo();
    RAM = BuildQueue(n);
    initMainMemo();
    readFromFile(inp_file,out_file);

    FreeMemo();
    printf("~ End memory simulation - Export data to file: %s ~\n",out_file);

    return 0;
}
