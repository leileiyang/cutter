#include "controller.h"

#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>


Controller::Controller(): context_(NULL), requester_(NULL), publisher_(NULL),
    ack_subscriber_(NULL)
{

}

Controller::~Controller() {
    zmq_close(requester_);
    zmq_close(publisher_);
    zmq_close(ack_subscriber_);
    zmq_ctx_destroy(context_);
}

bool Controller::PreparePubData() {
    int n = 0;
    do {
        zmq_msg_t hello;
        zmq_msg_init_size(&hello, strlen("GAS"));
        memcpy(zmq_msg_data(&hello), "GAS", strlen("GAS"));
        int rc = zmq_msg_send(&hello, publisher_, 0);
        zmq_msg_close(&hello);
        rc = 0;

        zmq_msg_t reply;
        rc = zmq_msg_init(&reply);
        rc = zmq_msg_recv(&reply, ack_subscriber_, ZMQ_DONTWAIT);
        sleep(1);
        if (rc >= 0) {
            break;
        }
        n++;
    } while (n < 5);
    if (n < 5) {
        return true;
    } else {
        return false;
    }
}

bool Controller::Initialize() {
    context_ = zmq_ctx_new();
    requester_ = zmq_socket(context_, ZMQ_REQ);
    publisher_ = zmq_socket(context_, ZMQ_PUB);
    ack_subscriber_ = zmq_socket(context_, ZMQ_REP);

    zmq_bind(publisher_, "tcp:://*:6001");
    zmq_bind(ack_subscriber_, "tcp://*:6000");

    if (!context_ || !requester_ || !publisher_ || !ack_subscriber_) {
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

int Controller::AutoMode() {
    PlcCmd cmd = {TASK_SWITCH_MODE, "auto"};
    return CommonCommand(cmd);
}

int Controller::MdiMode() {
    PlcCmd cmd = {TASK_SWITCH_MODE, "mdi"};
    return CommonCommand(cmd);
}

int Controller::ManualMode() {
    PlcCmd cmd = {TASK_SWITCH_MODE, "manual"};
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
