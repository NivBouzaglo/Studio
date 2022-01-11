
#include "../include/Action.h"
#include "../include/Studio.h"
using namespace std;


BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const { return status; }
BaseAction::~BaseAction(){

}
void BaseAction::complete() { status = COMPLETED; }

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    std::cout << errorMsg << std::endl;
}
std::string BaseAction:: toString() const{return "";}
std::string BaseAction::getErrorMsg() const { return errorMsg; }

//OpenTRainer- fill toString
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id) {
//    for (auto *i: customersList) {
//        customers.push_back(i);
//    }
//    customersList.clear();


    for (auto *customer : customersList) {
        if (customer->toString() == customer->getName() + " ,chp") {
            Customer *c = new CheapCustomer(customer->getName(), customer->getId());
            customers.push_back(c);
        } else if (customer->toString() == customer->getName() + " ,swt") {
            Customer *c = new SweatyCustomer(customer->getName(), customer->getId());
            customers.push_back(c);
        } else if (customer->toString() == customer->getName() + " ,fbd") {
            Customer *c = new FullBodyCustomer(customer->getName(), customer->getId());
            customers.push_back(c);
        } else if (customer->toString() == customer->getName() + " ,mcl") {
            Customer *c = new HeavyMuscleCustomer(customer->getName(), customer->getId());

            customers.push_back(c);
        }
    }



}

void OpenTrainer::act(Studio &studio) {
    if (studio.getNumOfTrainers() < trainerId|| studio.getTrainer(trainerId)->isOpen()) {
        error("Workout session does not exist or is already open");
    } else {
        studio.getTrainer(trainerId)->openTrainer();
        for (auto &customer: customers) {
            studio.getTrainer(trainerId)->addCustomer(customer);
        }
        this->complete();
    }
}

std::string OpenTrainer::toString() const {
    std::string s = "open " ;
    s=s + std::to_string(trainerId);
    for (auto & i : customers) {
        s = s + " "+i->toString() ;
    }
    if (this->getStatus() == COMPLETED) {
        return s + " Completed";
    } else {
        return s + " Error: " + this->getErrorMsg();
    }
}

OpenTrainer::~OpenTrainer(){
    for(auto *c: customers){
        delete c;
    }
    customers.clear();
}

OpenTrainer *OpenTrainer::clone() {
    OpenTrainer *t = new OpenTrainer(this->trainerId,this->customers);
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }

    return t;
}

//Order
Order::Order(int id) : trainerId(id) {}

void Order::act(Studio &studio) {
    if (studio.getNumOfTrainers() < trainerId || !studio.getTrainer(trainerId)->isOpen()) {
        error("Trainer does not exist or is not open");
    } else {
        for (Customer *customer: studio.getTrainer(trainerId)->getCustomers()) {
            std::vector<int> customer_order = customer->order(studio.getWorkoutOptions());
            studio.getTrainer(trainerId)->order(customer->getId(), customer_order, studio.getWorkoutOptions());
        }
        for (std::pair<int, Workout> order: studio.getTrainer(trainerId)->getOrders()) {
            if (order.first != -1) {
                std::cout << studio.getTrainer(trainerId)->getCustomer(order.first)->getName() << " Is Doing "
                          << order.second.getName() << std::endl;
            }

        }
        this->complete();
    }

}

std::string Order::toString() const {
    std::string s = "order " + std::to_string(trainerId);
    if (this->getStatus() == COMPLETED)
        return s +" Completed";
    else
        return s + " Error:" + this->getErrorMsg();

}

Order *Order::clone() {
    Order *t = new Order(this->trainerId);
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}


//MoveCustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio) {
    if ((studio.getTrainer(srcTrainer) == nullptr) || (studio.getTrainer(dstTrainer) == nullptr)) {
        error("Cannot move customer");
    }
    else if (!studio.getTrainer(srcTrainer)->isOpen() || !studio.getTrainer(dstTrainer)->isOpen()) {
        error("Cannot move customer");
    }
    else if (studio.getTrainer(srcTrainer)->getCustomer(id) == nullptr) {
        error("Cannot move customer");
    }
    else {
        std::vector<int> customer_order;
        Trainer* t =&*studio.getTrainer(srcTrainer);
        Customer* c = t -> getCustomer(id);
        customer_order = c->order(studio.getWorkoutOptions());
        if (studio.getTrainer(dstTrainer)->getCapacity() <=
            int(studio.getTrainer(dstTrainer)->getCustomers().size() + 1)) {
            error("Cannot move customer");
        } else {
            studio.getTrainer(dstTrainer)->order(id, customer_order, studio.getWorkoutOptions());
            studio.getTrainer(dstTrainer)->addCustomer(c);
            studio.getTrainer(srcTrainer)->removeCustomer(id);
            this->complete();
        }
    }
}

std::string MoveCustomer::toString() const {
    std::string src = std::to_string(srcTrainer);
    std::string dst = std::to_string(dstTrainer);
    std::string c_id = std::to_string(id);

    std::string s = "move " + src +" "+ dst +" "+ c_id;
    if (this->getStatus() == COMPLETED)
        return s + " Completed";
    else
        return s + " Error:" + this->getErrorMsg();

}

MoveCustomer *MoveCustomer::clone() {
    MoveCustomer *t = new MoveCustomer(srcTrainer ,dstTrainer,id);
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

//CLose
Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *t = studio.getTrainer(trainerId);
    if (t == nullptr || !t->isOpen())
        error("Trainer does not exist or is not open");
    else {
        std::cout << "Trainer " << trainerId << " closed. Salary " << t->getSalary() << " NIS" << std::endl;
        t->closeTrainer();
        this->complete();
    }
}

std::string Close::toString() const {
    std::string s = "close " + std::to_string(trainerId);
    if (this->getStatus() == COMPLETED)
        return s + " Completed";
    else
        return s + "Error: " + this->getErrorMsg();
}

Close *Close::clone() {
    Close *t = new Close(this->trainerId);
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

//CLoseAll
CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (int i = 0;i< int(studio.getTrainers().size()); i++) {
        if(studio.getTrainers()[i]->isOpen()){
            Close c = Close(i);
            c.act(studio);
        }}
    this->complete();
    studio.setOpen(false);
}

std::string CloseAll::toString() const {
    return "closeall,Completed";
}

CloseAll *CloseAll::clone() {
    CloseAll *t = new CloseAll();
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for (auto w: studio.getWorkoutOptions()) {
        std::cout << w.getName() << ", " << w.getType() << ", " << w.getPrice() << std::endl;
    }
    this->complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options,Completed";
}

PrintWorkoutOptions *PrintWorkoutOptions::clone() {
    PrintWorkoutOptions *t = new PrintWorkoutOptions();
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *t = studio.getTrainer(trainerId);
    if (!t->isOpen())
        std::cout << "Trainer " << trainerId << " status: " << " closed" << std::endl;
    else {
        std::cout << "Trainer " << trainerId << " status: " << " open" << std::endl;
        std::cout << "Customers:\0 " << std::endl;
        for (auto &i: t->getCustomers()) {
            std::cout << i->getId() << " " << i->getName() << std::endl;
        }
        std::cout << "Orders:\0 " << std::endl;
        for (int x = 0; x < int(t->getOrders().size()); x++) {
            if (t->getOrders()[x].first != -1) {
                std::cout << t->getOrders()[x].second.getName() << " " << t->getOrders()[x].second.getPrice() << " NIS "
                          << t->getOrders()[x].first << std::endl;
            }
        }
        std::cout << "Current Trainer's Salary: " << t->getSalary() << std::endl;
        this->complete();
    }
}
std::string PrintTrainerStatus::toString() const {
    return "status " + std::to_string(trainerId) + " Completed";
}

PrintTrainerStatus *PrintTrainerStatus::clone() {
    PrintTrainerStatus *t = new PrintTrainerStatus(this->trainerId);
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

//PrintActionslog
PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio) {
    // here back
    for (auto &i : studio.getActionsLog()) {
        std::cout << i->toString() << std::endl;
    }

    this->complete();
}

std::string PrintActionsLog::toString() const {
    return "log,Completed";
}

PrintActionsLog *PrintActionsLog::clone() {
    PrintActionsLog *t = new PrintActionsLog();
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio) {
    if(backup){
        *backup = studio;
    }
    else{
        backup = new Studio(studio);
    }
    this->complete();
}

std::string BackupStudio::toString() const {
    return "backup Completed";
}

BackupStudio *BackupStudio::clone() {
    BackupStudio *t = new BackupStudio();
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}

RestoreStudio::RestoreStudio() {}

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        this->error("No backup available");
    } else {
        studio = *backup;
        backup->clear();
        delete backup;
        backup= nullptr;
    }
    this->complete();
}

std::string RestoreStudio::toString() const {
    if (this->getStatus() == COMPLETED) {
        return "restore, Completed";
    } else {
        return "restore,Error:" + this->getErrorMsg();
    }
}

RestoreStudio *RestoreStudio::clone() {
    RestoreStudio *t = new RestoreStudio();
    if (this->getStatus() == COMPLETED)
    {
        t->complete();
    } else if (this->getStatus() == ERROR)
    {
        t->error(this->getErrorMsg());
    }
    return t;
}