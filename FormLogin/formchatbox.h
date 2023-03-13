#ifndef FORMCHATBOX_H
#define FORMCHATBOX_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FormChatBox; }
QT_END_NAMESPACE

class FormChatBox : public QMainWindow
{
    Q_OBJECT

public:
    FormChatBox(QWidget *parent = nullptr);
    ~FormChatBox();

private:
    Ui::FormChatBox *ui;
};
#endif // FORMCHATBOX_H
