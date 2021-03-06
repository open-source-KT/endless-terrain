#shader vertex
#version 330 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 a_normal;


uniform mat4 model;

layout(std140) uniform proj_and_view
{
    mat4 proj;
    mat4 view;
};

out vec3 Normal;
out vec3 FragPos;
out vec2 v_texCoords;

void main()
{
    gl_Position = proj * view * model * Position;
    FragPos = vec3(model * Position); //Fragment position in world space
    Normal = a_normal;
    v_texCoords = texCoords;
};


#shader fragment
#version 330 core


out vec4 FragColor;


struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};


in vec3 Normal;
in vec3 FragPos;  
in vec2 v_texCoords;


uniform vec3 u_viewPos;
uniform Light light;

uniform sampler2D u_Texture;

void main()
{   
    vec3 color = vec3(texture(u_Texture,v_texCoords));
    vec3 ambient = color*light.ambient;
   
    vec3 norm = normalize(Normal); 
    //The lighting calculations we used so far expect the light direction to be a direction from the fragment 
    //towards the light source, but people generally prefer to specify a directional light as a global direction pointing from the light source so we have to negate
    vec3 lightDir = normalize(-light.direction); 
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = (diff*color * light.diffuse);    

    vec3 viewDir = normalize(u_viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);             
    
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = color * spec * light.specular;

    vec3 result = (ambient + diffuse + specular); 
    FragColor = vec4(result,1.0);



    
};
