#include "misc.h"

void message(QMessageBox::Icon icon, QString msg)
{
    QMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setText(msg);
    msgBox.exec();
}
