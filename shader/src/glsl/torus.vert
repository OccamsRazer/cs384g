#version 130
attribute vec2 parametric;

uniform vec3 lightPosition;  // Object-space
uniform vec3 eyePosition;    // Object-space
uniform vec2 torusInfo;

varying vec2 normalMapTexCoord;

varying vec3 lightDirection;
varying vec3 halfAngle;
varying vec3 eyeDirection;
varying vec3 c0, c1, c2;

void main()
{
    normalMapTexCoord = parametric*vec2(-6.0,2.0);

    vec2 uvRadians = radians(360.0 * parametric);
    vec3 posSS = vec3( (torusInfo.x + torusInfo.y * cos(uvRadians.y))*cos(uvRadians.x),
                        (torusInfo.x + torusInfo.y * cos(uvRadians.y))*sin(uvRadians.x),
                        torusInfo.y * sin(uvRadians.y));
    gl_Position = gl_ModelViewProjectionMatrix * vec4(  posSS.x,
                                                        posSS.y,
                                                        posSS.z,
                                                        1);
    vec3 tangent = vec3 (   -1.0*(torusInfo.x + torusInfo.y * cos(uvRadians.y))*sin(uvRadians.x),
                            (torusInfo.x + torusInfo.y * cos(uvRadians.y))*cos(uvRadians.x),
                            0);
    tangent = normalize(tangent);

    vec3 gradient = vec3 (  -1.0*torusInfo.y *cos(uvRadians.x)*sin(torusInfo.y),
                            -1.0*torusInfo.y *sin(uvRadians.x)*sin(torusInfo.y),
                            torusInfo.y * cos(uvRadians.y));
    gradient = normalize(gradient);

    vec3 normal = cross(tangent, gradient);
    vec3 binormal = cross(normal, tangent);

    mat3 M = mat3(tangent,binormal,normal);
    mat3 Mt = transpose(M);

    vec3 posOS = M*posSS;

    eyeDirection = Mt*(eyePosition - posOS);
    lightDirection = Mt*(lightPosition - posOS);
    halfAngle = (lightDirection+eyeDirection)/2.0;
    c0 = tangent;
    c1 = binormal;
    c2 = normal;
}

