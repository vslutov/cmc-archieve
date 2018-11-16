#version 130

in vec2 screen_location;
out vec3 color;

uniform float GM;
uniform float R;
const float c = 3e8;

uniform vec3 camera;
uniform vec3 view;
uniform vec3 dx;
uniform vec3 dy;
uniform float disk_size;

uniform sampler2D milky_way;
uniform sampler2D disk;

vec3
get_ray_dir()
{
    return normalize(view + screen_location.x * dx + screen_location.y * dy);
}

void
main()
{
    color = vec3(0, 0, 0);
    float alpha = 1;
    vec3 pos = camera;
    float len_camera = length(camera);
    vec3 vel = c * get_ray_dir();
    int iteration = 0;
    while (true) {
        ++ iteration;

        float len = length(pos);
        vec3 a = -pos * (GM / (len * len * len));

        float dt = 5 * len / R;
        if (dt > 60) {
            dt = 60;
        }

        vec3 step = vel * dt;
        vec3 next = pos + step;
        vel = vel + a * dt;

        if (len > 1.5 * len_camera && dot(pos, vel) > 0 || iteration > 80) {
            vel = normalize(vel);
            float phi = atan(vel.y, vel.x) / 2 / 3.141592653589793 + 0.5;
            float psi = asin(vel.z) / 3.141592653589793 + 0.5;

            color += alpha * texture(milky_way, vec2(phi, psi)).xyz;
            alpha = 0;
            break;
        } else if (pos.z * next.z < 0) {
            float t = - pos.z / step.z;
            vec2 intersect = (pos + t * step).xy;
            float len_intersect = length(intersect);

            if (len_intersect < disk_size * R) {
                float multi = 3 * len_intersect / R - 3.1;
                if (multi < 0) {
                    multi = 0;
                } else if (multi > 1) {
                    multi = 1;
                }

                intersect = intersect / (disk_size * R * 2) + 0.5;
                vec4 disk_color = texture(disk, intersect);
                disk_color.w *= multi;
                color += alpha * disk_color.w * disk_color.xyz;
                alpha *= 1 - disk_color.w;
            }
        }

        if (len < R) { // black hole
            alpha = 0;
            break;
        }

        pos = next;
    }
}
