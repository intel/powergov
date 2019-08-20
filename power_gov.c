/*
Copyright (c) 2012, Intel Corporation

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Written by Martin Dimitrov, Carl Strickland */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "rapl.h"

unsigned int  set_rapl = 0;
unsigned int  print_info = 0;
unsigned int  print_energy_info = 0;
char         *progname;
const char   *version = "1.0";
char         *svalue = NULL;
char         *dvalue = NULL;
unsigned int  vval = 0;
double        fval = 0.0;

unsigned int  num_node = 0;
int           package = -1;
unsigned int  delay_us = 0;
double        delay_unit = 1000000.0;

int
print_pkg_rapl_power_limit_control(unsigned int node)
{
    int                            err = 0;
    pkg_rapl_power_limit_control_t pkg_plc;

    err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL PKG POWER LIMITS", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_POWER_LIMIT:", pkg_plc.power_limit_watts_1);
        fprintf(stdout, "%-30s %9u\n", "RAPL_ENABLE_POWER_LIMIT:", pkg_plc.limit_enabled_1);
        fprintf(stdout, "%-30s %9u\n", "RAPL_CLAMPING_LIMIT:", pkg_plc.clamp_enabled_1);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_TIME_WINDOW:", pkg_plc.limit_time_window_seconds_1);
        fprintf(stdout, "%-30s %9u\n", "RAPL_LOCK:", pkg_plc.lock_enabled);
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_POWER_LIMIT_2:", pkg_plc.power_limit_watts_2);
        fprintf(stdout, "%-30s %9u\n", "RAPL_ENABLE_POWER_LIMIT_2:", pkg_plc.limit_enabled_2);
        fprintf(stdout, "%-30s %9u\n", "RAPL_CLAMPING_LIMIT_2:", pkg_plc.clamp_enabled_2);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_TIME_WINDOW_2:", pkg_plc.limit_time_window_seconds_2);
        if (pkg_plc.lock_enabled) {
            fprintf(stdout, "The RAPL_LOCK must be disabled in BIOS\n");
        }
    }

    return err;
}

int
print_pp0_rapl_power_limit_control(unsigned int node)
{
    int                            err = 0;
    pp0_rapl_power_limit_control_t pp0_plc;

    err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL PP0 POWER LIMITS", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_POWER_LIMIT:", pp0_plc.power_limit_watts);
        fprintf(stdout, "%-30s %9u\n", "RAPL_ENABLE_POWER_LIMIT:", pp0_plc.limit_enabled);
        fprintf(stdout, "%-30s %9u\n", "RAPL_CLAMPING_LIMIT:", pp0_plc.clamp_enabled);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_TIME_WINDOW:", pp0_plc.limit_time_window_seconds);
        fprintf(stdout, "%-30s %9u\n", "RAPL_LOCK:", pp0_plc.lock_enabled);
        if (pp0_plc.lock_enabled) {
            fprintf(stdout, "The RAPL_LOCK must be disabled in BIOS\n");
        }
    }

    return err;
}

int
print_pp1_rapl_power_limit_control(unsigned int node)
{
    int                            err = 0;
    pp1_rapl_power_limit_control_t pp1_plc;

    err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL PP1 POWER LIMITS", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_POWER_LIMIT:", pp1_plc.power_limit_watts);
        fprintf(stdout, "%-30s %9u\n", "RAPL_ENABLE_POWER_LIMIT:", pp1_plc.limit_enabled);
        fprintf(stdout, "%-30s %9u\n", "RAPL_CLAMPING_LIMIT:", pp1_plc.clamp_enabled);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_TIME_WINDOW:", pp1_plc.limit_time_window_seconds);
        fprintf(stdout, "%-30s %9u\n", "RAPL_LOCK:", pp1_plc.lock_enabled);
        if (pp1_plc.lock_enabled) {
            fprintf(stdout, "The RAPL_LOCK must be disabled in BIOS\n");
        }
    }

    return err;
}

int
print_dram_rapl_power_limit_control(unsigned int node)
{
    int                             err = 0;
    dram_rapl_power_limit_control_t dram_plc;

    err = get_dram_rapl_power_limit_control(node, &dram_plc);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL DRAM POWER LIMITS", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_POWER_LIMIT:", dram_plc.power_limit_watts);
        fprintf(stdout, "%-30s %9u\n", "RAPL_ENABLE_POWER_LIMIT:", dram_plc.limit_enabled);
        fprintf(stdout, "%-30s %9u\n", "RAPL_CLAMPING_LIMIT:", dram_plc.clamp_enabled);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_TIME_WINDOW:", dram_plc.limit_time_window_seconds);
        fprintf(stdout, "%-30s %9u\n", "RAPL_LOCK:", dram_plc.lock_enabled);
        if (dram_plc.lock_enabled) {
            fprintf(stdout, "The RAPL_LOCK must be disabled in BIOS\n");
        }
    }

    return err;
}

int
print_rapl_power_limit_control(unsigned int power_domain,
                               unsigned int node)
{
    int err = 0;

    switch (power_domain) {
    case PKG:
        err = print_pkg_rapl_power_limit_control(node);
        break;
    case PP0:
        err = print_pp0_rapl_power_limit_control(node);
        break;
    case PP1:
        err = print_pp1_rapl_power_limit_control(node);
        break;
    case DRAM:
        err = print_dram_rapl_power_limit_control(node);
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

int
print_pkg_rapl_parameters(unsigned int node)
{
    int                   err = 0;
    pkg_rapl_parameters_t pkg_parameters;

    err = get_pkg_rapl_parameters(node, &pkg_parameters);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL PKG POWER INFO", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_THERMAL_SPEC_POWER:", pkg_parameters.thermal_spec_power_watts);
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_MIN_POWER:", pkg_parameters.minimum_power_watts);
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_MAX_POWER:", pkg_parameters.maximum_power_watts);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_MAX_TIME_WINDOW:", pkg_parameters.maximum_limit_time_window_seconds);
    }

    return err;
}

int
print_dram_rapl_parameters(unsigned int node)
{
    int                    err = 0;
    dram_rapl_parameters_t dram_parameters;

    err = get_dram_rapl_parameters(node, &dram_parameters);
    if (!err) {
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "| %-26s %7s %u |\n", "RAPL DRAM POWER INFO", "NODE", node);
        fprintf(stdout, "+--------------------------------------+\n");
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_THERMAL_SPEC_POWER:", dram_parameters.thermal_spec_power_watts);
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_MIN_POWER:", dram_parameters.minimum_power_watts);
        fprintf(stdout, "%-30s %8.0lfW\n", "RAPL_MAX_POWER:", dram_parameters.maximum_power_watts);
        fprintf(stdout, "%-30s %8.6lfs\n", "RAPL_MAX_TIME_WINDOW:", dram_parameters.maximum_limit_time_window_seconds);
    }

    return err;
}

int
print_rapl_accumulated_throttled_time(unsigned int power_domain,
                                      unsigned int node)
{
    int          err = 0;
    double       accumulated_throttled_time;

    switch (power_domain) {
    case PKG:
        err = get_pkg_accumulated_throttled_time(node, &accumulated_throttled_time);
        break;
    case PP0:
        err = get_pp0_accumulated_throttled_time(node, &accumulated_throttled_time);
        break;
    case DRAM:
        err = get_dram_accumulated_throttled_time(node, &accumulated_throttled_time);
        break;
    default:
        err = MY_ERROR;
        break;
    }

    if (!err) {
        fprintf(stdout, "%-30s %8.0lfs\n", "RAPL_PERF_STATUS:", accumulated_throttled_time);
    }

    return err;
}

int
write_rapl_power_limit_control(unsigned int power_domain,
                               unsigned int node,
                               double       power_limit_watts,
                               unsigned int limit_enabled,
                               unsigned int clamp_enabled,
                               double       limit_time_window_seconds,
                               unsigned int lock_enabled)
{
    int                             err = 0;
    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

    switch (power_domain) {
    case PKG:
        err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
        if (!err) {
            pkg_plc.power_limit_watts_1 = power_limit_watts;
            pkg_plc.limit_time_window_seconds_1 = limit_time_window_seconds;
            pkg_plc.limit_enabled_1 = limit_enabled;
            pkg_plc.clamp_enabled_1 = clamp_enabled;
            pkg_plc.lock_enabled = lock_enabled;
            err = set_pkg_rapl_power_limit_control(node, &pkg_plc);
        }
        break;
    case PP0:
        err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
        if (!err) {
            pp0_plc.power_limit_watts = power_limit_watts;
            pp0_plc.limit_time_window_seconds = limit_time_window_seconds;
            pp0_plc.limit_enabled = limit_enabled;
            pp0_plc.clamp_enabled = clamp_enabled;
            pp0_plc.lock_enabled = lock_enabled;
            err = set_pp0_rapl_power_limit_control(node, &pp0_plc);
        }
        break;
    case PP1:
        err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
        if (!err) {
            pp1_plc.power_limit_watts = power_limit_watts;
            pp1_plc.limit_time_window_seconds = limit_time_window_seconds;
            pp1_plc.limit_enabled = limit_enabled;
            pp1_plc.clamp_enabled = clamp_enabled;
            pp1_plc.lock_enabled = lock_enabled;
            err = set_pp1_rapl_power_limit_control(node, &pp1_plc);
        }
        break;
    case DRAM:
        err = get_dram_rapl_power_limit_control(node, &dram_plc);
        if (!err) {
            dram_plc.power_limit_watts = power_limit_watts;
            dram_plc.limit_time_window_seconds = limit_time_window_seconds;
            dram_plc.limit_enabled = limit_enabled;
            dram_plc.clamp_enabled = clamp_enabled;
            dram_plc.lock_enabled = lock_enabled;
            err = set_dram_rapl_power_limit_control(node, &dram_plc);
        }
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

int
write_rapl_power_limit_limit(unsigned int power_domain,
                             unsigned int node,
                             double       power_limit_watts)
{
    int                             err = 0;
    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

    switch (power_domain) {
    case PKG:
        err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
        if (!err) {
            pkg_plc.power_limit_watts_1 = power_limit_watts;
            err = set_pkg_rapl_power_limit_control(node, &pkg_plc);
        }
        break;
    case PP0:
        err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
        if (!err) {
            pp0_plc.power_limit_watts = power_limit_watts;
            err = set_pp0_rapl_power_limit_control(node, &pp0_plc);
        }
        break;
    case PP1:
        err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
        if (!err) {
            pp1_plc.power_limit_watts = power_limit_watts;
            err = set_pp1_rapl_power_limit_control(node, &pp1_plc);
        }
        break;
    case DRAM:
        err = get_dram_rapl_power_limit_control(node, &dram_plc);
        if (!err) {
            dram_plc.power_limit_watts = power_limit_watts;
            err = set_dram_rapl_power_limit_control(node, &dram_plc);
        }
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

int
write_rapl_power_limit_enable(unsigned int power_domain,
                              unsigned int node,
                              unsigned int limit_enabled)
{
    int                             err = 0;
    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

    switch (power_domain) {
    case PKG:
        err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
        if (!err) {
            pkg_plc.limit_enabled_1 = limit_enabled;
            err = set_pkg_rapl_power_limit_control(node, &pkg_plc);
        }
        break;
    case PP0:
        err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
        if (!err) {
            pp0_plc.limit_enabled = limit_enabled;
            err = set_pp0_rapl_power_limit_control(node, &pp0_plc);
        }
        break;
    case PP1:
        err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
        if (!err) {
            pp1_plc.limit_enabled = limit_enabled;
            err = set_pp1_rapl_power_limit_control(node, &pp1_plc);
        }
        break;
    case DRAM:
        err = get_dram_rapl_power_limit_control(node, &dram_plc);
        if (!err) {
            dram_plc.limit_enabled = limit_enabled;
            err = set_dram_rapl_power_limit_control(node, &dram_plc);
        }
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

int
write_rapl_power_limit_clamping(unsigned int power_domain,
                                unsigned int node,
                                unsigned int clamp_enabled)
{
    int                             err = 0;
    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

    switch (power_domain) {
    case PKG:
        err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
        if (!err) {
            pkg_plc.clamp_enabled_1 = clamp_enabled;
            err = set_pkg_rapl_power_limit_control(node, &pkg_plc);
        }
        break;
    case PP0:
        err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
        if (!err) {
            pp0_plc.clamp_enabled = clamp_enabled;
            err = set_pp0_rapl_power_limit_control(node, &pp0_plc);
        }
        break;
    case PP1:
        err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
        if (!err) {
            pp1_plc.clamp_enabled = clamp_enabled;
            err = set_pp1_rapl_power_limit_control(node, &pp1_plc);
        }
        break;
    case DRAM:
        err = get_dram_rapl_power_limit_control(node, &dram_plc);
        if (!err) {
            dram_plc.clamp_enabled = clamp_enabled;
            err = set_dram_rapl_power_limit_control(node, &dram_plc);
        }
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

int
write_rapl_power_limit_time(unsigned int power_domain,
                            unsigned int node,
                            double       limit_time_window_seconds)
{
    int                             err = 0;
    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

    switch (power_domain) {
    case PKG:
        err = get_pkg_rapl_power_limit_control(node, &pkg_plc);
        if (!err) {
            pkg_plc.limit_time_window_seconds_1 = limit_time_window_seconds;
            err = set_pkg_rapl_power_limit_control(node, &pkg_plc);
        }
        break;
    case PP0:
        err = get_pp0_rapl_power_limit_control(node, &pp0_plc);
        if (!err) {
            pp0_plc.limit_time_window_seconds = limit_time_window_seconds;
            err = set_pp0_rapl_power_limit_control(node, &pp0_plc);
        }
        break;
    case PP1:
        err = get_pp1_rapl_power_limit_control(node, &pp1_plc);
        if (!err) {
            pp1_plc.limit_time_window_seconds = limit_time_window_seconds;
            err = set_pp1_rapl_power_limit_control(node, &pp1_plc);
        }
        break;
    case DRAM:
        err = get_dram_rapl_power_limit_control(node, &dram_plc);
        if (!err) {
            dram_plc.limit_time_window_seconds = limit_time_window_seconds;
            err = set_dram_rapl_power_limit_control(node, &dram_plc);
        }
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return err;
}

// This MSR is updated approximately every 1 msec.
double
get_rapl_energy_info(unsigned int power_domain, unsigned int node)
{
    int          err;
    double       total_energy_consumed = 0.0;

    switch (power_domain) {
    case PKG:
        err = get_pkg_total_energy_consumed(node, &total_energy_consumed);
        break;
    case PP0:
        err = get_pp0_total_energy_consumed(node, &total_energy_consumed);
        break;
    case PP1:
        err = get_pp1_total_energy_consumed(node, &total_energy_consumed);
        break;
    case DRAM:
        err = get_dram_total_energy_consumed(node, &total_energy_consumed);
        break;
    default:
        err = MY_ERROR;
        break;
    }

    return total_energy_consumed;
}

void
do_print_info(unsigned int power_domain, unsigned int node)
{
    fprintf(stdout, "               +--------+                \n");
    fprintf(stdout, "               | NODE %u |               \n", node);
    fprintf(stdout, "               +--------+                \n");
    fprintf(stdout, "\n");

    if(is_supported_domain(power_domain)) {
        switch (power_domain) {
        case RAPL_PKG:
            assert(MY_ERROR != print_pkg_rapl_parameters(node));
            assert(MY_ERROR != print_pkg_rapl_power_limit_control(node));

            // perf status is supported only on server
            if(is_supported_domain(RAPL_DRAM)) {
                assert(MY_ERROR != print_rapl_accumulated_throttled_time(RAPL_PKG, node));
            }
            fprintf(stdout, "----------------------------------------\n");
            fprintf(stdout, "\n");
            break;
        case RAPL_PP0:
            assert(MY_ERROR != print_pp0_rapl_power_limit_control(node));

            // perf status is supported only on server
            if(is_supported_domain(RAPL_DRAM)) {
                assert(MY_ERROR != print_rapl_accumulated_throttled_time(RAPL_PKG, node));
            }
            fprintf(stdout, "----------------------------------------\n");
            fprintf(stdout, "\n");
            break;
        case RAPL_PP1:
            assert(MY_ERROR != print_pp1_rapl_power_limit_control(node));
            fprintf(stdout, "----------------------------------------\n");
            fprintf(stdout, "\n");
            break;
        case RAPL_DRAM:
            assert(MY_ERROR != print_dram_rapl_parameters(node));
            assert(MY_ERROR != print_dram_rapl_power_limit_control(node));
            assert(MY_ERROR != print_rapl_accumulated_throttled_time(RAPL_DRAM, node));
            fprintf(stdout, "----------------------------------------\n");
            fprintf(stdout, "\n");
            break;
        default:
            _exit(-1);
        }
    }

}


void
do_print_energy_info()
{
    int i = 0;
    int domain = 0;
    unsigned int node;
    double new_sample;
    double delta;
    double power;
    double **prev_sample = (double**) malloc(num_node * sizeof(double *));

    for (i = 0; i < num_node; i++) {
        prev_sample[i] = (double*) malloc(RAPL_NR_DOMAIN * sizeof(double));
    }

    if (-1 != package) {
        node = package;
        num_node = node + 1;
    } else {
        node = 0;
    }

    /* Print header */
    fprintf(stdout, "Node");
    if(is_supported_domain(RAPL_PKG)) {
        fprintf(stdout, ",PKG");
    }
    if(is_supported_domain(RAPL_PP0)) {
        fprintf(stdout, ",PP0");
    }
    if(is_supported_domain(RAPL_PP1)) {
        fprintf(stdout, ",PP1");
    }
    if(is_supported_domain(RAPL_DRAM)) {
        fprintf(stdout, ",DRAM");
    }
    fprintf(stdout, "\n");

    /* Read initial values */
    for (i = node; i < num_node; i++) {
        for (domain = 0; domain < RAPL_NR_DOMAIN; ++domain) {
            if(is_supported_domain(domain)) {
                prev_sample[i][domain] = get_rapl_energy_info(domain, i);
            }
        }
    }

    /* Begin sampling */
    while (1) {
        usleep(delay_us);

        for (i = node; i < num_node; i++) {
            fprintf(stdout, "%d", i);
            for (domain = 0; domain < RAPL_NR_DOMAIN; ++domain) {
                if(is_supported_domain(domain)) {
                    new_sample = get_rapl_energy_info(domain, i);
                    delta = new_sample - prev_sample[i][domain];
                    /* Handle wraparound */
                    if (delta < 0) {
                        delta += MAX_ENERGY_STATUS_JOULES;
                    }
                    prev_sample[i][domain] = new_sample;
                    power = delta / ((double) delay_us / delay_unit);
                    fprintf(stdout, ",%.0lf",power);
                }
            }
            fprintf(stdout, "\n");
        }
        fflush(stdout);
    }


    for (i = 0; i < num_node; i++) {
        free(prev_sample[i]); 
    }
    free(prev_sample); 
}

void
usage()
{
    fprintf(stdout, "\npower_gov %s \n\n", version);
    fprintf(stdout, "To enter interactive mode: \n");
    fprintf(stdout, "%s\n", progname);
    fprintf(stdout, "\nTo print energy info: \n");
    fprintf(stdout, "%s -e [delay_ms] [-p [package] optional]\n", progname);
    fprintf(stdout, "\nTo change RAPL settings: \n");
    fprintf(stdout, "%s -r [RAPL_setting] -s [setting_value] -d [power domain] [-p [package] optional]\n", progname);
    fprintf(stdout, "\n\t The available RAPL settings are:\n");
    fprintf(stdout, "\t POWER_LIMIT (watts, e.g. 65)\n");
    fprintf(stdout, "\t ENABLE_POWER_LIMIT (bool, e.g. 0)\n");
    fprintf(stdout, "\t CLAMPING_LIMIT (bool, e.g. 0, set to enable going below OS requested frequency)\n");
    fprintf(stdout, "\t TIME_WINDOW (seconds, e.g. 0.5)\n");
    fprintf(stdout, "\n\t The available power domains on this system are:\n");
    if(is_supported_domain(RAPL_PKG))
        fprintf(stdout, "\t PKG (package power domain)\n");
    if(is_supported_domain(RAPL_PP0))
        fprintf(stdout, "\t PP0 (core power domain)\n");
    if(is_supported_domain(RAPL_PP1))
        fprintf(stdout, "\t PP1 (uncore power domain)\n");
    if(is_supported_domain(RAPL_DRAM))
        fprintf(stdout, "\t DRAM (DRAM power domain)\n");
    fprintf(stdout, "\nTo print RAPL settings: \n");
    fprintf(stdout, "%s -i [-d [power domain] optional] [-p [package] optional]\n", progname);
    fprintf(stdout, "\nTo print this help menu: \n");
    fprintf(stdout, "%s -h \n", progname);
    fprintf(stdout, "\n");
}

int
interactive_mode_power_meter()
{
    unsigned int delay_ms_temp;

menu_1:
    fprintf(stdout, "Please supply the sampling delay in ms (e.g. 1000), or enter 0 to exit:\n");
    scanf("%u", &delay_ms_temp);
    if(0 == delay_ms_temp) {
        _exit(0);
    }
    if(delay_ms_temp > 50) {
        delay_us = delay_ms_temp * 1000;
        do_print_energy_info();
    } else {
        fprintf(stdout, "Delay must be greater than 50 ms.\n");
        goto menu_1;
    }
}

unsigned int
menu_select_power_domain()
{
    int             ch;
    unsigned int    power_domain;
menu_1:
    fprintf(stdout, "Please select a power domain:\n");
    fprintf(stdout, "\t1: PKG - Processor package domain\n");
    fprintf(stdout, "\t2: PP0 - Processor core domain\n");
    fprintf(stdout, "\t3: PP1 - Processor uncore domain (client only)\n");
    fprintf(stdout, "\t4: DRAM - Memory domain (server only)\n");
    fprintf(stdout, "\t5: Exit\n");

    scanf("%d", &ch);

    switch (ch) {
    case 1:
        power_domain = RAPL_PKG;
        break;
    case 2:
        power_domain = RAPL_PP0;
        break;
    case 3:
        power_domain = RAPL_PP1;
        break;
    case 4:
        power_domain = RAPL_DRAM;
        break;
    case 5:
        _exit(0);
    default:
        fprintf(stdout, "%d is an invalid option!\n", ch);
        goto menu_1;
    }

    if(!is_supported_domain(power_domain)) {
        fprintf(stdout, "\nThe selected power domain is not available on your machine!\n\n");
        goto menu_1;
    }

    return power_domain;
}

int
interactive_mode_print_info()
{

    int          node;
    unsigned int power_domain;

    power_domain = menu_select_power_domain();

    for (node = 0; node < num_node; node++) {
        do_print_info(power_domain, node);
    }
}

int
interactive_mode_set_rapl()
{

    int          ch;
    unsigned int node;
    unsigned int err = 0;

    unsigned int power_domain;
    unsigned int clamping_limit;
    double       power_limit;

    pkg_rapl_power_limit_control_t  pkg_plc;
    pp0_rapl_power_limit_control_t  pp0_plc;
    pp1_rapl_power_limit_control_t  pp1_plc;
    dram_rapl_power_limit_control_t dram_plc;

//--------------------------------------------------------
// Step 1: Select power domain
//--------------------------------------------------------
    power_domain = menu_select_power_domain();


//--------------------------------------------------------
// Step 2: Select enable/disable
//--------------------------------------------------------
menu_2:
    fprintf(stdout, "Please select to Enable of Disable RAPL power limit:\n");
    fprintf(stdout, "\t1: Enable power limit\n");
    fprintf(stdout, "\t2: Disable power limit\n");
    fprintf(stdout, "\t3: Exit\n");

    scanf("%d", &ch);

    switch (ch) {
    case 1:
        break;
    case 2:
        for (node = 0; node < num_node; node++) {
            switch (power_domain) {
            case PKG:
                assert(MY_ERROR != write_rapl_power_limit_enable(power_domain, node, 0));
                assert(MY_ERROR != print_pkg_rapl_power_limit_control(node));
                break;
            case PP0:
                assert(MY_ERROR != write_rapl_power_limit_enable(power_domain, node, 0));
                assert(MY_ERROR != print_pp0_rapl_power_limit_control(node));
                break;
            case PP1:
                assert(MY_ERROR != write_rapl_power_limit_enable(power_domain, node, 0));
                assert(MY_ERROR != print_pp1_rapl_power_limit_control(node));
                break;
            case DRAM:
                assert(MY_ERROR != write_rapl_power_limit_enable(power_domain, node, 0));
                assert(MY_ERROR != print_dram_rapl_power_limit_control(node));
                break;
            }
        }
        return 0;
    case 3:
        _exit(0);
    default:
        fprintf(stdout, "%d is an invalid option!\n", ch);
        goto menu_2;
    }

//--------------------------------------------------------
// Step 3: Select clamping limit
//--------------------------------------------------------
    if(RAPL_PKG == power_domain) {
menu_3:
        fprintf(stdout, "Please select to Enable of Disable RAPL clamping limit:\n");
        fprintf(stdout, "\t1: Enable clamping limit (full capability - overwrites OS settings)\n");
        fprintf(stdout, "\t2: Disable clamping limit (limited capability - RAPL only controls the turbo frequencies)\n");
        fprintf(stdout, "\t3: Exit\n");

        scanf("%d", &ch);
        switch (ch) {
        case 1:
            clamping_limit = 1;
            break;
        case 2:
            clamping_limit = 0;
            break;
        case 3:
            _exit(0);
        default:
            fprintf(stdout, "%d is an invalid option!\n", ch);
            goto menu_3;
        }
    }

    //--------------------------------------------------------
    // Step 4: Set the power limit
    //--------------------------------------------------------
    fprintf(stdout, "Please supply the power limit (e.g. 60), or enter 0 to exit:\n");
    scanf("%lf", &power_limit);
    if(0.0 == power_limit)
        _exit(0);
    assert((0.0 < power_limit) && (200.0 > power_limit));


    //--------------------------------------------------------
    // Write the settings
    //--------------------------------------------------------
    for (node = 0; node < num_node; node++) {
        assert(MY_ERROR != write_rapl_power_limit_control(power_domain, node, power_limit, 1, clamping_limit, 1.0, 0));
        assert(MY_ERROR != print_rapl_power_limit_control(power_domain, node));
    }

    return 0;
}


int
interactive_mode()
{
    int ch;

main_menu:
    fprintf(stdout, "Please select from the following options:\n");
    fprintf(stdout, "\t1: Print current RAPL settings\n");
    fprintf(stdout, "\t2: Start power meter\n");
    fprintf(stdout, "\t3: Change a RAPL setting\n");
    fprintf(stdout, "\t4: Print a detailed help menu\n");
    fprintf(stdout, "\t5: Exit\n");

    scanf("%d", &ch);

    switch (ch) {
    case 1:
        interactive_mode_print_info();
        break;
    case 2:
        interactive_mode_power_meter();
        break;
    case 3:
        interactive_mode_set_rapl();
        break;
    case 4:
        usage();
        break;
    case 5:
        _exit(0);
    default:
        fprintf(stdout, "%d is an invalid option!\n", ch);
        goto main_menu;

    }

    return 0;
}


void
cmdline(int argc, char **argv)
{
    int             opt;
    char           *vvalue = NULL;
    int             opt_s = 0;
    int             opt_r = 0;
    int             opt_d = 0;
    double          delay_ms_temp = 1000;

    progname = argv[0];

    if (argc < 2) {
        interactive_mode();
        _exit(0);
    }

    while ((opt = getopt(argc, argv, "ie:hr:s:d:p:")) != -1) {
        switch (opt) {
        case 'i':
            print_info = 1;
            break;
        case 'e':
            print_energy_info = 1;
            delay_ms_temp = atoi(optarg);
            if(delay_ms_temp > 50) {
                delay_us = delay_ms_temp * 1000;
            } else {
                fprintf(stdout, "Delay must be greater than 50 ms.\n");
                _exit(-1);
            }
            break;
        case 'p':
            package = atoi(optarg);
            break;
        case 'r':
            svalue = optarg;
            set_rapl = 1;
            opt_r = 1;
            break;
        case 's':
            vvalue = optarg;
            vval = (unsigned int)(atol(vvalue));
            fval = atof(vvalue);
            opt_s = 1;
            break;
        case 'd':
            dvalue = optarg;
            opt_d = 1;
            break;
        case 'h':
            usage();
            _exit(0);
            break;
        default:
            usage();
            _exit(-1);
        }
    }


    if (opt_s || opt_r) {
        if (!(opt_s && opt_r && opt_d)) {
            fprintf(stdout, "Options -r, -d and -s must be used together.\n");
            _exit(-1);
        }
    }
}


int
main(int argc, char **argv)
{
    int i = 0;

    // First init the RAPL library
    if (0 != init_rapl()) {
        fprintf(stdout, "Init failed!\n");
        return MY_ERROR;
    }
    num_node = get_num_rapl_nodes_pkg();

    cmdline(argc, argv);

    // Determine the node(s)/package(s) to access
    int          all_packages = 1;        // access all packages
    unsigned int node = 0;
    if (-1 != package) {
        all_packages = 0;
        if (0 <= package && num_node > package) {
            node = package;
        } else {
            fprintf(stdout, "Invalid package specified. This system has: %u packages.\n", num_node);
            return MY_ERROR;
        }
    }

    unsigned int power_domain = RAPL_PKG;

    if(NULL != dvalue) {
        if (strcmp(dvalue, "PKG") == 0)
            power_domain = RAPL_PKG;
        else if (strcmp(dvalue, "PP0") == 0)
            power_domain = RAPL_PP0;
        else if (strcmp(dvalue, "PP1") == 0)
            power_domain = RAPL_PP1;
        else if (strcmp(dvalue, "DRAM") == 0)
            power_domain = RAPL_DRAM;
        else {
            fprintf(stdout, "Invalid argument power domain\n");
            return MY_ERROR;
        }
    }

    if (print_energy_info)
        do_print_energy_info();

    for (i = node; i < num_node; i++) {
        if (print_info)
            do_print_info(power_domain, i);

        if (set_rapl) {
            if(!is_supported_domain(power_domain)) {
                fprintf(stdout, "\nThe selected power domain is not available on your machine!\n\n");
                return MY_ERROR;
            }

            if (strcmp(svalue, "POWER_LIMIT") == 0) {
                write_rapl_power_limit_limit(power_domain, i, fval);
            } else if (strcmp(svalue, "ENABLE_POWER_LIMIT") == 0) {
                write_rapl_power_limit_enable(power_domain, i, vval);
            } else if (strcmp(svalue, "CLAMPING_LIMIT") == 0) {
                write_rapl_power_limit_clamping(power_domain, i, vval);
            } else if (strcmp(svalue, "TIME_WINDOW") == 0) {
                assert(MY_ERROR != write_rapl_power_limit_time(power_domain, i, fval));
            } else {
                fprintf(stdout, "Invalid argument RAPL_SETTING\n");
                return MY_ERROR;
            }
            do_print_info(power_domain, i);

        }

        if (0 == all_packages)
            break;

    }

    //free memory
    terminate_rapl();
}
