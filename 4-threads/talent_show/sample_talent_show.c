#include "sample_talent_show.h"

#include <stdlib.h>

void* JurorStart(void* args) {
  JurorArgs* juror = args;
  Jury* jury = juror->jury;
  // Fill in.
  //wait for jury greetings
  sem_wait(jury->jury_greet_signal);
  AudienceJurorGreet(jury->audience, juror->id);
  while(true){
    sem_wait(juror->new_participant_signal);

    pthread_mutex_lock(jury->num_participants_lock);
    if(jury->num_participants == 0){
      pthread_mutex_unlock(jury->num_participants_lock);
      break;
    }
    pthread_mutex_unlock(jury->num_participants_lock);

    bool res = jury->evaluator(juror->id, jury->cur_participant);

    AudienceJurorEvaluatedParticipant(jury->audience, juror->id, jury->cur_participant, res);
    if(res){
      pthread_mutex_lock(jury->num_yes_lock);
      jury->num_yes++;
      pthread_mutex_unlock(jury->num_yes_lock);
    }
    sem_post(jury->juror_evaluated_signal);

  }
  AudienceJurorDone(jury->audience, juror->id);
  sem_post(jury->juror_done_signal);
  return NULL;
}

void* JuryStart(void* args) {
  Jury* jury = args;
  // Fill in.
  AudienceJuryGreet(jury->audience);

  for(int i = 0; i < jury->num_jurors; i++)
    sem_post(jury->jury_greet_signal);
    
  while(true){
    //take participants
    sem_wait(jury->new_participant_signal);
    //check if someone left, signal jurors
    for(int i = 0; i < jury->num_jurors; i++){
       sem_post(jury->jurors[i].new_participant_signal);
    }
    pthread_mutex_lock(jury->num_participants_lock);
    if(jury->num_participants == 0){
      pthread_mutex_unlock(jury->num_participants_lock);

      break;
    }
    pthread_mutex_unlock(jury->num_participants_lock);
    AudienceJuryAnnounceNewParticipant(jury->audience, jury->cur_participant);
    //wait for jurors judgement
    for(int i = 0; i < jury->num_jurors; i++)
      sem_wait(jury->juror_evaluated_signal); 
    //anonunce results
    AudienceJuryAnnounceParticipantScore(jury->audience, jury->cur_participant, jury->num_yes);
    jury->num_participants--;   
    jury->cur_participant++;
  }
  //wait for jurors
  for(int i = 0; i < jury->num_jurors; i++)
    sem_wait(jury->juror_done_signal);

  AudienceJuryDone(jury->audience);  
  sem_post(jury->all_participant_done_signal);
  return NULL;
}

void JurorInit(JurorArgs* juror, int id, Jury* jury) {
  juror->id = id;
  juror->jury = jury;
  // Initialize mutexes and semaphores if needed.
  juror->new_participant_signal = malloc(sizeof(sem_t));
  sem_init(juror->new_participant_signal, 0, 0);
}

void JurorDestroy(JurorArgs* juror) {
  // Destroy mutexes and semaphores if needed.
  sem_destroy(juror->new_participant_signal);
  free(juror->new_participant_signal);
}

void JuryInit(Jury* jury, Audience* audience, Evaluator evaluator,
	      int num_jurors) {
  jury->audience = audience;
  jury->evaluator = evaluator;
  jury->num_jurors = num_jurors;
  jury->num_participant = 0;
  jury->cur_participant = 0;
  jury->take_no_new_participants = false;
  // Initialize mutexes and semaphores if needed.
  jury->no_new_patricipants_lock = malloc(sizeof(pthread_mutex_t));
  jury->num_yes_lock = malloc(sizeof(pthread_mutex_t));
  jury->num_participants_lock = malloc(sizeof(pthread_mutex_t));

  jury->jury_greet_signal = malloc(sizeof(sem_t));
  jury->juror_evaluated_signal = malloc(sizeof(sem_t));
  jury->juror_done_signal = malloc(sizeof(sem_t));
  jury->new_participant_signal = malloc(sizeof(sem_t));
  jury->all_participant_done_signal = malloc(sizeof(sem_t));

  pthread_mutex_init(jury->no_new_patricipants_lock, NULL);
  pthread_mutex_init(jury->num_yes_lock, NULL);
  pthread_mutex_init(jury->num_participants_lock, NULL);

  sem_init(jury->jury_greet_signal, 0, 0);
  sem_init(jury->juror_evaluated_signal, 0, 0);
  sem_init(jury->juror_done_signal, 0, 0);
  sem_init(jury->new_participant_signal, 0, 0);
  sem_init(jury->all_participant_done_signal, 0, 0);

}

void JuryDestroy(Jury* jury) {
  for (int i = 0; i < jury->num_jurors; ++i) {
    JurorDestroy(&jury->jurors[i]);
  }
  // Destroy mutexes and semaphores if needed.
  pthread_mutex_destroy(jury->no_new_patricipants_lock);
  pthread_mutex_destroy(jury->num_yes_lock);
  pthread_mutex_destroy(jury->num_participants_lock);

  sem_destroy(jury->jury_greet_signal);
  sem_destroy(jury->juror_evaluated_signal);
  sem_destroy(jury->juror_done_signal);
  sem_destroy(jury->new_participant_signal);
  sem_destroy(jury->all_participant_done_signal);

  free(jury->no_new_patricipants_lock);
  free(jury->num_yes_lock);
  free(jury->num_participants_lock);
  free(jury->jury_greet_signal);
  free(jury->juror_evaluated_signal);
  free(jury->juror_done_signal);
  free(jury->new_participant_signal);
  free(jury->all_participant_done_signal);

}

void JuryStartAcceptingParticipants(Jury* jury) {
  pthread_t tid;
  for (int i = 0; i < jury->num_jurors; ++i) {
    JurorInit(&jury->jurors[i], i, jury);
    pthread_create(&tid, NULL, JurorStart, &jury->jurors[i]);
    pthread_detach(tid);
  }
  pthread_create(&tid, NULL, JuryStart, jury);
  pthread_detach(tid);
}

bool JuryTakeNewParticipant(Jury* jury) {
  // Fill in.
  pthread_mutex_lock(jury->num_participants_lock);
  if(jury->take_no_new_participants){
    pthread_mutex_unlock(jury->num_participants_lock);
    return false;
  }
  jury->num_participants++;
  pthread_mutex_unlock(jury->num_participants_lock);
  sem_post(jury->new_participant_signal);
  return true;
}

void JuryStopAcceptingParticipants(Jury* jury) {
  // Fill in. 
  pthread_mutex_lock(jury->num_participants_lock);
  jury->take_no_new_participants = true;
  pthread_mutex_unlock(jury->num_participants_lock);
  
  sem_post(jury->new_participant_signal);
  sem_wait(jury->all_participant_done_signal);
}
