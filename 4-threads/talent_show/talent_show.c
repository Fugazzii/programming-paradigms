#include "talent_show.h"

#include <stdlib.h>

void* JurorStart(void* args) {
  JurorArgs* juror = args;
  Jury* jury = juror->jury;
  AudienceJurorGreet(jury->audience, juror->id);
  for (int cur_participant = 0; ; ++cur_participant) {
    sem_wait(&juror->new_participant_available);
    pthread_mutex_lock(&jury->participants_lock);
    if (jury->take_no_new_participants &&
	cur_participant == jury->num_participants) {
      pthread_mutex_unlock(&jury->participants_lock);
      break;
    }
    pthread_mutex_unlock(&jury->participants_lock);
    bool yes = jury->evaluator(juror->id, cur_participant);
    AudienceJurorEvaluatedParticipant(jury->audience, juror->id,
				      cur_participant, yes);
    pthread_mutex_lock(&jury->num_yes_lock);
    jury->num_yes += yes;
    pthread_mutex_unlock(&jury->num_yes_lock);
    sem_post(&jury->juror_evaluated);
  }
  AudienceJurorDone(jury->audience, juror->id);
  sem_post(&jury->juror_done);
  return NULL;
}

void* JuryStart(void* args) {
  Jury* jury = args;
  AudienceJuryGreet(jury->audience);
  for (int cur_participant = 0; ; ++cur_participant) {
    sem_wait(&jury->new_participant_available);
    jury->num_yes = 0;
    for (int i = 0; i < jury->num_jurors; ++i) {
      sem_post(&jury->jurors[i].new_participant_available);
    }
    pthread_mutex_lock(&jury->participants_lock);
    if (jury->take_no_new_participants &&
	cur_participant == jury->num_participants) {
      pthread_mutex_unlock(&jury->participants_lock);
      break;
    }
    pthread_mutex_unlock(&jury->participants_lock);
    AudienceJuryAnnounceNewParticipant(jury->audience, cur_participant);
    for (int i = 0; i < jury->num_jurors; ++i) {
      sem_wait(&jury->juror_evaluated);
    }
    AudienceJuryAnnounceParticipantScore(jury->audience, cur_participant,
					 jury->num_yes);
  }
  for (int i = 0; i < jury->num_jurors; ++i) {
    sem_wait(&jury->juror_done);
  }
  AudienceJuryDone(jury->audience);
  sem_post(&jury->done);
  return NULL;
}

void JurorInit(JurorArgs* juror, int id, Jury* jury) {
  juror->id = id;
  juror->jury = jury;
  sem_init(&juror->new_participant_available, 0, 0);
}

void JurorDestroy(JurorArgs* juror) {
  sem_destroy(&juror->new_participant_available);
}

void JuryInit(Jury* jury, Audience* audience, Evaluator evaluator,
	      int num_jurors) {
  jury->audience = audience;
  jury->evaluator = evaluator;
  jury->num_jurors = num_jurors;
  jury->num_participants = 0;
  jury->take_no_new_participants = false;
  // Initialize mutexes and semaphores if needed.
  pthread_mutex_init(&jury->participants_lock, NULL);
  pthread_mutex_init(&jury->num_yes_lock, NULL);
  sem_init(&jury->new_participant_available, 0, 0);
  sem_init(&jury->juror_evaluated, 0, 0);
  sem_init(&jury->juror_done, 0, 0);
  sem_init(&jury->done, 0, 0);  
}

void JuryDestroy(Jury* jury) {
  // Destroy mutexes and semaphores if needed.
  pthread_mutex_destroy(&jury->participants_lock);
  pthread_mutex_destroy(&jury->num_yes_lock);
  sem_destroy(&jury->new_participant_available);
  sem_destroy(&jury->juror_evaluated);
  sem_destroy(&jury->juror_done);
  sem_destroy(&jury->done);
  for (int i = 0; i < jury->num_jurors; ++i) {
    JurorDestroy(&jury->jurors[i]);
  }
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
  pthread_mutex_lock(&jury->participants_lock);
  if (jury->take_no_new_participants) {
    pthread_mutex_unlock(&jury->participants_lock);
    return false;
  }
  ++jury->num_participants;
  pthread_mutex_unlock(&jury->participants_lock);
  sem_post(&jury->new_participant_available);
  return true;
}

void JuryStopAcceptingParticipants(Jury* jury) {
  pthread_mutex_lock(&jury->participants_lock);
  jury->take_no_new_participants = true;
  pthread_mutex_unlock(&jury->participants_lock);
  sem_post(&jury->new_participant_available);
  sem_wait(&jury->done);
}
