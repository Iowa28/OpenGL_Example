#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;

out vec4 fs_color;

uniform sampler2D myTexture;

void main()
{
	fs_color = texture(myTexture, vs_textcoord) * vec4(vs_color, 1.f);
}