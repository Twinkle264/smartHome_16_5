#include <iostream>
#include <sstream>
using namespace std;

int stateHome = 0;
enum switches {
    LIGHTS_INSIDE = 1,
    LIGHTS_OUTSIDE = 2,
    HEATERS = 4,
    WATER_PIPE_HEATING = 8,
    CONDITIONER = 16
};

bool checkInput(const string &text) {
    return (text == "yes" || text == "on");
}

void pipeHeating(int tempOut) {
    if (tempOut < 0 && !(stateHome & WATER_PIPE_HEATING)) {
        stateHome |= WATER_PIPE_HEATING;
        cout << "\tWater pipe heating is ON" << endl;
    } else if (tempOut > 5 && (stateHome & WATER_PIPE_HEATING)) {
        stateHome &= ~WATER_PIPE_HEATING;
        cout << "\tWater pipe heating is OFF" << endl;
    }
}

void houseHeating(int tempIn) {
    if (tempIn < 22 && !(stateHome & HEATERS)) {
        stateHome |= HEATERS;
        cout << "\tHeaters in house is ON" << endl;
    } else if (tempIn > 25 && (stateHome & HEATERS)) {
        stateHome &= ~HEATERS;
        cout << "\tHeaters in house is OFF" << endl;
    }
}

void houseConditioner(int tempIn){
    if (tempIn >= 30 && !(stateHome & CONDITIONER)){
        stateHome |= CONDITIONER;
        cout << "\tConditioner in house is ON" << endl;
    } else if (tempIn <= 25 && (stateHome & CONDITIONER)){
        stateHome &= ~CONDITIONER;
        cout << "\tConditioner in house is OFF" << endl;
    }
}

void lightGarden(int time, bool isMov) {
    int timeStart = 16, timeEnd = 5;
    if ((time >= timeStart || time <= timeEnd) && isMov) {
        stateHome |= LIGHTS_OUTSIDE;
        cout << "\tLight outside house is ON" << endl;
    } else if ((time >= timeStart || time <= timeEnd) && !isMov){
        stateHome &= ~LIGHTS_OUTSIDE;
        cout << "\tLight outside house is OFF" << endl;
    }
}

void lightColor(int time) {
    int lightColorMax = 5000, lightColorMin = 2700;
    int timeStart = 16, timeEnd = 20;

    cout << "\tLight color is ";
    if (time < timeStart) {
        cout << lightColorMax << "K" << endl;
    } else if (time < timeEnd) {
        cout << lightColorMax - (lightColorMax - lightColorMin) / (timeEnd - timeStart) * (time - timeStart) <<
             "K" << endl;
    } else {
        cout << lightColorMin << "K" << endl;
    }
}

void lightHouse(int time, bool isLight) {
    if (isLight && !(stateHome & LIGHTS_INSIDE)) {
        stateHome |= LIGHTS_INSIDE;
        cout << "\tLight in house is ON" << endl;
        lightColor(time);
    } else if (!isLight && (stateHome & LIGHTS_INSIDE)) {
        stateHome &= ~LIGHTS_INSIDE;
        cout << "\tLight in house is OFF" << endl;
    }
}

void timeNow(int hours) {
    cout << "\t------Current time is ";
    if (hours < 10) cout << '0';
    cout << hours << ":00------" << endl;
    cout << endl;
}

int main() {
    int temperatureOut = 0, temperatureIn = 0;
    string lightStr, movingStr;

    for (int i = 0; i < 2; ++i) {
        for (int hours = 0; hours < 24; ++hours) {

            timeNow(hours);

            string tmp;
            stringstream userInput;

            cout << "\tInput the temperature outside the house, the temperature inside the house," << endl;
            cout << "\tis there movement in the garden (yes/no), is the light on in the house (on/off):" << endl;
            getline(cin, tmp);

            userInput << tmp;
            userInput >> temperatureOut >> temperatureIn >> movingStr >> lightStr;
            cout << endl;

            pipeHeating(temperatureOut);
            lightGarden(hours, checkInput(movingStr));
            houseHeating(temperatureIn);
            houseConditioner(temperatureIn);
            lightHouse(hours, checkInput(lightStr));

            cout << endl;
        }
    }
    return 0;
}
