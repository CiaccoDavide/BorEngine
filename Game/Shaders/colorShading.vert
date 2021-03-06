#version 130 // referring to OpenGL 3.0

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P; // orthographic projection matrix

void main(){
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy; // .xy is called swizzeling in GLSL
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;

	fragmentUV = vec2(vertexUV.x, 1-vertexUV.y);
}