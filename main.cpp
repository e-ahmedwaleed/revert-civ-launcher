#include <string>
#include <fstream>
#include <dirent.h>
#include <regex>

using namespace std;

void revertLauncherLocation(string &dir, string &file_name);

int modified = 0;

int main() {
    DIR *dir;
    string dir_name = R"(C:\ProgramData\Epic\EpicGamesLauncher\Data\Manifests)";
    struct dirent *ent;
    if ((dir = opendir(dir_name.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            string file_name = ent->d_name;
            if (file_name.find(".item") != string::npos)
                revertLauncherLocation(dir_name, file_name);
        }
        closedir(dir);
        printf("%d file(s) modified.\n", modified);
    } else {
        /* could not open directory */
        perror("Error, please report back to the programmer");
    }
    return system("pause");
}

string loadFile(string &file_path);

void reWriteFile(string &file_path, string &file_contents);

void revertLauncherLocation(string &dir, string &file_name) {
    auto file_path = dir + '\\' + file_name;
    auto file_contents = loadFile(file_path);

    auto size = file_contents.size();

    if (file_contents.find("2KLauncher/LauncherPatcher.exe") != string::npos)
        file_contents = regex_replace(file_contents, regex(R"(2KLauncher\/LauncherPatcher.exe)"),
                                      "LaunchPad/LaunchPad.exe");

    if (file_contents.size() != size) {
        modified++;
        reWriteFile(file_path, file_contents);
    }
}

void reWriteFile(string &file_path, string &file_contents) {
    auto file = fopen(file_path.c_str(), "w");
    fprintf(file, "%s", file_contents.c_str());
    fclose(file);
}

string loadFile(string &file_path) {
    ifstream fs(file_path);
    string str((istreambuf_iterator<char>(fs)),
               istreambuf_iterator<char>());
    fs.close();
    return str[str.size() - 1] == '\n' ? str : str + "\n";
}
