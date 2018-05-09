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

   controller_.Initialize();
   if (xml_parser_.ParseXml("laser_param.xml")) {
       ;
   }
}

CutterMainPage::~CutterMainPage()
{
    delete ui;
}

void CutterMainPage::onPubCfgData(int index) {
    if (layer_data_changed_) {
        layer_data_changed_ = false;
        // publish cfg data
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
}

void CutterMainPage::onCraftUpdate(int level, const CraftData &data) {
    assert(level < CRAFT_LEVELS);

    gas_cfg_[current_layer_].gas_[level] = kGasMap[data.gas];
    gas_cfg_[current_layer_].pressure_[level] = data.pressure;

    lhc_cfg_[current_layer_].incr_enable_[level] = data.enable_incr;
    lhc_cfg_[current_layer_].incr_time_[level] = data.incr_time;
    lhc_cfg_[current_layer_].height_[level] = data.lift_height;

    laser_cfg_[current_layer_].peak_power_[level] = data.power;
    laser_cfg_[current_layer_].duty_ratio_[level] = data.ratio;
    laser_cfg_[current_layer_].pulse_frequency_[level] = data.frequency;

    delay_cfg_[current_layer_].stay_[level] = data.stay;
    delay_cfg_[current_layer_].blow_enable_[level] = data.enable_blow;
    delay_cfg_[current_layer_].laser_off_blow_time_[level] = data.blow_time;

    layer_data_changed_ = true;

}


void CutterMainPage::switchToProcess() {
    ui->container_->setCurrentIndex(0);
}

void CutterMainPage::switchToLayers() {
    ui->container_->setCurrentIndex(1);
    // Init layers cata
    ProcessCfg process;
    std::vector<CraftData> craftdata;

#define LOAD_LAYER_DATA(NO) do { \
    xml_parser_.GetLayerData(NO, process, craftdata); \
    ui->layer##NO##_data_->InitLayerCfg(process, craftdata); \
} while (0)

    LOAD_LAYER_DATA(0);
    LOAD_LAYER_DATA(1);
    LOAD_LAYER_DATA(2);
    LOAD_LAYER_DATA(3);
    LOAD_LAYER_DATA(4);
    LOAD_LAYER_DATA(5);
    LOAD_LAYER_DATA(6);
    LOAD_LAYER_DATA(7);
    LOAD_LAYER_DATA(8);
    LOAD_LAYER_DATA(9);
    LOAD_LAYER_DATA(10);
    LOAD_LAYER_DATA(11);
    LOAD_LAYER_DATA(12);
    LOAD_LAYER_DATA(13);
    LOAD_LAYER_DATA(14);
    LOAD_LAYER_DATA(15);
    LOAD_LAYER_DATA(16);
    LOAD_LAYER_DATA(17);
    LOAD_LAYER_DATA(18);
    LOAD_LAYER_DATA(19);
}

void CutterMainPage::onOpen() {
    controller_.OpenFile("/home/debian/linuxcnc/nc_files/examples/result.ngc");
}

void CutterMainPage::onStart() {
  controller_.AutoMode();
  controller_.Start();
}

void CutterMainPage::onStop() {
  controller_.Stop();
  controller_.ManualMode();
}

void CutterMainPage::onResume() {
  controller_.Resume();
}

void CutterMainPage::onPause() {
  controller_.Pause();
}
