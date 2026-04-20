#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();

private:
    // Ab yahan koi 'ui' object nahi hai, hum direct 'this' use karenge
};