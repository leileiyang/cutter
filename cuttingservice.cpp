#include "cuttingservice.h"

CuttingService::CuttingService(QObject *parent) : QObject(parent),
    sampler_(*this) {}


CuttingService::~CuttingService() {
    sampler_.quit();
    sampler_.wait();
}

bool CuttingService::Initialize() {
  if (controller_.Initialize()) {
    if (!controller_.PreparePubData()) {
      return false;
    }
    sampler_.start();
    return true;
  }
  return false;
}

void CuttingService::onOpen(const std::string &file_name) {
    controller_.OpenFile(file_name);
}

void CuttingService::onStart() {
  controller_.AutoMode();
  controller_.Start();
}

void CuttingService::onStop() {
  controller_.Stop();
  controller_.ManualMode();
}

void CuttingService::onResume() {
  controller_.Resume();
}

void CuttingService::onPause() {
  controller_.Pause();
}

int CuttingService::PubGasCfg(int layer, const GasCfg &gas_cfg) {
   return controller_.PubGasCfg(layer, gas_cfg);
}

int CuttingService::PubLhcCfg(int layer, const FollowerCfg &lhc_cfg) {
   return controller_.PubLhcCfg(layer, lhc_cfg);
}

void CuttingService::UpdateTaskStatus() {
   controller_.GetTaskStatus(task_status_);
}
