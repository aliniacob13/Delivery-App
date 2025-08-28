#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>
class MenuHelper
{
public:
static int choose_number_between_1_n(int n) {
    if (n < 1) {
      throw std::invalid_argument("n trebuie sa fie >= 1");
    }
    std::string line;
    while (true) {
      std::cout << "Please enter a number between 1 and " << n << ": ";
      if (!std::getline(std::cin >> std::ws, line)) {
        std::cin.clear();
        continue;
      }
      trim(line);  
      try {
        int x = std::stoi(line);
        if (x >= 1 && x <= n) {
          return x;
        }
        std::cout << "Numarul trebuie sa fie intre 1 si " << n << ".\n";
      }
      catch (const std::exception &e) {
        std::cout << "Va rugam introduceti un intreg\n";
      }
    }
  }
    static std::string promptClientOrStaff()
    {
        std::string input;
        while (true)
        {
            try
            {
                std::cout << "Client/Staff? ";
                if (!std::getline(std::cin, input))
                {
                    throw std::runtime_error("Eroare la citirea input-ului.");
                }
                for (char &c : input)
                {
                    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                }

                if (input == "client" || input == "staff")
                {
                    return input;
                }
                else
                {
                    throw std::invalid_argument("Trebuie sa fie \"client\" sau \"staff\"");
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Eroare la citirea input-ului: " << e.what() << ". Incearca din nou.\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "Eroare: " << e.what() << ". Incearca din nou.\n";
            }
        }
    }
    static void trim(std::string &s)
    {

        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
            s.erase(s.begin());
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
            s.pop_back();
    }
    static void toLower(std::string &s)
    {
        for (auto &c : s)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    static std::string DaorNu()
    {

        std::string line;
        while (true)
        {
            std::cout << "Da/Nu? ";
            
            if (!std::getline(std::cin >> std::ws, line))
            {
                std::cin.clear();
                continue;
            }
            trim(line);
            toLower(line);

            // lowercase
            for (auto &c : line)
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

            if (line == "da" || line == "nu")
                return line;

            std::cout << "Eroare la input. Trebuie sa fie ori \"da\" ori \"nu\". Incearca din nou.\n";
        }
    }
    static float read_non_negative_amount(std::istream& is) {
        std::string line;
        int value;
    
        while (true) {
            
            if (!std::getline(is, line)) {
                
                throw std::runtime_error("Nu am putut citi datele.");
            }
    
            try {
                
                size_t pos;
                value = std::stoi(line, &pos);
    
                
                if (pos != line.size()) {
                    throw std::invalid_argument("Caracter invalid gasit");
                }
    
                
                if (value < 0) {
                    throw std::invalid_argument("Numarul nu poate fi negativ.");
                }
    
                
                break;
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Input invalid (" << e.what()
                          << "). Introduceti un numar intreg pozitiv:\n";
                
            }
            catch (const std::out_of_range&) {
                std::cout << "Numarul e prea mare. Introduceti un numar intreg pozitiv:\n";
            }
        }
    
        return static_cast<float>(value);
    }

    static float read_non_negative_float(std::istream& is) {
        std::string line;
        float value;
    
        while (true) {
            
            if (!std::getline(is, line)) {
                
                throw std::runtime_error("Nu am putut citi datele.");
            }
    
            try {
                
                size_t pos;
                value = std::stof(line, &pos);
    
                
                if (pos != line.size()) {
                    throw std::invalid_argument("Caracter invalid gasit");
                }
    
                
                if (value < 0) {
                    throw std::invalid_argument("Numarul nu poate fi negativ.");
                }
    
                
                break;
            }
            catch (const std::invalid_argument& e) {
                std::cout << "Input invalid (" << e.what()
                          << "). Introduceti un numar pozitiv:\n";
                
            }
            catch (const std::out_of_range&) {
                std::cout << "Numarul e prea mare. Introduceti un numar pozitiv:\n";
            }
        }
    
        return value;
    }
};

