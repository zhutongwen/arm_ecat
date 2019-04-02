#ifndef ETHERCAT_SLAVE_H
#define ETHERCAT_SLAVE_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include "ecrt.h"

//extern std::vector<ec_pdo_entry_reg_t> EcatSlave::domain_regs;

class EcatSlave
{
public:
    EcatSlave();
    EcatSlave(uint16_t alias, uint16_t position);
    virtual ~EcatSlave() {}

    static std::vector<ec_pdo_entry_reg_t> domain_regs;
};

#endif
