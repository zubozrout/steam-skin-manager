#include <iostream>

using namespace std;

// Steam Skin Manager UI actions:

void MainWindow::on_apply_bundled_released()
{
    string command = install_skin_command;
    command += bundled_skins_path;
    command += (ui->comboBox->itemText(ui->comboBox->currentIndex())).toUtf8().constData();
    command += "/";

    ManageSkin(ui, false, command, true);
}

void MainWindow::on_apply_bundled_and_run_released()
{
    string command = install_skin_command;
    command += bundled_skins_path;
    command += (ui->comboBox->itemText(ui->comboBox->currentIndex())).toUtf8().constData();
    command += "/";

    ManageSkin(ui, true, command, true);
}

void MainWindow::on_comboBox_currentIndexChanged()
{
    string path = bundled_skins_path;
    string selected_skin = (ui->comboBox->itemText(ui->comboBox->currentIndex())).toUtf8().constData();
    string image_path = path + selected_skin + "/preview.png";

    // Load image preview if avaialble
    if(FileExists(image_path.c_str()))
        ui->preview->setPixmap(QPixmap(image_path.c_str()));
    else
        ui->preview->setPixmap(QPixmap("/usr/share/steam-skin-manager/images/image_missing.png"));

    // Load description if avaialble
    string description = "/usr/share/steam/skins/" + selected_skin + "/description.txt";
    QFile file(description.c_str());
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
        ui->skin_description->setText(QString::fromUtf8(file.readAll()));
}

void MainWindow::on_choosedirectory_released()
{
    QString custom_path = QFileDialog::getExistingDirectory (this, tr("Directory"));
    if (custom_path.isNull())
    {
        ui->custom_path_textbox->setText(custom_not_set);
        ui->custom_path_textbox->setStyleSheet("background-color:red; color:#ffffff;");
        return;
    }
    ui->custom_path_textbox->setText(custom_path.toUtf8().constData());
    ui->custom_path_textbox->setStyleSheet("background-color:green; color:#ffffff;");

    cout << custom_path.toUtf8().constData() << endl;
}

void MainWindow::on_apply_custom_released()
{
    if (ui->custom_path_textbox->text() == "" || ui->custom_path_textbox->text() == custom_not_set)
    {
        ui->custom_path_textbox->setText(custom_not_set);
        ui->custom_path_textbox->setStyleSheet("background-color:#333333; color:#ffffff;");
        return;
    }

    string command = install_skin_command;
    string skin_path = "'" + ui->custom_path_textbox->text().toStdString() + "'";

    cout << "Path: " << skin_path;

    command += skin_path;
    ManageSkin(ui, false, command, false);
}

void MainWindow::on_apply_custon_and_run_released()
{

    if (ui->custom_path_textbox->text() == "" || ui->custom_path_textbox->text() == custom_not_set)
    {
        ui->custom_path_textbox->setText(custom_not_set);
        ui->custom_path_textbox->setStyleSheet("background-color:#333333; color:#ffffff;");
        return;
    }

    string command = install_skin_command;
    string skin_path = "'" + ui->custom_path_textbox->text().toStdString() + "'";

    ManageSkin(ui, true, command, false);
}

void MainWindow::on_actionQuit_activated()
{
    cout << "Exiting" << endl;
    exit(EXIT_SUCCESS);
}

void MainWindow::on_actionStart_without_decorations_activated()
{
    start_steam(false);
}

void MainWindow::on_actionStart_with_decorations_activated()
{
    start_steam(true);
}

void MainWindow::on_actionRevert_to_the_default_skin_activated()
{
    if(!system(revert_skin_command.c_str()))
       cout << execution_error << endl;
}

void MainWindow::on_save_settings_released()
{
    SettingsFile(ui, false, complete_path);
}

void MainWindow::on_text_edit_textChanged()
{
    ui->save_status->setText("File modified and not saved");
}

void MainWindow::on_reload_button_released()
{
    SettingsFile(ui, true, complete_path);
}

void MainWindow::on_go_home_released()
{
    ui->webView->load(QUrl("/usr/share/steam-skin-manager/help/help.html"));
}

void MainWindow::on_go_backward_released()
{
    ui->webView->triggerPageAction(QWebPage::Back);
}

void MainWindow::on_go_forward_released()
{
    ui->webView->triggerPageAction(QWebPage::Forward);
}

void MainWindow::on_webView_loadFinished()
{
    ui->web_title->setText(ui->webView->title());
    ui->url_box->setText(ui->webView->url().toString());
}

void MainWindow::on_url_box_returnPressed()
{
    ui->webView->load(QUrl(ui->url_box->text()));
}
