/*
   client.cpp

   Test client for the tcpsockets classes. 

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

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"
#include <time.h>
#include <iostream>


using namespace std;

int main(int argc, char** argv)
{
//    if (argc != 3)
//    {
//        printf("usage: %s <port> <ip>\n", argv[0]);
//        exit(1);
//    }

    string message;
    TCPConnector* connector = new TCPConnector();
//    TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
    TCPStream* stream = connector->connect("127.0.0.1", 8888);


    message = "dsds";
    if (stream)
    {
        stream->send(argv[1]);
    }
    exit(0);
}
