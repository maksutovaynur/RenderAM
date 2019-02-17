#version 300 es
uniform vec4 pm1, pm2, pm3, pm4;
uniform vec3 vm1, vm2, vm3, mm1, mm2, mm3, vp, mp;
mat3 vm = mat3(vm1, vm2, vm3);
mat4 pm = mat4(pm1, pm2, pm3, pm4);
mat3 mm = mat3(mm1, mm2, mm3);
layout(location = 0) in vec3 vert;

out vec3 position;

void main()
{
    vec4 p;
    position = vert;
    p.xyz = vm*(mm*vert+mp-vp);
    p.w = 1.f;
    p = pm*p;
    gl_Position = p;
}
