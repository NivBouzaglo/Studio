#ifndef STUDIO_H_
#define STUDIO_H_

#include <fstream>
#include "iostream"
#include <sstream>
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

extern Studio* backup;

class Studio{
public:
    Studio();
    Studio(const Studio& studio);//Rule of 5 - Copy Constructor.
    Studio& operator=(const Studio& other);//copy assignment
    Studio(Studio &&other);//move constructor
    Studio& operator=(Studio &&other);//move assignment
    virtual ~Studio();//destructor
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainers();
    void setOpen(bool isOpen);
    void clear();
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif