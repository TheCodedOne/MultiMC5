#include "VanillaPage.h"
#include "ui_VanillaPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"

#include <meta/Index.h>
#include <meta/VersionList.h>
#include <dialogs/VersionSelectDialog.h>

VanillaPage::VanillaPage(QWidget *parent)
	: QWidget(parent), ui(new Ui::VanillaPage)
{
	ui->setupUi(this);
	auto vlist = ENV.metadataIndex()->get("net.minecraft");
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

	if (m_selectedVersion)
	{
		// ui->versionTextBox->setText(version->descriptor());
	}
	else
	{
		// ui->versionTextBox->setText("");
	}
}
