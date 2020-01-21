#include "UI.h"

Stamina::Stamina(int _texture, glm::vec3 _position, glm::vec3 _scale)
{
	texture = _texture;
	position = _position;
	scale = _scale;
}


void Stamina::setTexture(int value) {

	texture = value; }

int Stamina::getTexture() const {
	return texture; }

static const GLfloat g_vertex_buffer_data[] = {
-1.0f, -1.0f, 0.0f,
1.0f, -1.0f, 0.0f,
0.0f,  1.0f, 0.0f,
};

//code snipit example of cascading shader, will use for stamina bar
//vec3 testing(vec3 c)
//{//xyzw parameters for vec 4 colours xyz = rgb w = alpha
//	vec4 K = vec4(1.0, 1.0, 0.0, 0.0);
//	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
//}
//
//
//void mainImage(out vec4 fragColor, in vec2 fragCoord)
//{
//	vec2 border = vec2(0.05, 0.1);
//	vec4 backgroundColor = vec4(1, 1, 1, 1);//background color
//	vec2 uv = fragCoord / iResolution.xy;//fragment shader cordinates
//
//	// generate border mask
//	vec2 mask2 = step(border, uv) * step(uv, 1.0 - border);
//	float mask = mask2.x * mask2.y;
//
//	float progress = mod(iTime, 1.0); //changes the colour based on time 
//	float blend = ((uv.x - progress) <= 0.0 ? 1.0 : 0.0) * mask;
//	vec4 foreCol = vec4(testing(vec3(progress * 0.33333 - 0.1, 1.0, 1.0)), 1.0);
//	fragColor = foreCol * blend + backgroundColor * (0.9 - blend);//background
//}


