#include "interpreter.h"
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace std;

//այնպես ենք անում, որ մեծատառերի նկատմամբ զգայուն չլինի համակարգը
string toUpperCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return toupper(c); });
    return result;
}

//պարտադիր չէ մի բացատանիշ դնել հրամանը գրելու դեպքում, ինչքան ուզենք, կարող ենք գրել
string normalizeWhitespace(const string& str) {
    string result;
    bool inSpace = true;

    for (char c : str) {
        if (isspace(c)) {
            if (!inSpace && !result.empty()) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }

    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    return result;
}

Interpreter::Interpreter(ProblemDefinition* prob) : problem(prob) {
    for (const auto& var : problem->variables) {
        state[var.name] = var.initialValue;
    }
    history.push_back(state);
}

bool Interpreter::executeCommand(const string& commandName) {
    string normalizedCommand = toUpperCase(normalizeWhitespace(commandName));

    auto it = find_if(problem->commands.begin(), problem->commands.end(),
                      [&normalizedCommand](const CommandDefinition& cmd) {
                          return toUpperCase(normalizeWhitespace(cmd.name)) == normalizedCommand;
                      });

    if (it != problem->commands.end()) {
        it->action(state);
        history.push_back(state);
        return true;
    }

    cerr << "Անհայտ հրաման: " << commandName << endl;
    return false;
}

bool Interpreter::isSolved() const {
    return problem->goalChecker(state);
}

map<string, int> Interpreter::getState() const {
    return state;
}

const vector<map<string, int>>& Interpreter::getHistory() const {
    return history;
}

void Interpreter::visualizeState() const {
    for (const auto& var : problem->variables) {
        int value = state.at(var.name);
        cout << var.displayName << " [" << value << "/" << var.maxValue << "]: ";
        for (int i = 0; i < value; i++) {
            cout << "#";
        }
        cout << endl;
    }
}
