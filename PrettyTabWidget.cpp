#include "PrettyTabWidget.h"

PrettyTabWidget::PrettyTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setDocumentMode(true);
    setTabsClosable(true);
    setMovable(true);
    setElideMode(Qt::ElideRight);

    _stylesheet = QString("QStackedWidget { "
                  "background-image: url(\":/ThreeBrooks/background.svg\"); "
                  "background-repeat: none;"
                  "background-position: center center;}");
    setStyleSheet(_stylesheet);

}

void PrettyTabWidget::tabInserted(int index)
{
    Q_UNUSED(index)

    if(this->count() < 2) {
        this->tabBar()->hide();
    } else {
        this->tabBar()->show();
    }

    if(count() <= 0) {
        setStyleSheet(_stylesheet);
    } else {
        setStyleSheet(QString());
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

    if(count() <= 0) {
        setStyleSheet(_stylesheet);
    } else {
        setStyleSheet(QString());
    }

}

