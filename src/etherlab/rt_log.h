#ifndef RT_LOG_H
#define RT_LOG_H


#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fstream>
#include <thread>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <string>
#include <libgen.h>
#include <ios>
#include <iostream>
#include <fstream>

#include <vector>

extern std::string space;

#define RT_PRINT(x)     {vPrintdata.push_back( (char*)__FILE__ + space + std::to_string(__LINE__) + ": " + x + '\n');}

extern std::vector<std::string> vLogdata;
extern std::vector<std::string> vPrintdata;
void *RTPrintThread(void *data);

//////////////////////////////////////////////////////////////



#endif
