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
    cfg_data_changed_(false),
    current_layer_(-1),
    gas_cfg_(CRAFT_LAYERS, GasCfg()),
    lhc_cfg_(CRAFT_LAYERS, FollowerCfg()),
    laser_cfg_(CRAFT_LAYERS, LaserCfg()),
    delay_cfg_(CRAFT_LAYERS, DelayCfg())
{
    ui->setupUi(this);

    current_layer_ = ui->tabWidget->currentIndex();
    connect(ui->layer1_data_, SIGNAL(craftUpdate(int,CraftData)), this,
            SLOT(onCraftUpdate(int,CraftData)));

   controller_.Initialize();
}

CutterMainPage::~CutterMainPage()
{
    delete ui;
}

void CutterMainPage::onPubCfgData(int index) {
    if (cfg_data_changed_) {
        cfg_data_changed_ = false;
        // publish cfg data
    }
    current_layer_ = index;
}

void CutterMainPage::onCraftUpdate(int level, const CraftData &data) {
    assert(level < CRAFT_LEVELS);

    gas_cfg_[current_layer_].gas_[level] = kGasMap[data.gas];
    gas_cfg_[current_layer_].pressure_[level] = data.pressure;

    lhc_cfg_[current_layer_].incr_enable_[level] = data.enable_incr;
    lhc_cfg_[current_layer_].incr_time_[level] = data.incr_time;
    lhc_cfg_[current_layer_].height_[level] = data.height;

    laser_cfg_[current_layer_].peak_power_[level] = data.power;
    laser_cfg_[current_layer_].duty_ratio_[level] = data.ratio;
    laser_cfg_[current_layer_].pulse_frequency_[level] = data.frequency;

    delay_cfg_[current_layer_].stay_[level] = data.stay;
    delay_cfg_[current_layer_].blow_enable_[level] = data.enable_blow;
    delay_cfg_[current_layer_].laser_off_blow_[level] = data.blow_time;

    cfg_data_changed_ = true;

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
