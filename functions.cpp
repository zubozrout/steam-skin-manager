#include <iostream>
#include <dirent.h>
#include <QFileDialog>
#include <sys/wait.h>
#include <QTextStream>
#include <sys/utsname.h>

using namespace std;

// Steam Skin Manager functions:

// This function retuns true if a specified file exists:
bool FileExists(const char* filename)
{
    QFile file (filename);
    if (file.exists())
        return true;
    return false;
}

// This function adds all avaialable skins within a specified directory to a list. Doesn't check for theme validity and lists all directories.
void AvaialbleItems(Ui::MainWindow *ui, const char* PATH)
{
    DIR *dir = opendir(PATH);
    struct dirent *entry = readdir(dir);
    printf("The following Steam skins were found:\n");
    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            cout << entry->d_name << endl;
            ui->comboBox->addItem(entry->d_name);
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return;
}

// This function returns command output as a string.
string GetCommandOutput(string cmd)
{
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

// This function allows SSM to load and save a specified file.
void SettingsFile(Ui::MainWindow *ui, bool open, string file_path)
{
    file_path += "/" + registry_name;
    cout << "Text file path: " << file_path << "  [ Called operation: " << ((open == true)?"Open":"Save") << " ]" << endl;
    QFile file(file_path.c_str());
    if(open)
    {
        if(file.open(QIODevice::ReadOnly|QIODevice::Text))
            ui->text_edit->setPlainText(QString::fromUtf8(file.readAll()));
        ui->save_status->setText("");
    }
    else
    {
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << ui->text_edit->toPlainText();
            file.flush();
            file.close();
            cout << "Successfuly saved." << endl;
            ui->save_status->setText("Successfully saved");
        }
        else
        {
            cout << "An error occured while saving." << endl;
            ui->save_status->setText("Can't be saved");
            ui->save_status->setStyleSheet("background-color:red; color:#fff;");
        }
    }
    return;
}

void reap_child(int sig)
{
    int status;
    waitpid(-1, &status, WNOHANG);
}

// Run Steam with or without native window border according to the parameter.
void start_steam(bool decorations)
{
    signal(SIGCHLD, reap_child);
    int fork_rv = fork();
    if (fork_rv == 0)
    {
        if(!system(decorations ? "steam-wb" : "steam"))
            cout << steam_execution_error << endl;
        _exit(1);
    }
    else if (fork_rv == -1)
    {
        cout << "Failed to fork() and run Steam" << endl;
    }
}
