#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;

out vec4 fs_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fs_color = mix(texture(texture1, vs_textcoord), texture(texture2, vs_textcoord), 0.5);
	//fs_color = texture(texture1, vs_textcoord) * texture(texture2, vs_textcoord) * vec4(vs_color, 1.f);
}