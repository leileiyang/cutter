#include "controller.h"

#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>


Controller::Controller(): context_(NULL), requester_(NULL)
{

}

Controller::~Controller() {
    zmq_close(requester_);
    zmq_ctx_destroy(context_);
}


bool Controller::Initialize() {
    context_ = zmq_ctx_new();
    requester_ = zmq_socket(context_, ZMQ_REQ);
    if (!context_ || !requester_) {
        return false;
    }
    if (zmq_connect(requester_, "tcp://10.1.0.138:5555") != 0) {
        return false;
    }
    int opt = 1000;
    zmq_setsockopt(requester_, ZMQ_RCVTIMEO, &opt, sizeof(opt));
    return true;
}

int Controller::Start() {
    PlcCmd cmd = {TASK_START, ""};
    return CommonCommand(cmd);
}

int Controller::Pause() {
    PlcCmd cmd = {TASK_PAUSE, ""};
    return CommonCommand(cmd);
}


int Controller::Stop() {
    PlcCmd cmd = {TASK_STOP, ""};
    return CommonCommand(cmd);
}

int Controller::Resume() {
    PlcCmd cmd = {TASK_RESUME, ""};
    return CommonCommand(cmd);
}

int Controller::CommonCommand(const PlcCmd &cmd) {
    std::ostringstream ofs;
    boost::archive::text_oarchive oa(ofs);
    oa << cmd;
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, ofs.str().length());
    memcpy(zmq_msg_data(&msg), ofs.str().c_str(), ofs.str().length());
    int size = zmq_msg_send(&msg, requester_, 0);
    zmq_msg_close(&msg);
    if (size == -1) {
        return SEND_CMD_FAIL;
    }

    zmq_msg_t ack;
    int rc = zmq_msg_init(&ack);
    assert(rc == 0);
    rc = zmq_msg_recv(&ack, requester_, 0);
    zmq_msg_close(&ack);

    if (rc == -1 && errno == EAGAIN) {
        return SEND_CMD_TIMEOUT;
    } else if (rc == -1) {
        return SEND_CMD_FAIL;
    } else {
        return SEND_CMD_SUCCESS;
    }
}
