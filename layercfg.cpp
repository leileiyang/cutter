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


