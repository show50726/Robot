// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430

struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform int mode;
uniform MaterialInfo Material;
out vec4 vFragColor;

//lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0;//for material specular

void main(void)
{ 
    // Dot product gives us diffuse intensity
    float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color, force alpha to 1.0
    vFragColor = diff * diffuseColor*vec4(Material.Kd,1);

    // Add in ambient light
    vFragColor += ambientColor;
	if(mode==2){
		float graycolor = 0.299*vFragColor.r+0.587*vFragColor.g+0.114*vFragColor.b;
		vFragColor = vec4(graycolor,graycolor,graycolor,1.0);
	}
	else if(mode==3){
		vFragColor = vec4(1.0-vFragColor.r,1.0-vFragColor.g,1.0-vFragColor.b,1.0);
	}

    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								normalize(vVaryingNormal)));//反射角
    float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*vec4(Material.Ka,1)*spec;
    }
}
	
    