// RUN: cir-opt %s -cir-to-llvm -o - | FileCheck %s -check-prefix=MLIR

!u8i = !cir.int<u, 8>

module  {
  cir.func @stack_save() {
    %0 = cir.stack_save : !cir.ptr<!u8i>
    cir.return
  }

  cir.func @stack_restore(%arg0 : !cir.ptr<!u8i>) {
    cir.stack_restore %arg0 : !cir.ptr<!u8i>
    cir.return
  }
}

//      MLIR: module {
// MLIR-NEXT:  llvm.func @stack_save()
// MLIR-NEXT:    %0 = llvm.intr.stacksave : !llvm.ptr
// MLIR-NEXT:    llvm.return
// MLIR-NEXT:  }

// MLIR-NEXT:  llvm.func @stack_restore(%arg0: !llvm.ptr)
// MLIR-NEXT:    llvm.intr.stackrestore %arg0 : !llvm.ptr
// MLIR-NEXT:    llvm.return
// MLIR-NEXT:  }
// MLIR-NEXT: }