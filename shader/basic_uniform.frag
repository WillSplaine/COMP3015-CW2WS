#version 460


layout (binding=0) uniform sampler2D Tex1;
layout (binding=1) uniform sampler2D Tex2;
layout (binding = 2) uniform samplerCube skyboxText;


layout (location = 0) out vec4 FragColor;
const float PI = 3.14159265358979323846;

 in vec3 Position;
 in vec3 Normal;
 in vec3 vecPos;
 in vec2 TexCoord;

 uniform struct PBRLightInfo {
	vec4 Position;
	vec3 L;
} PBRLight[3];

uniform struct PBRMaterialInfo {
	float Rough;
	bool Metal;
	vec3 Color;
}PBRMaterial; 

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


float ggxDistribution( float nDotH ) {
 float alpha2 = PBRMaterial.Rough * PBRMaterial.Rough * PBRMaterial.Rough * PBRMaterial.Rough;
 float d = (nDotH * nDotH) * (alpha2 - 1) + 1;
 return alpha2 / (PI * d * d);
}

float geomSmith( float dotProd ) {
 float k = (PBRMaterial.Rough+ + 1.0) * (PBRMaterial.Rough + 1.0) / 8.0;
 float denom = dotProd * (1 - k) + k;
 return 1.0 / denom;
}

vec3 schlickFresnel( float lDotH ){
vec3 f0 = vec3(0.04);
if( PBRMaterial.Metal ) {
  f0 = PBRMaterial.Color;
  }
  return f0 + (1 - f0) * pow(1.0 - lDotH, 5);
}

vec3 microfacetModel(int lightIdx, vec3 position, vec3 n)
{
	vec3 diffuseBrdf = vec3(0.0);
	if (!PBRMaterial.Metal) {
		diffuseBrdf = PBRMaterial.Color;
  }

 vec3 l = vec3(0.0), lightI = PBRLight[lightIdx].L;
 if (PBRLight[lightIdx].Position.w == 0.0) // Dir Light
 {
	l = normalize(PBRLight[lightIdx].Position.xyz);
 }
 else
 {
	l = PBRLight[lightIdx].Position.xyz - position;
	float dist = length(l);
	l = normalize(l);
	lightI /= (dist * dist);

	vec3 v = normalize(-position);
	vec3 h = normalize(v + l);
	float nDotH = dot(n, h);
	float lDotH = dot(l, h);
	float nDotL = max(dot(n, l),1.0);
	float nDotV = dot(n, v);
	vec3 specBrdf = 0.25 * ggxDistribution(nDotH) * schlickFresnel(lDotH) * geomSmith(nDotL) * geomSmith(nDotV);

	return (diffuseBrdf + PI * specBrdf) * lightI * nDotL;
	}
}

 

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






void main() 
{
    float dist=abs(Position.z);
    float fogFactor =(Fog.MaxDist-dist)/(Fog.MaxDist-Fog.MinDist);
     fogFactor = pow(fogFactor, 1.2);
   //Calculate shaded colour
    vec3 shadeColor=blinnPhong(Position,normalize(Normal));
  // Mix fog colour and shade based on fog factor
    vec3 Color=mix(Fog.Color,shadeColor,fogFactor);

    vec3 skyBoxTexColor = texture(skyboxText, normalize(vecPos)).rgb;
 
    

	vec3 sum = vec3(0);
	vec3 n = normalize(Normal);
	for (int i = 0; i < 3; i++)
	{
		 sum += microfacetModel(i, Position, n);
	}
	sum += pow(sum, vec3(1.0/2.2));
    vec4 fogBlinn = vec4(Color,8.0) + vec4(blinnPhong(Position,normalize(Normal)),1.0);
	FragColor = vec4(sum, 1) + vec4 (fogBlinn) + vec4(skyBoxTexColor,1);
    

}

