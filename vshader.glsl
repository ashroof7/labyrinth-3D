#version 130

in vec3 vPosition;
in vec4 gl_Color;
in vec2 in_texcoord;

out vec4 ex_Color;
out	vec2 ex_texcoord; 

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform mat4 W;

uniform vec4  LightPosition = vec4(50.0, 1000.0, 0.0, 1.0);

out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

int shapeType;//0 indicates a sphere centered at origin, 1 indicates a cube,2 inidicates surface 

void main(){
	mat4 MWV = V * W *  M;
	vec3 normal = vPosition;
	normalize(normal);
	vec3 pos = (MWV * vec4(vPosition, 1.0)).xyz;
	fE = -pos;
	
	if( LightPosition.w == 0.0 )
        fL = ( V * LightPosition ).xyz;
    else
        fL = ( V * LightPosition ).xyz - pos;
	fN = (MWV * vec4(normal, 0.0)).xyz;
	
    gl_Position = (P * V * W * M) * vec4(vPosition, 1.0);
    ex_Color = gl_Color;
    ex_texcoord = in_texcoord;
}

    
