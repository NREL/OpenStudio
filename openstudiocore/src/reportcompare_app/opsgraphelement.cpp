#include "opsgraphelement.h"
#include "idoc.h"

#include <QDebug>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QElapsedTimer>

const char * const months[]
= {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
};

OpsGraphElement::OpsGraphElement(IDoc *doc, const QString &title, QWebElement rootElm)
    :ICompareElement(doc)
    ,title(title)
    ,isValid(false)
    ,gother(NULL)
{
    if(rootElm.tagName().toUpper() != "SVG"){
        svg = rootElm.findFirst("SVG");
    }
    else{
        svg = rootElm;
    }
    if(svg.isNull()){
       errorMsgLn("Can't find svg element.");
       return;
    }

    qDebug() << "###############" << svg.toPlainText();

    float w = svg.attribute("width", "560").toFloat();
    float h = svg.attribute("height", "300").toFloat();

    baseW = QSize(w, h);

    ticks = svg.findAll(".tick");
    if(ticks.count()==0){
        errorMsgLn("can't find 'tick' class in svg tag.");
        return;
    }

    gs = svg.findAll(".g");
    if(gs.count()==0){
        errorMsgLn("can't find 'g' class in svg tag.");
        return;
    }

    legends = svg.findAll(".legend");

    isValid = true;
}

OpsGraphElement::~OpsGraphElement()
{

}

void OpsGraphElement::setPosXY(float x, float y)
{
    svg.findFirst("g").setAttribute("transform"
                      , QString("translate(%1, %2)")
                                    .arg(x)
                                    .arg(y));
}

QString OpsGraphElement::Id()
{
    return title;
}

void OpsGraphElement::setRect(const QSize& s)
{
    baseW = s;
}

const QString mwp =
"this.textContent='';\
var elm = document.createElementNS('http://www.w3.org/2000/svg', 'tspan');\
elm.setAttribute('x', '0'); \
elm.setAttribute('dy', '1.2em'); \
elm.textContent='%1';\
this.appendChild(elm);\
var elm2 = document.createElementNS('http://www.w3.org/2000/svg', 'tspan');\
elm2.setAttribute('x', '0'); \
elm2.setAttribute('dy', '1.2em'); \
elm2.textContent='(%2)';\
this.appendChild(elm2);null;";

bool OpsGraphElement::Compare(ICompareElement *other)
{
    //const QString format = "this.textContent='<tspan x=\"0\" dy=\"1.2em\">%1</tspan><tspan x=\"0\" dy=\"1.2em\">%2</tspan>'";
    //QString format = "this.textContent='%1(%2)'";
    QList<QWebElement> gsls;
    gother = static_cast<OpsGraphElement*>(other);
    if(!gother){
        errorMsgLn("Object type is not mismatch");
        return false;
    }
    foreach (QWebElement elm, gother->gs) {
        gsls.append(elm.clone());
    }
    otherGS.append(gsls);

    QList<QWebElement> tls;
    QElapsedTimer timer;
    timer.start();
    foreach (QWebElement elm, gother->ticks) {
        QWebElement cnelm = elm.clone();
        QWebElement txt = cnelm.findFirst("text");
        QString month = txt.evaluateJavaScript("this.textContent").toString();
        QString sct = QString(mwp).arg(month).arg(other->projectName());
        txt.evaluateJavaScript(sct).toString();
        tls.append(cnelm);
    }
    otherTick.append(tls);
    return true;
}

//svg width = 560 * compare count.
//svg set height to = 310
//Month(tick) start 24.5 + 41 attr transform="translate(24.5,0)"
//Graph(g) start 6 + 41 attr transform="translate(6,0)"

#define GSTART 6.0f
#define GXPOS 41.0f
#define MSTART 24.5f
#define MXPOS 41.0f

void OpsGraphElement::Update()
{
    qDebug() << "OpsGraphElement::Update:" << __LINE__
                << ", gother:" << gother
                << ", this.isValid:" << isValid
                << ", Error:" << errMsg;

    int cmpCount = otherGS.count()+1;

    svg.setAttribute("width", QString::number(svgWidth()));
    svg.setAttribute("height", "310");

    if(gother&&gother->isValid&&this->isValid){

        QWebElement rootgs = gs.at(0).parent();
        QWebElement rootticks = ticks.at(0).parent();

        QWebElementCollection::iterator gsit = gs.begin();
        QWebElementCollection::iterator ticksit = ticks.begin();

        QWebElementCollection::iterator ogsit = gother->gs.begin();
        QWebElementCollection::iterator oticksit = gother->ticks.begin();

        for(int i=0; i<12;i++ ){
            int gxi = GSTART+GXPOS*cmpCount*i;
            int txi = MSTART+MXPOS*cmpCount*i;
            setWebElementPos(*gsit, gxi, 0);
            setWebElementPos(*ticksit, txi, 0);

            for(int j=0;j<otherGS.count();j++){
                QWebElement g = otherGS.at(j).at(i);
                QWebElement tk = otherTick.at(j).at(i);

                rootgs.appendInside(g);
                rootticks.appendInside(tk);

                gxi+=GXPOS;
                txi+=MXPOS;

                setWebElementPos(g, gxi, 0);
                setWebElementPos(tk, txi, 0);
            }

            gsit++;
            ticksit++;
            ogsit++;
            oticksit++;
        }

        float yit = 0;
        foreach (QWebElement lg, legends) {
            setWebElementPos(lg, this->svgWidth()-baseW.width(), yit);
            yit+=20;
        }
    }
}

float OpsGraphElement::svgWidth()
{
    return baseW.width()*(otherGS.count()+1);
}

void OpsGraphElement::setWebElementPos(QWebElement elm, float x, float y)
{
    QString tsl = QString("translate(%1,%2)").arg(x).arg(y);
    elm.setAttribute("transform", tsl);
}

