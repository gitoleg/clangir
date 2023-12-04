// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir-enable -emit-cir %s -o -  | FileCheck %s 

// CHECK:  cir.func @init(%arg0: !s32i
// CHECK:    [[TMP0:%.*]] = cir.alloca !s32i, cir.ptr <!s32i>, ["len", init] {alignment = 4 : i64}
// CHECK:    [[TMP1:%.*]] = cir.alloca !cir.ptr<!u8i>, cir.ptr <!cir.ptr<!u8i>>, ["saved_stack"] {alignment = 8 : i64}
// CHECK:    cir.store %arg0, [[TMP0]] : !s32i, cir.ptr <!s32i>
// CHECK:    [[TMP2:%.*]] = cir.load [[TMP0]] : cir.ptr <!s32i>, !s32i
// CHECK:    [[TMP3:%.*]] = cir.cast(integral, [[TMP2]] : !s32i), !u64i
// CHECK:    [[TMP4:%.*]] = cir.alloca !s32i, cir.ptr <!s32i>, [[TMP3]] : !u64i, ["vla"] {alignment = 16 : i64}
// CHECK:    [[TMP5:%.*]] = cir.stack_save : <!u8i>
// CHECK:    cir.store [[TMP5]], [[TMP1]] : !cir.ptr<!u8i>, cir.ptr <!cir.ptr<!u8i>>
// CHECK:    [[TMP6:%.*]] = cir.load [[TMP1]] : cir.ptr <!cir.ptr<!u8i>>, !cir.ptr<!u8i>
// CHECK:    cir.stack_restore [[TMP6]] : !cir.ptr<!u8i>
void init(int len) {
    int a[len];
}

