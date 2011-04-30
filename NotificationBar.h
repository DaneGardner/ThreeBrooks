#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QWidget>
#include <QFrame>
#include <QDialogButtonBox>

namespace Ui {
    class NotificationBar;
}

class NotificationBar : public QFrame
{
    Q_OBJECT

public:
    explicit NotificationBar(QWidget *parent = 0);
    ~NotificationBar();

    QString text() const;
    void setText(const QString &text);

    QIcon icon() const;
    void setIcon(const QIcon icon);

    QDialogButtonBox::StandardButtons standardButtons() const;
    void setStandardButtons(const QDialogButtonBox::StandardButtons standardButtons);

    bool modality() const;
    void setModality(const bool modality);

signals:
    void buttonClicked(QDialogButtonBox::StandardButton);
    void closing();

protected slots:
    void on_btnBox_clicked(QAbstractButton*);
    void on_btnClose_clicked();

protected:
    bool _modality;

private:
    Ui::NotificationBar *ui;
};

#endif // NOTIFICATIONBAR_H
