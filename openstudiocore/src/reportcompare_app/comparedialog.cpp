#include "comparedialog.h"
#include "ui_comparedialog.h"

#include <QWebEnginePage>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QFile>

//#define TESTDEBUG 1

CompareDialog::CompareDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompareDialog)
{
    ui->setupUi(this);
    cmpType = CMPTYPE_UNKNOW;

    QObject::connect(ui->btAddCmpPath
                     , SIGNAL(eventDropFilePath(const QString &))
                     , this
                     , SLOT(eventDropFilePath(const QString &)));

    ui->webView->setAcceptDrops(false);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint;
    setWindowFlags( flags );
    load2Compare = false;
}

bool CompareDialog::SetParam(int argc, char *argv[])
{
#ifndef TESTDEBUG
    if(argc>=4){
        return SetParam(argv[1], argv[2], argv[3]);
    }
   #else
	if (true){
		return SetParam("C:/Users/gorn/Desktop/op_proj/EEC Academy 9"
			, "C:/Users/gorn/Desktop/op_proj/EEC Academy 100"
			, "e");
	}
#endif
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

QString CompareDialog::loadFileToWebView(const QString& fn, QWebEngineView *webView, QString& htmlstr)
{
    FINDTARGETRES res;
    qDebug() << "File : " << fn;
    QString target;
    if(fn.endsWith(".html") || fn.endsWith(".htm")){
		qDebug() << "Fond report extension use path " << fn;
        target = fn;
    }
    else{
		qDebug() << "Not fond report extension use auto finder";
        target = findTargetPath(fn, res);
    }

    qDebug() << "Target : " << target;
    if(!target.isEmpty()){
        QFile f(target);
        if (f.open(QFile::ReadOnly | QFile::Text)){

            QTextStream in(&f);
            htmlstr = in.readAll();

            f.close();
        }

		webView->load(target);
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

    this->file1 = loadFileToWebView(this->file1, ui->webView, webStr1);

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
    QString ret = "";
    QString strlp = "";
    for(int i=0;i<fpls.count();i++){
        if(fpls.at(i) == "run"){
            ret = strlp;
            break;
        }
        strlp = fpls.at(i);
    }

    if(ret.isEmpty())
        return "????";
    else
        return ret;
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
			qDebug() << "Rege on " << newPath << " : " << reg;
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

QString CompareDialog::scanFolderPathWithEndWith(const QString& path, const QString& endwith, int recursive)
{
	recursive--;
	if (recursive < 0)
		return QString();

	QFileInfo rootFileInfo(path);
	if (rootFileInfo.isDir()){
		QDir dir(path);
		dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
		QStringList dirList = dir.entryList();
		for (int i = 0; i<dirList.size(); ++i)
		{
			QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
			bool match = newPath.endsWith(endwith);
			qDebug() << "Rege on " << newPath << " : is end with " << endwith;
			QFileInfo pathFileInfo(newPath);
			if (!match)
			{
				if (pathFileInfo.isDir()){
					QString matchPath = scanFolderPathWithEndWith(newPath, endwith, recursive);
					if (!matchPath.isEmpty()){
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
		bool match = path.endsWith(endwith);
		if (match)
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
	qDebug() << "Find path :" << filePath;
    static QRegExp becRegEx("(run[\\\\/]\\d[-]BEC[-]\\d[\\\\/]report\\.html)$");
    static QRegExp energyPlusRegEx("(run\\\\eplustbl\\.htm)$");
    static QRegExp openStudioRegEx("(run\\\\report\\.html)$");

    res = FINDTARGETRES_NONE;
    bool dup = false;
    if(!filePath.isEmpty()){
        QString matchPath;
        switch (cmpType) {
        case CMPTYPE_BEC:
            matchPath = scanFolderPathWithEndWith(filePath
                                                , "run/9-BEC-0/report.html", 3);
            break;
        case CMPTYPE_OPENSTUDIO:
            matchPath = scanFolderPathWithRegex(filePath
                                                , openStudioRegEx, 3);
            break;
        default:
            matchPath = scanFolderPathWithEndWith(filePath
                                                , "run/eplustbl.htm", 3);
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

static const QString jsscript =
        "<script>\r\n"
        "    window.tdiff = [];\r\n"
        "    fred = function(a, b) {\r\n"
        "        return a - b;\r\n"
        "    };\r\n"
        "\r\n"
        "    window.onload = function(e) {\r\n"
        "        console.log(\"window.onload\", e, Date.now(), window.tdiff,\r\n"
        "            (window.tdiff[1] = Date.now()) && window.tdiff.reduce(fred));\r\n"
        "        $COMPARE\r\n"
        "    }\r\n"
        "\r\n"
        "    function nextTableWithTitle(obj) {\r\n"
        "        obj.title = \"\";\r\n"
        "        var lastTitle = \"\";\r\n"
        "        var lastNode = null;\r\n"
        "        var isnext = false;\r\n"
        "        var tableNode = null;\r\n"
        "        while (obj.node != null) {\r\n"
        "            if (obj.node.nodeType == Node.ELEMENT_NODE) { //\r\n"
        "                if (obj.node.tagName.toUpperCase() == \"B\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H1\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H2\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H3\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H4\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H5\" ||\r\n"
        "                    obj.node.tagName.toUpperCase() == \"H6\") {\r\n"
        "                    lastTitle = obj.node.innerHTML;\r\n"
        "                    lastNode = obj.node;\r\n"
        "                    console.log(\"Last title \" + lastTitle);\r\n"
        "                } else if (obj.node.tagName.toUpperCase() == \"P\") {\r\n"
        "                    var ch = obj.node.firstChild;\r\n"
        "                    if (ch != null && ch.nodeType == Node.ELEMENT_NODE) {\r\n"
        "                        if (ch.tagName.toUpperCase() == \"B\") {\r\n"
        "                            lastTitle = ch.innerHTML;\r\n"
        "                            lastNode = ch;\r\n"
        "                            console.log(\"p.b Last title \" + lastTitle);\r\n"
        "                        }\r\n"
        "                    }\r\n"
        "                } else if (obj.node.tagName.toUpperCase() == \"TABLE\") {\r\n"
        "                    isnext = true;\r\n"
        "                    obj.found = true;\r\n"
        "                    break;\r\n"
        "                }\r\n"
        "            }\r\n"
        "            obj.node = obj.node.nextSibling;\r\n"
        "        }\r\n"
        "        obj.title = lastTitle;\r\n"
        "        obj.isnext = isnext;\r\n"
        "        obj.titlenode = lastNode\r\n"
        "        console.log(\"TRUE Last title \" + lastTitle);\r\n"
        "        return obj;\r\n"
        "    }\r\n"
        "\r\n"
        "    function lsTable(rootid) {\r\n"
        "        var elements = {};\r\n"
        "        var curnode = document.getElementById(rootid);\r\n"
        "        var obj = {\r\n"
        "            title: \"\",\r\n"
        "            titlenode: null,\r\n"
        "            node: curnode.firstChild,\r\n"
        "            isnext: false,\r\n"
        "            found: false\r\n"
        "        };\r\n"
        "        obj = nextTableWithTitle(obj);\r\n"
        "        var lasttt = obj.title;\r\n"
        "\r\n"
        "        if (obj.title != \"\")\r\n"
        "            elements[obj.title] = obj;\r\n"
        "\r\n"
        "        var lastNode = obj.node.nextSibling;\r\n"
        "        while (obj.found) {\r\n"
        "            var obj = {\r\n"
        "                title: \"\",\r\n"
        "                titlenode: null,\r\n"
        "                node: lastNode,\r\n"
        "                isnext: false,\r\n"
        "                found: false\r\n"
        "            };\r\n"
        "\r\n"
        "            obj = nextTableWithTitle(obj);\r\n"
        "            if (obj.found) {\r\n"
        "                elements[obj.title] = obj;\r\n"
        "                lastNode = lastNode = obj.node.nextSibling;\r\n"
        "            } else\r\n"
        "                break;\r\n"
        "        }\r\n"
        "        return elements;\r\n"
        "    }\r\n"
        "\r\n"
        "    function compare(label2, isRowHeader, isColumnHeader) {\r\n"
        "        tablels1 = lsTable(\"report1\");\r\n"
        "        tablels2 = lsTable(\"report2\");\r\n"
        "\r\n"
        "        for (var key in tablels1) {\r\n"
        "            if (tablels2.hasOwnProperty(key)) {\r\n"
        "                var table1 = tablels1[key];\r\n"
        "                var textnode = document.createTextNode(\", \" + label2);\r\n"
        "                table1.titlenode.parentNode.insertBefore(textnode, table1.titlenode.nextSibling);\r\n"
        "                var table2 = tablels2[key];\r\n"
        "                for (r = 0; r < table1.node.rows.length; r++) {\r\n"
        "                    for (c = 0; c < table1.node.rows[r].cells.length; c++) {\r\n"
        "                        if (r < table2.node.rows.length && c < table2.node.rows[r].cells.length) {\r\n"
        "                            var val = table1.node.rows[r].cells[c].innerHTML;\r\n"
        "                            var val2 = table2.node.rows[r].cells[c].innerHTML;\r\n"
        "                            if (isRowHeader && r == 0) {\r\n"
        "                                //table1.rows[r].cells[c].innerHTML = val + \",\" + label2;\r\n"
        "                            } else if (isColumnHeader && c == 0) {\r\n"
        "                                //table1.rows[r].cells[c].innerHTML = val + \",\" + label2;\r\n"
        "                            } else {\r\n"
        "                                table1.node.rows[r].cells[c].innerHTML = val + \",\" + val2;\r\n"
        "                            }\r\n"
        "                        }\r\n"
        "                    }\r\n"
        "                }\r\n"
        "            }\r\n"
        "        }\r\n"
        "    }\r\n"
        "</script>\r\n";

static QString b2s(bool val){
    if(val)
        return "true";
    else
        return "false";
}

void CompareDialog::LoadCompareFile(const QString& filePath)
{
    qDebug() << "filePath=" << filePath;
    file2 = filePath;
    file2 = loadFileToWebView(file2, ui->webView2, webStr2);
    reportName2 = getReportName(file2);
    qDebug() << "file2=" << file2;
    qDebug() << "reportName2=" << reportName2;
}

CompareDialog::CMPTYPE CompareDialog::getDocTypeFromTitle(const QString &title){
	qDebug() << "Title is " << title;
    if(title == "BEC Report")return CMPTYPE_BEC;
    if(title == "Results | OpenStudio")return CMPTYPE_OPENSTUDIO;
    if(title.indexOf("EnergyPlus")>=0) return CMPTYPE_ENYGYPLUS;
    return CMPTYPE_UNKNOW;
}

void CompareDialog::on_webView_loadFinished(bool arg1)
{
    //TODO: use this on load file finish
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

    if(!load2Compare)
        LoadCompareFile(this->file2);
}

void CompareDialog::DoCmp()
{
    QRegExp rx("(.+)<body>(.*)<\\/body>(.+)");
    rx.indexIn(webStr1);
    QStringList list1 = rx.capturedTexts();
    rx.indexIn(webStr2);
    QStringList list2 = rx.capturedTexts();

    QString strFinal;
    QString realjs;

    realjs = jsscript;
    realjs = realjs.replace("$COMPARE"
                            , CmpFunc());

    if (list1.count() == 4 && list2.count() == 4){
        strFinal = list1.at(1)
                + realjs
                + "<body>\n"
                //+"<div id=\"report1\" style=\"display: none;\">\n"
                + "<div id=\"report1\">\n"
                +list1.at(2) + "\n"
                +"</div>\n"
                +"<div id=\"report2\" style=\"display: none;\">\n"
                +list2.at(2)
                +"</div>\n"
                +"</body>"
                +list1.at(3);

        if(tfilePath.isEmpty())
            tfilePath = QString("%1/openstudio_cmp.html").arg(QDir::tempPath());

        QFile file(tfilePath);
        if (file.open(QIODevice::ReadWrite|QIODevice::Truncate)) {
            QTextStream stream(&file);
            stream << strFinal;
            file.flush();
            file.close();
        }
    }

    load2Compare = true;
    msgBox.close();
    ui->webView->load(tfilePath);
}

QString CompareDialog::CmpFunc()
{
    if (cmpType == CMPTYPE_OPENSTUDIO){
        return QString("compare('%1', %2, %3)")
                                .arg(reportName2)
                                .arg(b2s(true))
                                .arg(b2s(true));
    }else if(cmpType == CMPTYPE_ENYGYPLUS){
        return QString("compare('%1', %2, %3)")
                                .arg(reportName2)
                                .arg(b2s(true))
                                .arg(b2s(false));
    }else if(cmpType == CMPTYPE_BEC){
        return QString("compare('%1', %2, %3)")
                                .arg(reportName2)
                                .arg(b2s(true))
                                .arg(b2s(true));
    }
	return "compare('error', true, true)";
}

void CompareDialog::on_webView2_loadFinished(bool arg1)
{
    //TODO: LOAD FILE 2 AND COMPARE
	if (load2Compare){
		 ui->webView->page()->toPlainText([this](const QString &result){
			qDebug() << "html:";
			qDebug() << result;

            QRegExp rx("(.+)<body>(.*)<\\/body>(.+)");
            rx.indexIn(webStr2);
            QStringList list2 = rx.capturedTexts();
            QString strFinal;
            if (list2.count() == 4){
                strFinal = list2.at(2);
                strFinal = strFinal.replace("\"", "\\\"");
				strFinal = strFinal.replace("\r", "");
				strFinal = strFinal.replace("\n", "");
            }
			QString js02 =
                "document.getElementById(\"report2\").innerHTML = \""+strFinal+"\";"
                + CmpFunc()
                +";\n";
            

			ui->webView->page()->runJavaScript(js02);
		 });
	}
	else
	{
		DoCmp();
		load2Compare = true;
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
