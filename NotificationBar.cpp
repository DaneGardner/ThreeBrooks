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

    setModality(false);
    setVisible(false);
    setFocusPolicy(Qt::StrongFocus);
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

void NotificationBar::keyReleaseEvent(QKeyEvent *event)
{
    /* Why I can't just send the freakin' key event to the QDialogButtonBox is beyond me.  I Googled the crap out of the
       subject and it seems that there really isn't a good solution to the problem.  The only thing I could do was the
       following hack.  o_O
     */

    if(event->count() != 1) {
        QFrame::keyReleaseEvent(event);
        return;
    }

    QDialogButtonBox::ButtonRole keyRole;
    switch(event->key()) {
    case Qt::Key_Escape:
        keyRole = QDialogButtonBox::RejectRole;
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        keyRole = QDialogButtonBox::AcceptRole;
        break;
    default:
        QFrame::keyReleaseEvent(event);
        return;
    }

    foreach(QAbstractButton *button, ui->btnBox->buttons()) {
        QDialogButtonBox::ButtonRole buttonRole = ui->btnBox->buttonRole(button);
        if(keyRole == QDialogButtonBox::AcceptRole) {
            if(buttonRole == QDialogButtonBox::AcceptRole || buttonRole == QDialogButtonBox::YesRole ||
                    buttonRole == QDialogButtonBox::ApplyRole) {
                button->click();
                return;
            }
        } else if(keyRole == QDialogButtonBox::RejectRole) {
            if(buttonRole == QDialogButtonBox::RejectRole || buttonRole == QDialogButtonBox::NoRole) {
                button->click();
                return;
            }
        }
    }

    on_btnClose_clicked();
}


