/*
Copyright (c) 2012, Intel Corporation

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Written by Martin Dimitrov, Carl Strickland */
#include "cpuid.h"

void
cpuid(unsigned int  eax_in, unsigned int ecx_in,
      cpuid_info_t *info)
{
    unsigned int eax = 0;
    unsigned int ebx = 0;
    unsigned int ecx = 0;
    unsigned int edx = 0;

    asm(
        "mov %4, %%eax;"
        "mov %5, %%ecx;"
        "cpuid;"
        "mov %%eax, %0;"
        "mov %%ebx, %1;"
        "mov %%ecx, %2;"
        "mov %%edx, %3;"
        : "=r"(eax), "=r"(ebx), "=r"(ecx), "=r"(edx) /* output operands     */
        : "r"(eax_in), "r"(ecx_in)                   /* input operands      */
        : "%eax", "%ebx", "%ecx", "%edx"             /* clobbered registers */
    );

    info->eax = eax;
    info->ebx = ebx;
    info->ecx = ecx;
    info->edx = edx;
}

/*
 *
 * CPUID.CPU Identification
 * When CPUID executes with EAX set to 01H, version information is returned in EAX (see Figure 3-6). For example:
 * model, family, and processor type for the Intel Xeon processor 5100 series is as follows:
 * - Model . 1111B
 * - Family . 0101B
 * - Processor Type . 00B
*/

unsigned int
get_processor_signature()
{
    cpuid_info_t info;
    cpuid(0x1, 0, &info);
    return info.eax;
}

cpuid_info_t
get_processor_topology(unsigned int level)
{
    cpuid_info_t info;
    cpuid(0xb, level, &info);
    return info;
}

/* 
 * https://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration
 * Choosing CPUID Leaf Information for System Topology Enumeration
 */
unsigned int
get_cpuid_leaf()
{
    cpuid_info_t info;
    cpuid(0x0, 0, &info);
    return info.eax;
}
