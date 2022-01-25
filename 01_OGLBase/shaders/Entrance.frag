#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 lightDirection = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D meshTexture;		//Képtextúra: minden mesh-nek csak egy lehet.

void main()
{
	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 toLight = normalize(-lightDirection);
	
	float cosa = clamp(dot(normal, toLight), 0, 1);

	vec3 diffuse = cosa*Ld;

	fs_out_col = vec4(ambient + diffuse, 1) * texture(meshTexture, vs_out_tex);
}