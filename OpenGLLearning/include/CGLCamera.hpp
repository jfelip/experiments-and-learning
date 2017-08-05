#ifndef __CGLCAMERA_H_
#define __CGLCAMERA_H_

//
// Camera Class for OpenGL adapted from https://learnopengl.com/#!Getting-started/Camera
//

// Std. Includes
#include <vector>
#include <CVector3.hpp>

// GL Includes
#include <GL/glew.h>


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const double YAW        = -M_PI/2.0f;
const double PITCH      =  0.0f;
const double SPEED      =  3.0f;
const double SENSITIVTY =  0.000002f;
const double ZOOM       =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
template<class T_Real=GLfloat>
class Camera
{

public:
    // Camera Attributes
    vec3::Vector3<T_Real> Position;
    vec3::Vector3<T_Real> Front;
    vec3::Vector3<T_Real> Up;
    vec3::Vector3<T_Real> Right;
    vec3::Vector3<T_Real> WorldUp;
    // Eular Angles
    T_Real Yaw;
    T_Real Pitch;
    // Camera options
    T_Real MovementSpeed;
    T_Real MouseSensitivity;
    T_Real Zoom;

    // Constructor with vectors
    Camera( vec3::Vector3<T_Real> position = vec3::Vector3<T_Real>(0.0f, 10.0f, 0.0f),
    		vec3::Vector3<T_Real> up  = vec3::Vector3<T_Real>(0.0f, 1.0f, 0.0f),
			T_Real yaw = YAW,
			T_Real pitch = PITCH) : Front(vec3::Vector3<T_Real>(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(T_Real posX, T_Real posY, T_Real posZ, T_Real upX, T_Real upY, T_Real upZ, T_Real yaw, T_Real pitch) : Front(vec3::Vector3<T_Real>(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = vec3::Vector3<T_Real>(posX, posY, posZ);
        WorldUp = vec3::Vector3<T_Real>(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    void GetViewMatrix(T_Real view[16])
    {
    	view[0] = this->Right[0];
    	view[1] = this->Right[1];
    	view[2] = this->Right[2];
    	view[3] = 0;

    	view[4] = this->Up[0];
    	view[5] = this->Up[1];
    	view[6] = this->Up[2];
    	view[7] = 0;

		view[8]  = this->Front[0];
		view[9]  = this->Front[1];
		view[10] = this->Front[2];
		view[11] = 0;

		view[12] = this->Right[0] * this->Position[0] + this->Right[1] * this->Position[1] + this->Right[2] * this->Position[2];
		view[13] = this->Up[0] * this->Position[0] + this->Up[1] * this->Position[1] + this->Up[2] * this->Position[2];
		view[14] = this->Front[0] * this->Position[0] + this->Front[1] * this->Position[1] + this->Front[2] * this->Position[2];
		view[15] = 1;
    }

    // Processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, T_Real deltaTime)
    {
        T_Real velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position += this->Right * velocity;
        if (direction == RIGHT)
            this->Position -= this->Right * velocity;
        if (direction == UP)
            this->Position += this->Up * velocity;
        if (direction == DOWN)
            this->Position -= this->Up * velocity;

        std::cout<<"Camera position: " << this->Position << std::endl;
        std::cout<<"Camera orientation: " << this->Yaw << " " << this->Pitch << std::endl;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(T_Real xoffset, T_Real yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw   += YAW + xoffset;
        this->Pitch += PITCH + yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > M_PI/2.1f)
                this->Pitch = M_PI/2.1f;
            if (this->Pitch < -M_PI/2.1f)
                this->Pitch = -M_PI/2.1f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        this->updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(T_Real yoffset)
    {
        if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
            this->Zoom -= yoffset;
        if (this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if (this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        vec3::Vector3<T_Real> front;
        front[0] = cos(this->Yaw) * cos(this->Pitch);
        front[1] = sin(this->Pitch);
        front[2] = sin(this->Yaw) * cos(this->Pitch);
        front.normalize();
        this->Front = front;
        // Also re-calculate the Right and Up vector
        vec3::cross(this->Front, this->WorldUp, this->Right);
        this->Right.normalize();
        vec3::cross(this->Right, this->Front, this->Up);
        this->Up.normalize();
    }
};


#endif
