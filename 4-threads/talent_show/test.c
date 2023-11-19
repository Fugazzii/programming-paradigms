// Note tests do not check the order in which functions are called.
// On these sample tests we only evaluate final scores.

#include "talent_show.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef bool(*Test)();

void CheckTrue(bool b) {
  if (!b) {
    printf("JuryTakeNewParticipant returned false instead of true\n");
    exit(1);
  }
}

bool RandEvaluator(int participant_id, int juror_id) {
  sleep(rand() % 3);
  return rand() % 2;
}

bool AlwaysNo(int participant_id, int juror_id) {
  return false;
}

bool AlwaysYes(int participant_id, int juror_id) {
  return true;
}

void RunTest(char* name, Test t) {
  printf("RUNNING TEST: %s\n", name);
  if (t()) {
    printf("TEST %s SUCCEEDED\n", name);
  } else {
    printf("TEST %s FAILED\n", name);
  }
}

void* Stop(void* args) {
  JuryStopAcceptingParticipants((Jury*)args);
  return NULL;
}

bool DoNotAcceptAny() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 3);
  JuryStartAcceptingParticipants(&jury);
  pthread_t tid;
  pthread_create(&tid, NULL, Stop, &jury);
  sleep(3);
  bool failed = false;
  for (int i = 0; i < 10; ++i) {
    if (JuryTakeNewParticipant(&jury)) {
      failed = true;
    }
  }
  pthread_join(tid, NULL);
  JuryDestroy(&jury);
  if (failed) {
    printf("Jury should not have accepted any participants.\n");
    return false;
  }
  return AudienceCheckScores(&audience, 3, 10);
  
}

bool OneJurorNoParticipant() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 1);
  JuryStartAcceptingParticipants(&jury);
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 1, 0);
}

bool ThreeJurorNoParticipant() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 3);
  JuryStartAcceptingParticipants(&jury);
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 0);
}

bool OneJurorOneParticipantRandomScores() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 1);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 1, 1);  
}

bool TenJurorOneParticipantRandomScores() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 10);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 10, 1);
}

bool OneJurorOneParticipantAlwaysNo() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysNo, 1);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 1, 1);  
}

bool OneJurorOneParticipantAlwaysYes() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysYes, 1);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 1, 1);  
}


bool ThreeJurorOneParticipantRandomScores() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 3);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 1);  
}

bool ThreeJurorOneParticipantAlwaysNo() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysNo, 3);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 1);  
}

bool ThreeJurorOneParticipantAlwaysYes() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysYes, 3);
  JuryStartAcceptingParticipants(&jury);
  CheckTrue(JuryTakeNewParticipant(&jury));
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 1);  
}

bool ThreeJurorThreeParticipantRandomScores() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 3);
  JuryStartAcceptingParticipants(&jury);
  for (int i = 0; i < 3; ++i) {
    CheckTrue(JuryTakeNewParticipant(&jury));
  }
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 3);
}

bool ThreeJurorThreeParticipantAlwaysNo() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysNo, 3);
  JuryStartAcceptingParticipants(&jury);
  for (int i = 0; i < 3; ++i) {
    CheckTrue(JuryTakeNewParticipant(&jury));
  }
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 3);  
}

bool ThreeJurorThreeParticipantAlwaysYes() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, AlwaysYes, 3);
  JuryStartAcceptingParticipants(&jury);
  for (int i = 0; i < 3; ++i) {
    CheckTrue(JuryTakeNewParticipant(&jury));
  }
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 3, 3);
}

bool OneJurorTenParticipant() {
  Audience audience;
  AudienceInit(&audience);
  Jury jury;
  JuryInit(&jury, &audience, RandEvaluator, 1);
  JuryStartAcceptingParticipants(&jury);
  for (int i = 0; i < 10; ++i) {
    CheckTrue(JuryTakeNewParticipant(&jury));
  }
  JuryStopAcceptingParticipants(&jury);
  JuryDestroy(&jury);
  return AudienceCheckScores(&audience, 1, 10);
}

int main() {
  srand(time(0));
  RunTest("DoNotAcceptAny", DoNotAcceptAny);
  RunTest("OneJurorNoParticipant", OneJurorNoParticipant);
  RunTest("ThreeJurorNoParticipant", ThreeJurorNoParticipant);
  RunTest("OneJurorOneParticipantRandomScores", OneJurorOneParticipantRandomScores);
  RunTest("TenJurorOneParticipantRandomScores", TenJurorOneParticipantRandomScores);
  RunTest("OneJurorOneParticipantAlwaysNo", OneJurorOneParticipantAlwaysNo);
  RunTest("OneJurorOneParticipantAlwaysYes", OneJurorOneParticipantAlwaysYes);
  RunTest("ThreeJurorOneParticipantRandomScores", ThreeJurorOneParticipantRandomScores);
  RunTest("ThreeJurorOneParticipantAlwaysNo", ThreeJurorOneParticipantAlwaysNo);
  RunTest("ThreeJurorOneParticipantAlwaysYes", ThreeJurorOneParticipantAlwaysYes);
  RunTest("ThreeJurorThreeParticipantRandomScores", ThreeJurorThreeParticipantRandomScores);
  RunTest("ThreeJurorThreeParticipantAlwaysNo", ThreeJurorThreeParticipantAlwaysNo);
  RunTest("ThreeJurorThreeParticipantAlwaysYes", ThreeJurorThreeParticipantAlwaysYes);
  RunTest("OneJurorTenParticipant", OneJurorTenParticipant);
  return 0;
}
