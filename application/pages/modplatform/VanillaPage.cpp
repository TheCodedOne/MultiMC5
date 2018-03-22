#include "VanillaPage.h"
#include "ui_VanillaPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"

#include <meta/Index.h>
#include <meta/VersionList.h>
#include <dialogs/VersionSelectDialog.h>
#include <Filter.h>

VanillaPage::VanillaPage(QWidget *parent)
	: QWidget(parent), ui(new Ui::VanillaPage)
{
	ui->setupUi(this);
	ui->tabWidget->tabBar()->hide();
	connect(ui->versionList, &VersionSelectWidget::selectedVersionChanged, this, &VanillaPage::setSelectedVersion);
	filterChanged();
	connect(ui->alphaFilter, &QCheckBox::stateChanged, this, &VanillaPage::filterChanged);
	connect(ui->betaFilter, &QCheckBox::stateChanged, this, &VanillaPage::filterChanged);
	connect(ui->snapshotFilter, &QCheckBox::stateChanged, this, &VanillaPage::filterChanged);
	connect(ui->oldSnapshotFilter, &QCheckBox::stateChanged, this, &VanillaPage::filterChanged);
	connect(ui->releaseFilter, &QCheckBox::stateChanged, this, &VanillaPage::filterChanged);
}

void VanillaPage::opened()
{
	auto vlist = ENV.metadataIndex()->get("net.minecraft");
	ui->versionList->initialize(vlist.get());
	if(vlist->isLoaded())
	{
		setSelectedVersion(vlist->getRecommended());
	}
	else
	{
		vlist->load(Net::Mode::Online);
		auto task = vlist->getLoadTask();
		if(vlist->isLoaded())
		{
			setSelectedVersion(vlist->getRecommended());
		}
		if(task)
		{
			connect(task.get(), &Task::succeeded, this, &VanillaPage::versionListUpdated);
		}
	}
}

void VanillaPage::filterChanged()
{
	QStringList out;
	if(ui->alphaFilter->isChecked())
		out << "(old_alpha)";
	if(ui->betaFilter->isChecked())
		out << "(old_beta)";
	if(ui->snapshotFilter->isChecked())
		out << "(snapshot)";
	if(ui->oldSnapshotFilter->isChecked())
		out << "(old_snapshot)";
	if(ui->releaseFilter->isChecked())
		out << "(release)";
	auto regexp = out.join('|');
	ui->versionList->setFilter(BaseVersionList::TypeRole, new RegexpFilter(regexp, false));
}

VanillaPage::~VanillaPage()
{
	delete ui;
}

bool VanillaPage::shouldDisplay() const
{
	return true;
}

BaseVersionPtr VanillaPage::selectedVersion() const
{
	return m_selectedVersion;
}

void VanillaPage::on_btnChangeVersion_clicked()
{
	VersionSelectDialog vselect(ENV.metadataIndex()->get("net.minecraft").get(), tr("Change Minecraft version"), this);
	vselect.exec();
	if (vselect.result() == QDialog::Accepted)
	{
		BaseVersionPtr version = vselect.selectedVersion();
		if (version)
		{
			m_versionSetByUser = true;
			setSelectedVersion(version);
		}
	}
}

void VanillaPage::versionListUpdated()
{
	if(!m_versionSetByUser)
	{
		auto vlist = ENV.metadataIndex()->get("net.minecraft");
		setSelectedVersion(vlist->getRecommended());
	}
}

void VanillaPage::setSelectedVersion(BaseVersionPtr version)
{
	m_selectedVersion = version;
	if(m_selectedVersion)
	{
		qDebug() << "Version selected:" << version->descriptor();
	}
}
