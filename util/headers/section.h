#ifndef SECTION_H
#define SECTION_H

/*
 * Section
 *
 * A section of memory, used to keep track of next memory address.
 *
 * Used mainly by type Adapter.
 */

struct Section {
    void set(int a, int b, int c) {
        min = a;
        current = min;
        size = b;
        max = min + size - 1;
        type = c;
    }

    void setNextAddress(int i = 1) {
        if (current == max + 1) {
            cout<<"Memory limit exceeded"<<endl;
        }
        current += i;
    }

    int min;
    int current;
    int size;
    int max;
    int type;
};

#endif
