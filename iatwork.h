#ifndef IATWORK_H
#define IATWORK_H

#include <QObject>
#include "msp_cmn.h"
#include "msp_errors.h"

class Iatwork : public QObject
{
    Q_OBJECT

public:
    explicit Iatwork(QObject *parent = 0);
    ~Iatwork();

signals:
    void statedata(const QString&);

public slots:
    void iat_record_sample();

private:

};

#endif // IATWORK_H
