#pragma once

#include "shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


class Water{
    public:
        inline Water(){}
        Water(int resolution,float length);        
        ~Water();

        void Draw();
        void setShader(std::string path);   
        void genBuffer();

        shader waterShader;
        glm::vec3 waterColor;

        inline void setHeight(float newHeight){height=newHeight;}
        inline float getHeight(){return height;}
        inline void setExtraModel(glm::mat4 extra){extraModel = extra;}
    private:
        int resolution;
        float length;
        float offsetBtwn;
        glm::vec3 pos;
        glm::mat4 model;
        float height;
        glm::mat4 extraModel = glm::mat4(1.0f);

        void fillVertex(float*& buffer,int place,int x, int z);
        float scaleComponent(float c);
        unsigned int vao;
        unsigned int vbo;

        glm::mat4 getModel();



};