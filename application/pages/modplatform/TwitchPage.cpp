#include "TwitchPage.h"
#include "ui_TwitchPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"

TwitchPage::TwitchPage(QWidget *parent)
	: QWidget(parent), ui(new Ui::TwitchPage)
{
	ui->setupUi(this);
}

TwitchPage::~TwitchPage()
{
	delete ui;
}

bool TwitchPage::shouldDisplay() const
{
	return true;
}
