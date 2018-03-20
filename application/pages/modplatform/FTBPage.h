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

#pragma once

#include <QWidget>

#include "pages/BasePage.h"
#include <MultiMC.h>
#include "tasks/Task.h"
#include "modplatform/ftb/PackHelpers.h"

namespace Ui
{
class FTBPage;
}

class FtbListModel;
class FtbFilterModel;
class FtbPackDownloader;

class FTBPage : public QWidget, public BasePage
{
	Q_OBJECT

public:
	explicit FTBPage(QWidget *parent = 0);
	virtual ~FTBPage();
	virtual QString displayName() const override
	{
		return tr("FTB Legacy");
	}
	virtual QIcon icon() const override
	{
		return MMC->getThemedIcon("ftb_logo");
	}
	virtual QString id() const override
	{
		return "ftb";
	}
	virtual QString helpPage() const override
	{
		return "FTB-platform";
	}
	virtual bool shouldDisplay() const override;

	FtbPackDownloader* getFtbPackDownloader();
	bool isFtbModpackRequested();
	FtbModpack getSelectedModpack();
	QString getSelectedVersion();

private slots:
	void on_btnChooseFtbPack_clicked();
	void ftbPackDataDownloadSuccessfully();
	void ftbPackDataDownloadFailed();
	void onSortingSelectionChanged(QString data);
	void onVersionSelectionItemChanged(QString data);
	void onPackSelectionChanged(QModelIndex first, QModelIndex second);

private:
	bool ftbModpackRequested = false;
	FtbPackDownloader* ftbPackDownloader = nullptr;
	FtbModpack selectedPack;
	FtbModpack selected;
	QString selectedVersion;
	FtbListModel* listModel;
	FtbFilterModel* filterModel;

	Ui::FTBPage *ui;
};
