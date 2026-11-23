#ifndef Auth_h
#define Auth_h
#include <string>
#include <vector>

static const std::string AUTH_FILE = "auth.txt";
class Auth
{

public:
    static bool parseLine(const std::string &line, std::string &user, std::string &pass, std::string &data);
    static bool validField(const std::string &s, bool allowEmpty = false);
    static std::vector<std::string> readAllLines();
    static bool writeAllLines(const std::vector<std::string> &lines);
    bool userExists(const std::string &user);
    bool appendUser(const std::string &user, const std::string &pass, const std::string &data = "");
    bool updateUserData(const std::string &user, const std::string &data);
    bool registerUser(const std::string &user, const std::string &pass, const std::string &data = "");
    bool checkLogin(const std::string &user, const std::string &pass);
    bool readUserData(const std::string &user, std::string &data);
    bool clearUserData(const std::string &user);
    bool removeUser(const std::string &user);
    bool auth(std::string &currPass, std::string &currUser);
};

#endif