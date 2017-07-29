//============================================================================
// Name        : OpenGLLearning.cpp
// Author      : Javier Felip
// Version     :
// Copyright   : Copyright Intel Labs 
// Description : Hello World in C++, Ansi-style
//============================================================================

//TODO LIST!!
//TODO: Add option for face normal visualization
//TODO: Orbit-like navigation
//TODO: Go through all the project and add the const to the ConstPtr definitons

#include <iostream>
#include <random>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <CGLShader.hpp>
#include <primitives/GLPrimitives.h>
#include <CGLCamera.hpp>
#include <Material.hpp>
#include <CGLLight.hpp>
#include <CVector3.hpp>
#include <CReal.hpp>
#include <physics/CWorld.h>
#include <physics/CPositionBasedDynamics.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

//PBD Definitions
typedef double T_real;
typedef vec3::Vector3<T_real> Vector3;
void PBDCreateObjects( PBD::CWorld* pWorld );


template<typename T>
void projectionMatrix(T angle, T ratio, T near, T far, T mat[16]);

// Camera
Camera<GLfloat> camera(vec3::Vector3<GLfloat>(4.0f, -1.0f, -2.0f),  vec3::Vector3<GLfloat>(0.0f, 1.0f, 0.0f), M_PI,0);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;

GLfloat lastFrame = 0.0f;
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

bool wireframeMode = false;
bool normalMode = false;
bool constraintMode = false;
bool simStepKey = false;

// The MAIN function, from here we start the application and run the game loop
int main()
{

    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable( GL_PROGRAM_POINT_SIZE );

    // Build and compile our shader program
    std::string shaderPath = "/home/labuser/workspace/experiments-and-learning/OpenGLLearning/shaders/";
    Shader ourShader
	(
			shaderPath + "indicesTextureColorNormal.vs",
			"",
			shaderPath + "indicesTextureColorNormal.frag"
	);

    Shader normalShader
	(
			shaderPath + "normalVisualization.vs",
			shaderPath + "normalVisualization.geom",
			shaderPath + "normalVisualization.frag"
	);

    Shader pointCloudShader
            (
                    shaderPath + "pointCloudVisualization.vs",
                    shaderPath + "",
                    shaderPath + "pointCloudVisualization.frag"
            );

    Shader constraintShader
            (
                    shaderPath + "constraintVisualization.vs",
                    shaderPath + "",
                    shaderPath + "constraintVisualization.frag"
            );


    T_real simStep = 0.005;
    PBD::CWorld PBDWorld;
    PBDWorld.m_gravity = Vector3(0,0,-9.81);
    PBDCreateObjects( &PBDWorld );

    CGLParticleSystem<GLfloat> particleSystemsPointCloud;
    particleSystemsPointCloud.setPointSize(10);
    particleSystemsPointCloud.setParticleSystem(&PBDWorld);
    particleSystemsPointCloud.updateBuffers();


    CReferenceFrame<GLfloat> origin;
    //CSolidBox<GLfloat> floor(10.0,10.0,0.1);
    //CSolidCylinder<GLfloat> s(0.2,0.5,t);
    //CSolidCapsule<GLfloat> s(0.2,0.5,t_object);
    //CSolidCone<GLfloat> s;
    //CSolidArrow<GLfloat> s;
    //CCameraShape<GLfloat> s;
//    COpenGLPointCloud<GLfloat> s;
//
//    std::vector<GLfloat> pc_data;
//    for (uint i=0;i<1000;++i)
//    {
//    	for (uint j=0;j<1000;++j)
//    	{
//    		pc_data.push_back(i/100.0f);
//    		pc_data.push_back(j/100.0f);
//    		pc_data.push_back(1.0);
//    	}
//    }
//    s.setPoints(pc_data);
//    s.setColors(pc_data);
//    s.updateBuffers();
//
//    CMaterial<GLfloat> mat;
//    mat.setSpecular(1,1,1);
//    mat.setDiffuse(0.5,0.0,0.0);
//    mat.shininess = 5.0;

    CDirectionalLight<GLfloat> dirLight;
    dirLight.setDirection(0,-1,-1);
    dirLight.setSpecular(1,1,1);
    dirLight.setDiffuse(0.5,0.5,0.5);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
    	GLfloat currentFrame = glfwGetTime();
    	deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

        // PHYSICS ENGINE UPDATE
        if (simStepKey)
        {
            PBDWorld.step(simStep,0.1);
            //simStepKey = false;
        }

        // Clear the colorbuffer and the depth buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set wireframe or solid mode for polygon drawing
        if (wireframeMode)
        	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
        	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        // Create camera transformation
        GLfloat view[16] = {1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1};
        camera.GetViewMatrix(view);
        GLfloat projection[16] = {1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1};
        projectionMatrix<GLfloat>(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f,projection);

        // Pass view and projection matrices to the shader and viewer position
        ourShader.Use();
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
        glUniform3f(viewPosLoc, camera.Position.x(), camera.Position.y(), camera.Position.z());

        // Pass view and projection matrices to the point cloud shader
        pointCloudShader.Use();
        viewLoc = glGetUniformLocation(pointCloudShader.Program, "view");
        projLoc = glGetUniformLocation(pointCloudShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

    	//Prepare lights
    	dirLight.use(&ourShader);

    	//TODO: Loop through objects and materials to render
        // Pass view and projection matrices to the particle system shader
        constraintShader.Use();
        viewLoc = glGetUniformLocation(constraintShader.Program, "view");
        projLoc = glGetUniformLocation(constraintShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

        particleSystemsPointCloud.showConstraints(constraintMode);
        particleSystemsPointCloud.updateBuffers();
        particleSystemsPointCloud.draw(&constraintShader);

    		//Prepare material used by the rendered object
//    		mat.use(&ourShader);
//    		floor.draw(&ourShader);
    		origin.draw(&ourShader);


		//TODO: Loop through objects and materials to render the second pass for normals
    		//Prepare material used by the rendered object
		if (normalMode)
		{
			// Pass view and projection matrices to the normal shader and viewer position
	    	//normalShader.Use();
			viewLoc = glGetUniformLocation(normalShader.Program, "view");
			projLoc = glGetUniformLocation(normalShader.Program, "projection");
			viewPosLoc = glGetUniformLocation(normalShader.Program, "viewPos");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
			glUniform3f(viewPosLoc, camera.Position.x(), camera.Position.y(), camera.Position.z());

//    		floor.draw(&normalShader);
    		origin.draw(&normalShader);
		}



            // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    printf("exit");

    return 0;
}

template<typename T>
void projectionMatrix(T angle, T ratio, T near, T far, T mat[16])
{
	    T   tan_half_angle = tan(angle / 2);
	    mat[0] = 1.0 / (ratio * tan_half_angle);
	    mat[5] = 1.0 / tan_half_angle;
	    mat[10] = -(far + near) / (far - near);
	    mat[11] = -1;
	    mat[14] = -(2 * far * near) / (far - near);
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    	wireframeMode = !wireframeMode;
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        normalMode = !normalMode;
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        constraintMode= !constraintMode;
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        simStepKey = !simStepKey;
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


typedef double T_real;
typedef vec3::Vector3<T_real> Vector3;
void PBDCreateObjects( PBD::CWorld* pWorld )
{

    std::cout<<"Creating objects"<<std::endl;

    //Create a cube 5x5x0.05 m size with 5cm particles centered at 0,0,0 (This will be the floor)
    PBD::createParticleSystemSolidCube<T_real>(Vector3(-1.5,-1.5,0) , Vector3(3,3,0.1), pWorld, 0.1, 0, 0);

    //Create cube objects                         position              dimensions             partSize partWeight groupId
    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,3) , Vector3(0.2,0.2,0.2), pWorld, 0.1, 0.01, 1);
    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,4) , Vector3(0.2,0.2,0.2), pWorld, 0.1, 0.02, 2);

//    //Hang one object with a distance constraint
//    PBD::createParticleSystemSolidCube<T_real>(Vector3(0.5,0.5,4.5) , Vector3(0.1,0.1,0.1), pWorld, 0.1, 0, 3);   //Object to hang from
//    PBD::CConstantDistanceConstraint<>::Ptr hangConstraint (
//            new PBD::CConstantDistanceConstraint<>(
//                    &(psObject2->m_particles[0]),
//                    &(psFixedObject->m_particles[0]))
//    );
//    psFixedObject->m_internalConstraints.emplace_back(hangConstraint);

}