#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

uniform struct lightinfo{

    vec4 position; 
    vec3 la;
    vec3 l;
} light;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 vecPos;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform float Time;
uniform float Freq = 200;
uniform float Velocity = 100;
uniform float Amp = 0.6;

void main()
{
    vec4 pos = vec4(VertexPosition,1.0);

    float u = Freq*pos.x-Velocity*Time;
    pos.y = Amp*sin(u);
    vec3 n =vec3(0.0);
    n.xy = normalize(vec2(cos(u),1.0));

 

    TexCoord=VertexTexCoord;
    Normal = normalize(NormalMatrix*VertexNormal);
    Position = (ModelViewMatrix*vec4(VertexPosition,1.0)).xyz;
    gl_Position = MVP*vec4(VertexPosition,1.0);
}
