#include <QApplication>
#include <QIcon>
#include "mainwindow.h"
#include "style.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(Style::appStyleSheet());
    app.setWindowIcon(QIcon(QStringLiteral(":/assets/audiolocal-icon.png")));

    MainWindow w;
    w.show();
    return app.exec();
}
