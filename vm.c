//
// Created by terp on 24/08/24.
//

#include "vm.h"

#include <stdio.h>

#include "common.h"
#include "debug.h"

VM vm;

/******************************************************************************
 *                             PRIVATE FUNCTIONS                              *
 ******************************************************************************/

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, vm.ip - vm.chunk->code);
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN: {
                printValue(pop()); // to be changed later when we add more features
                printf("\n");
                return INTERPRET_OK;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

static void resetStack() {
    vm.stackTop = vm.stack;
}

/******************************************************************************
 *                             PUBLIC FUNCTIONS                               *
 ******************************************************************************/

void initVM() {
    resetStack();
}

void freeVM() {

}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

InterpretResult interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}