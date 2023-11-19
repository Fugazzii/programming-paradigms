#ifndef PARADIGMS_PROBLEMS_TALENT_SHOW_AUDIENCE_H_
#define PARADIGMS_PROBLEMS_TALENT_SHOW_AUDIENCE_H_

#include <stdbool.h>

typedef struct {
  bool yes[15][105];
  int score[105];
} Audience;

void AudienceJurorGreet(Audience* audience, int juror_id);
void AudienceJurorDone(Audience* audience, int juror_id);
void AudienceJuryGreet(Audience* audience);
void AudienceJuryDone(Audience* audience);
void AudienceJuryAnnounceNewParticipant(Audience* audience, int participant_id);
void AudienceJurorEvaluatedParticipant(Audience* audience, int juror_id, int participant_id, bool yes);
void AudienceJuryAnnounceParticipantScore(Audience* audience, int participant_id, int num_yes);

void AudienceInit(Audience* audience);
bool AudienceCheckScores(Audience* audience, int num_jurors, int num_participants);


#endif // PARADIGMS_PROBLEMS_TALENT_SHOW_AUDIENCE_H_
