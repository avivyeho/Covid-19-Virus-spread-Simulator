

#include "Cell.h"
#include <cmath>

// cellIteration is only to be executed if cell is infected/PDA gives true.
void Cell::cellIteration(std::vector<Cell>* hospitals) {
    // The base spread factor is read from features[4]
    if(features[4] == 0){
        spread_factor = 1;
    }else if(features[4] == 1){
        spread_factor = 1.5;
    }else if(features[4] == 2){
        spread_factor = 2.3;
    }

    // Update each feature
    updateFeatures(hospitals);

    // Iterate over the feature vector, each feature changing the spread factor in its own way
    // {measures,Infrastructure,Hospital,Essential,VirusProp,populationAge}
    for (unsigned int c = 0; c < features.size(); c++) {
        if (c == 0) {
            if (features[c] == 0) {
                // no measures were taken yet.
            } else if (features[c] == 1) {
                // spreading awareness.
                spread_factor -= 0.05;
            } else if (features[c] == 2) {
                // protective clothing.
                spread_factor -= 0.08;
            } else if (features[c] == 3) {
                // social distancing.
                spread_factor -= 0.15;
            } else if (features[c] == 4) {
                // closing heavely populated sites.
                spread_factor -= 0.40;
            } else if (features[c] == 5) {
                // curfew/quarantine.
                spread_factor -= 0.75;
            }
        } else if (c == 1) {
            if (features[c] == 0) {
                // people are allowed to drive wherever, whenever
                spread_factor += 0.05;
            } else if (features[c] == 1) {
                // telecom companies increase efforts to connect people digitally, so less people feel the need
                // to do so physically
                spread_factor -= 0.05;
            } else if (features[c] == 2) {
                // Parcs & Recreational areas are closed
                spread_factor -= 0.12;
            } else if (features[c] == 3) {
                // Roads are closed for non-essential movement
                spread_factor -= 0.27;
            } else if (features[c] == 4) {
                // borders are closed
                spread_factor -= 0.35;
            } else if (features[c] == 5) {
                // Schools are closed
                spread_factor -= 0.42;
            } else if (features[c] == 6){
                // airports are closed
                spread_factor -= 0.46;
            }else if(features[c] == 7){
                // public transport is decreased & discouraged
                spread_factor -= 0.53;
            }
        } else if (c == 2) {
            if (features[c] <= 1) {
                // no patients with COVID-19 : sufficient capacity of staff and beds
            } else if (features[c] <= 3) {
                // first patients with COVID-19
                spread_factor += 0.15;
            } else if (features[c] <= 5) {
                // increase in COVID-19 cases : decreasing hospital staff and beds
                spread_factor += 0.25;
            } else if (features[c] <= 7) {
                // facilities at 50% of their capacity
                spread_factor += 0.35;
            } else if (features[c] == 8) {
                // facilities at = 70% of their capacity : limited beds left
                spread_factor += 0.43;
            }else if (features[c] == 9) {
                // facilities at >= 100% of their capacity : total chaos
                spread_factor += 0.49;
            }

        } else if (c == 3) {
            // situation of essential workers.
            if (features[c] == 0) {
                // under 10% is infected, no consequences.
            } else if (features[c] == 1) {
                // 10-20% infected.
                spread_factor += 0.05;
            } else if (features[c] == 2) {
                // 20-30% infected.
                spread_factor += 0.07;
            } else if (features[c] == 3) {
                // 30-40% infected.
                spread_factor += 0.10;
            } else if (features[c] == 4) {
                // 40-50% infected.
                spread_factor += 0.14;
            } else if (features[c] == 5) {
                // 50-60% infected.
                spread_factor += 0.19;
            } else if (features[c] == 6) {
                // 60-70% infected.
                spread_factor += 0.25;
            } else if (features[c] == 7) {
                // 70-80% infected.
                spread_factor += 0.32;
            } else if (features[c] == 8) {
                // 80-90% infected.
                spread_factor += 0.40;
            } else if (features[c] == 9) {
                // 90-100% infected.
                spread_factor += 0.49;
            }
        }else if(c == 5){
            if (features[c] <= 1) {
                // age difference in population is stable
            } else if (features[c] == 2) {
                // more teens in population than elderly
                spread_factor -= 0.15;
            } else if (features[c] <= 5) {
                // slightly more adults or elderly in population
                spread_factor += 0.18;
            } else if (features[c] <= 7) {
                // age differences in population increases
                spread_factor += 0.32;
            } else if (features[c] <= 9) {
                // age difference in population is unstable
                spread_factor += 0.40;
            }
        }

    }

    // If spread is negative, we don't support this so we reset it to be 1
    if (spread_factor < 1) {
        spread_factor = 1;
    }

    // If there are less infected then people, we calculate new infected amount
    if (infectionCount < population.size()) {
        infectionCount = spread_factor * infectionCount;
    }

    // if this new amount is more than population size, we reset it to population size
    if (infectionCount > population.size()) {
        infectionCount = population.size();
    }

    // we set the correct amount of individuals as infected
    for (int i = 0; i < floor(infectionCount); i++) {
        population[i]->infected = true;
    }
}


void Cell::updateFeatures(std::vector<Cell>* hospitals) {
    measures();
    infrastructure();
    situationOfEssentialWorkers();
    AgeDifference();
    HospitalCapabilities(hospitals,this->xPos,this->yPos);
}


void Cell::measures() {
    int current_measure = features[0];

    if (current_measure < 5) {
        int determine_chance_setting = (100 - (floor(infectionCount) / population.size())) / 5;

        // generate a number between 1 and the determine_chance_setting.
        int chance = rand() % determine_chance_setting + 1;

        if (chance == 1) {
            features[0] =  current_measure + 1;
        } else {
            features[0] = current_measure;
        }
    } else {
        features[0] = current_measure;
    }
}

void Cell::situationOfEssentialWorkers() {
    int total_essential_workers = 0;
    int infected_essential_workers = 0;

    for (Individual* i : population) {
        if (i->essential) {
            total_essential_workers++;
            if (i->infected) {
                infected_essential_workers++;
            }
        }
    }

    int percentage_of_infected = (infected_essential_workers / total_essential_workers) * 100;

    if (percentage_of_infected <= 10) {
        features[3] =  0;
    } else if (percentage_of_infected <= 20) {
        features[3] = 1;
    } else if (percentage_of_infected <= 30) {
        features[3] = 2;
    } else if (percentage_of_infected <= 40) {
        features[3] = 3;
    } else if (percentage_of_infected <= 50) {
        features[3] = 4;
    } else if (percentage_of_infected <= 60) {
        features[3] = 5;
    } else if (percentage_of_infected <= 70) {
        features[3] = 6;
    } else if (percentage_of_infected <= 80) {
        features[3] = 7;
    } else if (percentage_of_infected <= 90) {
        features[3] = 8;
    } else if (percentage_of_infected <= 100) {
        features[3] = 9;
    }
}

void Cell::HospitalCapabilities(std::vector<Cell>* hospitals, int r, int c) {
    int distance =0;

    if(this->DeadOrAlive){
        distance = abs((*hospitals)[r].hospital.column - c);
        if(this->population.size() > (*hospitals)[r].hospital.capacity){
            distance +=3;
        }
    }
    this->features[2] = distance;
}

void Cell::AgeDifference() {

    int difference=0;
    double kid=0,teen=0,adult=0,elderly=0;

    for(auto &person: this->population){
        if(person->age <= 11)
            kid+=1;
        else if(person->age <= 25)
            teen +=1;
        else if(person->age <= 65)
            adult +=1;
        else if(person->age <=100)
            elderly +=1;
    }


    if((kid/this->population.size())*100 < 11)
        difference+=2;
    if((teen/ this->population.size())*100 <15)
        difference+=1;
    if((adult/ this->population.size())*100 > 48)
        difference+=3;
    if((elderly/ this->population.size())*100 >15)
        difference+=3;

    this->features[5] = difference;
}

void Cell::infrastructure() {
    int current_infra = features[1];

    if (current_infra < 8) {
        int determine_chance_setting = (100 - (floor(infectionCount) / population.size())) / 5;

        // generate a number between 1 and the determine_chance_setting.
        int chance = rand() % determine_chance_setting + 1;

        if (chance == 1) {
            features[1] =  current_infra + 1;
        } else {
            features[1] = current_infra;
        }
    } else {
        features[1] = current_infra;
    }
}

