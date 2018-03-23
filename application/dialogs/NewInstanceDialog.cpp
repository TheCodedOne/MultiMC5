/* Copyright 2013-2018 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MultiMC.h"
#include "NewInstanceDialog.h"
#include "ui_NewInstanceDialog.h"

#include <BaseVersion.h>
#include <icons/IconList.h>
#include <tasks/Task.h>
#include <InstanceList.h>

#include "VersionSelectDialog.h"
#include "ProgressDialog.h"
#include "IconPickerDialog.h"
#include "ChooseFtbPackDialog.h"

#include <QLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QValidator>

#include "widgets/PageContainer.h"
#include <pages/modplatform/VanillaPage.h>
#include <pages/modplatform/FTBPage.h>
#include <pages/modplatform/TwitchPage.h>
#include <pages/modplatform/ImportPage.h>

NewInstanceDialog::NewInstanceDialog(const QString & initialGroup, const QString & url, QWidget *parent)
	: QDialog(parent), ui(new Ui::NewInstanceDialog)
{
	ui->setupUi(this);

	setWindowIcon(MMC->getThemedIcon("new"));

	InstIconKey = "default";
	ui->iconButton->setIcon(MMC->icons()->getIcon(InstIconKey));

	auto groups = MMC->instances()->getGroups().toSet();
	auto groupList = QStringList(groups.toList());
	groupList.sort(Qt::CaseInsensitive);
	groupList.removeOne("");
	groupList.push_front(initialGroup);
	groupList.push_front("");
	ui->groupBox->addItems(groupList);
	int index = groupList.indexOf(initialGroup);
	if(index == -1)
	{
		index = 0;
	}
	ui->groupBox->setCurrentIndex(index);
	ui->groupBox->lineEdit()->setPlaceholderText(tr("No group"));

	m_buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Ok);
	m_buttons->button(QDialogButtonBox::Ok)->setDefault(true);

	connect(m_buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &QDialog::accept);
	connect(m_buttons->button(QDialogButtonBox::Help), &QPushButton::clicked, m_container, &PageContainer::help);

	m_container = new PageContainer(this);
	m_container->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
	ui->verticalLayout->insertWidget(2, m_container);

	m_container->addButtons(m_buttons);
	m_buttons->setFocus();

	if(!url.isEmpty())
	{
		m_container->selectPage("import");
		importPage->setUrl(url);
	}

	updateDialogState();

	restoreGeometry(QByteArray::fromBase64(MMC->settings()->get("NewInstanceGeometry").toByteArray()));
}

QList<BasePage *> NewInstanceDialog::getPages()
{
	importPage = new ImportPage(this);
	return
	{
		new VanillaPage(this),
		new FTBPage(this),
		new TwitchPage(this),
		importPage
	};
}

QString NewInstanceDialog::dialogTitle()
{
	return tr("New Instance");
}

NewInstanceDialog::~NewInstanceDialog()
{
	delete ui;
}

void NewInstanceDialog::setSuggestedPack(const PackSuggestion& pack)
{
	suggestion = pack;
	ui->instNameTextBox->setPlaceholderText(suggestion.name);

	auto allowOK = suggestion.valid && !instName().isEmpty();
	m_buttons->button(QDialogButtonBox::Ok)->setEnabled(allowOK);
}

void NewInstanceDialog::updateDialogState()
{
	/*
	QString suggestedName;
	if(ui->versionBox->isChecked())
	{
		suggestedName = ui->versionTextBox->text();
	}
	else if (ui->modpackBox->isChecked())
	{
		auto url = QUrl::fromUserInput(ui->modpackEdit->text());
		QFileInfo fi(url.fileName());
		suggestedName = fi.completeBaseName();
	}
	else if (ui->ftbBox->isChecked())
	{
		if(ftbPackDownloader->isValidPackSelected()) {
			suggestedName = ftbPackDownloader->getSuggestedInstanceName();
			ui->labelFtbPack->setText(selectedPack.name);
		}
	}

	bool allowOK = !instName().isEmpty() && (
				(ui->versionBox->isChecked() && m_selectedVersion) ||
				(ui->modpackBox->isChecked() && ui->modpackEdit->hasAcceptableInput()) ||
				(ui->ftbBox->isChecked() && ftbPackDownloader && ftbPackDownloader->isValidPackSelected() )
				);
	*/


}

QString NewInstanceDialog::instName() const
{
	auto result = ui->instNameTextBox->text();
	if(result.size())
	{
		return result.trimmed();
	}
	result = ui->instNameTextBox->placeholderText();
	if(result.size())
	{
		return result.trimmed();
	}
	return QString();
}

QString NewInstanceDialog::instGroup() const
{
	return ui->groupBox->currentText();
}
QString NewInstanceDialog::iconKey() const
{
	return InstIconKey;
}
QUrl NewInstanceDialog::modpackUrl() const
{
	/*
	if (ui->modpackBox->isChecked())
	{
		const QUrl url(ui->modpackEdit->text());
		if (url.isValid() && !url.isRelative() && !url.host().isEmpty())
		{
			return url;
		}
		else
		{
			return QUrl::fromLocalFile(ui->modpackEdit->text());
		}
	}
	else
	{
		return QUrl();
	}
	*/
	return QUrl();
}


void NewInstanceDialog::on_iconButton_clicked()
{
	IconPickerDialog dlg(this);
	dlg.execWithSelection(InstIconKey);

	if (dlg.result() == QDialog::Accepted)
	{
		InstIconKey = dlg.selectedIconKey;
		ui->iconButton->setIcon(MMC->icons()->getIcon(InstIconKey));
	}
}

void NewInstanceDialog::on_instNameTextBox_textChanged(const QString &arg1)
{
	updateDialogState();
}

void NewInstanceDialog::closeEvent(QCloseEvent* event)
{
	qDebug() << "New instance dialog close requested";
	if (m_container->prepareToClose())
	{
		qDebug() << "New instance dialog close approved";
		MMC->settings()->set("NewInstanceGeometry", saveGeometry().toBase64());
		qDebug() << "New instance dialog geometry saved";
		QDialog::closeEvent(event);
	}
}
