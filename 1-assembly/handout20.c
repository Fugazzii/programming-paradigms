typedef struct {        // size 16
    int violet;         // 0 size 4
    char* dash[2];      // 4 size 8
    char superboy[4];   // 12 size 4
} superhero;

static superhero **pixar(superhero *syndrome);

static superhero *theincredibles(short *frozone, superhero elastigirl)
{
    frozone += elastigirl.superboy[*frozone];
    ((superhero *)((superhero *) elastigirl.dash[0])->dash)->violet = 400;
    return *pixar(&elastigirl) + 10;
}
superhero elastigirl    SP+8
short *frozone          SP+4
saved pc                SP

frozone += elastigirl.superboy[*frozone];
           elastigirl.superboy[*frozone] = *(elastigirl.superboy + (*frozone))

R1 = M[SP+4];  // frozone
R2 = .2 M[R1]; //*frozone
R3 = SP+20;
R3 = R3 + R2;
R4 = .1 M[R3];
R5 = R4 *2;     //short pointer arithmetic
R6 = R1 + R5;
M[SP+4] = R6;

((superhero *)((superhero *) elastigirl.dash[0])->dash)->violet = 400;
 elastigirl.dash[0] = *(elasticgirl.dash)

R1 = M[SP+12];  //(elasticgirl.dash) // -(superhero *)
             //dash - (superhero *)

M[R1 + 4]= 400;

return *pixar(&elastigirl) + 10;
R1 = SP + 8;    //&elastigirl
SP = SP - 4;
M[SP] = R1;

CALL <PIXAR>;

SP = SP + 4;
R1 = M[RV]; // R2 contains superhero *

RV = R1 +160; //10*16
RET;