#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);

    //Exercicio d da lista 1
    color.r = 1 - color.r;
    color.g = 1 - color.g;
    color.b = 1 - color.b;

}
