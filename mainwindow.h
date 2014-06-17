#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_apply_bundled_released();

    void on_comboBox_currentIndexChanged();

    void on_apply_bundled_and_run_released();

    void on_choosedirectory_released();

    void on_apply_custom_released();

    void on_apply_custon_and_run_released();

    void on_actionQuit_activated();

    void on_actionStart_without_decorations_activated();

    void on_actionStart_with_decorations_activated();

    void on_actionRevert_to_the_default_skin_activated();

    void on_save_settings_released();

    void on_reload_button_released();

    void on_go_home_released();

    void on_go_backward_released();

    void on_go_forward_released();

    void on_webView_loadFinished();

    void on_url_box_returnPressed();

    void on_text_edit_textChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
