#include "GasCfg.h"
#include <zmq.h>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <string.h>

int main() {
  void *ctx = zmq_ctx_new();
  void *subscriber = zmq_socket(ctx, ZMQ_SUB);
  zmq_connect(subscriber, "tcp://localhost:6001");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "GAS", strlen("GAS"));

  zmq_msg_t hello;
  zmq_msg_init(&hello);
  zmq_msg_recv(&hello, subscriber, 0);

  void *sync = zmq_socket(ctx, ZMQ_REQ);
  zmq_connect(sync, "tcp://localhost:6000");
  zmq_msg_t reply;
  zmq_msg_init_size(&reply, strlen("Hello"));
  memcpy(zmq_msg_data(&reply), "Hello", strlen("Hello"));
  int rc = zmq_msg_send(&reply, sync, 0);
  std::cout << "rc: " << rc << std::endl;

  int count = 0;
  while (1) {
    int part_no = 0;
    do {
      sleep(0.1);
      zmq_msg_t part;
      int rc = zmq_msg_init(&part);
      assert(rc == 0);
      rc = zmq_msg_recv(&part, subscriber, ZMQ_DONTWAIT);
      if (rc < 0) {
        break;
      }
      part_no++;
      if (part_no == 2) {
        std::string buffer((char *)zmq_msg_data(&part));
        std::istringstream ifs(buffer);
        boost::archive::text_iarchive ia(ifs);
        GasCfg re_cfg;
        ia >> re_cfg;
        re_cfg.Show();
        count++;
        std::cout << "count: " << count << std::endl;
      }
      zmq_msg_close(&part);
      if (!zmq_msg_more(&part)) {
        break;
      }
    } while (true);
   }
  zmq_close(subscriber);
  return 0;
}

