#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <iterator>
#include "Entity.h"
#include "CommandCenter.h"

using namespace std;

void cmdTeleport(Entity& e, const list<string>& args) {
    if (args.size() != 2) {
        cout << "[Error] Teleport necesita 2 argumentos (x, y)." << endl;
        return;
    }
    try {
        auto it = args.begin();
        int newX = stoi(*it);
        advance(it, 1);
        int newY = stoi(*it);

        e.reset();
        e.move(newX, newY);
    } catch (...) {
        cout << "[Error] Argumentos invalidos en Teleport." << endl;
    }
}

struct HealCommand {
    Entity& entity;
    int counter = 0;

    HealCommand(Entity& e) : entity(e) {}

    void operator()(const list<string>& args) {
        if (args.empty()) {
            cout << "[Error] Heal necesita 1 argumento." << endl;
            return;
        }
        try {
            float amount = stof(args.front());
            entity.heal(amount);
            counter++;
        } catch (...) {
            cout << "[Error] Cantidad de curacion invalida." << endl;
        }
    }
};

int main() {

    Entity Robot("RoboCop", 0, 0);
    CommandCenter cc(Robot);

    cc.registerCommand("teleport", [&Robot](const list<string>& args) {
        cmdTeleport(Robot, args);
    });

    HealCommand healer(Robot);
    cc.registerCommand("heal", healer);

    cc.registerCommand("damage", [&Robot](const list<string>& args) {
        if (args.empty()) return;
        try {
            float dmg = stof(args.front());
            Robot.damage(dmg);
        } catch (...) { cout << "[Error] Damage invalido." << endl; }
    });

    cc.registerCommand("status", [&Robot](const list<string>&) {
        Robot.printStatus();
    });

    cc.registerCommand("reset", [&Robot](const list<string>&) {
        Robot.reset();
    });

    list<pair<string, list<string>>> restoreSteps;
    restoreSteps.push_back({"reset", {}});
    restoreSteps.push_back({"heal", {"100"}});
    restoreSteps.push_back({"status", {}});
    cc.registerMacro("full_restore", restoreSteps);

    list<pair<string, list<string>>> escapeSteps;
    escapeSteps.push_back({"teleport", {"100", "100"}});
    escapeSteps.push_back({"heal", {"20"}});
    cc.registerMacro("escape_tactic", escapeSteps);

    list<pair<string, list<string>>> suicideSteps;
    suicideSteps.push_back({"teleport", {"50", "50"}});
    suicideSteps.push_back({"damage", {"200"}});
    suicideSteps.push_back({"status", {}});
    cc.registerMacro("suicide_run", suicideSteps);

    cc.executeCommand("status", {});
    cc.executeCommand("teleport", {"10", "20"});
    cc.executeCommand("damage", {"40.5"});

    cc.executeCommand("status", {});

    cc.executeCommand("volar", {});
    cc.executeCommand("teleport", {"hola"});

    cc.executeMacro("escape_tactic");
    cc.executeMacro("suicide_run");

    cc.removeCommand("teleport");
    cc.executeCommand("teleport", {"10", "10"});

    cc.printHistory();
}