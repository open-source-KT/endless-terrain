
#include "include/renderer.h"
#include "include/shader.h"
#include "include/Texture.h"
#include "include/camera.h"
#include "include/UniformBuffer.h"
#include "include/sceneRenderer.h"

#include "include/random_terrain.h"
#include "include/Water.h"
#include "include/Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



void processInput(GLFWwindow *window);
void resetColors(std::vector<glm::vec3>& oldColors);
void setNonUboShaders(std::vector<shader*>& shaders,glm::mat4& proj,glm::mat4& view);



Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f,1.0f,0.0f),-90.0f,0.0f,10.0f);
float lastX = 960 / 2.0f;
float lastY = 540 / 2.0f;
bool firstMouse = true;


//timing
float deltaTime = 0.0f; 
float lastFrame = 0.0f; 


bool spacePressed = false;



void mouse_callback(GLFWwindow* window, double mouseX, double mouseY)
{
   if (firstMouse)
    {
        lastX = mouseX;
        lastY = mouseY;
        firstMouse = false;
    }

    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY; //reversed since y-coordinates go from bottom to top

    lastX = mouseX;
    lastY = mouseY;

    camera.processMouse(xoffset, yoffset);
}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
     camera.processScroll(yoffset);
}



int main(void)
{

    std::string title = "Procedurally generated terrain";
    Window w((uint32_t)960,(uint32_t)540,title);
    GLFWwindow* window = w.GetWindow();
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetScrollCallback(window,scroll_callback);

    GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND)); 
    glViewport(0,0,960,540);
    GLCall(glEnable(GL_MULTISAMPLE));


    {
   
    std::vector<glm::vec3> colors;
    resetColors(colors);


    Terrain terrain = Terrain("test.json");

    terrain.setCameraPos(&camera.Position);
    terrain.init();
    //Telling the camera to try and detect collisions. This is very rough and only detects if there is a collision when the forward button is pressed (not any other button) but it is getting there
    camera.setTerrain(&terrain);

    
    terrain.terrainShader.Bind();
    terrain.terrainShader.setUniformVec3f("light.direction",glm::vec3(0.6f,-0.6f,-0.3f));
    terrain.terrainShader.setUniformVec3f("light.ambient",glm::vec3(0.2f,0.2f,0.2f));
    terrain.terrainShader.setUniformVec3f("light.diffuse",glm::vec3(0.5f,0.5f,0.5f));
    terrain.terrainShader.setUniformVec3f("light.specular",glm::vec3(0.4f,0.4f,0.4f));
    terrain.terrainShader.UnBind();


    glm::mat4 model;
    glm::mat4 view = glm::lookAt(camera.Position,camera.Position + camera.Front ,camera.Up);
    glm::mat4 proj = glm::perspective(glm::radians(camera.fov),960/(float)540,0.1f,200.0f);


    UniformBuffer proj_and_view(2*sizeof(glm::mat4));
    std::string name = "proj_and_view";
    
    std::vector<uint32_t> shaderids;
    for(unsigned int s : terrain.uboShaders)
    {
        shaderids.push_back(s);
    }
    proj_and_view.BindShaders(shaderids,0,name,2*sizeof(glm::mat4));
    proj_and_view.UpdateBufferPoint(0,&proj[0][0],sizeof(glm::mat4),0);
    proj_and_view.UpdateBufferPoint(0,&view[0][0],sizeof(glm::mat4),sizeof(glm::mat4));


    sceneRenderer renderer;
    renderer.setCameraPosition(camera.Position);
    renderer.setUniformBuffer(&proj_and_view);
    renderer.setTerrain(&terrain);
    

    while(!glfwWindowShouldClose(window)){



		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        
        model = glm::mat4(1.0f);
        view = glm::lookAt(camera.Position,camera.Position + camera.Front ,camera.Up);
		proj = glm::perspective(glm::radians(camera.fov),960/(float)540,0.1f,200.0f);
		
        renderer.setproj(proj);
        renderer.setView(view);

        w.Run(&renderer);
        
        if(spacePressed)
        {
            resetColors(colors);
            terrain.newColors(colors);
            spacePressed = false;
        }

        
        terrain.checkBounds();
	    processInput(window);


     }

	}


    

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACK, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {

        spacePressed = true;
       
    }

   
    //Just for debugging
    if(glfwGetKey(window,GLFW_KEY_0)==GLFW_PRESS)
    {
        std::cout << camera.Position.y << "\n";
    }
    

};


 void resetColors(std::vector<glm::vec3>& oldColors)
    {
        for(int i=0; i < 15; i++)
        {
            oldColors.push_back(glm::vec3(randdouble(0,1),randdouble(0,1),randdouble(0,1)));
        }
    }

 void setNonUboShaders(std::vector<shader*>& shaders,glm::mat4& proj,glm::mat4& view)
 {
     for(int i=0;i<shaders.size();i++)
     {
        shaders[i]->Bind();
        shaders[i]->setUniformMat4f("proj",proj);
        shaders[i]->setUniformMat4f("view",view);
        shaders[i]->UnBind();
     }
 }
