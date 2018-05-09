#include "layercfg.h"
#include "ui_layercfg.h"

#include <assert.h>

LayerCfg::LayerCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerCfgForm)
{
    ui->setupUi(this);
}

LayerCfg::~LayerCfg()
{
    delete ui;
}

 void LayerCfg::onCuttingUpdate() {
     CraftData data;
     ui->cutting1_->onDataUpdate(data);
     emit craftUpdate(CRAFT_CUTTING, data);
 }

 void LayerCfg::onPierce1Update() {
     CraftData data;
     ui->pierce1_->onDataUpdate(data);
     emit craftUpdate(CRAFT_FIRST, data);
 }

 void LayerCfg::onPierce2Update() {
     CraftData data;
     ui->pierce2_->onDataUpdate(data);
     emit craftUpdate(CRAFT_SECOND, data);
 }

 void LayerCfg::onPierce3Update() {
     CraftData data;
     ui->pierce3_->onDataUpdate(data);
     emit craftUpdate(CRAFT_THIRD, data);
 }

 void LayerCfg::InitProcessCfg(const ProcessCfg &process_cfg) {
     process_cfg_ = process_cfg;
     ui->no_lift_chb_->setChecked(process_cfg_.no_lift);
     ui->no_follow_chb_->setChecked(process_cfg_.no_follow);
     ui->keep_air_chb_->setChecked(process_cfg_.keep_air);
     ui->skip_chb_->setChecked(process_cfg_.skip);

     ui->striping_chb_->setChecked(process_cfg_.striping);
     ui->pre_pierce_chb_->setChecked(process_cfg_.pre_pierce);
     ui->cutting_chb_->setChecked(process_cfg_.cutting > 1);
     ui->cooling_chb_->setChecked(process_cfg_.cooling);
     ui->cutting_spb_->setValue(process_cfg_.cutting -1);
     switch (process_cfg_.craft_level) {
     case CRAFT_FIRST:
         ui->pierce1_rb_->setChecked(true);
         break;
     case CRAFT_SECOND:
         ui->pierce2_rb_->setChecked(true);
         break;
     case CRAFT_THIRD:
         ui->pierce3_rb_->setChecked(true);
         break;
     case CRAFT_CUTTING:
     default:
         ui->no_pierce_rb_->setChecked(true);
         break;
     }
 }

 void LayerCfg::InitLayerCfg(const ProcessCfg &process_cfg, \
                      std::vector<CraftData> &datas) {

     assert(datas.size() == 4);
     InitProcessCfg(process_cfg);
     ui->cutting1_->onDataUpdate(datas[CRAFT_CUTTING], false);
     ui->pierce1_->onDataUpdate(datas[CRAFT_FIRST], false);
     ui->pierce2_->onDataUpdate(datas[CRAFT_SECOND], false);
     ui->pierce3_->onDataUpdate(datas[CRAFT_THIRD], false);
 }

 void LayerCfg::onProcessUpdate() {
     process_cfg_.no_lift = ui->no_lift_chb_->isChecked();
     process_cfg_.no_follow = ui->no_follow_chb_->isChecked();
     process_cfg_.keep_air = ui->keep_air_chb_->isChecked();
     process_cfg_.skip = ui->skip_chb_->isChecked();

     process_cfg_.striping = ui->striping_chb_->isChecked() && \
         !process_cfg_.skip;

     process_cfg_.pre_pierce = ui->pre_pierce_chb_->isChecked() && \
         !process_cfg_.skip;

     if (!process_cfg_.skip) {
       process_cfg_.cutting = ui->cutting_chb_->isChecked() ? \
           1 + ui->cutting_spb_->value() : 1;

     } else {
       process_cfg_.cutting = 0;
     }
     process_cfg_.cooling = ui->cooling_chb_->isChecked() && \
         !process_cfg_.skip;

     if (ui->no_pierce_rb_->isChecked()) {
         process_cfg_.craft_level = CRAFT_CUTTING;
     } else if (ui->pierce1_rb_->isChecked()) {
         process_cfg_.craft_level = CRAFT_FIRST;
     } else if (ui->pierce2_rb_->isChecked()) {
         process_cfg_.craft_level = CRAFT_SECOND;
     } else if (ui->pierce3_rb_->isChecked()) {
         process_cfg_.craft_level = CRAFT_THIRD;
     }
     emit processUpdate(process_cfg_);
 }

