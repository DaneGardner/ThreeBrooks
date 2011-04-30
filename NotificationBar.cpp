#include "NotificationBar.h"
#include "ui_NotificationBar.h"

NotificationBar::NotificationBar(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NotificationBar)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255,255,225));
    setPalette(palette);

    this->setVisible(false);

    setModality(false);
}

NotificationBar::~NotificationBar()
{
    delete ui;
}

QString NotificationBar::text() const
{
    return ui->lblText->text();
}

void NotificationBar::setText(const QString &text)
{
    ui->lblText->setText(text);
}

QIcon NotificationBar::icon() const
{
    //TODO:
    return QIcon();
}

void NotificationBar::setIcon(const QIcon icon)
{
    //TODO:
}

QDialogButtonBox::StandardButtons NotificationBar::standardButtons() const
{
    return ui->btnBox->standardButtons();
}

void NotificationBar::setStandardButtons(const QDialogButtonBox::StandardButtons standardButtons)
{
    ui->btnBox->setStandardButtons(standardButtons);
}

bool NotificationBar::modality() const
{
    return _modality;
}

void NotificationBar::setModality(const bool modality)
{
    _modality = modality;
}


void NotificationBar::on_btnBox_clicked(QAbstractButton *button)
{
    this->hide();
    emit buttonClicked(ui->btnBox->standardButton(button));
    emit closing();
    this->close();
}

void NotificationBar::on_btnClose_clicked()
{
    this->hide();
    emit buttonClicked(QDialogButtonBox::NoButton);
    emit closing();
    this->close();
}
