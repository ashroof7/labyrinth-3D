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

uniform vec4  LightPosition = vec4(20.0, 20.0, 0, 1.0);

out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform int shapeType;//0 indicates a sphere centered at origin, 1 indicates a cube,2 inidicates surface 
uniform int mask;

void main(){
	mat4 MWV = V * W *  M;
	vec3 normal;
	int done = 0;
	if(shapeType == 0)
		normal = vPosition;
	else if(shapeType == 1){
		int b = 0;
		float x = vPosition.x;
		float y = vPosition.y;
		float z = vPosition.z;
		int si = 0;
		int sj = 0;
		normal = vec3(0.0,0.0,0.0);
		if(x > 0)si=1;
		else si = 0;
		if(z > 0)sj=1;
		else sj = 0;
		int not_here = 0;
		for(int i = si;i<=si+1;i++)
			for(int j = sj;j<=sj+1;j++)
			{
				if((mask & (1<< (i * 3 + j))) != 0)
				{
					float xAdd = i == si ? 0.5 : -0.5;
					float zAdd = j == sj ? 0.5 : -0.5;
					normal+=vec3(xAdd, y, zAdd);
					not_here++;
				}
			}
	}
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

    
