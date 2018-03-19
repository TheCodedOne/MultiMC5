#include "FTBPage.h"
#include "ui_FTBPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"

FTBPage::FTBPage(QWidget *parent)
	: QWidget(parent), ui(new Ui::FTBPage)
{
	ui->setupUi(this);
	ftbPackDownloader = new FtbPackDownloader();

	connect(ftbPackDownloader, &FtbPackDownloader::ready, this, &FTBPage::ftbPackDataDownloadSuccessfully);
	connect(ftbPackDownloader, &FtbPackDownloader::packFetchFailed, this, &FTBPage::ftbPackDataDownloadFailed);

	ftbPackDownloader->fetchModpacks(false);
}

FTBPage::~FTBPage()
{
	delete ui;
}

bool FTBPage::shouldDisplay() const
{
	return true;
}

bool FTBPage::isFtbModpackRequested()
{
	return ftbModpackRequested;
}

FtbPackDownloader *FTBPage::getFtbPackDownloader()
{
	return ftbPackDownloader;
}

void FTBPage::on_btnChooseFtbPack_clicked()
{
	/*
	ChooseFtbPackDialog dl(ftbPackDownloader->getModpacks());
	dl.exec();
	if(dl.result() == QDialog::Accepted) {
		selectedPack = dl.getSelectedModpack();
		ftbPackDownloader->selectPack(selectedPack, dl.getSelectedVersion());
	}
	updateDialogState();
	*/
}

void FTBPage::ftbPackDataDownloadSuccessfully()
{
	/*
	ui->packDataDownloadStatus->setText(tr("(Data download complete)"));
	ui->ftbBox->setEnabled(true);
	*/
}

void FTBPage::ftbPackDataDownloadFailed()
{
	/*
	ui->packDataDownloadStatus->setText(tr("(Data download failed)"));
	*/
}

