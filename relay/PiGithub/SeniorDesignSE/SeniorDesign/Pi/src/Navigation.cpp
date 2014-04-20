#include "Navigation.h"

int testHeading = -1;

//initializes values, some may not be initialized yet
void Navigation::setup()
{
	x = 0;
	testCounter = 0;
	y = 0;
	currentTime = -1;
	simulationBegan = false;
	stabilizeSet = false;
	timerSet = false;
	checkStabilize = false;
	wallLocated = false;
} 

// the big navigation code
// takes in velocity and heading from nav data
// called by xbee
// first
// it tries to find a wall
// if it finds one, it turns until the wall is on the right
// otherwise, it charges forward until it find an obstacle/wall
// when found, it sets wall located to true so that it does not go through this again
// next
// if there is an obstacle on the right, and no obstacle in front, it goes forward
// if there is an obstacle in front, it will either turn right or left
// it turns right when there is no obstacle on the right, and left when there is an obstacle
// when the drone turns, it stops and hovers for a moment, setting the hover timer
// after that, it turns until there is an obstacle on the right
// it stops for a moment after turning and stabilizes and then continues moving forward
// it returns a suggested direction that xbee passes to udp which sends the command to the drone
int Navigation::navigate(float xVelocity, float yVelocity, int heading)
{
	checkSonar();

	if(!wallLocated)
	{
		//this code should be enabled when actually flying
		//commented out for testing purposes
		/*if (obstacleFront || obstacleLeft || obstacleRight || obstacleBack)
		{
			suggestedDirection = TURN_RIGHT;
			if(obstacleRight)
			{
				wallLocated = true;
			}
		}
		else
		{
			suggestedDirection = MOVE_FORWARD;
		}*/
		suggestedDirection = HOVER;

	}
	else
	{
		if (timerSet)
		{
			cout << timerSet << endl;
			cout << "Hover timer running" << endl;
			if(difftime(time(NULL),hoverTimer) > .5 && difftime(time(NULL),hoverTimer) < 5)
			{
				cout << "HOVERING" << endl;
				timerSet = false;
				if(!obstacleRight)
				{
					suggestedDirection = 6;
				}
				else if(obstacleRight)
				{
					suggestedDirection = 6;
				}
			
			checkStabilize = true;
			}
		}
		else if (checkStabilize)
		{
			cout << "PEREPARING TO STABILIZE" << endl;
			if(!obstacleFront && obstacleRight)
			{
				stabilizeTimer = time(NULL);
				stabilizeSet = true;
				checkStabilize = false;
			}
		}
		else if (stabilizeSet)
		{
			cout << "STABILIZING!" << endl;
			if(difftime(time(NULL), hoverTimer) > .5)
			{
				stabilizeSet = false;
			}
			suggestedDirection = 5;
		}
		else if (!obstacleRight || obstacleFront)
		{
			cout << "Obstacle encountered!" << endl;
			suggestedDirection = 5;
			hoverTimer = time(NULL);
			timerSet = true;
		}
		else if (obstacleRight && obstacleFront)
		{
			cout << "Obstacle encountered!" << endl;
			suggestedDirection = HOVER;
			hoverTimer = time(NULL);
			timerSet = true;
 		}
		else if (obstacleRight && !obstacleFront)
		{
			cout << "Obstacle on right, but none in front "  << endl;
			suggestedDirection = 2; //go foward?
		}

	}
	return suggestedDirection;
}

// Here, checking sonar shuold be implemented.
// Right now, it sets the obstacle flags based off of whatever time 
// it is. Eventually, the sonar values will be used to set the obstacle flags.
int Navigation::checkSonar()
{
	//simulated sonar code...
	if (!simulationBegan)
	{
		wallLocated = true;
		simulationBegan = true;
		simulationTimer = time(NULL);
	}
	if (difftime(time(NULL), simulationTimer) > 4.5)
	{
		cout << "Forward!" << endl;
		obstacleRight = true;
	}
	else if (difftime(time(NULL), simulationTimer) > 2.5)
	{
		cout << "Turn right!" << endl;
		obstacleRight = false;
	}
	else if (difftime(time(NULL),simulationTimer) > -1)
	{
		cout << "Foward!" << endl;
		obstacleRight = true;
		obstacleFront = false;
	}

}

//Code for implementing a safe return to home function that involves retracing steps
//Possibly outdated, never tested
void Navigation::updatePosition(float xVelocity, float yVelocity)
{
	x += xVelocity * difftime(currentTime,time(&timer)) / 1000000;
	y += yVelocity * difftime(currentTime,time(&timer)) / 1000000;
	currentTime = time(&timer);
	cout << "X: " << x << " Y: " << y << endl;
}

//Code for implementing a safe return to home function that involves retracing steps
//Possibly outdated, never tested
void Navigation::saveValue(float heading)
{
	waypoint wp;
	wp.distanceX = x;
	wp.distanceY = y;
	wp.heading = heading;
	waypoints.push(wp);
	x = 0;
	y = 0;
}

//Code for implementing a safe return to home function that involves retracing steps
//Possibly outdated, never tested
void Navigation::goHome(Pi *pi)
{
	x = 0;
	y = 0;
	waypoint wp;
	float goHomeX, goHomeY;
	int goHomeHeading;
	int currentHeading;
	navdata_demo navdata;
	while(!waypoints.empty())
	{
		wp = waypoints.top();
		waypoints.pop();
		goHomeX = wp.distanceX;
		goHomeY = wp.distanceY; 
		goHomeHeading = wp.heading / 100;
		navdata = pi->getNavData();
		currentHeading = navdata.psi / 100;
		int desiredHeading;
		while(goHomeHeading != currentHeading)
		{
			pi->blastCommand(ROTATE);
		}

		pi->blastCommand(HOVER);

		while(goHomeX > x && goHomeY > y)
		{
			navdata = pi->getNavData();
			updatePosition(navdata.vx,navdata.vy);
			switch(checkSonar())
			{
				case ALL_CLEAR:
					pi->blastCommand(MOVE_FORWARD);
					break;
				case OBSTACLE_ON_RIGHT:
					desiredHeading = ((int)navdata.psi + 10) % 360;
					while(!(navdata.psi < desiredHeading + 2 && navdata.psi > desiredHeading -2))
						pi->sendCommand(ROTATE);
					pi->blastCommand(HOVER);
					break;
				case OBSTACLE_ON_LEFT:
					desiredHeading = ((int)navdata.psi - 10) % 360;
					while(!(navdata.psi < desiredHeading + 2 && navdata.psi > desiredHeading -2))
						pi->sendCommand(ROTATE);
					pi->blastCommand(HOVER);
					break;
				case OBSTACLE_IN_FRONT:
					desiredHeading = ((int)navdata.psi + 10) % 360;
					while(!(navdata.psi < desiredHeading + 2 && navdata.psi > desiredHeading -2))
						pi->sendCommand(ROTATE);
					pi->blastCommand(HOVER);
					break;
			}
		}
	}
	pi->blastCommand(LAND);
}

