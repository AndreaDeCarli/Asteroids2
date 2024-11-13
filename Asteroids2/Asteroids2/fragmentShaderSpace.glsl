#version 330 core
in vec4 ourColor;
out vec4 FragColor;  
uniform vec2 resolution;
uniform float time;
 
 
const vec3 skyColor = vec3(0.0, 0.0, 0.0);
// Funzione hash semplice per generare un valore pseudo-casuale
float random(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 78.233))) );
}


void main()
{

    vec2 normalizedCoords = 2.0 * gl_FragCoord.xy / resolution.xy - 1.0;

	//factor is going to be the black and white map that determines where the stars are
	vec3 factor = vec3(0.0, 0.0, 0.0);
	//starsColor is going to be the color given to the stars according to the factor
	vec3 starsColor = vec3(ourColor[0]-0.1, ourColor[1]-0.1, ourColor[2]-0.1);

    int numberOfStars = 50;
    float starsDensity = 1.0;
    float starsScale = 0.006;

    
    for (int i = 0; i < numberOfStars; i++)
    {
        vec2 circleCenter = vec2(random(vec2(float(i),time*0.0001)) * 2.0 - 1.0,
            random(vec2( time*0.0001, float(i))) * 2.0 - 1.0);

        float distanceToCenter = length(normalizedCoords - circleCenter);

        float starsHeight = smoothstep(starsScale, 0.0, distanceToCenter);

        factor += vec3(starsDensity) * starsHeight;
       
    }
	

    FragColor = vec4(mix(skyColor, starsColor, factor), 1.0);
}