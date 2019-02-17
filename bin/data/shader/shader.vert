#version 300 es
uniform vec3 vp, mm1, mm2, mm3, mp;
uniform mat3 vm;
uniform mat4 mpro;
mat4 pm = mpro;
uniform mat3 mm;
layout(location = 0) in vec2 tex;
layout(location = 1) in vec3 vert;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tang0;

out vec2 texCoord;
out vec3 outNormal;
out vec3 relPos;
out vec3 position;
out mat3 tr;
out mat3 vim;

vec3 GenTang(vec3 v)
{
    vec3 r = cross(v, vec3(1.f, 0.f, 0.f));
    if(length(r)<0.0001f)
        r = cross(v, vec3(0.f, 1.f, 0.f));
    if(length(r)<0.0001f)
        r = cross(v, vec3(0.f, 0.f, 1.f));
    return r;
}

void main()
{
    vec4 p;
    relPos = vert;
    position = vm*(mm*relPos+mp-vp);
    p.xyz = position;
	p.w = 1.f;
    p = pm*p;
	gl_Position = p;
	texCoord = tex;
    vim = vm;
    outNormal = normalize(mm*normal);
    vec3 tang;
    if(length(tang0)<0.00001f) tang = GenTang(normal);
    else tang = tang0;
    tang = normalize(mm*tang);
    vec3 bitang = cross(outNormal, tang);
    tr = mat3(tang, bitang, outNormal);
}
