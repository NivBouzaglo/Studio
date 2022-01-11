
#include "../include/Studio.h"
#include "iostream"

extern Studio *backup;
using namespace std;

Studio::Studio() {
    open = false;
    trainers = std::vector<Trainer *>();
    workout_options = std::vector<Workout>();
    actionsLog = std::vector<BaseAction *>();
}

Studio::Studio(const std::string &configFilePath) {
    Studio();
    std::ifstream file(configFilePath);
    int index = 1, w_id = 0;
    while (file.good()) {
        char arr[1000];
        file.getline(arr, 1000);
        if (arr[0] == '#' || arr[0] == '\0' || index == 2) {
            index++;
            continue;
        } else if (index == 5) {
            int i = 0;
            while (arr[i] != '\0') {
                if (arr[i] != ',') {
                      //Trainer *tr = new Trainer(int(arr[i])-48);
                    trainers.push_back(new Trainer(int(arr[i]) - 48));
                }
                i++;
            }
        } else {
            int i = 0, j = 0;
            string name, price;
            WorkoutType type;
            while (arr[j] != '\0') {
                if (arr[j] == ',') {
                    i++;
                    j++;
                } else {
                    if (i == 0) {
                        name = name + arr[j];
                    } else if (i == 1) {
                        if (arr[j] == 'A') {
                            type = ANAEROBIC;
                        } else if (arr[j] == 'C') {
                            type = CARDIO;
                        } else if (arr[j] == 'M') {
                            type = MIXED;
                        }
                    } else if (i == 2) {
                        price = price + arr[j];
                    }
                }
                j++;
            }
            stringstream pr(price);
            int p;
            pr >> p;
            workout_options.push_back(Workout(w_id, name, p, type));
            w_id++;
        }
        index++;
    }
    file.close();

}

void Studio::start() {
    cout << "studio is now open!" << endl;
    open = true;
    std::string input;
    std::string word;
    int c_id = 0;
    while (open) {
        cout << "please choose an action:" << endl;
        std::getline(cin, input);
        istringstream ss(input);
        ss >> word;
        if (word == "open") {
            ss >> word;
            std::vector<Customer *> customers = std::vector<Customer *>();
            stringstream i(word);
            int id = 0;
            i >> id;
            if (!this->trainers[id]->isOpen()) {
                while (ss >> word) {
                    bool get_in = false;
                    string name = "", c_type = "";
                    for (char j: word) {
                        if (j == ',') get_in = true;
                        else if (!get_in) name = name + j;
                        else c_type = c_type + j;
                    }
                    if (c_type == "swt") {
                        SweatyCustomer *c = new SweatyCustomer(name, c_id);
                        customers.push_back(c);
                    } else if (c_type == "fbd") {
                        FullBodyCustomer *c = new FullBodyCustomer(name, c_id);
                        customers.push_back(c);
                    } else if (c_type == "mcl") {
                        HeavyMuscleCustomer *c = new HeavyMuscleCustomer(name, c_id);
                        customers.push_back(c);
                    } else if (c_type == "chp") {
                        CheapCustomer *c = new CheapCustomer(name, c_id);
                        customers.push_back(c);
                    }
                    c_id++;
                }
            }
            OpenTrainer *openTrainer = new OpenTrainer(id, customers);
            for(auto *c: customers){
                delete c;
            }
            customers.clear();
            openTrainer->act(*this);
            actionsLog.push_back(openTrainer);

        } else if (word == "order") {
            ss >> word;
            stringstream t_id(word);
            int id = 0;
            t_id >> id;
            auto *order = new Order(id);
            order->act(*this);
            actionsLog.push_back(order);
        } else if (word == "move") {
            int c = 0, f = 0, t = 0;//customer ,from , to
            ss >> word;
            stringstream from(word);
            from >> f;
            ss >> word;
            stringstream to(word);
            to >> t;
            ss >> word;
            stringstream customer(word);
            customer >> c;
            auto *move = new MoveCustomer(f, t, c);
            move->act(*this);
            actionsLog.push_back(move);
        } else if (word == "close") {
            ss >> word;
            stringstream t_id(word);
            int tid = 0;
            t_id >> tid;
            auto *close = new Close(tid);
            close->act(*this);
            actionsLog.push_back(close);
        } else if (word == "closeall") {
            auto *ca = new CloseAll();
            ca->act(*this);
            actionsLog.push_back(ca);
            setOpen(false);
            clear();
        } else if (word == "workout_options") {
            auto *p = new PrintWorkoutOptions();
            p->act(*this);
            actionsLog.push_back(p);
        } else if (word == "status") {
            ss >> word;
            stringstream t_id(word);
            int id = 0;
            t_id >> id;
            auto *status = new PrintTrainerStatus(id);
            status->act(*this);
            actionsLog.push_back(status);
        } else if (word == "log") {
            auto *actions_log = new PrintActionsLog();
            actions_log->act(*this);
            actionsLog.push_back(actions_log);
        } else if (word == "backup") {
            BackupStudio *b = new BackupStudio();
            b->act(*this);
            actionsLog.push_back(b);
        } else if (word == "restore") {
            RestoreStudio *r = new RestoreStudio();
            r->act(*this);
            actionsLog.push_back(r);
        }
    }

    clear();
}


int Studio::getNumOfTrainers() const {
    return int(trainers.size());
}

Trainer *Studio::getTrainer(int tid) {
    if (this->getNumOfTrainers() < tid)
        return nullptr;
    else
        return trainers[tid];
}

std::vector<Trainer *> Studio::getTrainers() { return trainers; }

void Studio::setOpen(bool i) { open = i; }

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}



std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}


Studio::Studio(const Studio &studio) {//copy constructor


    this->open = studio.open;
    for (auto * tr: studio.trainers) {
        this->trainers.push_back(new Trainer(*tr));
    }
    for (auto &wo: studio.workout_options) {
        this->workout_options.push_back(wo);
    }

    for (auto *ba: studio.actionsLog) {
        this->actionsLog.push_back(ba->clone());
    }
}

Studio &Studio::operator=(const Studio &other) {//copy assignment
    if (this != &other) {
        clear();

        this->open = other.open;
        for (auto *tr: other.trainers) {
            this->trainers.push_back(new Trainer(*tr));
        }
        for (auto &wo: other.workout_options) {
            this->workout_options.push_back(wo);
        }

        for (auto *ba: other.actionsLog) {

            this->actionsLog.push_back(ba->clone());
        }
    }
    return *this;
}

Studio::Studio(Studio &&other) {//move constructor
    this->open = other.open;
    this->workout_options = move(other.workout_options);
    this->actionsLog = move(other.actionsLog);
    this->trainers = move(other.trainers);

}

Studio &Studio::operator=(Studio &&other) {//move assignment
    if (this != &other) {
       // delete this;
       clear();

        this->trainers = move(other.trainers);
        this->open = other.open;
        this->workout_options = move(other.workout_options);
        this->actionsLog = move(other.actionsLog);
    }
    return *this;
}

Studio::~Studio() {//destructor
    clear();
}

void Studio::clear() {
    for (auto *tr: trainers) {
        delete tr;
    }
    trainers.clear();
    for (auto *act: actionsLog) {
        delete act;
    }
    actionsLog.clear();
    workout_options.clear();

}