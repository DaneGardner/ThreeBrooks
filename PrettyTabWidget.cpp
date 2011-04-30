#include "PrettyTabWidget.h"
#include <QDebug>

PrettyTabWidget::PrettyTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->setDocumentMode(true);
    this->setTabsClosable(true);
    this->setMovable(true);
    this->setElideMode(Qt::ElideRight);
}

void PrettyTabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    if(this->count() < 2) {
        this->tabBar()->hide();
    } else {
        this->tabBar()->show();
    }
}

void PrettyTabWidget::tabRemoved(int index)
{
    Q_UNUSED(index)

    if(this->count() < 2) {
        this->tabBar()->hide();
    } else {
        this->tabBar()->show();
    }
}
