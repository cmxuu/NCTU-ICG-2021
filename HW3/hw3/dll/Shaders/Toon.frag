#version 430
uniform sampler2D texture;
uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform vec3 Kd;
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

	// Set Toon Shading
	float level, intensity;
	level = dot(N, L);
	if (level > 0.75) intensity = 0.8;
	else if (level > 0.30) intensity = 0.6;
	else intensity = 0.4;
	color = vec4(Kd, 1.0) * albedo * intensity;

	// Set Edge
	float edge_intensity = 1 - max(dot(V, N), 0);
	vec4 edge_color = vec4(1, 1, 1, 1)  * pow(edge_intensity, 3);
	if (EdgeFlag) color += edge_color;
}