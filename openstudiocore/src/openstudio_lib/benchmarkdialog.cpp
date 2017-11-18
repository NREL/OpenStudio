#include "benchmarkdialog.hpp"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QInputDialog>
#include <QCryptographicHash>
#include <QIntValidator>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>

//TODO:EXPORT IMPORT.
//TODO:SHOW REPORT HTML.
//TODO:REPORT COMPARE IS ESCAPE FIRST COLUMN.

#define ADMIN "admin"
#define BASEKEY "building_standard"

void BenchmarkDialog::setupBenchmarkValues(QSettings& settingBase, QSettings *settingsReg)
{
    int i=0;
    while (1) {
        QString key = QString("building_standard%1").arg(i);
        if(settingBase.contains(key)){
            if(settingsReg){
                settingsReg->setValue(key, settingBase.value(key).toString());
            }

            QStringList kv = settingBase.value(key).toString().split("=");
            if(kv.size()==2){
                qDebug() << "key:" << kv.at(0) << ", value:" << kv.at(1);
                double value;
                bool toOK;
                value = kv.at(1).toDouble(&toOK);
                if(!toOK)
                    value = 0.0f;
                this->addBenchmark(key, kv.at(0), value);
            }
        }
        else
            break;
        i++;
    }
}

size_t BenchmarkDialog::valuesCount() const
{
    return values.count();
}

BenchmarkValue *BenchmarkDialog::valueAt(size_t idx) const
{
    return values.at(idx);
}

double BenchmarkDialog::getValueByName(const QString &name)
{
    QList<BenchmarkValue*>::iterator it = values.begin();
    while (it!=values.end()) {
        BenchmarkValue* value = (*it);
        if(value->name() == name)
            return value->value();
        it++;
    }
    return 0.0;
}

void BenchmarkDialog::resetToDefault()
{
    qDebug() << __FUNCTION__;
    QMessageBox msgBox;
    msgBox.setText("Reset to default value.");
    msgBox.setInformativeText("Do you want to reset to default?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        QString organizationName = QCoreApplication::organizationName();
        QString applicationName = QCoreApplication::applicationName();
        QSettings settingsReg(QSettings::SystemScope
                              , organizationName
                              , applicationName);

        for (int idx = 0; idx < values.size(); ++idx) {
            BenchmarkValue* bv = values.at(idx);
            bv->deleteLater();
            settingsReg.remove(bv->key());
        }

        values.clear();

        QSettings settingsINI( _defaultConfigPath, QSettings::IniFormat );
        setNewPassword(ADMIN);
        setupBenchmarkValues(settingsINI, &settingsReg);
    }
}

BenchmarkDialog::BenchmarkDialog(const QString &defaultConfigPath, QWidget *parent)
    : QDialog(parent, Qt::Dialog)
{
    QWidget* widget;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *buttonsLayout;

    vlayout = new QVBoxLayout(this);
    vlayout->setObjectName(QStringLiteral("centerLayout"));
    this->setLayout(vlayout);

    widget = new QWidget(this);
    vlayout->addWidget(widget);

    buttonsLayout = new QHBoxLayout(widget);
    buttonsLayout->setObjectName(QStringLiteral("buttonsLayout"));
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonsLayout->addItem(horizontalSpacer);

    btReset = new QPushButton(widget);
    btReset->setObjectName(QStringLiteral("btReset"));
    btReset->setText("Reset");
    btReset->hide();
    connect(btReset, SIGNAL(clicked()), this, SLOT(eventReset()));
    buttonsLayout->addWidget(btReset);

    btExport = new QPushButton(widget);
    btExport->setObjectName(QStringLiteral("btExport"));
    btExport->setText("Export");
    btExport->hide();
    connect(btExport, SIGNAL(clicked()), this, SLOT(eventExport()));
    buttonsLayout->addWidget(btExport);

    btImport = new QPushButton(widget);
    btImport->setObjectName(QStringLiteral("btImport"));
    btImport->setText("Import");
    btImport->hide();
    connect(btImport, SIGNAL(clicked()), this, SLOT(eventImport()));
    buttonsLayout->addWidget(btImport);

    btChangPassword = new QPushButton(widget);
    btChangPassword->setObjectName(QStringLiteral("btChangePassword"));
    btChangPassword->setText("Change Password");
    btChangPassword->hide();
    connect(btChangPassword, SIGNAL(clicked()), this, SLOT(eventChangePassword()));
    buttonsLayout->addWidget(btChangPassword);

    btEdit = new QPushButton(widget);
    btEdit->setObjectName(QStringLiteral("btEdit"));
    btEdit->setText("Edit");
    connect(btEdit, SIGNAL(clicked()), this, SLOT(eventEdit()));
    buttonsLayout->addWidget(btEdit);

    btAccept = new QPushButton(widget);
    btAccept->setObjectName(QStringLiteral("btAccept"));
    btAccept->setText("Accept");
    btAccept->hide();
    connect(btAccept, SIGNAL(clicked()), this, SLOT(eventAccept()));
    buttonsLayout->addWidget(btAccept);

    btClose = new QPushButton(widget);
    btClose->setObjectName(QStringLiteral("btClose"));
    btClose->setText("Close");
    connect(btClose, SIGNAL(clicked()), this, SLOT(eventClose()));
    buttonsLayout->addWidget(btClose);

    vlayout->addWidget(widget);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    qDebug() << "organizationName:" << organizationName;
    qDebug() << "applicationName:" << applicationName;

    QSettings settingsReg(QSettings::SystemScope
                          , organizationName
                          , applicationName);
    _defaultConfigPath = defaultConfigPath;
    QSettings settingsINI( _defaultConfigPath, QSettings::IniFormat );
    QSettings *ptSetting = NULL;
    bool unknowRegister = false;
    if(settingsReg.contains(QString(BASEKEY))){
        ptSetting = &settingsReg;
        qDebug() << "On registry";
    }
    else{
        unknowRegister = true;
        ptSetting = &settingsINI;
        qDebug() << "On INI : " << settingsINI.allKeys();
    }

    if(unknowRegister){
        qDebug() << "REG:" << settingsReg.allKeys();
        setNewPassword(ADMIN);
        setupBenchmarkValues(settingsINI, &settingsReg);
    }
    else
        setupBenchmarkValues(settingsReg, NULL);

    setWindowTitle("Setting Benchmark(kWh)");
    setEditable(false);
}

BenchmarkDialog::~BenchmarkDialog()
{
    qDebug() << __FUNCTION__;
}

void BenchmarkDialog::setEditable(bool isEdit)
{
    QList<BenchmarkValue*>::iterator it = values.begin();
    while (it!=values.end()) {
        (*it)->setEditable(isEdit);
        it++;
    }
}

void BenchmarkDialog::addBenchmark(const QString&key, const QString &name, double value)
{
    BenchmarkValue* bv = new BenchmarkValue(key, name, value, this);
    bv->setObjectName(name+"@"+value);
    vlayout->insertWidget(values.size(), bv);
    values.append(bv);
    qDebug() << __FUNCTION__;
}

QString BenchmarkDialog::hashOfUsernamePassword(const QString &username, const QString &password)
{
    QString string = username+"_123456_"+password;
    QCryptographicHash hasher(QCryptographicHash::Md5);
    hasher.addData(string.toStdString().c_str());
    QByteArray res(hasher.result());
    return res;
}

bool BenchmarkDialog::isCorrectPass(const QString &pass)
{
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();

    QSettings settingsReg(QSettings::SystemScope
                          , organizationName
                          , applicationName);

    QString hs = settingsReg.value(BASEKEY).toString();
    QString userInput = hashOfUsernamePassword("admin", pass);
    return userInput==hs;
}

void BenchmarkDialog::setNewPassword(const QString &pass)
{
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();

    QSettings settingsReg(QSettings::SystemScope
                          , organizationName
                          , applicationName);

    settingsReg.setValue(BASEKEY, hashOfUsernamePassword(ADMIN, pass));
}

void BenchmarkDialog::eventExport()
{
    qDebug() << __FUNCTION__;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Benchmark configure."),
                               QString(),
                               tr("Configure (*.bvs)"));
    if(!fileName.isEmpty()){
        QSettings settingsINI( fileName, QSettings::IniFormat );
        for (int idx = 0; idx < values.size(); ++idx) {
            BenchmarkValue* bv = values.at(idx);
            settingsINI.setValue(bv->key(), bv->toString());
        }
    }
}

void BenchmarkDialog::eventImport()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Benchmark configure."),
                               QString(),
                               tr("Configure (*.bvs)"));
    if(!fileName.isEmpty()){
        QSettings settingsINI( fileName, QSettings::IniFormat );

        QSettings settingsReg(QSettings::SystemScope
                              , QCoreApplication::organizationName()
                              , QCoreApplication::applicationName());

        for (int idx = 0; idx < values.size(); ++idx) {
            BenchmarkValue* bv = values.at(idx);
            bv->deleteLater();
            settingsReg.remove(bv->key());
        }

        values.clear();

        setupBenchmarkValues(settingsINI, &settingsReg);
    }
}

void BenchmarkDialog::eventEdit()
{
    qDebug() << __FUNCTION__;
    bool currentOK;
    QString currentPass = QInputDialog::getText(this, "Setting password", "Please Enter Current Password", QLineEdit::Password, QString(), &currentOK);

    if(!currentOK)
        return;

    if(isCorrectPass(currentPass)){
        setEditable(true);
        btAccept->show();
        btChangPassword->show();
        btReset->show();
        btExport->show();
        btImport->show();
        btEdit->hide();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Password is incorrected.");
        msgBox.exec();
    }
}

void BenchmarkDialog::eventAccept()
{
    qDebug() << __FUNCTION__;
    setEditable(false);

    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settingsReg(QSettings::SystemScope
                          , organizationName
                          , applicationName);

    QList<BenchmarkValue*>::iterator it = values.begin();
    while (it!=values.end()) {
        BenchmarkValue* bv = (*it);
        settingsReg.setValue(bv->key(), bv->toString());
        it++;
    }

    btAccept->hide();
    btEdit->show();
    btEdit->setEnabled(true);
    btClose->setEnabled(true);
    btChangPassword->hide();
    btExport->hide();
    btImport->hide();
    btReset->hide();
}

void BenchmarkDialog::eventClose()
{
    qDebug() << __FUNCTION__;
    this->deleteLater();
}

void BenchmarkDialog::eventChangePassword()
{
    qDebug() << __FUNCTION__;
    bool currentOK, newOK, confirmOK;
    QString currentPass = QInputDialog::getText(this, "Setting password", "Please Enter Current Password", QLineEdit::Password, QString(), &currentOK);
    if(currentOK){
        if(!isCorrectPass(currentPass)){
            QMessageBox msgBox;
            msgBox.setText("Password is incorrected.");
            msgBox.exec();
        }
        else{
            QString newPass = QInputDialog::getText(this, "Setting password", "Please Enter New Password", QLineEdit::Password, QString(), &newOK);
            if(newOK){
                QString confirmPass = QInputDialog::getText(this, "Setting password", "Please Enter Confirm Password", QLineEdit::Password, QString(), &confirmOK);
                qDebug() << "newPass:" << newPass << ", confirmPass:" << confirmPass;
                if((newPass == confirmPass)){
                    setNewPassword(newPass);
                    setEditable(false);
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setText("New password is incorrected.");
                    msgBox.exec();
                }
            }
        }
    }
}

void BenchmarkDialog::eventReset()
{
    resetToDefault();
}

BenchmarkValue::BenchmarkValue(const QString &key, const QString& name, double value, QWidget *parent)
    :QWidget(parent)
{
    hl = new QHBoxLayout(this);
    hl->setObjectName(QStringLiteral("horizontalLayout"));
    hl->setContentsMargins(0, 0, 0, 0);
    lnName = new QLineEdit(this);
    lnName->setObjectName(QStringLiteral("lineEdit_Name"));
    hl->addWidget(lnName);
    dValue = new QDoubleSpinBox(this);
    dValue->setDecimals(0);
    dValue->setMinimum(0);
    dValue->setMaximum(50000);
    dValue->setObjectName(QStringLiteral("DoubleSpinBox_Value"));
    hl->addWidget(dValue);
    lnName->setText(name);
    dValue->setValue(value);
    _key = key;
    this->setLayout(hl);
}

BenchmarkValue::~BenchmarkValue()
{
    qDebug() << __FUNCTION__;
}

void BenchmarkValue::setName(const QString &name)
{
    lnName->setText(name);
}

void BenchmarkValue::setValue(double value)
{
    dValue->setValue(value);
}

void BenchmarkValue::setEditable(bool isEdit)
{
    lnName->setReadOnly(!isEdit);
    dValue->setReadOnly(!isEdit);
    QPalette palette;
    QColor color;
    if(!isEdit)
        color = this->palette().color(QPalette::Window);
    else
        color = this->palette().color(QPalette::BrightText);

    palette.setColor(QPalette::Base, color);
    //palette.setColor(QPalette::Text,Qt::white);
    lnName->setPalette(palette);
    dValue->setPalette(palette);
}

QString BenchmarkValue::toString()
{
    return lnName->text() + "=" + QString::number(dValue->value());
}

QString BenchmarkValue::name()
{
    return lnName->text();
}

QString BenchmarkValue::key() const
{
    return _key;
}

double BenchmarkValue::value() const
{
    return dValue->value();
}
