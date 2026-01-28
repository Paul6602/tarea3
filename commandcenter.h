#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <functional>
#include "Entity.h"

using Command = std::function<void(const std::list<std::string>&)>;

class CommandCenter {

    Entity& entity;
    std::map<std::string, Command> commands;

    using MacroSteps = std::list<std::pair<std::string, std::list<std::string>>>;
    std::map<std::string, MacroSteps> macros;

    std::list<std::string> history;

public:
    CommandCenter(Entity& e) : entity(e) {}

    void registerCommand(const std::string& name, Command cmd) {
        commands[name] = cmd;
    }

    void registerMacro(const std::string& name, const MacroSteps& steps) {
        macros[name] = steps;
    }

    bool executeCommand(const std::string& name, const std::list<std::string>& args) {
        std::map<std::string, Command>::iterator it = commands.find(name);
        if (it != commands.end()) {
            std::string preStatus = entity.getStatus();
            it->second(args);
            std::string postStatus = entity.getStatus();
            std::string logEntry = "CMD: " + name;
            if (!args.empty()) {
                logEntry += " [args: ";
                for (const auto& s : args) logEntry += s + " ";
                logEntry += "]";
            }
            logEntry += " || PRE: [" + preStatus + "] -> POST: [" + postStatus + "]";
            history.push_back(logEntry);

            return true;
        }
        else {
            std::cout << "[Error] Comando no reconocido: " << name << std::endl;
            return false;
        }
    }

    void executeMacro(const std::string& name) {
        std::map<std::string, MacroSteps>::iterator it = macros.find(name);

        if (it != macros.end()) {
            std::cout << ">>> Iniciando Macro: " << name << std::endl;

            const MacroSteps& steps = it->second;

            for (MacroSteps::const_iterator stepIt = steps.begin(); stepIt != steps.end(); ++stepIt) {
                std::string cmdName = stepIt->first;
                std::list<std::string> cmdArgs = stepIt->second;

                if (commands.find(cmdName) == commands.end()) {
                    std::cout << "[Error] Macro abortado. Paso '" << cmdName << "' no existe." << std::endl;
                    break;
                }

                executeCommand(cmdName, cmdArgs);
            }
            std::cout << "<<< Fin Macro: " << name << std::endl;
        } else {
            std::cout << "[Error] Macro no encontrado: " << name << std::endl;
        }
    }

    void removeCommand(const std::string& name) {
        std::map<std::string, Command>::iterator it = commands.find(name);
        if (it != commands.end()) {
            commands.erase(it);
            std::cout << "[Info] Comando '" << name << "' eliminado." << std::endl;
        } else {
            std::cout << "[Warning] No se pudo eliminar '" << name << "', no existe." << std::endl;
        }
    }

    void printHistory() const {
        std::cout << "\n--- Historial de Ejecucion ---" << std::endl;
        for (std::list<std::string>::const_iterator it = history.begin(); it != history.end(); ++it) {
            std::cout << *it << std::endl;
        }
        std::cout << "------------------------------\n" << std::endl;
    }
};

#endif