#ifndef PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_
#define PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_

#include "audience.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_NUM_JURORS 10

typedef bool(*Evaluator)(int participant_id, int juror_id);

void* JurorStart(void* args);
void* JuryStart(void* args);

typedef struct {
  int id;
  void* jury;
  sem_t new_participant_available;
} JurorArgs;

typedef struct {
  Audience* audience;
  Evaluator evaluator;
  int num_jurors;
  JurorArgs jurors[MAX_NUM_JURORS];
  int num_participants;
  int cur_participant;
  bool take_no_new_participants;
  int num_yes;

  // Add mutexes and semaphores if needed.
  pthread_mutex_t participants_lock;  // REMOVE
  sem_t new_participant_available;  // REMOVE
  sem_t juror_evaluated;  // REMOVE  
  sem_t juror_done;  // REMOVE
  sem_t done;  // REMOVE
  pthread_mutex_t num_yes_lock;  // REMOVE
} Jury;

void JurorInit(JurorArgs* juror, int id, Jury* jury);
void JurorDestroy(JurorArgs* juror);

void JuryInit(Jury* jury, Audience* audience, Evaluator evaluator,
	      int num_jurors);
void JuryDestroy(Jury* jury);
void JuryStartAcceptingParticipants(Jury* jury);
bool JuryTakeNewParticipant(Jury* jury);
void JuryStopAcceptingParticipants(Jury* jury);

#endif // PARADIGMS_PROBLEMS_TALENT_SHOW_TALENT_SHOW_H_
