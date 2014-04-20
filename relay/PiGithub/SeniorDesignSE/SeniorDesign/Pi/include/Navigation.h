#include <stack>
#include <iostream>
#include <time.h>
#include "ConstantsAndHeaders.h"
#include "Pi.h"

using namespace std;

class Navigation 
{
public:
	void setup();
	int navigate(float xV, float yV, int heading);
	void goHome(Pi *pi);
private:
	void updatePosition(float xV, float yV);
	int checkSonar();
	void saveValue(float heading);

	float x;
	float y;
	struct waypoint {
		float distanceX;
		float distanceY;
		float heading;
	};
	stack<waypoint> waypoints;
	float currentTime;
	time_t timer;
	int navigationStatus;
	int suggestedDirection;
	int testCounter;
	float test_timer;
	bool wallLocated;
	bool obstacleFront;
	bool obstacleLeft;
	bool obstacleRight;
	bool obstacleBack;
	bool timerSet;
	time_t hoverTimer;
	time_t stabilizeTimer;
	bool checkStabilize;
	bool stabilizeSet;
	time_t simulationTimer;
	bool simulationBegan;
};

