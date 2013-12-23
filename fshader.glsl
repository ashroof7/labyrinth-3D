#version 130

in vec4 ex_Color;
in vec2 ex_texcoord;

out vec4 out_Color;

uniform sampler2D tex_sampler;

void main(void){
	out_Color = texture(tex_sampler, ex_texcoord) * ex_Color;
	// out_Color =  ex_Color;
}

