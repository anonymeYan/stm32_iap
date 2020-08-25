#ifndef YMODEMRECEIVE_H
#define YMODEMRECEIVE_H

#include <QObject>

class YmodemReceive : public QObject
{
    Q_OBJECT
public:
    explicit YmodemReceive(QObject *parent = nullptr);

signals:

public slots:
};

#endif // YMODEMRECEIVE_H
