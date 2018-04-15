#ifndef LAYERCFG_H
#define LAYERCFG_H

#include <QWidget>

#include <vector>

#include "dev_cfg/PlcCfg.h"

struct CraftData;

namespace Ui {
class LayerCfgForm;
}

class LayerCfg : public QWidget
{
    Q_OBJECT

public:
    explicit LayerCfg(QWidget *parent = 0);
    ~LayerCfg();

    void InitProcessCfg(const ProcessCfg &process_cfg);
    void InitLayerCfg(const ProcessCfg &process_cfg, \
                      std::vector<CraftData> &datas);

public slots:
    void onCuttingUpdate();
    void onPierce1Update();
    void onPierce2Update();
    void onPierce3Update();
    void onProcessUpdate();
signals:
    /* may be used in the future
    void gasCfgChanged();
    void lhcCfgChanged();
    void laserCfgChanged();
    void delayCfgChanged();
    void cfgDataChanged();
    */

    void craftUpdate(int level, const CraftData &data);
    void processUpdate(const ProcessCfg &process_cfg);

private:
    Ui::LayerCfgForm *ui;

    ProcessCfg process_cfg_;

};

#endif // PLCCFG_H
