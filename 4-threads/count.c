#define NUM_COUNTERS 4
#define NUM_TEXTS 10

sem_t counters[NUM_COUNTERS];
sem_t done;

int CountSymbol(char* start, int len, char c);

void* Counter(void* arg) {
  int id = *(int*)arg;
  for (int i = 0; i < NUM_TEXTS; ++i) {
    sem_wait(&counters[id]);
    int begin = strlen(text[i]) * i;
    int end = std::min(strlen(text[i]),
		       strlen(text[i]) * (i + 1));
    int count = CountSymbol(text[i] + start, end - begin, 'a');
    mutex_lock(&lock);
    total_count += count;
    mutex_unlock(&lock);
    sem_post(&done);
  }
}

void* Logger(void*  arg) {
  for (int i = 0; i < NUM_TEXTS; ++i)  {
    total_count = 0;
    for (int j = 0; j < NUM_COUNTERS; ++j) {
      sem_post(&counters[j]);
    }
    printf("Text %d : Count %d", i, total_count);
  }
}

int main() {
  int id[NUM_COUNTERS];
  pthread_t tid;  
  for (int i = 0l; i < NUM_COUNTERS; ++i) {
    id[i] = i;
    pthread_create(&tid, Counter, NULL, &id[i]);
  }
  pthread_create(&tid, Logger, NULL, NULL);
  pthread_exit(NULL);
}

