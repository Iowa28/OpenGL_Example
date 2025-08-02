#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_textcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPos0;
uniform vec3 cameraPos;

void main()
{
    //fs_color = mix(texture(texture1, vs_textcoord), texture(texture2, vs_textcoord), 0.5);
	//fs_color = texture(texture1, vs_textcoord) * texture(texture2, vs_textcoord) * vec4(vs_color, 1.f);
    
    // Ambient light
    float lightIntencity = .75f;
    vec3 ambientLight = vec3(1.f, 1.f, 1.f) * lightIntencity;
    
    // Deffuse light
    vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
    vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
    float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
    vec3 diffuseFinal = diffuseColor * diffuse;
    
    // Specular light
    vec3 lightToPosDirVec = normalize(lightPos0 - vs_position);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(vs_position - cameraPos);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30);
    vec3 specularFinal = vec3(1.f, 1.f, 1.f) * specularConstant;
    
    fs_color = texture(texture1, vs_textcoord) * vec4(vs_color, 1.f)
    * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}