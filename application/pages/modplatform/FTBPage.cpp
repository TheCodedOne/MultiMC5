#include "FTBPage.h"
#include "ui_FTBPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"
#include "modplatform/ftb/FtbPackDownloader.h"
#include <FtbListModel.h>

FTBPage::FTBPage(QWidget *parent)
	: QWidget(parent), ui(new Ui::FTBPage)
{
	ui->setupUi(this);
	ftbPackDownloader = new FtbPackDownloader();

	connect(ftbPackDownloader, &FtbPackDownloader::ready, this, &FTBPage::ftbPackDataDownloadSuccessfully);
	connect(ftbPackDownloader, &FtbPackDownloader::packFetchFailed, this, &FTBPage::ftbPackDataDownloadFailed);

	ftbPackDownloader->fetchModpacks(false);

	filterModel = new FtbFilterModel(this);
	listModel = new FtbListModel(this);
	filterModel->setSourceModel(listModel);

	ui->packList->setModel(filterModel);
	ui->packList->setSortingEnabled(true);
	ui->packList->header()->hide();
	ui->packList->setIndentation(0);

	filterModel->setSorting(FtbFilterModel::Sorting::ByName);

	for(int i = 0; i < filterModel->getAvailableSortings().size(); i++)
	{
		ui->sortByBox->addItem(filterModel->getAvailableSortings().keys().at(i));
	}

	ui->sortByBox->setCurrentText(filterModel->getAvailableSortings().key(filterModel->getCurrentSorting()));

	connect(ui->sortByBox, &QComboBox::currentTextChanged, this, &FTBPage::onSortingSelectionChanged);
	connect(ui->packVersionSelection, &QComboBox::currentTextChanged, this, &FTBPage::onVersionSelectionItemChanged);
	connect(ui->packList->selectionModel(), &QItemSelectionModel::currentChanged, this, &FTBPage::onPackSelectionChanged);

	ui->modpackInfo->setOpenExternalLinks(true);
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
	listModel->fill(ftbPackDownloader->getModpacks());
}

void FTBPage::ftbPackDataDownloadFailed()
{
	qDebug() << "Stuff went missing while grabbing FTB pack list or something...";
}

void FTBPage::onPackSelectionChanged(QModelIndex now, QModelIndex prev)
{
	ui->packVersionSelection->clear();
	FtbModpack selectedPack = filterModel->data(now, Qt::UserRole).value<FtbModpack>();

	ui->modpackInfo->setHtml("Pack by <b>" + selectedPack.author + "</b>" + "<br>Minecraft " + selectedPack.mcVersion + "<br>"
				"<br>" + selectedPack.description + "<ul><li>" + selectedPack.mods.replace(";", "</li><li>") + "</li></ul>");

	bool currentAdded = false;

	for(int i = 0; i < selectedPack.oldVersions.size(); i++) {
		if(selectedPack.currentVersion == selectedPack.oldVersions.at(i)) {
			currentAdded = true;
		}
		ui->packVersionSelection->addItem(selectedPack.oldVersions.at(i));
	}

	if(!currentAdded) {
		ui->packVersionSelection->addItem(selectedPack.currentVersion);
	}

	selected = selectedPack;
	// FIXME: implement accept flow
	// ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!selected.broken);
}

void FTBPage::onVersionSelectionItemChanged(QString data)
{
	if(data.isNull() || data.isEmpty()) {
		selectedVersion = "";
		return;
	}

	selectedVersion = data;
}

FtbModpack FTBPage::getSelectedModpack()
{
	return selected;
}

QString FTBPage::getSelectedVersion()
{
	return selectedVersion;
}

void FTBPage::onSortingSelectionChanged(QString data)
{
	filterModel->setSorting(filterModel->getAvailableSortings().value(data));
}
