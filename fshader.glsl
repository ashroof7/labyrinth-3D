#version 130

in vec4 ex_Color;
in vec2 ex_texcoord;

out vec4 out_Color;

in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;
uniform int shapeType;
uniform sampler2D tex_sampler;

void main(void){
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	vec3 H = normalize(L+E);
	vec4 ambient = AmbientProduct;
	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd*DiffuseProduct;
	  float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;
    if( dot(L, N) < 0.0 )
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 fColor = ambient + diffuse + specular;
   	fColor.a = 1.0;
   	if(shapeType == 1)
	out_Color =(texture(tex_sampler, ex_texcoord)) * fColor;
	else if(shapeType == 0)
	out_Color = fColor;
}

