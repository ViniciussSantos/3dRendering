#version 400
layout (location = 0) in vec4 vPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 v2fcolor;
void main(){
gl_Position = model * vPosition;
float gray = (gl_Position.z + 1) * 0.5;
v2fcolor = vec4 (gray, gray, gray, 1);
gl_Position = projection * view * gl_Position;// se jogar para fora do (-1,-1,-1)-(1,1,1) vai cortar
}
