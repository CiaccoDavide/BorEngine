#version 130 // referring to OpenGL 3.0

in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

void main(){
	gl_Position.xy = vertexPosition; // .xy is called swizzeling in GLSL
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
}