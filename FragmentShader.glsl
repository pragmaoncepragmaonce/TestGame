#version 330 core
out vec4 FragColor;
out vec4 FragColor2;
out vec4 FragColor3;


in vec2 TexCoord;


// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture1, TexCoord);
	FragColor2 = texture(texture2, TexCoord);
	FragColor3 = texture(texture3, TexCoord);
	
	
}