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
    // Coordinate normalizzate del frammento
    vec2 normalizedCoords = 2.0 * gl_FragCoord.xy / resolution.xy - 1.0;

    // Colore iniziale delle nuvole
	vec3 cloudColor = vec3(0.0, 0.0, 0.0);
	vec3 starsColor = vec3(ourColor[0]-0.1, ourColor[1]-0.1, ourColor[2]-0.1);

    // Parametri delle nuvole
    int numberOfClouds = 50;
    float cloudDensity = 3.0;
    float cloudScale = 0.005;

    // Genera le nuvole come sovrapposizione di cerchi
    for (int i = 0; i < numberOfClouds; i++)
    {
        // Centro casuale del cerchio
        vec2 circleCenter = vec2(random(vec2(float(i),time*0.0001)) * 2.0 - 1.0,
            random(vec2( time*0.0001, float(i))) * 2.0 - 1.0);

        // Distanza del frammento dal centro del cerchio
        float distanceToCenter = length(normalizedCoords - circleCenter);

        
        // Densità della nuvola in questo punto
        float cloudHeight = smoothstep(cloudScale, 0.0, distanceToCenter);

        // Aggiungi la densità della nuvola al colore finale
        cloudColor += vec3(cloudDensity) * cloudHeight;

       
    }

    // Calcola il colore finale del frammento, interpolando tra cielo e nuvole
    FragColor = vec4(mix(skyColor, starsColor, cloudColor), 1.0);
}