#include "sampler.h"

#include "cuttingservice.h"

Sampler::Sampler(CuttingService &service, QObject *parent):
    QThread(parent), service_(service), exit_(false) {}

void Sampler::Exit() {
  exit_ = true;
}


void Sampler::run() {
  while (!exit_) {
    service_.UpdateTaskStatus();
    sleep(0.01);
  }
}
