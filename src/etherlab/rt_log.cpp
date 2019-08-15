#include "rt_log.h"

std::string space = " ";

std::vector<std::string> vLogdata;
std::vector<std::string> vPrintdata;



uint32_t log_write_counter = 0;
uint32_t log_read_cunter = 0;
log_data_t log_data[LOG_BUFF];

uint32_t printf_write_counter = 0;
uint32_t printf_read_cunter = 0;
std::string print_data[PRINT_BUFF];

void *RTPrintThread(void *data)
{

    std::ofstream rt_print_file;
    std::ofstream rt_log_file;

    rt_print_file.close();
    rt_log_file.close();

    rt_print_file.open("/tmp/print_file.txt", std::ios::out | std::ios::trunc);
//    rt_print_file.open("./print_file.txt", std::ios::out | std::ios::trunc);

    rt_log_file.open("/tmp/log_file.txt", std::ios::out | std::ios::trunc);
//    rt_log_file.open("./log_file.txt", std::ios::out | std::ios::trunc);

    if(!rt_print_file.is_open())
    {
        std::cout << "start RTPrintThread error!!!" << std::endl;
        return 0;
    }

    if(!rt_log_file.is_open())
    {
        std::cout << "start RTLogThread error!!!" << std::endl;
        return 0;
    }

    {
        std::cout << "start RTPrintThread..." << std::endl;
        std::cout << "start RTLogThread..." << std::endl;
        while(1)
        {
            //rtprintf
            while(printf_read_cunter < printf_write_counter)
            {
                rt_print_file << print_data[printf_read_cunter % PRINT_BUFF];
                std::cout << print_data[printf_read_cunter % PRINT_BUFF];
                printf_read_cunter++;
            }
            //log
            while(log_write_counter > log_read_cunter)
            {
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].counter << ' ';
                for(int i=0; i<6; i++)
                {
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].target_pos << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].actual_pos << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].target_vel << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].actual_vel << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].actual_cur << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].mode_display << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].status_word << ' ';
                    rt_log_file << log_data[log_read_cunter % LOG_BUFF].motor[i].error_code << ' ';
                }
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.gx << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.gy << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.gz << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.ax << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.ay << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.az << ' ';
                rt_log_file << log_data[log_read_cunter % LOG_BUFF].imu.counter;
                log_read_cunter++;
                rt_log_file << std::endl;
            }
            usleep(10000);
        }
    }
}


