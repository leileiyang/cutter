#ifndef LAYERCFG_H
#define LAYERCFG_H

#include <QWidget>

#include "dev_cfg/PlcCfg.h"

struct CraftData;

struct JobCfg {
  int craft_level;
  bool no_follow;
  bool keep_air;
  bool no_lift_;
};

namespace Ui {
class LayerCfgForm;
}

class LayerCfg : public QWidget
{
    Q_OBJECT

public:
    explicit LayerCfg(QWidget *parent = 0);
    ~LayerCfg();

public slots:
    void onCuttingUpdate();
    void onPierce1Update();
    void onPierce2Update();
    void onPierce3Update();
signals:
    /* may be used in the future
    void gasCfgChanged();
    void lhcCfgChanged();
    void laserCfgChanged();
    void delayCfgChanged();
    void cfgDataChanged();
    */

    void craftUpdate(int level, const CraftData &data);

private:
    Ui::LayerCfgForm *ui;
};

#endif // PLCCFG_H
