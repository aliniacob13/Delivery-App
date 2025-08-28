#pragma once
#include <string>
#include <fstream>

class DBConfig {
    std::string server, user, pass, name;
    public:
    std::string getServer() const { return server; }
    std::string getUser() const { return user; }
    std::string getPass() const { return pass; }
    std::string getName() const { return name; }
    void setPass(const std::string& p) { pass = p; }
    void setServer(const std::string& s) { server = s; }
    void setUser(const std::string& u) { user = u; }
    void setName(const std::string& n) { name = n; }
};

inline DBConfig load_db_config(const std::string& path = ".env") {
    DBConfig cfg;
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Nu pot deschide " + path);

    std::string line;
    while (std::getline(f, line)) {
        // suport mic pentru CRLF pe Windows
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.rfind("DB_SERVER=", 0) == 0)  cfg.setServer(line.substr(10));  
        else if (line.rfind("DB_USER=",   0) == 0) cfg.setUser(line.substr(8));    
        else if (line.rfind("DB_PASS=",   0) == 0) cfg.setPass(line.substr(8));   
        else if (line.rfind("DB_NAME=",   0) == 0) cfg.setName(line.substr(8));
   }

    if (cfg.getServer().empty() || cfg.getUser().empty() || cfg.getName().empty()) {
        throw std::runtime_error("Lipsesc câmpuri obligatorii din .env (DB_SERVER/DB_USER/DB_NAME).");
    }
    return cfg;
}