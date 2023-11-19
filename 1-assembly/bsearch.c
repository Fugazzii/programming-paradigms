typedef struct {        // size 8
  int age;              //0 size 4
  char* name;           //4 size 4
} Owner;                

typedef struct {    // size 16
  int mileage;      //0 size 4
  bool lights[4];   //4 size 4
  Owner owner;      //8 size 8
} Car;

typedef int (*StrCmp)(char *, char *);

int bsearch(Car *cars, int n, char *person, StrCmp cmp_fn) {
  int l = 0;
  int r = n;
  while (l < r) {
    short m = (l + r) / 2;
    r = m;
    if (cmp_fn((Owner *)(cars[m].lights)->name, person) < 0) {
      l = m;
    }
  }
  return l;
}

SP = SP - 10;
  int l = 0;
M[SP+6] = 0;

  int r = n;
R1 = M[SP+18];
M[SP+2] = R1;

  while (l < r) 
R1 = M[SP+6];
R2 = M[SP+2];
BGE R1, R2, PC+100
    short m = (l + r) / 2;
R1 = M[SP+6];
R2 = M[SP+2];    
R3 = R1 + R2;
R4 = R3/2;
M[SP] = .2 R4;
    r = m;
R1 = .2 M[SP];
M[SP+2] = R1;
    if (cmp_fn((Owner *)(cars[m].lights)->name, person) < 0) {
R1 = M[SP+22]; //load person

R2 = M[SP+14];
R3 = .2 M[SP];
R4 = R3*16;
R5 = R2 + R4;   //cars + m
R6 = .1 M[R5+4];//get lights (bool) 
R7 = M[R6+4];   //get name
R8 = M[SP+26];

SP = SP - 8;
M[SP+4] = R1;
M[SP] = R7;

CALL R8

SP = SP+8;
BGE RV, 0, PC+12
      l = m;
R1 = .2 M[SP];
M[SP+6] = R1;
JMP PC-104 //4*26

  return l;
RV = M[SP+6];
SP = SP + 10;
RET;

