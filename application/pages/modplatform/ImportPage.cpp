#include "ImportPage.h"
#include "ui_ImportPage.h"

#include "MultiMC.h"
#include "FolderInstanceProvider.h"
#include "dialogs/CustomMessageBox.h"
#include "dialogs/ProgressDialog.h"
#include "dialogs/NewInstanceDialog.h"

class UrlValidator : public QValidator
{
public:
	using QValidator::QValidator;

	State validate(QString &in, int &pos) const
	{
		const QUrl url(in);
		if (url.isValid() && !url.isRelative() && !url.isEmpty())
		{
			return Acceptable;
		}
		else if (QFile::exists(in))
		{
			return Acceptable;
		}
		else
		{
			return Intermediate;
		}
	}
};

ImportPage::ImportPage(NewInstanceDialog* dialog, QWidget *parent)
	: QWidget(parent), ui(new Ui::ImportPage), dialog(dialog)
{
	ui->setupUi(this);
	/*
	ui->modpackEdit->setValidator(new UrlValidator(ui->modpackEdit));
	connect(ui->modpackEdit, &QLineEdit::textChanged, this, &NewInstanceDialog::updateDialogState);
	connect(ui->modpackBox, &QRadioButton::clicked, this, &NewInstanceDialog::updateDialogState);
	*/
}

ImportPage::~ImportPage()
{
	delete ui;
}

bool ImportPage::shouldDisplay() const
{
	return true;
}

void ImportPage::setUrl(const QString& url)
{
	qDebug() << "Selecting URL:" << url;
}

void ImportPage::on_modpackBtn_clicked()
{
	/*
	const QUrl url = QFileDialog::getOpenFileUrl(this, tr("Choose modpack"), modpackUrl(), tr("Zip (*.zip)"));
	if (url.isValid())
	{
		if (url.isLocalFile())
		{
			ui->modpackEdit->setText(url.toLocalFile());
		}
		else
		{
			ui->modpackEdit->setText(url.toString());
		}
	}
	*/
}

/*
QUrl NewInstanceDialog::modpackUrl() const
{
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
	return QUrl();
}
*/

/*
	else if (ui->modpackBox->isChecked())
	{
		auto url = QUrl::fromUserInput(ui->modpackEdit->text());
		QFileInfo fi(url.fileName());
		suggestedName = fi.completeBaseName();
	}
	(ui->modpackBox->isChecked() && ui->modpackEdit->hasAcceptableInput()) ||
*/