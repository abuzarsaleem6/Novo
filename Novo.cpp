#include <QDir>
#include <QApplication>
#include "QtWidgetsApplication1.h"

int main(int argc, char* argv[])
{
    // Create necessary directories
    QDir().mkpath("data/Users");
    QDir().mkpath("data/Posts");
    QDir().mkpath("data/Following");
    QDir().mkpath("data/Notifications");
    QDir().mkpath("data/Admin");
    QDir().mkpath("data/Messages");

    // NEW: Initialize admin on first run
    QFile adminFile("data/Admin/admin_credentials.txt");
    if (!adminFile.exists()) {
        if (adminFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&adminFile);
            out << "admin123|Admin#123|Platform Administrator\n";
            out << "moderator|mod#A123|Content Moderator\n";
            adminFile.close();
            qDebug() << "Admin credentials file created";
        }
    }

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}