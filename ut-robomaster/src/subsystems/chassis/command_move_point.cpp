#include "command_move_point.hpp"

namespace commands
{

void  CommandOdomPoint::initialize() 
{
    //initialize the target points, eventually read the waypoints from CV board
    desiredX = 0.5f;
    desiredY = 1.0f;

}

void  CommandOdomPoint::execute() 
{ 
    //?
    Vector2f inputMove = Vector2f(0.0f, 0.0005f);

    //get current position from odometry
    currentPosition = odometry->getPosition();
    currYaw = odometry->getChassisYaw();
    currX = currentPosition.getX();
    currY = currentPosition.getY();
    
    //calculate the angle to the target point, for moving the chassis to the target point
    //returns -pi to pi
    desiredYaw = atan2f(desiredY - currY, desiredX - currX);
    
    currYaw += M_PI;
    desiredYaw += M_PI;

    float outputRot, offset;
    float integral = 0, deriv;
    prevError = 0.0f;
    //turn the chassis first until reached desiredYaw
    //threshold is 0.1 radians
    for(float error = desiredYaw - currYaw; abs(error) > angle_threshold; error = desiredYaw - currYaw){
        float output;
        outputRot = 0.0f;
        output = error * p;
        integral = (integral + error) * i;
        deriv = (error - prevError) * d;

        output = output + integral + deriv;

        //need to spin the chassis CCW
        if (output > 0.5f)
        {
            outputRot = -0.05f;
        }
        //need to spin the chassis CW
        else if (output < -0.5f)
        {
            outputRot = 0.05f;
        }

        offset = 0.0f;
        if(error > 1.0f){
            offset = -0.05f;
        }
        else if(error < -1.0f){
            offset = 0.05f;
        }
        
        
        chassis->input(Vector2f(0.0f), outputRot + offset); //rotate the chassis

        prevError = error;
        //wait for a bit
        while(!refreshTimer.execute()){}

        currYaw = odometry->getChassisYaw();
    }
    
    //now rotated to correct angle, move to the target point
    chassis->input(Vector2f(0.0f), 0.0f); //stop the rotation

    inputMove = inputMove.rotate(desiredYaw); //rotate the movement vector relative to the desired yaw
    //move to the target point
    chassis->input(inputMove, 0.0f); 


    //keep moving until reached the target point
    //threshold is 0.1m
    for(float dist = sqrt(pow(desiredX - currX, 2) + pow(desiredY - currY, 2)); dist > dist_threshold; 
    dist = sqrt(pow(desiredX - currX, 2) + pow(desiredY - currY, 2))){
        currX = currentPosition.getX();
        currY = currentPosition.getY();
    }
    
    inputMove.setX(0); 
    inputMove.setY(0); 

    chassis->input(inputMove, 0.0f); 

}

void  CommandOdomPoint::end(bool) {chassis->input(Vector2f(0.0f), 0.0f);}

bool  CommandOdomPoint::isFinished() const { 
    //check if the chassis is at the target point within some threshold
    // if (fabs(currX - desiredX) < 0.1f && fabs(currY - desiredY) < 0.1f)
    // {
    //     return true;
    // }
    // else
    // {
    //     return false;
    // }   

 }
}  // namespace commands