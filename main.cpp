#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {

  QSurfaceFormat format;

  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 0);

  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
