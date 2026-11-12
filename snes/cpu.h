
#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#include "statehandler.h"

typedef uint8_t (*CpuReadHandler)(void* mem, uint32_t adr);
typedef void (*CpuWriteHandler)(void* mem, uint32_t adr, uint8_t val);
typedef void (*CpuIdleHandler)(void* mem, bool waiting);

typedef struct Cpu Cpu;

// What the before-instruction hook did at an instruction boundary.
typedef enum CpuHookAction {
  cpuHookContinue, // run the guest instruction at k:pc
  cpuHookHandled,  // the hook did the work and moved k:pc; no guest instruction runs
  cpuHookStop      // nothing runs; the caller's run loop returns
} CpuHookAction;

typedef CpuHookAction (*CpuInstructionHook)(void* context, Cpu* cpu);

// What one cpu_runOpcode call did.
typedef enum CpuStep {
  cpuStepGuest,  // a guest instruction, interrupt entry, reset or wait cycle
  cpuStepNative, // the hook handled the instruction
  cpuStepStop    // the hook asked the run to stop
} CpuStep;

struct Cpu {
  // reference to memory handler, pointers to read/write/idle handlers
  void* mem;
  CpuReadHandler read;
  CpuWriteHandler write;
  CpuIdleHandler idle;
  // registers
  uint16_t a;
  uint16_t x;
  uint16_t y;
  uint16_t sp;
  uint16_t pc;
  uint16_t dp; // direct page (D)
  uint8_t k; // program bank (PB)
  uint8_t db; // data bank (B)
  // flags
  bool c;
  bool z;
  bool v;
  bool n;
  bool i;
  bool d;
  bool xf;
  bool mf;
  bool e;
  // power state (WAI/STP)
  bool waiting;
  bool stopped;
  // interrupts
  bool irqWanted;
  bool nmiWanted;
  bool intWanted;
  bool intDelay;
  bool resetWanted;
  // before-instruction hook, not part of the machine state
  CpuInstructionHook instructionHook;
  void* instructionHookContext;
};

Cpu* cpu_init(void* mem, CpuReadHandler read, CpuWriteHandler write, CpuIdleHandler idle);
void cpu_free(Cpu* cpu);
void cpu_reset(Cpu* cpu, bool hard);
void cpu_handleState(Cpu* cpu, StateHandler* sh);
CpuStep cpu_runOpcode(Cpu* cpu);
void cpu_setInstructionHook(Cpu* cpu, CpuInstructionHook hook, void* context);
void cpu_nmi(Cpu* cpu);
void cpu_setIrq(Cpu* cpu, bool state);

#endif
