#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ִ��͸�ӳ���
	// ��ʹ������ͶӰ���󣬶���wԪ���Ա��ֲ��䣬������һ��ʵ���Ϻ������塣���ǣ�
	// ��ʹ��͸��ͶӰ��ʱ����Ǳ�����ˣ�����Ϊ�˱�֤������ͶӰ�����¶���Ч�͵��������С�
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;

    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;

	// ���ǿ�����һ��������Ӱƫ�ƣ�shadow bias���ļ����������Ӱʧ��(Shadow Acne)
	// һ��0.005��ƫ�ƾ��ܰﵽ�ܴ��æ��������Щ�����¶Ⱥܴ���Ȼ�������Ӱʧ�档
	// ��һ�����ӿɿ��İ취�ܹ����ݱ��泯����ߵĽǶȸ���ƫ������ʹ�õ�ˣ�
	// ����������һ��ƫ���������ֵ0.05����һ����Сֵ0.005�������ǻ��ڱ��淨�ߺ͹��շ���ġ�
	// ������ذ������ı��漸�����Դ��ֱ���õ���ƫ�ƾͺ�С��������������Ĳ������ֱ���õ���ƫ�ƾ͸���
	vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // ��鵱ǰƬ���Ƿ�����Ӱ��
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF - �����Ӱ�������
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	// ����һ�����Ǻڰ�������������곬���˹��������׶��Զƽ�档����Կ�����Ƭ��ɫ�������ǳ����ڹ�Դ��׶�ļ�Զ����
	// ��һ����ȹ��Զƽ�滹ҪԶʱ������ͶӰ�����z�������1.0����������£�GL_CLAMP_TO_BORDER���Ʒ�ʽ�������ã�
	// ��Ϊ���ǰ������zԪ�غ������ͼ��ֵ�����˶Աȣ�������Ϊ����1.0��z����true��
	// ����������Ҳ�ܼ򵥣�ֻҪͶӰ������z�������1.0�����ǾͰ�shadow��ֵǿ����Ϊ0.0��
	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);   
	shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}