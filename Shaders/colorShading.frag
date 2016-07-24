#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

 uniform float time;
uniform sampler2D tex;

void main(){
	
	vec4 textureColor = texture(tex, fragmentUV); // return rgba

	color = vec4(
		fragmentColor.r * (cos(fragmentPosition.x*16 + time) + 1.0) * 0.5,
		fragmentColor.g * (cos(fragmentPosition.y*4 + time) + 1.0) * 0.5,
		fragmentColor.b * (sin(fragmentPosition.x*32 + time) + 1.0) * 0.5,
		fragmentColor.a
	) * textureColor;

	// color = textureColor * fragmentColor;
}