#ifndef CRAFTPARAM_H
#define CRAFTPARAM_H

#include <QWidget>

namespace Ui {
class CraftParam;
}

struct CraftData {
    bool enable_incr;
    double incr_time;
    int gas;
    double pressure;
    double height;
    double power;
    double ratio;
    double frequency;
    double focus;
    double stay;
    bool enable_blow;
    double blow_time;
};

class CraftParam : public QWidget
{
    Q_OBJECT

public:
    explicit CraftParam(QWidget *parent = 0);
    ~CraftParam();

    CraftData data_;

    void onDataUpdate(CraftData &data, bool dir = true);

signals:
    void dataChanged();

public slots:
    void onDataUpdate();
    void onDisable(bool disable);
    void onEnable(bool enable);


private:
    Ui::CraftParam *ui;
};

#endif // CRAFTPARAM_H
