#ifndef CUTTERMAINPAGE_H
#define CUTTERMAINPAGE_H

#include <QWidget>
#include <vector>
#include "dev_cfg/GasCfg.h"
#include "dev_cfg/LaserCfg.h"
#include "dev_cfg/FollowerCfg.h"
#include "dev_cfg/DelayCfg.h"
#include "dev_cfg/PlcCfg.h"

#include "xmlparser/LaserXmlParser.h"
#include "cuttingservice.h"

namespace Ui {
class CutterMainPage;
}

struct CraftData;

class CutterMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit CutterMainPage(QWidget *parent = 0);
    ~CutterMainPage();

    bool Initialize();

public slots:
    void onPubCfgData(int index);
    void onCraftUpdate(int level, const CraftData &data);
    void onProcessCfgUpdate(const ProcessCfg &data);

    void onOpen();
    void onStart();
    void onStop();
    void onResume();
    void onPause();

    // switch page
    void switchToProcess();
    void switchToLayers();

private:
    Ui::CutterMainPage *ui;

    void LoadDeviceCfg();
    void LoadLayersData();
    void CraftDataToDeviceCfg(int layer, const std::vector<CraftData> &craftdatas);
    void CraftDataToDeviceCfg(int layer, int craft_level, const CraftData &craftdata);
    std::vector<CraftData> DeviceCfgToCrafData(int layer);

    bool layer_data_changed_;
    int current_layer_;

    std::vector<ProcessCfg> process_cfg_;
    std::vector<GasCfg> gas_cfg_;
    std::vector<FollowerCfg> lhc_cfg_;
    std::vector<LaserCfg> laser_cfg_;
    std::vector<DelayCfg> delay_cfg_;
    CuttingService service_;
    LaserXmlParser xml_parser_;
};

#endif // CUTTERMAINPAGE_H
