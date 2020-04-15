#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <zmq.h>
#include "dev_cfg/PlcCfg.h"
#include "dev_cfg/FollowerCfg.h"
#include "dev_cfg/GasCfg.h"
#include "dev_cfg/TaskStatus.h"

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
    int OpenFile(std::string file_name);
    int Start();
    int Pause();
    int Stop();
    int Resume();
    int AutoMode();
    int MdiMode();
    int ManualMode();
    int CommonCommand(const PlcCmd &cmd);
    bool PreparePubData();
    int PubGasCfg(int layer, const GasCfg &gas_cfg);
    int PubLhcCfg(int layer, const FollowerCfg &lhc_cfg);
    int GetTaskStatus(TaskStatus &task_status);

private:
    void *context_;
    void *requester_;
    void *publisher_;
    void *ack_subscriber_;
    void *status_subscriber_;

    int SendMessage(void *socket, const char *content, int flags);
    int PubCfg(const char *device, int layer, const std::ostringstream &ofs);

};

#endif // CONTROLLER_H
