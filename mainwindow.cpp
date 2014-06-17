#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.cpp"
#include "functions.cpp"
#include "uifunctions.cpp"
#include "actions.cpp"

#include <iostream>

using namespace std;

// Main Window
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    bool debug = true;

    ui->setupUi(this);

    // Call functions on start:
    AvaialbleItems(ui, bundled_skins_path.c_str());
    on_comboBox_currentIndexChanged();

    // Get current GTK theme:
    string theme = GetCommandOutput("dconf read /org/gnome/desktop/interface/gtk-theme | sed \"s/'\\(.*\\)'/\\1/\"");
    // Remove new line from the end of the theme string:
    theme.erase(remove(theme.begin(), theme.end(), '\n'), theme.end());
    printf("Current system theme is: %s\n", theme.c_str());
    // Choose current theme in the list:
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(QString::fromStdString(theme)));
    // Show app version to the GUI
    ui->versionlabel->setText(("<b>" + app_version + "</b>").c_str());

    SettingsFile(ui, true, complete_path);

    SystemInfo(ui, debug);
}

MainWindow::~MainWindow()
{
    delete ui;
}

