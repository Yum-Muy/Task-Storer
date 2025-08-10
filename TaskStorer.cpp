#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

bool is_first_time = true;
int selection = NULL;

std::vector<std::string> tasks;

static void lbr() {
    std::cout << "\n";
}

static void str_and_break(std::string input) {
    std::cout << input; lbr();
}

static void intro() {
    str_and_break("Welcome to the Task Manager (totally original)!");
    str_and_break("Here, you can make and see your tasks here.");
    str_and_break("#----------#");
}

static void list_choices() {
    str_and_break("Select an option:");
    str_and_break("- 0  -  Exit");
    str_and_break("- 1  -  Add Task(s)");
    str_and_break("- 2  -  Delete Task(s)");
    str_and_break("- 3  -  View Current List");
}

static void recieve_input() {
    std::string input;
    bool valid = false;

    while (!valid) {
        std::cout << "-> ";
        std::getline(std::cin, input);

        bool all_digits = true;
        for (char c : input) {
            if (!isdigit(c)) {
                all_digits = false;
                break;
            }
        }

        if (!input.empty() && all_digits) {
            try {
                selection = std::stoi(input);
                if (selection >= 0 && selection <= 3) {
                    valid = true;
                }
                else {
                    str_and_break("Invalid input. Try again.");
                }
            }
            catch (const std::out_of_range&) {
                str_and_break("Invalid input. Try again.");
            }
        }
        else {
            str_and_break("Invalid input. Try again.");
        }
    }
}

static std::vector<std::string> parse_tasks(std::string input) {
    std::string current_word = "";
    std::vector<std::string> vector;

    if (input[input.size()] != '\\') {
        input += "\\";
    }

    for (int i = 0; i < input.size(); i++) {
        std::string current_chr = input.substr(i, 1);
        if (current_chr == "\\") {
            if (!current_word.empty()) {
                vector.push_back(current_word);
                current_word = "";
            }
        }
        else {
            current_word += current_chr;
        }
    }
    if (!current_word.empty()) {
        vector.push_back(current_word);
    }
    return vector;
}

static bool check_dupes(std::vector<int> vector) {
    int s = vector.size();

    for (int i = 0; i < s - 1; i++) {
        for (int j = i + 1; j < s; j++) {
            if (vector[i] == vector[j]) return true;
        }
    }

    return false;
}

static void show_current_tasklist() {
    str_and_break("The current tasklist: ");
    for (int i = 0; i < tasks.size(); i++) {
        str_and_break("- " + std::to_string(i) + ": " + tasks[i]);
    }
}

static bool out_of_bounds(std::vector<int> vector) {
    for (int i = 0; i < vector.size(); i++) {
        //str_and_break(std::to_string(vector[i]) + ": " + std::to_string(tasks.size()));
        if (vector[i] > tasks.size()) return true;
    }
    return false;
}

static bool check_if_nums(std::vector<std::string> vector) {
    for (std::string str: vector) {
        for (char chr : str) {
            if (!isdigit(chr)) {
                return false;
            }
        }
    }

    return true;
}

static std::vector<int> convert_to_nums(std::vector<std::string> vector) {
    std::vector<int> vector2;

    for (std::string str : vector) {
        vector2.push_back(std::stoi(str));
    }

    return vector2;
}

static void choice(int choice) {

    if (choice == 0) { // exit
        str_and_break("Goodbye!");
        exit(0);
    }
    else if (choice == 1) { // add tasks
        std::string input;
        std::cout << "Add task(s) (duplicates allowed, use \"\\\" to seperate):";
        std::getline(std::cin >> std::ws, input);
        // -------------------------------------------------------------------------

        std::vector<std::string> vector = parse_tasks(input);

        str_and_break("These will be added: ");
        for (int i = 0; i < vector.size(); i++) {
            str_and_break("- " + std::to_string(i) + ": " + vector[i]);
            tasks.push_back(vector[i]);
        }
    }
    else if (choice == 2) { // remove tasks
        std::string input;

        if (tasks.size() == 0) {
            str_and_break("There are no tasks yet.");
            return;
        }

        show_current_tasklist();

        str_and_break("Which tasks would you like to remove? (Select the index/indices. Duplicates not allowed, use \"\\\" to seperate):");
        std::getline(std::cin >> std::ws, input);
        // -------------------------------------------------------------------------

        std::vector<std::string> vector = parse_tasks(input);

        if (!check_if_nums(vector)) {
            str_and_break("(At least) one statement has a non-number! Try again.");
            return;
        }

        std::vector<int> vector2 = convert_to_nums(vector);

        if (check_dupes(vector2)) {
            str_and_break("Duplicate indices dectected! Try again.");
            return;
        }
        if (out_of_bounds(vector2)) {
            str_and_break("One or more indices were found out of bounds! Try again.");
            return;
        }

        std::ranges::sort(vector2, std::ranges::greater());
        for (int a : vector2) {
        }

        str_and_break("These will be removed: ");
        for (int i = vector2.size() - 1; i >= 0; i--) {
            str_and_break("- Index " + std::to_string(vector2[i]) + ": " + tasks[vector2[i]]);
        }

        for (int i = 0; i < vector2.size(); i++) {
            tasks.erase(tasks.begin() + vector2[i]);
        }

    }
    else if (choice == 3) { // show tasks
        if (tasks.size() == 0) {
            str_and_break("There are no tasks yet.");
            return;
        }
        show_current_tasklist();
    }
}

int main() {
    while (true) {
        if (is_first_time) {
            intro();
            is_first_time = false;
        }

        list_choices();
        recieve_input();


        str_and_break("#--------------------#");
        choice(selection);
        str_and_break("#--------------------#");
    }


    return 0;
}
