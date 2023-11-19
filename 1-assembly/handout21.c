typedef struct {        // size 20
    int* * garydanko;   // 0 size 4
    int aqua[3];        // 4 size 12
    char* quince;       // 16 size 4
} appetizer;

typedef struct {        //size 32
    short ame[2];       // 0 size 4
    appetizer farallon; // 4 size 20
    char bacar[8];      // 24 size 8
} dessert;

dessert* dinnerisserved(short *boulevard, appetizer *jardiniere);

int *bonappetit(dessert azie, char **indigo)
{
    appetizer oola;
    dessert* catch;
    azie.bacar[azie.ame[2]] += catch->farallon.aqua[4];
    ((appetizer *)(((dessert *)(&oola.quince))->farallon.garydanko))->quince = 0;
    return (*dinnerisserved((short *) &indigo, &oola)).farallon.aqua;
}
indigo      SP +60 
azie        SP +28 
saved pc    SP +24
oola        SP + 4
catch       SP

SP = SP - 24;
    azie.bacar[azie.ame[2]] += catch->farallon.aqua[4];

R1 = M [SP];
R2 = M[R1 + 24]; //catch->farallon.aqua[4]; 8 +16

R3 = .2 M[SP+32] //azie.ame[2]

R4 = SP + 52;
R5 = R4 + R3;
R6 = .1 M[R5]; //azie.bacar[azie.ame[2]]

R7 = R2 + R6;
M[R5] = .1 R7;

((appetizer *)(((dessert *)(&oola.quince))->farallon.garydanko))->quince = 0;
R1 = M[SP + 24];     //((dessert *)(&oola.quince))->farallon.garydanko
M[R1 + 16] = 0;


    return (*dinnerisserved((short *) &indigo, &oola)).farallon.aqua;
R1 = SP + 60;
R2 = SP + 4;    
SP = SP - 8;
M[SP] = R1;
M[SP + 4] = R2;
CALL <dinnerisserved>

SP = SP + 8;

RV = RV + 8;

SP = SP + 24;
RET;
