#version 130

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D frameTex;

void main()
{
	vec4 originalColor = texture(frameTex, vs_out_tex);
	fs_out_col = originalColor;
}