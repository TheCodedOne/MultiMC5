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

namespace Ui
{
class ImportPage;
}

class NewInstanceDialog;

class ImportPage : public QWidget, public BasePage
{
	Q_OBJECT

public:
	explicit ImportPage(NewInstanceDialog* dialog, QWidget *parent = 0);
	virtual ~ImportPage();
	virtual QString displayName() const override
	{
		return tr("Import from zip");
	}
	virtual QIcon icon() const override
	{
		return MMC->getThemedIcon("viewfolder");
	}
	virtual QString id() const override
	{
		return "import";
	}
	virtual QString helpPage() const override
	{
		return "Zip-import";
	}
	virtual bool shouldDisplay() const override;

	void setUrl(const QString & url);

private slots:
	void on_modpackBtn_clicked();

private:
	Ui::ImportPage *ui = nullptr;
	NewInstanceDialog* dialog = nullptr;
};

