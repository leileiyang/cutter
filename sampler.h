#ifndef SAMPLER_H
#define SAMPLER_H

#include <QObject>
#include <QThread>

class CuttingService;

class Sampler: public QThread
{
    Q_OBJECT
public:
    Sampler(CuttingService &service, QObject *parent = 0);
    void Exit();

protected:
    virtual void run();

private:
    CuttingService &service_;
    bool exit_;
};

#endif // SAMPLER_H
