#include "comparedialog.h"
#include "ui_comparedialog.h"

#include <QtWebKitWidgets/QWebFrame>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include "genbecreport.h"
#include "idoc.h"
#include "enegyplusdoc.h"
#include "becdoc.h"
#include "openstudiodoc.h"

CompareDialog::CompareDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompareDialog)
{
    ui->setupUi(this);
    QString err;
    cmpType = CMPTYPE_UNKNOW;

    QObject::connect(ui->btAddCmpPath
                     , SIGNAL(eventDropFilePath(const QString &))
                     , this
                     , SLOT(eventDropFilePath(const QString &)));

    ui->webView->setAcceptDrops(false);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint;
    setWindowFlags( flags );
}

bool CompareDialog::SetParam(int argc, char *argv[])
{
    if(argc>=4){
        return SetParam(argv[1], argv[2], argv[3]);
    }
    else {

        QMessageBox msgBox;
        msgBox.setText("Unexpected parameter to launch ReportCompare");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox;
        msgBox.exec();
        return false;
    }
}

CompareDialog::~CompareDialog()
{
    delete ui;
    qDebug() << "CompareDialog::~CompareDialog()";
}

QString CompareDialog::loadFileToWebView(const QString& fn, QWebView* webView)
{
    FINDTARGETRES res;
    qDebug() << "File : " << fn;
    QString target;
    if(fn.endsWith(".html") || fn.endsWith(".htm")){
        target = fn;
    }
    else{
        target = findTargetPath(fn, res);
    }

    qDebug() << "Target : " << target;
    if(!target.isEmpty()){
        webView->load("file:///"+target);
        fileCmps.append(target);
    }
    msgRes(res, fn);
    return target;
}

bool CompareDialog::SetParam(const QString& file1, const QString &file2, const QString& type)
{
    this->type = type;
    this->file1 = file1;
    this->file2 = file2;

    this->file1.replace("file:///", "");
    this->file2.replace("file:///", "");

    QString modeStr;
    if(type.toLower() == "enegyplus" || type.toLower() == "e"){
        SetCmpType(CMPTYPE_ENYGYPLUS);
        modeStr = "Enegy plus";
    }
    else if(type.toLower() == "bec" || type == "b"){
        SetCmpType(CMPTYPE_BEC);
        modeStr = "BEC";
    }
    else if(type.toLower() == "openstudio" || type.toLower() == "o"){
        SetCmpType(CMPTYPE_OPENSTUDIO);
        modeStr = "Open studio";
    }
    else{
        SetCmpType(CMPTYPE_UNKNOW);
        modeStr = "??????";
        QString msg = QString("ReportCompare was not support path1:'%1'\n, path2:'%2'\n, type:'%3'\n")
                .arg(this->file1).arg(file2).arg(type);
        QMessageBox::critical(this, "Enexpected to launch ReportCompare", msg);
        return false;
    }

    this->file1 = loadFileToWebView(this->file1, ui->webView);

    ui->lbInfo->setText(QString("Compare report source file is %1 in %2 mode")
                        .arg(this->file1)
                        .arg(modeStr));

    ui->chkShowTree->setVisible(false);
    ui->chkShowTree->setChecked(false);
    on_chkShowTree_clicked(false);
    msgBox.exec();
    return true;
}

QString CompareDialog::getReportName(const QString &filePath)
{
    QString fp = filePath;
    fp.replace("\\", "/");
    QStringList fpls = fp.split("/");
    qDebug() << "fpls.count = " << fpls.count();
    if(fpls.count()>=4){
        return fpls[fpls.count()-4];
    }
    return "????";
}

void CompareDialog::on_chkShowTree_clicked(bool checked)
{
    ui->wCenter->setVisible(checked);
    ui->wRight->setVisible(checked);
}

QString CompareDialog::scanFolderPathWithRegex(const QString& path, const QRegExp& reg, int recursive)
{
    recursive--;
    if(recursive < 0)
        return QString();

    QFileInfo rootFileInfo(path);
    if(rootFileInfo.isDir()){
        QDir dir(path);
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList dirList = dir.entryList();
        for (int i=0; i<dirList.size(); ++i)
        {
            QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
            bool match = newPath.contains(reg);
            QFileInfo pathFileInfo(newPath);
            if(!match)
            {
                if(pathFileInfo.isDir()){
                    QString matchPath = scanFolderPathWithRegex(newPath, reg, recursive);
                    if(!matchPath.isEmpty()){
                        return matchPath;
                    }
                }
            }
            else{
                return newPath;
            }
        }
    }
    else{
        bool match = path.contains(reg);
        if(match)
            return path;
    }

    return QString();
}

void CompareDialog::SetCmpType(CompareDialog::CMPTYPE type)
{
    cmpType = type;
}

bool CompareDialog::isDuplicatePath(QString filePath)
{
    QStringList::const_iterator cit = fileCmps.begin();
    while (cit!=fileCmps.end()) {
       QString cf = (*cit);
       if(cf == filePath){
           return true;
       }
       cit++;
    }
    return false;
}

void CompareDialog::msgRes(FINDTARGETRES res, QString filePath)
{
    if(res == FINDTARGETRES_CANTFIND)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Can't find result at path %1").arg(filePath));
        msgBox.exec();
    }
    else if(res == FINDTARGETRES_DUP)
    {
        QMessageBox msgBox;
        msgBox.setText("Select path is duplicate.");
        msgBox.exec();
    }
}

void CompareDialog::doCmp(QString filePath)
{
    if(filePath.startsWith("file:///")){
        filePath.remove(0, 8);
    }
    qDebug() << "Do compare at path:" << filePath;
    FINDTARGETRES res;
    QString targetPath =  findTargetPath(filePath, res);
    if(!targetPath.isEmpty()){
        LoadCompareFile(targetPath);
    }
    msgRes(res, filePath);
}

QString CompareDialog::findTargetPath(const QString &filePath, CompareDialog::FINDTARGETRES& res)
{
    static QRegExp becRegEx("(run[\\\\/]\\d[-]BEC[-]\\d[\\\\/]report\\.html)$");
    static QRegExp energyPlusRegEx("(run[\\\\/]\\d[-]EnergyPlus[-]\\d[\\\\/]eplustbl\\.htm)$");
    static QRegExp openStudioRegEx("(run[\\\\/]\\d[-]UserScript[-]\\d[\\\\/]report\\.html)$");

    res = FINDTARGETRES_NONE;
    bool dup = false;
    if(!filePath.isEmpty()){
        QString matchPath;
        switch (cmpType) {
        case CMPTYPE_BEC:
            matchPath = scanFolderPathWithRegex(filePath
                                                , becRegEx, 3);
            break;
        case CMPTYPE_OPENSTUDIO:
            matchPath = scanFolderPathWithRegex(filePath
                                                , openStudioRegEx, 3);
            break;
        default:
            matchPath = scanFolderPathWithRegex(filePath
                                                , energyPlusRegEx, 3);
            break;
        }
        dup = isDuplicatePath(matchPath);
        if(dup){
            res = FINDTARGETRES_DUP;
            return QString();
        }
        else{
            return matchPath;
        }
    }
    res = FINDTARGETRES_CANTFIND;
    return QString();
}

void CompareDialog::on_btAddCmpPath_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this,
        tr("Open report folder"));

    if(!filePath.isEmpty())
        doCmp(filePath);
}

void CompareDialog::eventDropFilePath(const QString &filePath)
{
    doCmp(filePath);
}

void CompareDialog::LoadCompareFile(const QString& filePath)
{
    qDebug() << "filePath=" << filePath;
    file2 = filePath;
    file2 = loadFileToWebView(file2, ui->webView2);
    reportName2 = getReportName(file2);
    qDebug() << "file2=" << file2;
    qDebug() << "reportName2=" << reportName2;
}

CompareDialog::CMPTYPE CompareDialog::getDocTypeFromTitle(const QString &title){
    if(title == "BEC Report")return CMPTYPE_BEC;
    if(title == "Results | OpenStudio")return CMPTYPE_OPENSTUDIO;
    if(title.indexOf("EnergyPlus")>=0) return CMPTYPE_ENYGYPLUS;
    return CMPTYPE_UNKNOW;
}

void CompareDialog::on_webView_loadFinished(bool arg1)
{
    qDebug() << __FUNCTION__ ;
    (void)arg1;
    CMPTYPE type = getDocTypeFromTitle(ui->webView->title());
    if(type != cmpType){
        QString str = QString("File '%1' is unexpected type %2:%3..%4.").arg("")
                .arg(type).arg(cmpType).arg(ui->webView->title());
        QMessageBox::warning(this, "Warning", str);
        ui->btAddCmpPath->setDisabled(true);
        return;
    }

    if(cmpType == CMPTYPE_ENYGYPLUS){
        QString projectName1 = getReportName(file1);
        doc = QSharedPointer<IDoc>(new EnegyPlusDoc(projectName1, ui->webView));
    }
    else if(cmpType == CMPTYPE_BEC){
        QString projectName1 = getReportName(file1);
        doc = QSharedPointer<IDoc>(new BECDoc(projectName1, ui->webView));
    }
    else if(cmpType == CMPTYPE_OPENSTUDIO){
        QString projectName1 = getReportName(file1);
        doc = QSharedPointer<IDoc>(new OpenStudioDoc(projectName1, ui->webView));
    }
    LoadCompareFile(this->file2);
}

void CompareDialog::on_webView2_loadFinished(bool arg1)
{
    qDebug() << __FUNCTION__ ;
    msgBox.close();
    (void)arg1;
    CMPTYPE type = getDocTypeFromTitle(ui->webView2->title());
    if(type != cmpType){
        QString str = QString("File '%1' is unexpected type.").arg(this->file2);
        QMessageBox::warning(this, "Warning", str);
        ui->btAddCmpPath->setDisabled(true);
        return;
    }

    fileCmps.append(this->file2);
    if(cmpType == CMPTYPE_ENYGYPLUS){
        QString projectName2 = getReportName(file2);
        doc->doCmp(projectName2, ui->webView2);
    }
    else if(cmpType == CMPTYPE_BEC){
        QString projectName2 = getReportName(file2);
        doc->doCmp(projectName2, ui->webView2);
    }
    else if(cmpType == CMPTYPE_OPENSTUDIO){
        QString projectName2 = getReportName(file2);
        doc->doCmp(projectName2, ui->webView2);
    }
}


void CompareDialog::on_webView_loadProgress(int progress)
{
    msgBox.setText(QString("Loading report 1 progress(%1 percent).").arg(progress));
}


void CompareDialog::on_webView2_loadProgress(int progress)
{
    msgBox.setText(QString("Loading report 2 progress(%1 percent).").arg(progress));
}
