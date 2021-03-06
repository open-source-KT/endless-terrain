#pragma once

#include <vector>
#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>

struct Config
{
    int octaves = 7;
    float bias = 1.8;
    int seed = 423;

    int x = 256;
    int y = 256;
    float height = 10.0f;
    
    float posX = 0.0f;
    float posY = 0.0f;
    float offset = 0.2f;

    float collisionOffset = 0.25;
    GLenum primitive = GL_TRIANGLES;


    bool trees = false;
    int gridX = 50;
    int gridY = 50;
    std::string treeModel = "res/models/tree.obj";
    std::string treeShader = "res/shaders/simple_model.shader";
    bool treeUniformBuffer = false;
    bool instancing = false; //If you set this to true make sure to use a shader that works with instancing and vice versa
   
    float treeChance = 20; //It is 1 over this number so 1 in 20 chance
    int maxNumInGrid = 1;



    /* This mostly determines how the index buffer will be written to and determines the default primitive in the Draw function if you don't pass any paramters.
    Note that if you set this to GL_POINTS for example and in the Draw function give a paramter of GL_TRIANGLES you will get a black screen because specifying GL_POINTS
    means that it will not generate an index buffer, if you want to have both triangles and points then specify as GL_TRIANGLES and pass GL_POINTS to Draw() when you  want to draw points*/

    bool staticColor = false;
    bool texture = false;
    std::string textureLocation = "";
    int textureSlot = 0;
    bool textureRepeat = false;
    float xTextureRepeatOffset = 1.0f;
    float yTextureRepeatOffset = 1.0f;
    GLenum wrapMode = GL_REPEAT;

    std::vector<float> color1 = {0.317,0.149,0.149};
    std::vector<float> color2 = {0.239,0.576,0};

    //This shader doesn't take in texture coordinates it just takes colors. for textures change to a shader such as basic.shader or your own shader
    std::string shaderLocation = "res/shaders/color2.shader"; 
    std::string textureUniformName = "u_Texture"; 
    bool uniformBuffer = false;
    bool geometryShader = false;
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    bool genNormals = false;
    //lighting
    bool perFaceNormals = true;


    bool waterTrue = false;
    float waterHeight = -20.0f;
    glm::vec3 waterColor = {0.0f,1.0f,0.0f};


};  
