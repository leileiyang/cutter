#ifndef CUTTINGSERVICE_H
#define CUTTINGSERVICE_H

#include <QObject>

#include <string>

#include "controller.h"
#include "sampler.h"

class CuttingService : public QObject
{
    Q_OBJECT
public:
    explicit CuttingService(QObject *parent = 0);
    ~CuttingService();
    bool Initialize();
    void UpdateTaskStatus();

signals:

public slots:
    void onOpen(const std::string &file_name);
    void onStart();
    void onStop();
    void onResume();
    void onPause();

    int PubGasCfg(int layer, const GasCfg &gas_cfg);
    int PubLhcCfg(int layer, const FollowerCfg &lhc_cfg);

private:
  Controller controller_;
  TaskStatus task_status_;
  Sampler sampler_;

};

#endif // CUTTINGSERVICE_H
