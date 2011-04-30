#ifndef PRETTYTABWIDGET_H
#define PRETTYTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class PrettyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit PrettyTabWidget(QWidget *parent = 0);

protected :
    void tabInserted(int index);
    void tabRemoved(int index);

};

#endif // PRETTYTABWIDGET_H
