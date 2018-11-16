#version 130

in vec3 vertex_location;
out vec2 screen_location;

void
main() {
    gl_Position.xyz = vertex_location;
    gl_Position.w = 1.0;
    screen_location = vertex_location.xy;
}

