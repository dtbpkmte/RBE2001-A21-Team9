#include <Navigation.h>
#include <RBE1001Lib.h>



extern Rangefinder ultrasonic;

bool Navigation::isOnRoadA(){
    bool ok = currRoad == ROAD_A;
    return ok;
}
bool Navigation::isOnRoadB(){
    bool ok = currRoad == ROAD_B;
    return ok;    
}
bool Navigation::isOnRoadC(){
    bool ok = currRoad == ROAD_C;
    return ok;    
}
bool Navigation::isOnRoadD(){
    bool ok = currRoad == ROAD_D;
    return ok;    
}

void Navigation::updateDestination (int key){
  if(key == 16){
    currDest = HOUSE;
  } else if (key == 17){
    currDest = PLATFORM1;
  } else if (key == 18){
    currDest = PLATFORM2;
  }

}


TURN Navigation::CalcTurn(void){
    switch(currRoad){
        case ROAD_A:
        if(currDest == PLATFORM1){
            currRoad = ROAD_A;
            Serial.printf("Still on road A \n");
            return TURN_STRAIGHT;
        
        } else if (currDest != PLATFORM1){
            if(currDest == HOUSE){
                currRoad = ROAD_B;
                Serial.printf("Update : currRoad is now road B \n");

                return TURN_RIGHT;
            
            } else if (currDest == PLATFORM2){
                printf("Please put the robot to RoadD");
                currRoad = ROAD_D;

            }
        }
        break;

        case ROAD_B:
        if(currDest == HOUSE){
            currRoad = ROAD_B;
             Serial.printf("Still on road B \n");

            return TURN_STRAIGHT;
        } else if(currDest == PLATFORM1){
             Serial.printf("Update currRoad is now road A \n");
            currRoad = ROAD_A;
            return TURN_LEFT;
        } else if (currDest == PLATFORM2){
            printf(" PLease put the robot to Road D");
            currRoad = ROAD_D;
        }

        case ROAD_C: 
        if(currDest == HOUSE){
            currRoad = ROAD_C;
            Serial.printf("Still on road C \n");

            return TURN_STRAIGHT;
        } else if(currDest == PLATFORM2){
            currRoad = ROAD_D;
            Serial.printf("Update currRoad is now road D \n");

            return TURN_RIGHT;
        } else if (currDest == PLATFORM1){
            printf(" PLease put the robot to Road B");
            currRoad = ROAD_B;
        }

        case ROAD_D:
        if(currDest == HOUSE){
            currRoad = ROAD_C;
            Serial.printf("Update currRoad is now road C \n");
            return TURN_LEFT;
        } else if(currDest == PLATFORM2){
            currRoad = ROAD_D;
            Serial.printf("Still on road D \n");
            return TURN_STRAIGHT;
        } else if (currDest == PLATFORM1){
            printf(" PLease put the robot to Road B");
            currRoad = ROAD_B;
        }
    }
}