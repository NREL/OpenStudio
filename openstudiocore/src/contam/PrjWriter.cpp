#include "PrjReader.hpp"
//#include <iostream>
#include <stdlib.h>

namespace openstudio {
namespace contam {
namespace prj {

QString writeEmptySection(QString label)
{
    QString string;
    if(label.isNull())
        string += QString("0\n");
    else
        string += QString("0 ! %1\n").arg(label);
    return string + QString("-999\n");
}

}
}
}
