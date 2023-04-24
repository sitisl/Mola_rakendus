#include "FormName.h"
#include "FormMessenger.h"

FormName::FormName(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //Create shortcut for return key, to login when pressing enter
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    QObject::connect(shortcut, SIGNAL(activated()), ui.btnJatka, SLOT(click()));
    ui.btnAvatar->setFixedSize(80, 80);
    ui.btnAvatar->setStyleSheet("background-color: transparent; border: none;");
    ui.btnAvatar->setIconSize(ui.btnAvatar->size());
    avatarPath = QString(":/avatars/icons/Asset 1.png");
    QIcon avatarIcon = QIcon(avatarPath);
    ui.btnAvatar->setIcon(avatarIcon);
    
}

FormName::~FormName()
{}

void FormName::on_btnJatka_clicked()
{
    QString strUserName = ui.lineNimi->text();
    if (strUserName != "" && strUserName.length() < 25)
    {
        emit switchToMessenger(1, strUserName, avatarPath);
    }
    else if (strUserName == "")
    {
        ui.statusBar->showMessage("Sisesta kasutajanimi!", 3000);
    }
    else
    {
        ui.statusBar->showMessage("Kasutajanimi liiga pikk!", 3000);
        ui.lineNimi->clear();
    }

}

void FormName::on_btnAvatar_clicked()
{
	QDialog* avatarDialog = new QDialog(this);
	QVBoxLayout* avatarLayout = new QVBoxLayout(avatarDialog);
	avatarDialog->setLayout(avatarLayout);
	avatarDialog->setWindowTitle("Avatar");

	QScrollArea* scrollArea = new QScrollArea(avatarDialog);
	scrollArea->setWidgetResizable(true);
	avatarLayout->addWidget(scrollArea);

	QWidget* avatarWidget = new QWidget(scrollArea);
	scrollArea->setWidget(avatarWidget);

	QGridLayout* avatarGrid = new QGridLayout(avatarWidget);
	avatarWidget->setLayout(avatarGrid);

    const int NUM_AVATARS = 11;
    const QString AVATAR_PATHS[NUM_AVATARS] = {
        ":/avatars/icons/Asset 1.png",
        ":/avatars/icons/Asset 2.png",
        ":/avatars/icons/Asset 3.png",
        ":/avatars/icons/Asset 4.png",
        ":/avatars/icons/Asset 5.png",
        ":/avatars/icons/Asset 6.png",
        ":/avatars/icons/Asset 7.png",
        ":/avatars/icons/Asset 8.png",
        ":/avatars/icons/Asset 9.png",
        ":/avatars/icons/Asset 10.png",
        ":/avatars/icons/Asset 11.png"
    };

    for (int i = 0; i < NUM_AVATARS; i++) {
        QIcon Icon(AVATAR_PATHS[i]);
        QPushButton* avatarButton = new QPushButton(avatarWidget);
        avatarButton->setFixedSize(80, 80);
        avatarButton->setIconSize(avatarButton->size());
        avatarButton->setIcon(Icon);
        avatarGrid->addWidget(avatarButton, i / 3, i % 3);
        connect(avatarButton, &QPushButton::clicked, this, [this, path = AVATAR_PATHS[i]]() { insertAvatar(path); });
        connect(avatarButton, &QPushButton::clicked, avatarDialog, &QDialog::reject);
    }

	QPushButton* closeButton = new QPushButton(tr("Close"), avatarDialog);
    avatarLayout->addWidget(closeButton);
	connect(closeButton, &QPushButton::clicked, avatarDialog, &QDialog::reject);

    avatarDialog->exec();
}

void FormName::insertAvatar(const QString & avatarIconPath)
{
    avatarPath = avatarIconPath;
    QIcon avatarIcon = QIcon(avatarIconPath);
	ui.btnAvatar->setIcon(avatarIcon);
}



