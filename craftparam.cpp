#include "craftparam.h"
#include "ui_craftparam.h"

CraftParam::CraftParam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CraftParam)
{
    ui->setupUi(this);
    ui->progressive_time_->setEnabled(ui->enable_progressive_->isChecked());
    ui->blow_time_->setEnabled(ui->enable_blow_->isChecked());
}

CraftParam::~CraftParam()
{
    delete ui;
}

void CraftParam::onDisable(bool disable) {
    setDisabled(disable);
}

void CraftParam::onEnable(bool enable) {
    setEnabled(enable);
}

void CraftParam::onDataUpdate() {
    data_.enable_incr = ui->enable_progressive_->isChecked();
    data_.incr_time = ui->progressive_time_->value();
    data_.gas = ui->gas_type_->currentIndex();
    data_.pressure = ui->pressure_->value();
    data_.lift_height = ui->height_->value();
    data_.power = ui->power_->value();
    data_.ratio = ui->ratio_->value();
    data_.frequency = ui->frequency_->value();
    data_.enable_blow = ui->enable_blow_->isChecked();
    data_.blow_time = ui->blow_time_->value();
    data_.focus = ui->focus_->value();
    data_.stay = ui->stay_time_->value();
    emit dataChanged();
}

void CraftParam::onDataUpdate(CraftData &data, bool dir) {
    if (dir) {
        data = data_;
    } else {
        data_ = data;
        ui->enable_progressive_->setChecked(data_.enable_incr);
        ui->progressive_time_->setValue(data_.incr_time);
        ui->gas_type_->setCurrentIndex(data_.gas);
        ui->pressure_->setValue(data_.pressure);
        ui->height_->setValue(data_.lift_height);
        ui->power_->setValue(data_.power);
        ui->ratio_->setValue(data_.ratio);
        ui->frequency_->setValue(data.frequency);
        ui->enable_blow_->setChecked(data_.enable_blow);
        ui->blow_time_->setValue(data_.blow_time);
        ui->focus_->setValue(data_.focus);
        ui->stay_time_->setValue(data_.stay);
    }
}
