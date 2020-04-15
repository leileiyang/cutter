#include "cuttermainpage.h"
#include "ui_cuttermainpage.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsEllipseItem>
#include <QtGui/QMatrix>

#include "craftparam.h"

const static int kGasMap[] = {
  GAS_AIR,
  GAS_O2,
  GAS_N2,
  GAS_HIGH_AIR,
  GAS_HIGH_O2,
  GAS_HIGH_N2,
};



CutterMainPage::CutterMainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutterMainPage),
    layer_data_changed_(false),
    current_layer_(-1),
    process_cfg_(CRAFT_LAYERS, ProcessCfg()),
    gas_cfg_(CRAFT_LAYERS, GasCfg()),
    lhc_cfg_(CRAFT_LAYERS, FollowerCfg()),
    laser_cfg_(CRAFT_LAYERS, LaserCfg()),
    delay_cfg_(CRAFT_LAYERS, DelayCfg())
{
    ui->setupUi(this);

    current_layer_ = ui->tabWidget->currentIndex();

#define CONNECT_LAYERDATA_UPATE(SENDER) \
    connect(SENDER, SIGNAL(craftUpdate(int,CraftData)), this, \
            SLOT(onCraftUpdate(int,CraftData))); \
\
    connect(SENDER, SIGNAL(processUpdate(ProcessCfg)), this, \
            SLOT(onProcessCfgUpdate(ProcessCfg))); \

   CONNECT_LAYERDATA_UPATE(ui->layer0_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer1_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer2_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer3_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer4_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer5_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer6_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer7_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer8_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer9_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer10_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer11_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer12_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer13_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer14_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer15_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer16_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer17_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer18_data_)
   CONNECT_LAYERDATA_UPATE(ui->layer19_data_)

}

CutterMainPage::~CutterMainPage()
{
    delete ui;
}

bool CutterMainPage::Initialize() {
   if (!service_.Initialize()) {
     return false;
   }
   if (xml_parser_.ParseXml("laser_param.xml")) {
     LoadDeviceCfg();
     LoadLayersData();
     return true;
   }
   return false;
}

void CutterMainPage::LoadDeviceCfg() {
    std::vector<CraftData> craftdatas;
    for (int i = 0; i < CRAFT_LAYERS; i++) {
      xml_parser_.GetLayerData(i, process_cfg_[i], craftdatas);
      CraftDataToDeviceCfg(i,craftdatas);
    }
}

void CutterMainPage::CraftDataToDeviceCfg(int layer,
    const std::vector<CraftData> &craftdatas) {

  for(unsigned int i = 0; i < craftdatas.size(); i++) {
    CraftDataToDeviceCfg(layer, i, craftdatas[i]);
  }
}

void CutterMainPage::CraftDataToDeviceCfg(int layer, int craft_level,
    const CraftData &craftdata) {

    assert(craft_level < CRAFT_LEVELS);

    gas_cfg_[layer].gas_[craft_level] = kGasMap[craftdata.gas];
    gas_cfg_[layer].pressure_[craft_level] = craftdata.pressure;

    lhc_cfg_[layer].incr_enable_[craft_level] = craftdata.enable_incr;
    lhc_cfg_[layer].incr_time_[craft_level] = craftdata.incr_time;
    lhc_cfg_[layer].height_[craft_level] = craftdata.jet_height;

    laser_cfg_[layer].peak_power_[craft_level] = craftdata.power;
    laser_cfg_[layer].duty_ratio_[craft_level] = craftdata.ratio;
    laser_cfg_[layer].pulse_frequency_[craft_level] = craftdata.frequency;

    delay_cfg_[layer].stay_[craft_level] = craftdata.stay;
    delay_cfg_[layer].blow_enable_[craft_level] = craftdata.enable_blow;
    delay_cfg_[layer].laser_off_blow_time_[craft_level] = craftdata.blow_time;
}

void CutterMainPage::LoadLayersData() {
    // Init layers cata
    ui->layer0_data_->InitLayerCfg(process_cfg_[0], DeviceCfgToCrafData(0));
    ui->layer1_data_->InitLayerCfg(process_cfg_[1], DeviceCfgToCrafData(1));
    ui->layer2_data_->InitLayerCfg(process_cfg_[2], DeviceCfgToCrafData(2));
    ui->layer3_data_->InitLayerCfg(process_cfg_[3], DeviceCfgToCrafData(3));
    ui->layer4_data_->InitLayerCfg(process_cfg_[4], DeviceCfgToCrafData(4));
    ui->layer5_data_->InitLayerCfg(process_cfg_[5], DeviceCfgToCrafData(5));
    ui->layer6_data_->InitLayerCfg(process_cfg_[6], DeviceCfgToCrafData(6));
    ui->layer7_data_->InitLayerCfg(process_cfg_[7], DeviceCfgToCrafData(7));
    ui->layer8_data_->InitLayerCfg(process_cfg_[8], DeviceCfgToCrafData(8));
    ui->layer9_data_->InitLayerCfg(process_cfg_[9], DeviceCfgToCrafData(9));
    ui->layer10_data_->InitLayerCfg(process_cfg_[10], DeviceCfgToCrafData(10));
    ui->layer11_data_->InitLayerCfg(process_cfg_[11], DeviceCfgToCrafData(11));
    ui->layer12_data_->InitLayerCfg(process_cfg_[12], DeviceCfgToCrafData(12));
    ui->layer13_data_->InitLayerCfg(process_cfg_[13], DeviceCfgToCrafData(13));
    ui->layer14_data_->InitLayerCfg(process_cfg_[14], DeviceCfgToCrafData(14));
    ui->layer15_data_->InitLayerCfg(process_cfg_[15], DeviceCfgToCrafData(15));
    ui->layer16_data_->InitLayerCfg(process_cfg_[16], DeviceCfgToCrafData(16));
    ui->layer17_data_->InitLayerCfg(process_cfg_[17], DeviceCfgToCrafData(17));
    ui->layer18_data_->InitLayerCfg(process_cfg_[18], DeviceCfgToCrafData(18));
    ui->layer19_data_->InitLayerCfg(process_cfg_[19], DeviceCfgToCrafData(19));
}

std::vector<CraftData> CutterMainPage::DeviceCfgToCrafData(int layer) {
    std::vector<CraftData> datas;
    CraftData craftdata;
    for (int i = 0; i < CRAFT_LEVELS; i++) {
        craftdata.gas = gas_cfg_[layer].gas_[i] - GAS_AIR;
        craftdata.pressure = gas_cfg_[layer].pressure_[i];

        craftdata.enable_incr = lhc_cfg_[layer].incr_enable_[i];
        craftdata.incr_time = lhc_cfg_[layer].incr_time_[i];
        craftdata.jet_height = lhc_cfg_[layer].height_[i];

        craftdata.power = laser_cfg_[layer].peak_power_[i];
        craftdata.ratio = laser_cfg_[layer].duty_ratio_[i];
        craftdata.frequency = laser_cfg_[layer].pulse_frequency_[i];

        craftdata.stay = delay_cfg_[layer].stay_[i];
        craftdata.enable_blow = delay_cfg_[layer].blow_enable_[i];
        craftdata.blow_time = delay_cfg_[layer].laser_off_blow_time_[i];
        datas.push_back(craftdata);
    }
    return datas;
}

void CutterMainPage::onPubCfgData(int index) {
    if (layer_data_changed_) {
        layer_data_changed_ = false;
        // publish cfg data
        service_.PubGasCfg(current_layer_, gas_cfg_[current_layer_]);
        service_.PubLhcCfg(current_layer_, lhc_cfg_[current_layer_]);
    }
    current_layer_ = index;
}

void CutterMainPage::onProcessCfgUpdate(const ProcessCfg &data) {
    process_cfg_[current_layer_].no_lift = data.no_lift;
    process_cfg_[current_layer_].no_follow = data.no_follow;
    process_cfg_[current_layer_].skip = data.skip;
    process_cfg_[current_layer_].keep_air = data.keep_air;
    process_cfg_[current_layer_].pre_pierce = data.pre_pierce;
    process_cfg_[current_layer_].striping = data.striping;
    process_cfg_[current_layer_].cooling = data.cooling;
    process_cfg_[current_layer_].cutting = data.cutting;
    process_cfg_[current_layer_].craft_level = data.craft_level;

    layer_data_changed_ = true;
}

void CutterMainPage::onCraftUpdate(int level, const CraftData &data) {
    CraftDataToDeviceCfg(current_layer_, level, data);
    layer_data_changed_ = true;
}

void CutterMainPage::switchToProcess() {
    ui->container_->setCurrentIndex(0);
}

void CutterMainPage::switchToLayers() {
    ui->container_->setCurrentIndex(1);
    LoadLayersData();
}

void CutterMainPage::onOpen() {
    service_.onOpen("/home/debian/linuxcnc/nc_files/examples/rect.ngc");
}

void CutterMainPage::onStart() {
  service_.onStart();
}

void CutterMainPage::onStop() {
  service_.onStop();
}

void CutterMainPage::onResume() {
  service_.onResume();
}

void CutterMainPage::onPause() {
  service_.onPause();
}
