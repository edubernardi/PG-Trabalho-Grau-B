#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);

    //Exercicio fe da lista 1
    //primeiro é calculada a média do pixel
    float mean = (color.r + color.g + color.b)/3;

    //caso o pixel possua canal vermelho menor de 0.7, ele se torna greyscale
    if (color.r < 0.7){
        color.r = mean;
        color.g = mean;
        color.b = mean;
    }

}
