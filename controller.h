#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <zmq.h>
#include "dev_cfg/PlcCfg.h"

#define SEND_CMD_SUCCESS 0
#define SEND_CMD_FAIL   -1
#define SEND_CMD_TIMEOUT -2


class Controller
{
public:
    Controller();
    ~Controller();
    bool Initialize();
    int Start();
    int Pause();
    int Stop();
    int Resume();
    int CommonCommand(const PlcCmd &cmd);

private:
    void *context_;
    void *requester_;
};

#endif // CONTROLLER_H
