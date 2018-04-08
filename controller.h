#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <zmq.h>
#include "dev_cfg/PlcCfg.h"

#define SEND_CMD_SUCCESS 0
#define SEND_CMD_FAIL   -1
#define SEND_CMD_TIMEOUT -2

enum TASK_MODE {
    TASK_MODE_AUTO,
    TASK_MODE_MDI,
    TASK_MODE_MANUAL,
};


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
    int AutoMode();
    int MdiMode();
    int ManualMode();
    int CommonCommand(const PlcCmd &cmd);
    bool PreparePubData();

private:
    void *context_;
    void *requester_;
    void *publisher_;
    void *ack_subscriber_;
};

#endif // CONTROLLER_H
