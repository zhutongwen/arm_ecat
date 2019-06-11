#include "rt_log.h"

std::string space = " ";

std::vector<std::string> vPrintdata;
void *RTPrintThread(void *data)
{
    static std::ofstream RT_print_file;//("RT_print_file.txt");
    RT_print_file.close();
//    RT_print_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
    RT_print_file.open("./RT_print_file.txt", std::ios::out | std::ios::trunc);
    if(RT_print_file.is_open())
    {
        std::cout << "start RTPrintThread..." << std::endl;
        while(1)
        {
            uint32_t logsize = vPrintdata.size();
            if(logsize)
            {
                std::cout << "printdata size: " <<vPrintdata.size() << std::endl;
                for(uint32_t i=0; i< logsize; i++)
                {
                    std::cout << vPrintdata.at(i);
                    RT_print_file << vPrintdata.at(i);
                }
                vPrintdata.erase(vPrintdata.begin(), vPrintdata.begin()+logsize);
                RT_print_file.flush();
            }
            sleep(1);
        }
    }
    else
    {
        std::cout << "start RTPrintThread error!!!" << std::endl;
    }
    return 0;
}

std::vector<std::string> vLogdata;
void *RTLogThread(void *data)
{
    static std::ofstream RT_log_file;//("log_file.txt");
    RT_log_file.close();
//    log_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
    RT_log_file.open("./RT_log_file.txt", std::ios::out | std::ios::trunc);
    if(RT_log_file.is_open())
    {
        std::cout << "start RTLogThread..." << std::endl;
        while(1)
        {
            uint32_t logsize = vLogdata.size();
            if(logsize)
            {
                std::cout << "logdata size: " <<vLogdata.size() << std::endl;
                for(uint32_t i=0; i< logsize; i++)
                {
                    RT_log_file << vLogdata.at(i);
                }
                vLogdata.erase(vLogdata.begin(), vLogdata.begin()+logsize);
                RT_log_file.flush();
            }
            sleep(1);
        }
    }
    else
    {
       std::cout << "start RTLogThread error!!!" << std::endl;
    }
    return 0;
}



