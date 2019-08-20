Intel(r) Power Governor    {#mainpage}
=======================

Intel(r) Power Governor is a software utility and library, which allows developers 
to (a) monitor power and (b) regulate power, at very fine time granularities 
(few tens of milliseconds). Power monitoring/control is available for the 
following power domains: 
- per package domain (CPU socket)
- per core domain (all the cpu cores on a package)
- per uncore domain (uncore components, e.g. integrated graphics, client parts
  only) 
- per memory node (memory local to a package, server parts only) 
To do this, the tool uses an architected capability in
Intel(r) processors which is called RAPL (Runtime Average Power Limiting).
RAPL is available on Intel(r) codename Sandy Bridge and later processors. 


How to use it
-----------------------------------

Prerequisites: 
This tool uses the msr and cpuid kernel modules. You may have to do: 
> modprobe msr 

> modprobe cpuid 

On RedHat, you may have to run: 
> mk_msr_dev_redhat.sh

To build: 
> make

To run: 
> ./power_gov

To generate the doxygen documentation: 
> doxygen doxygen/config/doxy_config 


Using the library
-----------------------------------

Intel(r) Power Governor library allows developers to design dynamic power management
solutions, which optimize the power consumption of their machines in accordance 
with their performance or quality of service needs. 
To get started using the library, see the short example below. 

Below is a simple example, of using the library in order to create a tool 'set_limit',
which limits the CPU package (for all packages) power consumption to 70 Watts. 
To compile, link against the stardard math library and librapl:  
    gcc set_limit.c -I. -L. -lm -o set_limit ./librapl.a

~~~{.c}
#include "rapl.h"
#define MY_POWER_LIMIT 70
 
int
main(int argc, char **argv)
{
    int i, num_node;
    pkg_rapl_power_limit_control_t  pkg_plc;
     
    // Always intialize the power_gov library first
    init_rapl();

    num_node = get_num_rapl_nodes_pkg();

    for (i = 0; i < num_node; i++) {

        // Read, modify and write the power limit control structure
        get_pkg_rapl_power_limit_control(i, &pkg_plc);
        pkg_plc.power_limit_watts_1 = MY_POWER_LIMIT;
        set_pkg_rapl_power_limit_control(i, &pkg_plc);
   }

    // Terminate the power_gov library
   return terminate_rapl();
}
~~~

Below is another example, of using the library in order to create 
power meter, which reports the CPU package (for all packages) power consumption every 100ms. 
To compile, link against the stardard math library and librapl:  
    gcc power_limit.c -I. -L. -lm -o power_limit ./librapl.a

~~~{.c}

#include "rapl.h"
#include <stdio.h>
#include <stdlib.h>


unsigned int  delay_us   = 100000.0;
double        delay_unit = 1000000.0;

int
main(int argc, char **argv)
{

    int i, num_node;
    double energy_consumed, delta;
    double *samples;

    // Always intialize the power_gov library first
    init_rapl();

    num_node = get_num_nodes();
    samples = (double*) malloc(num_node * sizeof(double));

    while(1){

        usleep(delay_us);

        for (i = 0; i < num_node; i++) {

            // Sample the total energy consumed, and convert to Watts.  
            get_pkg_total_energy_consumed(i, &energy_consumed);
            delta = energy_consumed - samples[i];
            samples[i] = energy_consumed;
            printf("%.0lf,", delta / ((double) delay_us / delay_unit));
        }
        printf("\n");
    }

    // Terminate the power_gov library
    return terminate_rapl();
}
~~~

Known Limitations / Issues / BKMs
-----------------------------------

- The DRAM RAPL is not enabled in BIOS by default.
To enable in BIOS, go to Memory Configuration and change the mode from
'Performance' to 'Power Efficient'. Then select 'Mode 1'. This is the 
VR (voltage regulator) mode of power estimation. The accuracy of this mode
is highly dependent on the OEM platform. For Intel reference platforms the 
accuracy of DRAM power estimation may produce up to ~30% error. 

- Even though all the RAPL MSRs are exposed for writing in power_gov, writing to 
some of them appears to be disabled (which is not documented). For example, it
appears that you cannot disable power limiting for PKG domain. In order to 
'disable', just make the power limit back to what it was by default (e.g. 130) 

 
Revision History
-----------------------------------
- Enabled suport for IVB-EP, HSW-EP, BDW-EP, SKX server, CLX server
(The PP0 power plane does not report any readings on HSW, BDW, SKX


