#define NUM_TEXTS 100
#define NUM_COUNTERS 4

//გადაუყვება start მისამართიდან len ცალ სიმბოლოს და ითვლის თუ რამდენჯერ შეხვდა სიმბოლო c
size_t countSymbol(char * start, size_t len, char c);


char * texts[NUM_TEXTS];


void * Counter(void * d) {
	int id = *(int *)d;
	
}

void * Logger(void * d) {
	
}


int main() {
	pthread_t threads[NUM_COUNTERS];
	index = 0;
	for (int i = 0; i<NUM_COUNTERS; i++) {
		pthread_create(threads+i, NULL, Counter, &i);
	}

	pthread_create(threads+i, NULL, Logger, NULL);

	pthread_exit(0);
	return 0;
}