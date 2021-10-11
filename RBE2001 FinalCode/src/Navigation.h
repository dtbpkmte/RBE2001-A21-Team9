#include<list>
#include <RBE1001Lib.h>


enum TURN {TURN_RIGHT = -90, TURN_STRAIGHT = 0, TURN_LEFT = 90, TURN_UTURN = 180};

class Navigation
{ 
    enum ROAD {ROAD_A, ROAD_B, ROAD_C,ROAD_D};
    enum DEST {HOUSE,PLATFORM1, PLATFORM2};

    ROAD currRoad = ROAD_A; //Random choose road A is the start Road
    DEST currDest = PLATFORM1;

public:
    //Keep track with the robot on the field
    ROAD GetRoad(void){return currRoad;}
    DEST GetDestination (void){return currDest; }
    TURN CalcTurn(void); //calculates next turn, depends on the currRoad
    void updateDestination(int key);
    // bool isOnRoadA();
    // bool isOnRoadB();
    // bool isOnRoadC();
    // bool isOnRoadD();
    bool ReachDestination(void)
    {
        bool value = false;
        switch(currDest){
            case HOUSE:
            if (currRoad == ROAD_B || currRoad == ROAD_C){
                value = true;
            }
            break;

            case PLATFORM1:
            if (currRoad == ROAD_A){
                value = true;
            }
            break;

            case PLATFORM2:
            if(currRoad == ROAD_D){
                value = true;
            }

            default:
            break;         
        }
        return value;
    }
};
