#include <iostream>

using namespace std;

// Steam Skin Manager UI functions:

// Shows the state of Steam and system info related to the current user.
void SystemInfo(Ui::MainWindow *ui, bool tout)
{
    // Kernel info
    struct utsname uname_data;
    uname(&uname_data);

    // commnad output info -- # not much nice, would be fine if substitued by some better way
    string number_of_installed_items = GetCommandOutput("echo $((`find " + complete_path + "/steam/SteamApps/" + user_name + " -maxdepth 1 -mindepth 1 -type d | wc -l` + `find " + complete_path + "/steam/SteamApps/common" + " -maxdepth 1 -mindepth 1 -type d | wc -l`))");
    string used_space = GetCommandOutput("du -ch " + complete_path + "/steam/" + " | tail -1 | cut -f1 | tr -d '\n'");
    string free_space = GetCommandOutput("df -h " + complete_path + "/steam/" + " | tail -1 | sed -e 's/ \\+/ /g' | cut -f4 -d' ' | tr -d '\n'");
    string graphics_card = GetCommandOutput("lspci | grep -i 'vga' | cut -f3 -d':' | sed 's/^ //' | tr -d '\n'");

    // Skin Manager architecture:
    char const* arch =
#if defined(__i386__)
            "x86"
#elif defined(__x86_64__)
            "x86_64"
#else
        #error "unknown platform"
#endif
    ;

    // Info-output
    ui->locationpath->setText((complete_path + "/steam").c_str());
    ui->numberofgamesnum->setText(number_of_installed_items.c_str());
    ui->spacesize->setText(used_space.c_str());
    ui->freespacesize->setText(free_space.c_str());
    ui->kerneltype->setText(uname_data.sysname);
    ui->kernelversion->setText(uname_data.release);
    ui->systemarch->setText(uname_data.machine);
    ui->graphicscard->setText(graphics_card.c_str());
    ui->ssmarchitecture->setText(arch);
    string ver = (tout ? "dbg" : "rel");
    ui->ssmversion->setText((ver + " " + app_version).c_str());

    if (tout)
    {
        cout << "Steam path: " << complete_path << "/steam" << endl;
        cout << "Num of installed games:" << number_of_installed_items << endl;
        cout << "Used space: " << used_space << endl;
        cout << "Free space: " << free_space << endl;
        cout << "Kernel: " << uname_data.sysname << endl;
        cout << "Kernel version: " << uname_data.release << endl;
        cout << "System marchitecture: " << uname_data.machine << endl;
        cout << "Graphics card: " << graphics_card << endl;
        cout << "Steam Skin Manager architecture: " << arch << endl;
    }

    return;
}

// Complex function that applies skin and can optionally execute Steam after that.
void ManageSkin(Ui::MainWindow *ui, bool execute_steam, string command, bool consider_buttons)
{
    bool buttons;

    if(consider_buttons)
    {
        if(ui->checkbuttons->isChecked())
        {
            command += skins_without_buttons;
            buttons = false;
        }
        else
        {
            command += skins_with_buttons;
            buttons = true;
        }
    }

    printf("%s\n", command.c_str());

    int status_code = system(command.c_str());
    if(status_code != -1 && status_code != 127)
    {
        ui->preview->setStyleSheet("background-color:green;");
    }
    else
    {
        ui->preview->setStyleSheet("background-color:red;");
    }

    if(execute_steam)
    {
        if(!consider_buttons || buttons)
        {
            start_steam(false);
        }
        else
        {
            start_steam(true);
        }
    }
}
