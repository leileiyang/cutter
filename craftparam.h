#ifndef CRAFTPARAM_H
#define CRAFTPARAM_H

#include <QWidget>

#include "xmlparser/LaserXmlParser.h"

namespace Ui {
class CraftParam;
}

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
