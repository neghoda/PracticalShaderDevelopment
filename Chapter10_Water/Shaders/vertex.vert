#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

out vec3 fragPos;
out vec2 uv;
out mat3 TBN;


void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);

    vec3 T = normalize(vec3(model * vec4(tangent, 1.0)));
    vec3 B = normalize(vec3(model * vec4(cross(tangent, normal), 1.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 1.0)));
   
    TBN = mat3(T, B, N);
    fragPos = vec3(model * vec4(position, 1.0));
    uv = vec2(texCoord.x, -texCoord.y);
}
