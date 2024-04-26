#version 460

 in vec3 Position;
 in vec3 Normal;
 in vec2 TexCoord;


 // Define texture samplers
 layout (binding=0) uniform sampler2D Tex1;
 layout (binding=1) uniform sampler2D Tex2;

 //Define output color
layout (location = 0) out vec4 FragColor;


uniform struct LightInfo{

    vec4 Position; 
    vec3 La;
    vec3 L;
} Light;

uniform struct MaterialInfo{
 vec3 Kd;
 vec3 Ka;
 vec3 Ks;
 float Shininess;
 int texChoice;
 int Disc;
}Material;

uniform struct FogInfo{
float MaxDist;
float MinDist;
vec3 Color;
}Fog;

//define constants
const int levels=6;
const float scaleFactor=1.0/levels;


//BlinnPhong Lighting
vec3 blinnPhong (vec3 position, vec3 n){
    vec3 diffuse=vec3(0),spec= vec3(0);

    if(Material.texChoice == 1)
    { 
     vec3 texColor = texture(Tex1,TexCoord).rgb;
      vec3 ambient = Light.La*texColor;
     vec3 s = normalize(Light.Position.xyz-position);
     float sDotN = max(dot(s, n), 0.0);
    diffuse = texColor*floor(sDotN*levels)*scaleFactor;
        if (sDotN>0.0){
            vec3 v=normalize(-position.xyz);
            vec3 h=normalize(v+s);
            spec=Material.Ks*pow(max(dot(h,n),0.0), Material.Shininess); 
        }
    return  ambient+(diffuse+spec)*Light.L;
    }
     if(Material.texChoice == 2)
        {
         vec4 tex1Color=texture(Tex1,TexCoord);  
         vec4 tex2Color=texture(Tex2,TexCoord);
         vec3 texColor=mix(tex1Color.rgb,tex2Color.rgb,tex2Color.a);

          vec3 ambient = Light.La*texColor;
     vec3 s = normalize(Light.Position.xyz-position);
     float sDotN = max(dot(s, n), 0.0);
    diffuse = texColor*floor(sDotN*levels)*scaleFactor;
        if (sDotN>0.0){
            vec3 v=normalize(-position.xyz);
            vec3 h=normalize(v+s);
            spec=Material.Ks*pow(max(dot(h,n),0.0), Material.Shininess); 
        }
    return  ambient+(diffuse+spec)*Light.L;
    }
    }




void main() {
    //Calculate distance and Fog Factor
    float dist=abs(Position.z);
    float fogFactor =(Fog.MaxDist-dist)/(Fog.MaxDist-Fog.MinDist);
     fogFactor = pow(fogFactor, 1.2);
   //Calculate shaded colour
    vec3 shadeColor=blinnPhong(Position,normalize(Normal));
  // Mix fog colour and shade based on fog factor
    vec3 Color=mix(Fog.Color,shadeColor,fogFactor);
    //output final colour
 FragColor = vec4(Color,8.0) + vec4(blinnPhong(Position,normalize(Normal)),1.0);
 
 }
 

