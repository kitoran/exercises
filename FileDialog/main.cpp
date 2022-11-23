#include <QApplication>
#include <QFileDialog>

#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);
    if(cmdOptionExists(argv, argv+argc, "--file-selection"))
    {
        QString res;
        if(cmdOptionExists(argv, argv+argc, "--save")) {
            res = QFileDialog::getSaveFileName(NULL, QString(), QString(), QString(), NULL);
        } else {
            res = QFileDialog::getOpenFileName(NULL, QString(), QString(), QString(), NULL);
        }
        printf("%s", res.toUtf8().data());
        if(res.isEmpty()) return EXIT_FAILURE;
        else return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
