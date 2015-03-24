
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
    normalMapTexCoord = parametric*vec2(-6,2);

    vec2 uvRadians = radians(360.0 * parametric);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(  (torusInfo.x + torusInfo.y * cos(uvRadians.y))*cos(uvRadians.x),
                                                        (torusInfo.x + torusInfo.y * cos(uvRadians.y))*sin(uvRadians.x),
                                                        torusInfo.y * sin(uvRadians.y),
                                                        1);
    eyeDirection = vec3(0);  // XXX fix me
    lightDirection = vec3(0);  // XXX fix me
    halfAngle = vec3(0);  // XXX fix me
    c0 = vec3(0);  // XXX fix me
    c1 = vec3(0);  // XXX fix me
    c2 = vec3(0);  // XXX fix me
}

