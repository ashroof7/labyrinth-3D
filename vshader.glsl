#version 130

in vec3 vPosition;
in vec4 in_Color;
in vec2 in_texcoord;

out vec4 ex_Color;
out	vec2 ex_texcoord; 

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform mat4 W;

void main(){
    gl_Position = (P * V * W * M) * vec4(vPosition, 1.0);
    ex_Color = in_Color;
    ex_texcoord = in_texcoord;
}

    