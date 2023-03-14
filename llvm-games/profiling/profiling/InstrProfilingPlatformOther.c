/*===- InstrProfilingPlatformOther.c - Profile data default platform ------===*\
|*
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
|* See https://llvm.org/LICENSE.txt for license information.
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
|*
\*===----------------------------------------------------------------------===*/

#include <stdlib.h>
#include <stdio.h>

#include "InstrProfiling.h"
#include "InstrProfilingInternal.h"

extern char __start_llvm_prf_cnts;
extern char __stop_llvm_prf_cnts;
extern char __start_llvm_prf_names;
extern char __stop_llvm_prf_names;
extern __llvm_profile_data __start_llvm_prf_data;
extern __llvm_profile_data __stop_llvm_prf_data;

static uint32_t *OrderFileFirst = NULL;

/*!
 * \brief Register an instrumented function.
 *
 * Calls to this are emitted by clang with -fprofile-instr-generate.  Such
 * calls are only required (and only emitted) on targets where we haven't
 * implemented linker magic to find the bounds of the sections.
 */
COMPILER_RT_VISIBILITY void __llvm_profile_register_function(void *Data_)
{
}


COMPILER_RT_VISIBILITY void __llvm_profile_register_names_function(void *NamesStart, uint64_t NamesSize)
{
}


COMPILER_RT_VISIBILITY const __llvm_profile_data *__llvm_profile_begin_data(void)
{
    return &__start_llvm_prf_data;
}


COMPILER_RT_VISIBILITY const __llvm_profile_data *__llvm_profile_end_data(void)
{
    return &__stop_llvm_prf_data;
}


COMPILER_RT_VISIBILITY const char *__llvm_profile_begin_names(void)
{
    return &__start_llvm_prf_names;
}


COMPILER_RT_VISIBILITY const char *__llvm_profile_end_names(void)
{
    return &__stop_llvm_prf_names;
}


COMPILER_RT_VISIBILITY char *__llvm_profile_begin_counters(void)
{
    return &__start_llvm_prf_cnts;
}


COMPILER_RT_VISIBILITY char *__llvm_profile_end_counters(void)
{
    return &__stop_llvm_prf_cnts;
}


/* TODO: correctly set up OrderFileFirst. */
COMPILER_RT_VISIBILITY uint32_t *__llvm_profile_begin_orderfile(void)
{
    return OrderFileFirst;
}


COMPILER_RT_VISIBILITY ValueProfNode *__llvm_profile_begin_vnodes(void)
{
  return 0;
}


COMPILER_RT_VISIBILITY ValueProfNode *__llvm_profile_end_vnodes(void)
{
    return 0;
}


COMPILER_RT_VISIBILITY ValueProfNode *CurrentVNode = 0;
COMPILER_RT_VISIBILITY ValueProfNode *EndVNode = 0;

COMPILER_RT_VISIBILITY int __llvm_write_binary_ids(ProfDataWriter *Writer)
{
    return 0;
}

// TODO not sure about this
//int INSTR_PROF_PROFILE_RUNTIME_VAR = 1;

