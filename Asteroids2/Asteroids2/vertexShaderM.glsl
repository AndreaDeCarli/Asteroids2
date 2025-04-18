#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1

out vec4 ourColor; // output a color to the fragment shader
uniform mat4 Projection;  //vARIABILE DI TIPO uniform, rimane la stessa per ogni vertice della primitiva e viene passata dall'esterno
uniform mat4 Model;
uniform int flagP;
uniform vec4 GameColor;
void main(){
    if (flagP == 0) {
        gl_Position = Projection * Model * vec4(aPos, 1.0);
        ourColor = GameColor; // set ourColor to the input color we got from the vertex data
    }
	else if(flagP == 1) {
		gl_Position = Projection * Model * vec4(aPos, 1.0);
        ourColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
}  

