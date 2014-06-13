#ifndef BAUDCOINADDRESSVALIDATOR_H
#define BAUDCOINADDRESSVALIDATOR_H

#include <QRegExpValidator>

/** Base48 entry widget validator.
   Corrects near-miss characters and refuses characters that are no part of base48.
 */
class BaudcoinAddressValidator : public QValidator
{
    Q_OBJECT
public:
    explicit BaudcoinAddressValidator(QObject *parent = 0);

    State validate(QString &input, int &pos) const;

    static const int MaxAddressLength = 35;
signals:

public slots:

};

#endif // BAUDCOINADDRESSVALIDATOR_H
