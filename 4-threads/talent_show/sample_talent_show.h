#ifndef PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_
#define PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_

#include "audience.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_NUM_JURORS 10

typedef bool(*Evaluator)(int participant_id, int juror_id);

typedef struct {
  int id;
  void* jury;
  // Add mutexes and semaphores if needed.
  //signal for new participant
  sem_t *new_participant_signal;
} JurorArgs;

typedef struct {
  Audience* audience;
  Evaluator evaluator;
  int num_jurors;
  JurorArgs jurors[MAX_NUM_JURORS];
  int num_participants;
  int cur_participant; 
  bool take_no_new_participants; //critical region
  int num_yes; //critical region

  // Add mutexes and semaphores if needed.
  pthread_mutex_t *no_new_patricipants_lock;
  pthread_mutex_t *num_yes_lock;
  pthread_mutex_t *num_participants_lock;

  //0->num_jurors
  sem_t *jury_greet_signal; 
  //semaphore wait for all jury to evaluate
  sem_t *juror_evaluated_signal;
  //semaphore wait for all jury to finish
  sem_t *juror_done_signal; //0->num_jurors;
  //new participant available
  sem_t *new_participant_signal;
  //wait until all are done
  sem_t *all_participant_done_signal;  
} Jury;

void JuryInit(Jury* jury, Audience* audience, Evaluator evaluator,
	      int num_jurors);
void JuryDestroy(Jury* jury);
void JuryStartAcceptingParticipants(Jury* jury);
bool JuryTakeNewParticipant(Jury* jury);
void JuryStopAcceptingParticipants(Jury* jury);

#endif // PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_
