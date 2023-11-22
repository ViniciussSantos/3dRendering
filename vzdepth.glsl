#version 400

layout( location = 0) in vec4 vPosition;

out vec4 v2fcolor;

void main(){
    gl_Position = vPosition;
    float gray = (gl_Position.z +1) * 0.5;
    v2fcolor = vec4(gray,gray,gray, 1);
}
