#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;
out vec3 fs_out_data;		//Ez adja át a vertexpozíciót a FrameBufferObject-nek.

// irány fényforrás: fény iránya
uniform vec3 lightDirection = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

//Mezők textúrái:
uniform sampler2D grassTexture;
uniform sampler2D hillTexture;

//Mezők kiemeléséhez szükséges adatok:
uniform int buildModeOn;						//Bool-ként nem akarta átadni.
uniform vec3 selectedTileUpperLeftCorner;
uniform vec3 selectedTileUpperRightCorner;
uniform vec3 selectedTileLowerLeftCorner;
uniform vec3 selectedTileLowerRightCorner;
uniform int canBuildHere;						//És ezt sem.


bool isSelected(vec3 vs_out_pos){
	if(vs_out_pos.x >= selectedTileUpperLeftCorner.x &&
	   vs_out_pos.x <= selectedTileUpperRightCorner.x &&
	   vs_out_pos.z >= selectedTileUpperLeftCorner.z &&
	   vs_out_pos.z <= selectedTileLowerLeftCorner.z
	){
		return true;
	} else {
		return false;
	}
}

void colorTile(){
	if(canBuildHere == 1){
		fs_out_col = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}else{
		fs_out_col = vec4(1.0f, 0.0f, 0.0f, 1.0f);		//Ha nem, akkor piros.
	}
}

void applyTexture(){
	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 toLight = normalize(-lightDirection);
	
	float cosa = clamp(dot(normal, toLight), 0, 1);

	vec3 diffuse = cosa*Ld;

	if(vs_out_pos.y < 0.1f){
		fs_out_col = vec4(ambient + diffuse, 1) * texture(grassTexture, vs_out_tex);
	} else if(vs_out_pos.y < 1.0f){
		//Folytonos interpoláció a két textúra között:
		fs_out_col = vec4(ambient + diffuse, 1)  * ( (1-vs_out_pos.y) * texture(grassTexture, vs_out_tex) + vs_out_pos.y * texture(hillTexture, vs_out_tex) );
	} else {
		fs_out_col = vec4(ambient + diffuse, 1) * texture(hillTexture, vs_out_tex);
	}
}

void main()
{
	fs_out_data = vs_out_pos;

	if(buildModeOn == 1 && isSelected(vs_out_pos)){
		colorTile();
	} else {
		applyTexture();
	}
}