#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);

    //Exercício a, questão 1
    // Manter apenas a info do canal G
    color.r = color.g;
    color.b = color.g;

}
