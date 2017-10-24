#include <fstream>
#include <iostream>
#include <sstream>

#include <sys/stat.h>

using namespace std;

std::string configOrConnectOption;
std::string configFilePath;
int  createConfig();
int  connect();
bool doesConfigExist(std::string configFile);
void printBadInputAndExit();

int main() 
{
    mkdir("$home/try", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    std::cout << "Create a config or connect? (cr/co)";
    std::cin >> configOrConnectOption;

    if(configOrConnectOption == "cr"){
        createConfig();
    }
    else if(configOrConnectOption == "co")
    {
        std::cout << "Enter name of config (and/or path): ";
        std::cin >> configFilePath;
        configFilePath = "WPA2_CONF/" + configFilePath;
        if(doesConfigExist(configFilePath)){
            connect();
        }
        else
        {
            std::string ans;
            std::cout << "The config does not exist, create one?(y/n)";
            std::cin >> ans;

            if(ans == "y")
            {
                createConfig();
            }
            else if(ans == "n")
            {
                std::cout << "Nothing to do here... Exiting." << std::endl;
                exit(1);
            }
            else
            {
                printBadInputAndExit();
            }
        }
    }
    else
    {
        printBadInputAndExit();
    }
    return 0;
}

int createConfig()
{
    string homeEnVar(getenv("HOME"));
    string path = homeEnVar + "/WPA2_CONF/";
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::stringstream ss;
    std::string configFileName;
    std::string SSID;
    std::string username;
    std::string password;

    std::cout << "Name of config file: ";
    std::cin  >> configFileName;
    configFileName = path + configFileName;
    ifstream ifExist;
    ifExist.open(configFileName);
    if (ifExist.is_open())
    {
        printf("Config file already exists!");
        return 0;
    }
    std::cout << "SSID: ";
    std::cin  >> SSID;
    std::cout << "Usernme: ";
    std::cin  >> username;
    std::cout << "password: ";
    std::cin  >> password;

    ss << "network={"                       << std::endl;
    ss << "ssid=\""     << SSID     << "\"" << std::endl;
    ss << "scan_ssid=1"                     << std::endl;
    ss << "key_mgmt=WPA-EAP"                << std::endl;
    ss << "identity=\"" << username << "\"" << std::endl;
    ss << "password=\"" << password << "\"" << std::endl;
    ss << "eap=PEAP"                        << std::endl;
    ss << "phase1=\"peaplabel=0\""          << std::endl;
    ss << "phase2=\"auth=MSCHAPV2\""        << std::endl;
    ss << "}"                               << std::endl;

    std::ofstream outStream;
    outStream.open(configFileName);

    if(outStream.is_open())
    {
        outStream << ss.str();
        outStream.close();
    }
    else
    {
        outStream.close();
        std::cout << "Could not open config file..." << std::endl;
        exit(1);
    }
    return 0;
}

int connect()
{

    std::string interface;

    system("sudo iw dev");
    std::cout << "Which interface would you like to use? ";
    std::cin >> interface;

    std::string wpaSupplicantCmd = "sudo wpa_supplicant -i " + interface + " -c " + configFilePath + " &";
    system(wpaSupplicantCmd.c_str());
    system("sleep 10");

    std::string dhcpcdCmd = "dhcpcd " + interface;
    system(dhcpcdCmd.c_str());

}

bool doesConfigExist(std::string configFile)
{
    std::ifstream testInput;
    testInput.open(configFile);

    return testInput.is_open();
}

void printBadInputAndExit()
{
    std::cout << "Invalid input... Exiting.";
    exit(1);
}
