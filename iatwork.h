#ifndef IATWORK_H
#define IATWORK_H

#include <QObject>

class Iatwork : public QObject
{
    Q_OBJECT

public:
    explicit Iatwork(QObject *parent = 0);
    ~Iatwork();

signals:

public slots:
    void iat_record_sample();

private:

};

#endif // IATWORK_H
