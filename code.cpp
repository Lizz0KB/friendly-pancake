#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 13





enum STATES{
	NO_ATTACK = 0, 
	UNDER_ATTACK = 1,
	KNIGHT = 2
};


struct cell {
	
	int attackers_id[9]; // array: id's of attakers cells
	uint8_t attackers_id_lenght;
	int k, K;
};

/*   GLOBAL VARIABLES   */
const int8_t PRIO[9] = {3, 2, 1, -1, -2, -3, -4, -5, -6};
uint32_t max = N * N;
struct cell board[N * N];
uint8_t sorted_id[N * N];
uint8_t* zobriss_table; 
uint32_t zobriss;


void set_attackability_map(int x, int y, int pos) {
	// what is x and y ? -> [coordX = cur_id % N] && [coordY = cur_id / N]
	// -> [1, 2] [2, 1] [-1, 2] [-2, 1] [1, -2] [2, -1] [-1, -2] [-2, -1]

	board[pos].attackers_id[(board[pos].attackers_id_lenght)++] = pos;


	if ((0 <= x + 1) && (x + 1 < N) && (0 <= y + 2) && (y + 2 < N)) 
		board[(x + 1) + N * (y + 2)].attackers_id[(board[(x + 1) + N * (y + 2)].attackers_id_lenght)++] = pos;
	if ((0 <= x + 2) && (x + 2 < N) && (0 <= y + 1) && (y + 1 < N)) 
		board[(x + 2) + N * (y + 1)].attackers_id[(board[(x + 2) + N * (y + 1)].attackers_id_lenght)++] = pos;
	if ((0 <= x - 1) && (x - 1 < N) && (0 <= y + 2) && (y + 2 < N)) 
		board[(x - 1) + N * (y + 2)].attackers_id[(board[(x - 1) + N * (y + 2)].attackers_id_lenght)++] = pos;
	if ((0 <= x - 2) && (x - 2 < N) && (0 <= y + 1) && (y + 1 < N)) 
		board[(x - 2) + N * (y + 1)].attackers_id[(board[(x - 2) + N * (y + 1)].attackers_id_lenght)++] = pos;
	if ((0 <= x + 1) && (x + 1 < N) && (0 <= y - 2) && (y - 2 < N)) 
		board[(x + 1) + N * (y - 2)].attackers_id[(board[(x + 1) + N * (y - 2)].attackers_id_lenght)++] = pos;
	if ((0 <= x + 2) && (x + 2 < N) && (0 <= y - 1) && (y - 1 < N)) 
		board[(x + 2) + N * (y - 1)].attackers_id[(board[(x + 2) + N * (y - 1)].attackers_id_lenght)++] = pos;
	if ((0 <= x - 1) && (x - 1 < N) && (0 <= y - 2) && (y - 2 < N)) 
		board[(x - 1) + N * (y - 2)].attackers_id[(board[(x - 1) + N * (y - 2)].attackers_id_lenght)++] = pos;
	if ((0 <= x - 2) && (x - 2 < N) && (0 <= y - 1) && (y - 1 < N)) 
		board[(x - 2) + N * (y - 1)].attackers_id[(board[(x - 2) + N * (y - 1)].attackers_id_lenght)++] = pos;


}


void qsort(int low, int high, int len) {
	
	if (high < low) return;
	
	int pivot = (board[sorted_id[(low + high) / 2]].attackers_id_lenght) * len + sorted_id[(low + high) / 2];
	int i = low, j = high;
	uint8_t temp;
	while (i <= j) {
		while ((board[sorted_id[i]].attackers_id_lenght) * len + sorted_id[i] < pivot) i++;
		while ((board[sorted_id[j]].attackers_id_lenght) * len + sorted_id[j] > pivot) j--;
		if (i <= j) {
			temp = sorted_id[i];
			sorted_id[i] = sorted_id[j];
			sorted_id[j] = temp;
			i++;
			j--;
		}
	}
	if (low < j) qsort(low, j, len);
	if (i < high) qsort(i, high, len);

}


void print_table() {
	int i, j;
	for (i = 0; i < N * N; ++i) {
		printf("Map_elem: %d, K: %d, k: %d", board[i].attackers_id_lenght, board[i].K, board[i].k);
		printf("array: [ ");
		for (j = 0; j < board[i].attackers_id_lenght; ++j) printf("%d ", board[i].attackers_id[j]);
		printf("]");
		printf("\n");
	}
}


int check(uint8_t* map) {

	int i;
	for (i = 0; i < N * N; ++i) if (map[i] == NO_ATTACK) return 0;
	return 1;

}


void hashing() {

	srand(time(NULL));
	int i, j, k, K;
	for (i = 0; i < N / 2; i++) {
		for (j = 0; j < N / 2; j++) {
			K = rand();
			k = rand();
			///* -> */board[N * i + j].id = (N)*i + j;
			board[N * i + j].k = k;
			board[N * i + j].K = K;
			//board[N * i + (N - j - 1)].id = (N)*i + (N - j - 1);
			board[N * i + (N - j - 1)].k = k;
			board[N * i + (N - j - 1)].K = K;
			//board[N * (N - i - 1) + j].id = (N)*i + (N - j - 1);
			board[N * (N - i - 1) + j].k = k;
			board[N * (N - i - 1) + j].K = K;
			//* -> */board[N * (N - i - 1) + (N - j - 1)].id = (N)*i + j;
			board[N * (N - i - 1) + (N - j - 1)].k = k;
			board[N * (N - i - 1) + (N - j - 1)].K = K;
		}
	}

	if (N % 2 == 1) {
		for (i = 0; i < N; ++i) {
            k = rand(), K = rand();
			board[i * N + (N / 2)].k = k;
			board[N * (N / 2) + i].k = k;
            board[i * N + (N / 2)].K = K;
			board[N * (N / 2) + i].K = K;
		}
		//board[N * (N / 2 + 1) + (N / 2 + 1)].k = rand();
	}
}



int permutation(uint8_t* map, int i, int knight_count){
    //printf("STATUS: %d\n", i);
    if(i >= N * N) {
       //printf("1");
        return -1;
    }
    if(knight_count >= max){
        //printf("2");
        return -1;
    }
        
    if(check(map) == 1){
		max = knight_count;
        printf("SUCCESS: %d \n", knight_count);
		int j;
		for(j = 0; j < N * N; ++j){
			printf("%d ", map[j]);
			if(j % N == (N - 1)) printf("\n");
    	}
        return 1;
    }
	if(map[sorted_id[i]] > 0) {
		permutation(map, i + 1, knight_count);
		return -1;
	}
    int8_t cur_max = -1, cur_sum;
    uint8_t cur_pos;
    uint8_t* res = (uint8_t*)malloc(sizeof(uint8_t) * board[sorted_id[i]].attackers_id_lenght);
    int j, k, s = 0;
    uint8_t map_cpy[N * N];
    //memcpy(map_cpy, map, N * N);


    
    for(j = 0; j < board[sorted_id[i]].attackers_id_lenght; j++){ // [0 10 17]
        cur_pos = board[sorted_id[i]].attackers_id[j];// 0
       // printf("\ncur pos:%d ", cur_pos);
        cur_sum = 0;
        for(k = 0; k < board[cur_pos].attackers_id_lenght; ++k){ // 0 10 17
            cur_sum += PRIO[map[board[cur_pos].attackers_id[k]]];
            //printf("cur sum: %d ", cur_sum);
             // 9 + 9 + 9
        }
        if(cur_sum > cur_max){
            cur_max = cur_sum;
            s = 0;
            memset(res, 0, board[sorted_id[i]].attackers_id_lenght);
            res[s++] = cur_pos;
        }
        else if(cur_sum = cur_max)
            res[s++] = cur_pos;
        
    }
    //printf("S = %d\n", s);
    uint8_t x, y;

    for(j = 0; j < s; ++j){
        //printf("REACH!1");
        memcpy(map_cpy, map, N * N);
        
        x = res[j] % N;
        y = res[j] / N;
        map_cpy[x + N * y] ++;
        if ((0 <= x + 1) && (x + 1 < N) && (0 <= y + 2) && (y + 2 < N)) 
		    map_cpy[(x + 1) + N * (y + 2)]++;
	    if ((0 <= x + 2) && (x + 2 < N) && (0 <= y + 1) && (y + 1 < N)) 
		    map_cpy[(x + 2) + N * (y + 1)]++;
	    if ((0 <= x - 1) && (x - 1 < N) && (0 <= y + 2) && (y + 2 < N)) 
		    map_cpy[(x - 1) + N * (y + 2)]++;
	    if ((0 <= x - 2) && (x - 2 < N) && (0 <= y + 1) && (y + 1 < N)) 
		    map_cpy[(x - 2) + N * (y + 1)]++;
	    if ((0 <= x + 1) && (x + 1 < N) && (0 <= y - 2) && (y - 2 < N)) 
	    	map_cpy[(x + 1) + N * (y - 2)]++;
	    if ((0 <= x + 2) && (x + 2 < N) && (0 <= y - 1) && (y - 1 < N)) 
		    map_cpy[(x + 2) + N * (y - 1)]++;
	    if ((0 <= x - 1) && (x - 1 < N) && (0 <= y - 2) && (y - 2 < N)) 
		    map_cpy[(x - 1) + N * (y - 2)]++;
	    if ((0 <= x - 2) && (x - 2 < N) && (0 <= y - 1) && (y - 1 < N)) 
		    map_cpy[(x - 2) + N * (y - 1)]++;
        //printf("REACH!2");
        permutation(map_cpy, i + 1, knight_count + 1);
  
    }
    
    return 1;

}


int main() {
	zobriss_table = (uint8_t*)calloc(INT_MAX, sizeof(uint8_t)); 
	memset(zobriss_table, 0, INT_MAX); // 0 - wasn't, 1 - was

	

	uint8_t map[N * N]; // map build for each iteration 
	memset(map, 0, N * N);
    


	int len = floor(log10(abs(N * N))) + 1;
	len = pow(10, len);

    zobriss = 0;

	int i, j;
	for (i = 0; i < N*N; ++i) {
        sorted_id[i] = i;
		board[i].attackers_id_lenght = 0;
	}
	
	

    for(i = 0; i < N * N; ++i) {
        set_attackability_map(i % N, i / N, i);
        
    }
    printf("Zobriss: %d\n", zobriss);

    qsort(0, N * N - 1, len);
    hashing();
     for(i = 0; i < N * N; ++i) {
        
        zobriss ^= board[i].k;
        printf("%d \n", zobriss);
    }
   	print_table();
	printf("\n\n\n");
    


    permutation(map, 0, 0);
	

	
}