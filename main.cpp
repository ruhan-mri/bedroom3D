//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fan_on = false;
float fan_rotateAngle_Y = 0.0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
//BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cube_vertices[] = {
        0.25f, 0.25f, -0.25f, 0.3f, 0.8f, 0.5f,
        -0.25f, 0.25f, -0.25f, 0.5f, 0.4f, 0.3f,
        -0.25f, -0.25f, -0.25f, 0.2f, 0.7f, 0.3f,
        0.25f, -0.25f, -0.25f, 0.6f, 0.2f, 0.8f,
        0.25f, 0.25f, 0.25f, 0.8f, 0.3f, 0.6f,
        -0.25f, 0.25f, 0.25f, 0.4f, 0.4f, 0.8f,
        -0.25f, -0.25f, 0.25f, 0.2f, 0.3f, 0.6f,
        0.25f, -0.25f, 0.25f, 0.7f, 0.5f, 0.4f
    };
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };*/
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };*/
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };
    
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    
    //axis line 

    float axisVertices[] = {
        // X-axis
        0.0f, 0.0f, 0.0f,   // Start point
        5.0f, 0.0f, 0.0f,   // End point

        // Y-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 5.0f, 0.0f,   // End point

        // Z-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 0.0f, 5.0f    // End point
    };


    //axis line VBO,VAO
    unsigned int axisVAO, axisVBO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    // Bind the axis VAO and VBO
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    // Main
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //ourShader.use();

    // render loop
    // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            // -----
            processInput(window);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            // activate shader
            ourShader.use();

            // pass projection matrix to shader (note that in this case it could change every frame)
            
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection);

            // camera/view transformation
            //glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 view = camera.GetViewMatrix();
            ourShader.setMat4("view", view);

            
            glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        
            translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + 5.0f, translate_Y + 5.0f, translate_Z + 5.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
            model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

            glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


            // Axis line
            {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
                rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
                model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
                ourShader.setMat4("model", model);

                glUseProgram(ourShader.ID);
                glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 1.0f, 0.0f, 0.0f);  // Set line color (red)
                glBindVertexArray(axisVAO);
                glDrawArrays(GL_LINES, 0, 2);

                // Draw the y-axis line
                glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 0.0f, 1.0f, 0.0f);  // Set line color (green)
                glDrawArrays(GL_LINES, 2, 2);

                // Draw the z-axis line
                glUniform3f(glGetUniformLocation(ourShader.ID, "lineColor"), 0.0f, 0.0f, 1.0f);  // Set line color (blue)
                glDrawArrays(GL_LINES, 4, 2);
            }
        



            // chair 
            {
                float chairX = 1.6f, chairY = 0.8f, chairZ = 0.75f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX, chairY, chairZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * 2.0f, scale_Z * .05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 1 back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX - .48f, chairY - .48f, chairZ));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 2 back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX + .48f, chairY - .48f, chairZ));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 3 front
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX + .42f, chairY + .42f, chairZ - 0.375f));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 4 front
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX - .42f, chairY + .42f, chairZ - 0.375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Back side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX , chairY - 0.5f, chairZ + 0.625f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f , scale_Y * .05f, scale_Z * 1.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        

            // table 
            {
                float tableX = 1.6f, tableY = 2.3f, tableZ = 1.5f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX, tableY, tableZ ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 4.0f, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 1 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.875f, tableY + 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 2 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.875f, tableY + 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 3 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.875f, tableY - 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 4 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.875f, tableY - 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // table back side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX , tableY + 1.0f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.05f, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // right side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.9875f, tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f, scale_Y, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // left side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.9875f, tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f, scale_Y, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // upper side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX , tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        
            // Bed
            {
                float bedX = 3.8f, bedY = 1.3f, bedZ = 0.75f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX, bedY, bedZ + 0.125f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 8.0f, scale_Z * 0.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


                // leg 1
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.875f, bedY + 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 2
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.875f, bedY + 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            
                // leg 3
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.875f, bedY - 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 4
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.875f, bedY - 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Head side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX , bedY + 2.0, bedZ + 0.25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.05f, scale_Z * 0.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // pillow right
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.45f, bedY + 1.7f , bedZ + .25f ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.5f , scale_Y * 0.75f, scale_Z * 0.25f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // pillow left
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.45f, bedY + 1.7f, bedZ + .25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.5f, scale_Y * 0.75f, scale_Z * 0.25f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }


            // Floor
            {
                float floorX = 0.0f, floorY = 0.0f, floorZ = 0.0f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(floorX, floorY, floorZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 20.0f, scale_Y * 14.0f, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // Wall
            {
                float wallX = 0.0f, wallY = 0.0f, wallZ = 0.0f;

                // right side wall
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX + 5.0f, wallY, wallZ + 2.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f , scale_Y * 14.0f, scale_Z * 10.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // left side wall
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX - 5.0f, wallY, wallZ + 2.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f, scale_Y * 14.0f, scale_Z * 10.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                //// front side wall 
                //translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX , wallY + 3.5f, wallZ + 2.5f));
                //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 20.0f, scale_Y * 0.05f, scale_Z * 10.0f));
                //model = translateMatrix * scaleMatrix;

                //ourShader.setMat4("model", model);
                //glBindVertexArray(VAO);
                //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // front side wall 1
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX, wallY + 3.5f, wallZ + 4.25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 20.0f, scale_Y * 0.05f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // front side wall 2
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX, wallY + 3.5f, wallZ + 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 20.0f, scale_Y * 0.05f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // front side wall 3
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX, wallY + 3.5f, wallZ + 2.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 8.0f, scale_Y * 0.05f, scale_Z * 4.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // front side wall 4
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX + 4.75f, wallY + 3.5f, wallZ + 2.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y * 0.05f, scale_Z * 4.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // front side wall 5
                translateMatrix = glm::translate(identityMatrix, glm::vec3(wallX - 4.75f, wallY + 3.5f, wallZ + 2.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y * 0.05f, scale_Z * 4.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        
            // right Window
            {
                float winX = 3.25f, winY = 3.5f, winZ = 2.5f;

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX , winY , winZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ + .5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ + 0.98f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ - .5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ - 0.98f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            }

            // Left Window
            {
                float winX = -3.25f, winY = 3.5f, winZ = 2.5f;

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ + .5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ + 0.98f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ - .5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(winX, winY, winZ - 0.98f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 5.0f, scale_Y * 0.05f, scale_Z * 0.1f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            }

            // Fan
            {
                float fanX = 2.5f, fanY = 1.5f, fanZ = 4.5f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(fanX , fanY, fanZ ));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(fan_rotateAngle_Y), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5f  , scale_Y * 0.5f, scale_Z * 0.5f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.5f, scale_Z * 0.1f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);   

                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5f, scale_Y * 4.0f, scale_Z * 0.1f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                if (fan_on) {
                    fan_rotateAngle_Y -= 0.1f;
                }

                glBindVertexArray(VAO);


                translateMatrix = glm::translate(identityMatrix, glm::vec3(fanX, fanY , fanZ + .25f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1f, scale_Y * 0.1f, scale_Z ));
                model = translateMatrix * rotateYMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // Ceil
            {
                float floorX = 0.0f, floorY = 0.0f, floorZ = 5.0f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(floorX, floorY, floorZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 20.0f, scale_Y * 14.0f, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // chair 2
            {
                float chairX = -1.6f, chairY = 0.8f, chairZ = 0.75f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX, chairY, chairZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * 2.0f, scale_Z * .05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 1 back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX - .48f, chairY - .48f, chairZ));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 2 back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX + .48f, chairY - .48f, chairZ));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 3 front
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX + .42f, chairY + .42f, chairZ - 0.375f));//
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 4 front
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX - .42f, chairY + .42f, chairZ - 0.375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * .25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Back side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(chairX, chairY - 0.5f, chairZ + 0.625f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 2.0f, scale_Y * .05f, scale_Z * 1.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }


            // table  2
            {
                float tableX = -1.6f, tableY = 2.3f, tableZ = 1.5f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX, tableY, tableZ));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 4.0f, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 1 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.875f, tableY + 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 2 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.875f, tableY + 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 3 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.875f, tableY - 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Leg 4 Back
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.875f, tableY - 0.875f, tableZ - 0.75f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 3.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // table back side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX, tableY + 1.0f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.05f, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // right side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX + 0.9875f, tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f, scale_Y, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // left side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX - 0.9875f, tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.05f, scale_Y, scale_Z * 2.0f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // upper side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(tableX, tableY + 0.75f, tableZ + 0.5f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y, scale_Z * 0.05f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // Bed 2
            {
                float bedX = -3.8f, bedY = 1.3f, bedZ = 0.75f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX, bedY, bedZ + 0.125f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 8.0f, scale_Z * 0.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


                // leg 1
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.875f, bedY + 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 2
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.875f, bedY + 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 3
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.875f, bedY - 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // leg 4
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.875f, bedY - 1.875f, bedZ - .375f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.25f, scale_Y * 0.25f, scale_Z * 1.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // Head side
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX, bedY + 2.0, bedZ + 0.25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.05f, scale_Z * 0.5f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // pillow right
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX + 0.45f, bedY + 1.7f, bedZ + .25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.5f, scale_Y * 0.75f, scale_Z * 0.25f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                // pillow left
                translateMatrix = glm::translate(identityMatrix, glm::vec3(bedX - 0.45f, bedY + 1.7f, bedZ + .25f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 1.5f, scale_Y * 0.75f, scale_Z * 0.25f));
                model = translateMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }


            // Fan 2
            {
                float fanX = -2.5f, fanY = 1.5f, fanZ = 4.5f;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(fanX, fanY, fanZ));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(fan_rotateAngle_Y), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5f, scale_Y * 0.5f, scale_Z * 0.5f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 4.0f, scale_Y * 0.5f, scale_Z * 0.1f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.5f, scale_Y * 4.0f, scale_Z * 0.1f));
                model = translateMatrix * rotateYMatrix * scaleMatrix;
                ourShader.setMat4("model", model);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                if (fan_on) {
                    fan_rotateAngle_Y -= 0.1f;
                }

                glBindVertexArray(VAO);


                translateMatrix = glm::translate(identityMatrix, glm::vec3(fanX, fanY, fanZ + .25f));
                rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 0.0f, 1.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 0.1f, scale_Y * 0.1f, scale_Z));
                model = translateMatrix * rotateYMatrix * scaleMatrix;

                ourShader.setMat4("model", model);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            // render boxes
            //for (unsigned int i = 0; i < 10; i++)
            //{
            //    // calculate the model matrix for each object and pass it to shader before drawing
            //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            //    model = glm::translate(model, cubePositions[i]);
            //    float angle = 20.0f * i;
            //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //    ourShader.setMat4("model", model);

            //    glDrawArrays(GL_TRIANGLES, 0, 36);
            //}

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fan_on = !fan_on;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 1;
        else if (rotateAxis_Y) rotateAngle_Y -= 1;
        else rotateAngle_Z -= 1;
    }
    /*if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        lookAtX += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        lookAtX -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }*/

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

