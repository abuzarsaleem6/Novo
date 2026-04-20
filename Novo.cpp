#include "QtWidgetsApplication1.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Husld");

    // Global dark palette (fallback for non-styled elements)
    QPalette pal;
    pal.setColor(QPalette::Window, QColor("#0C0C10"));
    pal.setColor(QPalette::WindowText, QColor("#E2E2EC"));
    pal.setColor(QPalette::Base, QColor("#141420"));
    pal.setColor(QPalette::AlternateBase, QColor("#18182A"));
    pal.setColor(QPalette::Text, QColor("#E2E2EC"));
    pal.setColor(QPalette::Button, QColor("#1C1C2C"));
    pal.setColor(QPalette::ButtonText, QColor("#E2E2EC"));
    pal.setColor(QPalette::Highlight, QColor("#4040E0"));
    pal.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    pal.setColor(QPalette::PlaceholderText, QColor("#404060"));
    app.setPalette(pal);

    MainWindow w;
    w.resize(1280, 820);
    w.show();

    return app.exec();
}