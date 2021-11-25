#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex1;

uniform vec4 corColorizadora;

void main()
{
    color = texture(tex1, TexCoord);

    //Exercicio b da lista 1
    //float mean = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;
    //color.r = mean;
    //color.g = mean;
    //color.b = mean;

}
