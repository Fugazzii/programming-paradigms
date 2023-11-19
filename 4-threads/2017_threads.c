#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

using namespace std;

#define NUM_CUSTOMERS 20

#define NUM_OPERATORS 4

int customersLeft = NUM_CUSTOMERS;
typedef struct 
{
    int line_num;
    sem_t lock;
}Line;
typedef struct 
{
    sem_t requested; // 0
    sem_t customers[NUM_CUSTOMERS];
}OperatorCheckOut;

Line l;
OperatorCheckOut ops;
sem_t lock_left;
int answers[NUM_CUSTOMERS];
int cust2oper[NUM_CUSTOMERS]; //id's
sem_t operators[NUM_OPERATORS];


void init_locks(){
    l.line_num = 0;
    sem_init(&l.lock, 0, 1);
    sem_init(ops.requested, 0, 0);
    for(int i = 0; i < NUM_CUSTOMERS; i++){
        sem_init(ops.customers[i], 0, 0);
    }
    for(int i = 0; i < NUM_OPERATORS; i++){
        sem_init(&operators[i], 0, 0);
    }
    sem_init(&lock_left, 0, 1);

}

void Customer(void *args) {
	//TODO: რიგში ჩადგომა და ლოდინი სანამ რომელიმე ოპერატორი გამოიძახებს
    sem_wait(&l.lock);
    int num = l.line_num;
    l.line_num++;
    sem_post(&l.lock);

    sem_post(&ops.requested);
    sem_wait(&ops.customers[num]);
    int op_id = cust2oper[num];
	fillApplication();    
	//TODO: ელოდება სანამ ოპერატორი გადაამოწმებს მონაცემებს
    sem_post(&operators[op_id]);
    sem_wait(&ops.customers[num]);
	//TODO: ბეჭდავს პასუხს(Yes/No)
    int ans = answers[num];
    if(ans){
        printf("Yes\n");
    }else{
        printf("No\n");
    }
    return NULL;
}

void Operator(void *args) {
    int id = *(int *)args;
    int approved = 0;
    int id = 0;
    int i = 0;
	while (true) {
        sem_wait(&lock_left);
        if(customersLeft == 0){
            sem_post(&lock_left);            
            break;
        }
        int currNum = NUM_CUSTOMERS-customersLeft;
		//TODO: ელოდება სანამ მომხმარებელი ჩადგება რიგში
        sem_wait(&ops.requested);
		//TODO: მომხმარებლის გამოძახება და ლოდინი სანამ მომხმარებელი შეავსებს ფორმას
        cust2oper[currNum] = id;
        sem_post(&ops.customers[currNum]);
        sem_wait(&operators[id]);
		int res = checkApplication();//აბრუნებს შედეგს - 1 თანხმობა, 0 უარყოფა 
        approved += res;
		//TODO: მომხმარებლისთვის პასუხის დაბრუნება
        answers[currNum] = res;
        sem_post(&ops.customers[currNum]);
        customersLeft--;
        sem_post(&lock_left);            
	}
	//TODO: ბეჭდავს მის მიერ დამტკიცებული სესხების რაოდენობიის
    printf("Approved: %d\n", approved);
    return NULL;

}

int main() {
	//მომხმარებლების ნაკადების გაშვება
	for (int i=0; i<NUM_CUSTOMERS; i++) {
		pthread_t * t = malloc(sizeof(pthread_t));
		pthread_create(t, NULL, Customer, NULL);
	}

	//ოპერატორების ნაკადების გაშვება
	for (int i=0; i<NUM_OPERATORS; i++) {
		pthread_t * t = malloc(sizeof(pthread_t));
		pthread_create(t, NULL, Operator, NULL);
	}

	pthread_exit(0);
}