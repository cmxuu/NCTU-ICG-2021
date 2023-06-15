#version 430

uniform sampler2D texture;
uniform vec3 WorldCamPos;
uniform vec3 WorldLightPos;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform int  gloss;
uniform bool EdgeFlag;

in vec2 uv;
in vec3 normal;
in vec4 worldPos;

out vec4 color;
void main()
{
	// Init Vector
	vec3 L, N, V, R;
	L = normalize(WorldLightPos - vec3(worldPos));
	N = normalize(normal);
	V = normalize(WorldCamPos - vec3(worldPos));
	R = normalize(reflect(-L, N));

	vec4 albedo = texture2D(texture, uv);

	// Set Phone Shading
	vec4 ambient, diffuse, specular;
	ambient = vec4(La, 1.0) * vec4(Ka, 1.0) * albedo;
	diffuse = vec4(Ld, 1.0) * vec4(Kd, 1.0) * albedo * max(dot(L, N), 0);
	specular = vec4(Ls, 1.0) * vec4(Ks, 1.0) * pow(max(dot(V, R), 0), gloss/4); 
	color = ambient + diffuse + specular;

	// Set Edge
	float edge_intensity = 1 - max(dot(V, N), 0);
	vec4 edge_color = vec4(1, 1, 1, 1)  * pow(edge_intensity, 3);
	if (EdgeFlag) color += edge_color;
} 