
uniform vec4 LMa; // Light-Material ambient
uniform vec4 LMd; // Light-Material diffuse
uniform vec4 LMs; // Light-Material specular
uniform float shininess;

uniform sampler2D normalMap;
uniform sampler2D decal;
uniform sampler2D heightField;
uniform samplerCube envmap;

uniform mat3 objectToWorld;

varying vec2 normalMapTexCoord;
varying vec3 lightDirection;
varying vec3 eyeDirection;
varying vec3 halfAngle;
varying vec3 c0, c1, c2;

void main()
{
    vec3 normalLight = normalize(lightDirection);

    vec3 bumpVec = normalize(texture2D(normalMap, normalMapTexCoord).rgb *2.0 - 1.0);
    float diffuseWeight = max(dot(bumpVec, normalLight), 0);

    float specWeight = max(normalize(halfAngle).z, 0);

    mat3 M = mat3(c0, c1, c2);
    vec3 reflectSS = reflect(normalize(eyeDirection),bumpVec);
    vec3 reflectOS = M * reflectSS;
    vec3 reflectWS = objectToWorld * reflectOS;
    reflectWS = normalize(reflectWS);

    gl_FragColor = 0.5*(texture2D(decal, normalMapTexCoord) + LMa + LMd * diffuseWeight + LMs * pow(specWeight, shininess)) + 0.6*textureCube(envmap, -1.0*reflectWS);
}
