#include "c_controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    C_Controller w;
    w.show();
    return a.exec();
}
