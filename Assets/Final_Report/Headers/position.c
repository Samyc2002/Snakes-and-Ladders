#include <time.h>
#include "position.h"

int search(int n, int arr[], int len);

int generate(int upper, int lower) {
    return ((rand() % (upper - lower +1)) + lower);
}

int long_snakes;
int short_snakes;

int* position_snakes() {

    srand(time(0));

    int long_snakes_length = generate(20, 15);
    int short_snakes_length = generate(15, 10);
    long_snakes = generate(20, 15);
    short_snakes = generate(10, 5);
    int long_snakes_pos[long_snakes];
    int short_snakes_pos[short_snakes];
    long_snakes_pos[0] = generate(99, 11);
    short_snakes_pos[0] = generate(99, 11);

    while(short_snakes_pos[0] == long_snakes_pos[0]) {

        short_snakes_pos[0] = generate(99, 11);
    }

    int* store = (int*)malloc((long_snakes+short_snakes)*sizeof(int));
    store[0] = long_snakes_pos[0];
    store[1] = short_snakes_pos[0];

    int i=1;

    for(int j=2; j<long_snakes; j++) {

        int x = generate(99, 11);
        while(search(x, store, j)) {
            x = generate(99, 11);
        }
        long_snakes_pos[i] = x;
        store[j] = x;
        i++;
    }

    i=1;

    for(int j=2+long_snakes; j<=(long_snakes+short_snakes); j++) {

        int x = generate(99, 11);
        while(search(x, store, j)) {
            x = generate(99, 11);
        }
        short_snakes_pos[i] = x;
        store[j] = x;
        i++;
    }

    FILE* snakes;
    snakes = fopen("Snakes.txt", "w");

    fprintf(snakes, "%d %d\n%d %d\n", long_snakes, short_snakes, long_snakes_length, short_snakes_length);

    for(int i=0; i<long_snakes; i++) {

        fprintf(snakes, "%d ", long_snakes_pos[i]);
    }

    fprintf(snakes, "\n");

    for(int i=0; i<short_snakes; i++) {

        fprintf(snakes, "%d ", short_snakes_pos[i]);
    }

    fclose(snakes);

    return store;
}

void position_ladders(int* array) {

    srand(time(0));

    int long_ladders_length = generate(15, 10);
    int short_ladders_length = generate(10, 5);
    int long_ladders = generate(15, 10);
    int short_ladders = generate(10, 6);
    int long_ladders_pos[long_ladders];
    int short_ladders_pos[short_ladders];
    long_ladders_pos[0] = generate(90, 2);
    short_ladders_pos[0] = generate(90, 2);

    while(short_ladders_pos[0] == long_ladders_pos[0]) {

        short_ladders_pos[0] = generate(90, 2);
    }

    int n = long_snakes+short_snakes;
    int store[n+long_ladders+short_ladders];
    for(int i=0; i<n; i++) {
        store[i] = array[i];
    }
    store[n] = long_ladders_pos[0];
    store[n+1] = short_ladders_pos[0];

    int i=1;

    for(int j=2+n; j<long_ladders+n; j++) {

        int x = generate(90, 2);
        while(search(x, store, j)) {
            x = generate(90, 2);
        }
        long_ladders_pos[i] = x;
        store[j] = x;
        i++;
    }

    i=1;

    for(int j=2+n+long_ladders; j<=(n+long_ladders+short_ladders); j++) {

        int x = generate(90, 2);
        while(search(x, store, j)) {
            x = generate(90, 2);
        }
        short_ladders_pos[i] = x;
        store[j] = x;
        i++;
    }

    FILE* ladders;
    ladders = fopen("Ladders.txt", "w");

    fprintf(ladders, "%d %d\n%d %d\n", long_ladders, short_ladders, long_ladders_length, short_ladders_length);

    for(int i=0; i<long_ladders; i++) {

        fprintf(ladders, "%d ", long_ladders_pos[i]);
    }

    fprintf(ladders, "\n");

    for(int i=0; i<short_ladders; i++) {
        
        fprintf(ladders, "%d ", short_ladders_pos[i]);
    }

    fclose(ladders);
}

void position() {

    int* array = position_snakes();
    position_ladders(array);
}

int search(int n, int arr[], int len) {
    for(int i=0; i<len; i++) {
        if(arr[i] == n) {
            return 1;
        }
    }
    return 0;
}