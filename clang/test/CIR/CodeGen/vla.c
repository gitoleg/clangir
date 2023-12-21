// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir-enable -emit-cir %s -o -  | FileCheck %s 

// CHECK:  cir.func @f0(%arg0: !s32i
// CHECK:    [[TMP0:%.*]] = cir.alloca !s32i, cir.ptr <!s32i>, ["len", init] {alignment = 4 : i64}
// CHECK:    [[TMP1:%.*]] = cir.alloca !cir.ptr<!u8i>, cir.ptr <!cir.ptr<!u8i>>, ["saved_stack"] {alignment = 8 : i64}
// CHECK:    cir.store %arg0, [[TMP0]] : !s32i, cir.ptr <!s32i>
// CHECK:    [[TMP2:%.*]] = cir.load [[TMP0]] : cir.ptr <!s32i>, !s32i
// CHECK:    [[TMP3:%.*]] = cir.cast(integral, [[TMP2]] : !s32i), !u64i
// CHECK:    [[TMP4:%.*]] = cir.stack_save : !cir.ptr<!u8i>
// CHECK:    cir.store [[TMP4]], [[TMP1]] : !cir.ptr<!u8i>, cir.ptr <!cir.ptr<!u8i>>
// CHECK:    [[TMP5:%.*]] = cir.alloca !s32i, cir.ptr <!s32i>, [[TMP3]] : !u64i, ["vla"] {alignment = 16 : i64}
// CHECK:    [[TMP6:%.*]] = cir.load [[TMP1]] : cir.ptr <!cir.ptr<!u8i>>, !cir.ptr<!u8i>
// CHECK:    cir.stack_restore [[TMP6]] : !cir.ptr<!u8i>
void f0(int len) {
    int a[len];
}

//     CHECK: cir.func @f1
// CHECK-NOT:   cir.stack_save
// CHECK-NOT:   cir.stack_restore
//     CHECK:   cir.return
int f1(int n) {
  return sizeof(int[n]);
}

// CHECK: cir.func @f2
// CHECK:   cir.stack_save
// DONT_CHECK:   cir.stack_restore
// CHECK:   cir.return
int f2(int x) {
  int vla[x];
  return vla[x-1];
}

// CHECK: cir.func @f3
// CHECK:   cir.stack_save
// CHECK:   cir.stack_restore
// CHECK:   cir.return
void f3(int count) {
  int a[count];

  do {  } while (0);
  if (a[0] != 3) {}
}


//     CHECK: cir.func @f4
// CHECK-NOT:   cir.stack_save
// CHECK-NOT:   cir.stack_restore
//     CHECK:   cir.return
void f4(int count) {
  // Make sure we emit sizes correctly in some obscure cases
  int (*a[5])[count];
  int (*b)[][count];
}

// CHECK: cir.func @f5
// CHECK: {{.*}} = cir.stack_save : !cir.ptr<!u8i> 
// CHECK: {{.*}} = cir.alloca !s8i, cir.ptr <!s8i>, {{%.*}}
// CHECK: cir.loop while(cond : {
// CHECK:     {{%.*}} = cir.stack_save : !cir.ptr<!u8i> 
// CHECK:     cir.stack_restore {{%.*}} : !cir.ptr<!u8i> 
// CHECK: cir.stack_restore {{%.*}} : !cir.ptr<!u8i> 
void f5(unsigned x) {
  char s1[x];
  while (1) {
    char s2[x];
    if (x > 5) //TODO: stack restore here is missed
      break;
  }
}

// Check no errors happen
void function1(short width, int data[][width]) {} 
void function2(short width, int data[][width][width]) {}
void f6(void) {
     int bork[4][13][15];

     function1(1, bork[2]);
     function2(1, bork);    
}
/*
%5 = cir.const(#cir.int<1> : !s32i) : !s32i loc(#loc119)
%6 = cir.cast(integral, %5 : !s32i), !u64i
%7 = cir.load %0 : cir.ptr <!s32i>, !s32i
%8 = cir.const(#cir.int<3> : !s32i) : !s32i
%9 = cir.binop(add, %7, %8) : !s32i
%10 = cir.cast(integral, %9 : !s32i), !u64i
%11 = cir.stack_save : !cir.ptr<!u8i>
%12 = cir.binop(mul, %6, %10) : !u64i
%20 = cir.alloca !s8i, cir.ptr <!s8i>, %10 : !u64i, ["vla"] {alignment = 16 : i64}
*/
static int GLOB;
int f7(int n)
{
  GLOB = 0;
  char b[1][n+3];

  __typeof__(b[GLOB++]) c;
  return GLOB;
}

double f8(int n, double (*p)[n][5]) {
    return p[1][2][3];
}

int f9(unsigned n, char (*p)[n][n+1][6]) {
    __typeof(p) p2 = (p + n/2) - n/4;

  return p2 - p;
}
