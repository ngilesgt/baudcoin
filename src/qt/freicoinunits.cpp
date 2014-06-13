#include "baudcoinunits.h"

#include <QStringList>

BaudcoinUnits::BaudcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<BaudcoinUnits::Unit> BaudcoinUnits::availableUnits()
{
    QList<BaudcoinUnits::Unit> unitlist;
    unitlist.append(BDC);
    unitlist.append(mBDC);
    unitlist.append(uBDC);
    return unitlist;
}

bool BaudcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case BDC:
    case mBDC:
    case uBDC:
        return true;
    default:
        return false;
    }
}

QString BaudcoinUnits::name(int unit)
{
    switch(unit)
    {
    case BDC: return QString("BDC");
    case mBDC: return QString("mBDC");
    case uBDC: return QString::fromUtf8("μBDC");
    default: return QString("???");
    }
}

QString BaudcoinUnits::description(int unit)
{
    switch(unit)
    {
    case BDC: return QString("Baudcoins");
    case mBDC: return QString("Milli-Baudcoins (1 / 1,000)");
    case uBDC: return QString("Micro-Baudcoins (1 / 1,000,000)");
    default: return QString("???");
    }
}

mpq BaudcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case uBDC: return mpq("100/1");
    case mBDC: return mpq("100000/1");
    default:
    case BDC:  return mpq("100000000/1");
    }
}

int BaudcoinUnits::amountDigits(int unit)
{
    switch(unit)
    {
    case BDC: return 8; // <100,000,000 (# digits, without commas)
    case mBDC: return 11; // <100,000,000,000
    case uBDC: return 14; // <100,000,000,000,000
    default: return 0;
    }
}

int BaudcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case BDC: return 8;
    case mBDC: return 5;
    case uBDC: return 2;
    default: return 0;
    }
}

QString BaudcoinUnits::format(int unit, const mpq& n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    mpq q = n * COIN / factor(unit);
    std::string str = FormatMoney(q, fPlus);
    int diff = 8 - decimals(unit);
    if(diff > 0)
        str.erase(str.length() - diff, diff);
    return QString::fromStdString(str);
}

QString BaudcoinUnits::formatWithUnit(int unit, const mpq& amount, bool plussign)
{
    return format(unit, amount, plussign) + QString(" ") + name(unit);
}

bool BaudcoinUnits::parse(int unit, const QString &value, mpq *val_out)
{
    mpq ret_value;
    if (!ParseMoney(value.toStdString(), ret_value))
        return false;
    if(val_out)
    {
        *val_out = ret_value * factor(unit) / COIN;
    }
    return true;
}

int BaudcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant BaudcoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}
