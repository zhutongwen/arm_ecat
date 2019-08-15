/*
   server.cpp

   Test server for the tcpsockets classes which handles connections
   iteratively.

   ------------------------------------------

   Copyright (c) 2013 Vic Hargrave

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <tcp_thread.h>

#include "ecat_master.h"

void *TcpThread(void *data)
{
//    if (argc < 2 || argc > 4)
//    {
//        printf("usage: server <port> [<ip>]\n");
//        exit(1);
//    }

    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (1)//(argc == 3)
    {
        acceptor = new TCPAcceptor(8888, "127.0.0.1");
       // acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);
    }

    if (acceptor->start() == 0)
    {
        while (1)
        {
            stream = acceptor->accept();
            if (stream != NULL)
            {
                ssize_t len;
                char line[256];
                string receive_data;
                while ((len = stream->receive(line, sizeof(line))) > 0)
                {
                    line[len] = 0;
                    receive_data = line;

                    EMaster.cmd.push_back(receive_data);

                    if(receive_data == "start") EMaster.m_state = 0;
                    else if(receive_data == "en") EMaster.m_state = 2;
                    else if(receive_data == "hm") EMaster.m_state = 4;
                    else if(receive_data == "rc") EMaster.m_state = 6;

                    std::cout << "receive cmd: " << receive_data <<std::endl;
                }
                delete stream;
            }
        }
    }
    exit(0);
}

