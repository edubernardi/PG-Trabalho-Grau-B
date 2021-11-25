#version 450 core
in vec4 ourColor;
in vec2 TexCoord;

out vec4 color;

// pixels da textura
uniform sampler2D tex1;

void main()
{
    color = texture(tex1, TexCoord);

    //Exercicio e da lista 1
    //primeiro é calculada a média do pixel
    float mean = (color.r + color.g + color.b)/3;

    //depois é feita a binarização da média com k = 0.5
    float k = 0.5;
    if (mean > k){
        mean = 1;
    } else {
        mean = 0;
    }

    //por fim todos os canais recebem o valor da media
    color.r = mean;
    color.g = mean;
    color.b = mean;

}
