#include "QtWidgetsApplication1.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Load Google Fonts or fallback
    QFontDatabase::addApplicationFont(":/fonts/Sora-Regular.ttf");

    app.setFont(QFont("Sora", 10));

    // Global dark palette
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#0D0D0F"));
    palette.setColor(QPalette::WindowText, QColor("#E8E8ED"));
    palette.setColor(QPalette::Base, QColor("#14141A"));
    palette.setColor(QPalette::AlternateBase, QColor("#1A1A22"));
    palette.setColor(QPalette::Text, QColor("#E8E8ED"));
    palette.setColor(QPalette::Button, QColor("#1E1E28"));
    palette.setColor(QPalette::ButtonText, QColor("#E8E8ED"));
    palette.setColor(QPalette::Highlight, QColor("#5B5BF6"));
    palette.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));
    palette.setColor(QPalette::PlaceholderText, QColor("#555566"));
    app.setPalette(palette);

    MainWindow w;
    w.resize(1280, 820);
    w.show();

    return app.exec();
}