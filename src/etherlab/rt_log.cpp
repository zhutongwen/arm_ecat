#include "rt_log.h"

std::string space = " ";

std::vector<std::string> vLogdata;
std::vector<std::string> vPrintdata;
void *RTPrintThread(void *data)
{
    static std::ofstream RT_print_file;
    static std::ofstream RT_log_file;

    RT_print_file.close();
    RT_log_file.close();

    //RT_print_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
    RT_print_file.open("./RT_print_file.txt", std::ios::out | std::ios::trunc);

    //log_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
    RT_log_file.open("./RT_log_file.txt", std::ios::out | std::ios::trunc);

    if(!RT_print_file.is_open())
    {
        std::cout << "start RTPrintThread error!!!" << std::endl;
        return 0;
    }

    if(!RT_log_file.is_open())
    {
        std::cout << "start RTLogThread error!!!" << std::endl;
        return 0;
    }
    {
        std::cout << "start RTPrintThread..." << std::endl;
        std::cout << "start RTLogThread..." << std::endl;
        while(1)
        {
            uint32_t u32PrintSize = vPrintdata.size();
            if(u32PrintSize)
            {
                for(uint32_t i=0; i< u32PrintSize; i++)
                {
                    std::cout << vPrintdata.at(i);
                    RT_print_file << vPrintdata.at(i);
                }
                vPrintdata.erase(vPrintdata.begin(), vPrintdata.begin()+u32PrintSize);
                RT_print_file.flush();
            }

            uint32_t u32logsize = vLogdata.size();
            if(u32logsize)
            {
                for(uint32_t i=0; i< u32logsize; i++)
                {
                    RT_log_file << vLogdata.at(i);
                }
                vLogdata.erase(vLogdata.begin(), vLogdata.begin()+u32logsize);
                RT_log_file.flush();
            }
            sleep(1);
        }
    }
}


