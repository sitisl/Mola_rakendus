#include "formchatbox.h"
#include "ui_formchatbox.h"

FormChatBox::FormChatBox(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FormChatBox)
{
    ui->setupUi(this);
}

FormChatBox::~FormChatBox()
{
    delete ui;
}

