#include <QApplication>
#include "mainwindow.h"

// This tool is licensed under the terms of GNU GPL3 license
// and so it is avaiable for free to everyone.
// You can copy it, share it, modify it and of course use it.

// Created by: Martin Kozub <zubozrout@gmail.com>
// because Linux matters ... :)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
