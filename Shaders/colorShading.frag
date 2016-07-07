#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main(){
	// color = vec3(1.0, 0.894, 0.549);
	color = vec4(
		fragmentColor.r * (cos(fragmentPosition.x*16 + time) + 1.0) * 0.5,
		fragmentColor.g * (sin(fragmentPosition.y*4 + time) + 1.0) * 0.5,
		fragmentColor.b * (sin(fragmentPosition.x*32 + time) + 1.0) * 0.5,
		fragmentColor.a
	);
}