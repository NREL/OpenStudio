#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include <QDialog>
#include <memory>
#include <QMessageBox>
#include <QTemporaryFile>

namespace Ui {
class CompareDialog;
}

class QWebElementCollection;
class QWebEngineView;
class IDoc;

class CompareDialog : public QDialog
{
    Q_OBJECT

public:
    enum CMPTYPE { CMPTYPE_UNKNOW, CMPTYPE_OPENSTUDIO, CMPTYPE_ENYGYPLUS, CMPTYPE_BEC };
    enum FINDTARGETRES { FINDTARGETRES_NONE=0, FINDTARGETRES_DUP, FINDTARGETRES_CANTFIND};
    explicit CompareDialog(QWidget *parent = 0);
    bool SetParam(int argc, char *argv[]);
    bool SetParam(const QString &file1, const QString &file2, const QString &type);
    ~CompareDialog();
    QString scanFolderPathWithRegex(const QString& path, const QRegExp& reg, int recursive);
	QString scanFolderPathWithEndWith(const QString& path, const QString& endwith, int recursive);
    void SetCmpType(CompareDialog::CMPTYPE type);
    bool isDuplicatePath(QString filePath);
    void doCmp(QString filePath);
    QString findTargetPath(const QString& filePath, CompareDialog::FINDTARGETRES &res);
    void msgRes(FINDTARGETRES res, QString filePath);
	QString loadFileToWebView(const QString& fn, QWebEngineView *webView, QString& htmlstr);
    void LoadCompareFile(const QString &filePath);
    CMPTYPE getDocTypeFromTitle(const QString& title);

    void DoCmp();
    void DoCmpJS2();
    QString CmpFunc();
private slots:
    void on_chkShowTree_clicked(bool checked);
    void on_btAddCmpPath_clicked();
    void eventDropFilePath(const QString& filePath);
    void on_webView_loadFinished(bool arg1);
    void on_webView2_loadFinished(bool arg1);
    void on_webView_loadProgress(int progress);
    void on_webView2_loadProgress(int progress);

private:
    Ui::CompareDialog *ui;
    QString seplustbl0, seplustbl1, sost0, sost1;
    QString file1, file2, reportName2, type;
    QStringList fileCmps;
    CMPTYPE cmpType;
    QString getReportName(const QString& filePath);
    QMessageBox msgBox;
    QStringList msgList;
    QString webStr1;
    QString webStr2;
    bool load2Compare;
    QString tfilePath;
};

#endif // COMPAREDIALOG_H
